/*
  ==============================================================================

    Metronome.h
    Created: 17 May 2017 10:05:38am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/// \brief Time reference for all the audio track
class Metronome
{
public:
	Metronome();
	~Metronome();

	void setAudioParameter(int m_samplesPerBlock, double m_sampleRate);
	void update();

	double getNextSample();
	MidiMessage getNextMidiMsg();

	int getSamplesToNextBeat();
	int timeToSample(double ms);
	int BPMtoPeriodInSample(int m_bpm);

private:
	int BPM;
	int samplesTime;
	int samplesPerBlock;
	double sampleRate;

	int samplePeriod;
	double nextSample;

	int timeStamp;
};

