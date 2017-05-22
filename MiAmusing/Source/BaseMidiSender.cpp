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

BaseMidiSender::BaseMidiSender()
{
	maxSize = 128;
	midiTimes = std::vector<double>(128, 0);
	midiOffTimes = std::vector<double>(128, 0);
	midiTimesSize = 0;
	midiOfftimesSize = 0;

	noteOffSent = true;
	noteNumber = 60;
	duration = 10000;	
}

BaseMidiSender::~BaseMidiSender()
{
}

void BaseMidiSender::setAudioManager(AudioManager* m_audioManager)
{
	audioManager = m_audioManager;
}

void BaseMidiSender::setPeriod(int m_period)
{
	period = m_period;
}

void BaseMidiSender::setMidiTime(int idx, int newTime)
{
	/*
	DBG("----before------");
	int S = (midiTimes.size() > midiOffTimes.size()) ? midiTimes.size() : midiOffTimes.size();
	for (int i = 0; i < S; ++i)
	{
		if (i >= midiTimes.size())
			DBG("        " + (String)midiOffTimes[i]);
		else if (i >= midiOffTimes.size())
			DBG((String)midiTimes[i]);
		else
			DBG((String)midiTimes[i] + " " + (String)midiOffTimes[i]);
	}
	DBG("----------");
	*/

	if (idx < maxSize)
	{
		midiTimes[idx] = newTime;
		if(newTime+duration>period) // verif si on depasse pas le temps de la periode !
			midiOffTimes[idx] = newTime + duration - period;
		else
			midiOffTimes[idx] = newTime + duration;
		++midiTimesSize;
		++midiOfftimesSize;
	}
	else
	{
		for (int i = 0; i < idx - maxSize - 1 ; ++i)
		{
			midiTimes[maxSize + i] = 0;
			midiOffTimes[maxSize + i] = 0;
			++midiTimesSize;
			++midiOfftimesSize;
		}
		midiTimes[idx] = newTime;
		if (newTime + duration>period) // verif si on depasse pas le temps de la periode !
			midiOffTimes[idx] = (newTime + duration - period);
		else
			midiOffTimes[idx] = (newTime + duration);
		++midiTimesSize;
		++midiOfftimesSize;
	}
	
	
}

void BaseMidiSender::process(int time)
{
	//int b = midiTimesSize;
	//DBG("midiTimes.size() = " + (String)midiTimesSize);
	//DBG("midiOffTimes.size() = " + (String)midiOfftimesSize);
	//DBG("time = " + (String)time);

	for (int i = 0; i < midiTimesSize; i++)
	{
		if (time == midiTimes[i])
		{
			//DBG("On : " + (String)i);
			MidiMessage midiMsg = MidiMessage::noteOn(1, noteNumber, (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			return;
		}
			
	}
	for (int i = 0; i < midiOfftimesSize; i++)
	{
		if (time == midiOffTimes[i])
		{
			//DBG("Off : " + (String)i);
			MidiMessage midiMsgOff = MidiMessage::noteOff(1, noteNumber);
			audioManager->sendMidiMessage(midiMsgOff);
			return;
		}

	}
	
}