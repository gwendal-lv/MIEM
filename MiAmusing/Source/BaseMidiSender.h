/*
  ==============================================================================

    BaseMidiSender.h
    Created: 18 May 2017 11:13:19am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include<vector>
//#include "AudioManager.h"
namespace Amusing
{
	class AudioManager;
}

class BaseMidiSender
{
public:
	BaseMidiSender(int m_period);
	~BaseMidiSender();

	void setAudioManager(Amusing::AudioManager* m_audioManager);
	void setMidiTime(int idx, int newTime);

	void process(int time);

private:
	std::vector<double> midiTimes; // times to send MIDI
	std::vector<double> midiOffTimes;
	bool wasUnder;
	bool noteOffSent;

	// parameter of the notes to send
	int noteNumber;
	int velocity;
	int duration;
	int period; // period, to be sure we don't set a noteOff signal after the end of the period...

	// reference to the audioManager to send the MIDI
	Amusing::AudioManager* audioManager;
};

