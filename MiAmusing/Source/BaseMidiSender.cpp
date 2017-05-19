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

BaseMidiSender::BaseMidiSender(AudioManager* m_audioManager, int m_period)
{
	audioManager = m_audioManager;
	period = m_period;

	wasUnder = true;
	noteOffSent = true;
	noteNumber = 36;
	duration = 1000;
	//midiTimes.push_back(0);
	DBG("blbbl");
}

BaseMidiSender::~BaseMidiSender()
{
}

void BaseMidiSender::setMidiTime(int idx, int newTime)
{
	//midiTimes.
	//DBG("test size = " + (String)test.size());
	//DBG("idx = " + (String)idx);
	//DBG("size = " + (String)midiTimes.size());
	DBG("----before------");
	for (int i = 0; i < midiTimes.size(); ++i)
	{
		DBG((String)midiTimes[i] + " " + (String)midiOffTimes[i]);
	}
	DBG("----------");
	if (idx < midiTimes.size())
	{
		midiTimes[idx] = newTime;
		if(newTime+duration>period) // verif si on depasse pas le temps de la periode !
			midiOffTimes[idx] = newTime + duration - period;
		else
			midiOffTimes[idx] = newTime + duration;
	}
	else
	{
		for (int i = 0; i < idx - midiTimes.size(); ++i)
		{
			midiTimes.push_back(0);
			midiOffTimes.push_back(0);
		}
		midiTimes.push_back(newTime);
		midiOffTimes.push_back(newTime + duration);
		if (newTime + duration>period) // verif si on depasse pas le temps de la periode !
			midiOffTimes.push_back(newTime + duration - period);
		else
			midiOffTimes.push_back(newTime + duration);
	}

	DBG("----after------");
	for (int i = 0; i < midiTimes.size();++i)
	{
		DBG((String)midiTimes[i] + " " + (String)midiOffTimes[i]);
	}
	DBG("----------");
}

void BaseMidiSender::process(int time)
{
	//DBG("processing : " + (String)time);
	/*
	for (int i = 0; i < midiTimes.size() -1; ++i)
	{
		if (time >= midiTimes[i] && time < midiTimes[i+1])
		{
			if (wasUnder) // was the previous time under the interval [midiTime[i];midiTime[i+1][
			{
				// send MIDI
				DBG("midi sent : " + (String)i + " at position : " + (String)time);
				MidiMessage midiMsgOn = MidiMessage::noteOn(10, noteNumber, (uint8)100);
				
				audioManager->sendMidiMessage(midiMsgOn);
				
				wasUnder = false;// the position is then under the midiTime[i+1]
				noteOffSent = false; // the note off needs to be sent
				return; // unnecessary to go futher
			}
			else // we were already in this segment --> verify if it's time to send the note off
			{
				if (time > midiOffTimes[i] && noteOffSent == false) // if note off time and not sent yet ->send
				{
					MidiMessage midiMsgOff = MidiMessage::noteOff(10, noteNumber);
					audioManager->sendMidiMessage(midiMsgOff);
					noteOffSent = true; // the note off msg has been sent
					
				}
				return;
			}
		}
		else
		{
			wasUnder = true; // because of the break, we will never test the segments above the actual position
							// so if we're not in the tested segment, we are necesseraly under the correct segment
		}
	}
	if (time > midiTimes[midiTimes.size()-1])
	{
		if (wasUnder)
		{
			//DBG("midi sent : " + (String)(midiTimes.size() - 1));
			MidiMessage midiMsg = MidiMessage::noteOn(10, noteNumber, (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			wasUnder = false;// the position is then under the midiTime[i+1]
		}
		else
		{
			if (time > midiOffTimes[midiTimes.size() - 1] && noteOffSent == false) // if note off time and not sent yet ->send
			{
				MidiMessage midiMsgOff = MidiMessage::noteOff(10, noteNumber);
				audioManager->sendMidiMessage(midiMsgOff);
				noteOffSent = true; // the note off msg has been sent
			}
		}
	}
	else
		wasUnder = true;
		*/
	for (int i = 0; i < midiTimes.size(); ++i)
	{
		if (time == midiTimes[i])
		{
			DBG("On : " + (String)i);
			MidiMessage midiMsg = MidiMessage::noteOn(10, noteNumber, (uint8)100);
			audioManager->sendMidiMessage(midiMsg);
			return;
		}
			
	}
	for (int i = 0; i < midiOffTimes.size(); ++i)
	{
		if (time == midiOffTimes[i])
		{
			//DBG("Off : " + (String)i);
			MidiMessage midiMsgOff = MidiMessage::noteOff(10, noteNumber);
			audioManager->sendMidiMessage(midiMsgOff);
			return;
		}

	}
}