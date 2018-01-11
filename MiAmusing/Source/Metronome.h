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

	void setAudioParameter(double m_sampleRate, int m_BPM);
	void update();
	void reset();
	int getNumSamplesToNextBeat();
	int getCurrentBeat();
	int getPeriodInSamples();
	int getCurrentT();

private:
	int BPM;
	int samplesTime;
	
	double sampleRate;

	int periodInSamples;
	double nextSample;

	int currentT;

	int timeStamp;

	int samplesLeftBeforeBeat;
	int currentBeats;
	int numOfBeats;
};

