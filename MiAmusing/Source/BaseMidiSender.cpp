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

	noteOffSent = true;
	channel = 1;
	duration = 10000;

	lastNote = 0;
}

TimeLine::~TimeLine()
{
	MidiMessage midiMsgOff = MidiMessage::noteOff(channel, lastNote);
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
	}
}

void TimeLine::setMidiTime(int idx, int newTime, int m_noteNumber)
{
	
	if (idx < maxSize)
	{
		//DBG("<");
		if (idx < midiTimesSize)
		{
			midiTimes[idx] = newTime;
			if (newTime + duration > period) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = newTime + duration - period;
			else
				midiOffTimes[idx] = newTime + duration;
			notes[idx] = m_noteNumber;
		}
		else
		{
			//DBG(">");
			for (int i = 0; i < idx - midiTimesSize - 1; ++i)
			{
				midiTimes[midiTimesSize + i] = 0;
				midiOffTimes[midiTimesSize + i] = 0;
				notes[midiTimesSize + i] = 0;
				++midiTimesSize;
				++midiOfftimesSize;
			}
			midiTimes[idx] = newTime;
			if (newTime + duration > period) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = (newTime + duration - period);
			else
				midiOffTimes[idx] = (newTime + duration);
			notes[idx] = m_noteNumber;
			++midiTimesSize;
			++midiOfftimesSize;
		}
		//DBG("create note : " + (String)notes[idx]);
		//DBG("BMS : number of corners is now : " + (String)midiTimesSize);
	}
}

void TimeLine::setMidiChannel(int m_chan)
{
	channel = m_chan;
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
	for (int i = 0; i < midiTimesSize; i++)
	{
		if (time == midiTimes[i])
		{
			//DBG("Play note : " + (String)notes[i]);
			MidiMessage midiMsg = MidiMessage::noteOn(channel, notes[i], (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			lastNote = notes[i];
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