/*
  ==============================================================================

    BaseMidiSender.cpp
    Created: 18 May 2017 11:13:19am
    Author:  ayup1

  ==============================================================================
*/

#include "BaseMidiSender.h"
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioManager.h"

using namespace Amusing;
using namespace std;

TimeLine::TimeLine()
{
	midiTimesSize = 0;
	midiOfftimesSize = 0;

	
	channel = 1;
	duration = 10000;

	speed = 1.0f;
	continuous = false;
	

	lastNote = 0;
	t0 = 0;
	position = 0;

	offset = 0;
	padding = 0;
}

TimeLine::~TimeLine()
{
	/*if (!continuous)
	{
		MidiMessage midiMsgOff = MidiMessage::noteOff(channel, lastNote);
		audioManager->sendMidiMessage(midiMsgOff);
	}
	else
	{
		for (int i = 0; i < midiTimesSize; i++)
		{
			MidiMessage midiMsg = MidiMessage::noteOff(channel, notes[i], (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			lastNote = notes[i];
		}
	}*/

	//audioManager->sendMidiMessage(midiMsgOff);
}

void TimeLine::setAudioManager(AudioManager* m_audioManager)
{
	audioManager = m_audioManager;
}

void TimeLine::setPeriod(int m_period)
{
	if (m_period != period)
	{
		
		for (int i = 0; i < midiTimesSize; i++)
		{
			midiTimes[i] = round((double)midiTimes[i] * (double)m_period / (double)period);
			midiOffTimes[i] = round((double)midiOffTimes[i] * (double)m_period / (double)period);
		}
		period = m_period;
		currentPeriod = period / speed;
	}
	testMidi();
}

void TimeLine::setMidiTime(int idx, int newTime, int m_noteNumber,float m_velocity)
{
	newTime =  round((double)newTime * (double)currentPeriod / (double)period);
	while (newTime > currentPeriod)
		newTime -= currentPeriod;
	if (idx < maxSize)
	{
		//DBG("<");
		if (idx < midiTimesSize)
		{
			midiTimes[idx] = newTime;
			if (newTime + duration > currentPeriod) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = newTime + duration - currentPeriod;
			else
				midiOffTimes[idx] = newTime + duration;
			notes[idx] = m_noteNumber;
			velocity[idx] = (int)m_velocity;
		}
		else
		{
			//DBG(">");
			for (int i = 0; i < idx - midiTimesSize - 1; ++i)
			{
				midiTimes[midiTimesSize + i] = 0;
				midiOffTimes[midiTimesSize + i] = 0;
				notes[midiTimesSize + i] = 0;
				velocity[midiTimesSize + i] = 0;
				++midiTimesSize;
				++midiOfftimesSize;
			}
			midiTimes[idx] = newTime;
			if (newTime + duration > currentPeriod) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = (newTime + duration - currentPeriod);
			else
				midiOffTimes[idx] = (newTime + duration);
			notes[idx] = m_noteNumber;
			velocity[idx] = (int)m_velocity;
			++midiTimesSize;
			++midiOfftimesSize;
		}
		//DBG("create note : " + (String)notes[idx]);
		//DBG("BMS : number of corners is now : " + (String)midiTimesSize);
	}
	testMidi();
}

void TimeLine::setMidiChannel(int m_chan)
{
	channel = m_chan;
}

void TimeLine::setSpeed(float newSpeed)
{
	if (continuous == true)
		continuous = false;
	if (newSpeed != speed)
	{
		int newPeriod = round((float)currentPeriod / (newSpeed / speed));
		for (int i = 0; i < midiTimesSize; i++)
		{
			midiTimes[i] = round((double)midiTimes[i] * (double)newPeriod / (double)currentPeriod);
			midiOffTimes[i] = round((double)midiOffTimes[i] * (double)newPeriod / (double)currentPeriod);
		}
		currentPeriod = newPeriod;
		speed = newSpeed;
		
	}
	testMidi();
}

void TimeLine::setId(int m_Id)
{
	Id = m_Id;
}

int TimeLine::getId()
{
	return Id;
}

