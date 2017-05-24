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
	//midiTimes = std::vector<double>(128, 0);
	//midiOffTimes = std::vector<double>(128, 0);
	midiTimesSize = 0;
	midiOfftimesSize = 0;

	noteOffSent = true;
	noteNumber = 60;
	duration = 10000;	
}

TimeLine::~TimeLine()
{
}

void TimeLine::setAudioManager(AudioManager* m_audioManager)
{
	audioManager = m_audioManager;
}

void TimeLine::setPeriod(int m_period)
{
	period = m_period;
}

void TimeLine::setMidiTime(int idx, int newTime)
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
		if (idx < midiTimesSize)
		{
			midiTimes[idx] = newTime;
			if (newTime + duration > period) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = newTime + duration - period;
			else
				midiOffTimes[idx] = newTime + duration;
		}
		else
		{
			for (int i = 0; i < idx - midiTimesSize - 1; ++i)
			{
				midiTimes[maxSize + i] = 0;
				midiOffTimes[maxSize + i] = 0;
				++midiTimesSize;
				++midiOfftimesSize;
			}
			midiTimes[idx] = newTime;
			if (newTime + duration > period) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = (newTime + duration - period);
			else
				midiOffTimes[idx] = (newTime + duration);
			++midiTimesSize;
			++midiOfftimesSize;
		}
		//DBG("BMS : number of corners is now : " + (String)midiTimesSize);
	}
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