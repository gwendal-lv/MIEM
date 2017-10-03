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
	newTime =  (int)round((double)newTime * (double)currentPeriod / (double)period);
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

bool TimeLine::isNoteOnTime(int m_position, int i, bool &m_end, int &m_channel, int &m_note, uint8 &m_velocity)
{

	if (i < midiTimesSize)
	{
		m_end = false; // on a pas encore atteint la fin de la liste de notes (au cas où il y en a plusieurs à jouer au même moment)
		if (m_position == midiTimes[i])
		{
			if (velocity[i] != 0)
			{
				m_channel = channel;
				m_note = notes[i];
				m_velocity = (uint8)velocity[i];
				return true;
			}
			else // si velocité == 0, pas besoin d'envoyer de note vu qu'on l'entend pas...
				return false;


		}
		else
			return false;

	}
	else
	{
		m_end = true;
		return false;
	}
	
}

bool TimeLine::isNoteOffTime(int m_position, int i, bool &m_end, int &m_channel, int &m_note)
{
	if (i < midiOfftimesSize)
	{
		m_end = false;
		if (m_position == midiOffTimes[i])
		{

			if (velocity[i] != 0)
			{
				m_channel = channel;
				m_note = notes[i];
				return true;
			}
			else // si velocité == 0, pas besoin d'envoyer de note vu qu'on l'entend pas...
				return false;
		}
		else
			return false;

	}
	else
	{
		m_end = true;
		return false;
	}
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
		while (time >= currentPeriod)
			time -= currentPeriod;
		if (oldTime == period - 1)
			t0 = time + 1;
		//time += offset;
		position = time;
		/*if (time == 0)
			t0 = 1;*/

		
		
		bool m_On;
		int m_channel, m_note;
		uint8 m_velocity;

		bool m_end = false;
		int i = 0;
		while (m_end == false)
		{
			if (isNoteOnTime(time, i, m_end, m_channel, m_note, m_velocity))
			{
				MidiMessage midiMsg = MidiMessage::noteOn(m_channel, m_note, m_velocity);
				audioManager->sendMidiMessage(midiMsg);
			}
			i++;
		}

		m_end = false;
		i = 0;
		while (m_end == false)
		{
			if (isNoteOffTime(time, i, m_end, m_channel, m_note))
			{
				MidiMessage midiMsgOff = MidiMessage::noteOff(m_channel, m_note);
				audioManager->sendMidiMessage(midiMsgOff);
			}
			i++;
		}

		
	}
}

double TimeLine::getRelativePosition()
{
	return ( (double)position)/(double)currentPeriod;
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
			newOffset += currentPeriod;
	else if (newOffset > period)
		while (newOffset > period)
			newOffset -= currentPeriod;

	if (newOffset != offset)
	{
		//applyOffSet(newOffset - offset);
		offset = newOffset;
	}
	testMidi();
}

void TimeLine::applyOffSet(int offset)
{
	for (int i = 0; i < midiTimesSize; i++)
	{
		midiTimes[i] += offset;
		midiOffTimes[i] += offset;
		if (offset > 0)
		{
			while (midiTimes[i] > period)
				midiTimes[i] -= period;
			while (midiOffTimes[i] > period)
				midiOffTimes[i] -= period;
		}
		else
		{
			while (midiTimes[i] < 0)
				midiTimes[i] += period;
			while (midiOffTimes[i] < period)
				midiOffTimes[i] += period;
		}
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