void TimeLine::process(int time)
{
	//int b = midiTimesSize;
	//DBG("midiTimes.size() = " + (String)midiTimesSize);
	//DBG("midiOffTimes.size() = " + (String)midiOfftimesSize);
	//DBG("time = " + (String)time);
	//DBG("midiTimesSize : " + (String)midiTimesSize);
	//time -= offset;
	if (!continuous)
	{
		time += offset;
		int oldTime = time;
		time += t0;
		//time += offset;
		while (time >= currentPeriod + padding)
			time -= (currentPeriod + padding);
		if (oldTime == period + padding - 1)
			t0 = time + 1;
		//time += offset;
		position = time;
		/*if (time == 0)
			t0 = 1;*/

		for (int i = 0; i < midiTimesSize; i++)
		{
			//if (midiTimes[i] > period)
			//	DBG("connard");
			if (time == midiTimes[i])
			{
				if((uint8)velocity[i] != 0)
					DBG("t : " + (String)midiTimes[i]);
				//DBG("Play note : " + (String)notes[i]);
				MidiMessage midiMsg = MidiMessage::noteOn(channel, notes[i], (uint8)velocity[i]);
				audioManager->sendMidiMessage(midiMsg);
				lastNote = notes[i];
				//DBG("position = " + (String)(time ));
				lastNotePosition = time;
				return;
			}

		}
		for (int i = 0; i < midiOfftimesSize; i++)
		{
			if (time == midiOffTimes[i])
			{
				//DBG("Off : " + (String)i);
				MidiMessage midiMsgOff = MidiMessage::noteOff(channel, notes[i]);
				audioManager->sendMidiMessage(midiMsgOff);
				return;
			}

		}
	}
}

double TimeLine::getRelativePosition()
{
	if (((double)position) / (double)(offset + currentPeriod + padding)>1)
		DBG("position > 1");
	return ( (double)position)/(double)(offset + currentPeriod+padding);
}

void TimeLine::playNoteContinuously()
{
	continuous = true;
	for (int i = 0; i < midiTimesSize; i++)
	{
			MidiMessage midiMsg = MidiMessage::noteOn(channel, notes[i], (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			lastNote = notes[i];			
	}
}

void TimeLine::alignWith(TimeLine *ref, double phase)
{
	// to align this timeLine with the reference timeLine, first we need to have the same period, then we'll apply the phase
	
	// to have the same period, we can use setPeriod or setSpeed
	// since changing the speed would need to resend the information of the new speed to the presenter, we'll first try by changing the period
	DBG((String)(ref->getPeriod()) + " " + (String)period);
	DBG((String)(ref->getSpeed()) + " " + (String)speed);
	if (period != ref->getPeriod())
		setPeriod(ref->getPeriod());
	if (speed != ref->getSpeed())
		setSpeed(ref->getSpeed());

	phase = 1 - phase;

	// now that we have the same period, we apply the phase
	int newOffset =  round(phase * (double)period);
	if (newOffset < 0)
		while (newOffset < 0)
			newOffset += (currentPeriod+padding);
	else if (newOffset > period + padding)
		while (newOffset > period + padding)
			newOffset -= currentPeriod + padding;

	if (newOffset != offset)
	{
		//applyOffSet(newOffset - offset);
		offset = newOffset;
	}
	testMidi();
}

void TimeLine::resize(TimeLine *ref, double IP, double factor)
{
	// apply a resize factor while an invariable point (IP) keeps its position
	// !!! the two intersections points of the two timeLines need to be align before !!!

	if (factor > 0.1)
	{
		// first we compute the position of the IP in the reference timeLines
		int IPref = round(IP * (double)ref->getPeriod() / (double)ref->getSpeed());

		// as the two timeLines are "align", we know the IP position in the current timeLine :
		int IPcurrent = offset - (ref->getPeriod() - IPref);//IPref + offset;//- offset;
		while (IPcurrent > currentPeriod)
			IPcurrent -= currentPeriod;

		// we need to compute have this position in percent so we could compute its new position after the resize
		double ip = (double)IPcurrent / (double)currentPeriod;

		// apply the real resize
		int newPeriod = round(factor * ((double)ref->getPeriod() / (double)ref->getSpeed()));
		setPeriod(newPeriod);

		// compute the new position of the IP
		double newIP = ip * newPeriod;

		// add the difference of the new IP and the old IP positions to the offset
		offset += IPcurrent - newIP;
		while (offset > ((double)ref->getPeriod() / (double)ref->getSpeed()))
			offset -= ((double)ref->getPeriod() / (double)ref->getSpeed());

		// the last step is to assure that current timeLine won't repeat until the next collision, 
		// so we have to set the period to the same as the reference without altering the notes position
		//period = ref->getPeriod();

		// probleme au prochain coup, quand on fera le calcul du facteur, on ne fera pas le bon resize pcq on a plus la vraie periode...
		// ou bien j'essaie un truc "padding" comme le offset mais à la fin ! 
		padding = ref->getPeriod() - (offset + newPeriod);
		if (padding < 0)
			DBG("padding negatif");
	}
}

float TimeLine::getSpeed()
{
	return speed;
}

int TimeLine::getPeriod()
{
	return period;
}

void TimeLine::testMidi()
{
	/*for (int i = 0; i < midiTimesSize; i++)
	{
		if (midiTimes[i] > currentPeriod)
			DBG("connard");
	}*/
}