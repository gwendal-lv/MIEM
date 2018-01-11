/*
  ==============================================================================

    Metronome.cpp
    Created: 17 May 2017 10:05:38am
    Author:  ayup1

  ==============================================================================
*/

#include "Metronome.h"

#include<cmath>

Metronome::Metronome() : BPM(120), samplesTime(0), periodInSamples(0), samplesLeftBeforeBeat(0)
{
	numOfBeats = 4;
	currentBeats = 0;
	currentT = 0;
}

Metronome::~Metronome()
{
}

void Metronome::update()
{

	/////
	if (periodInSamples != 0)
	{
		--samplesLeftBeforeBeat;
		if (samplesLeftBeforeBeat < 0)
		{
			samplesLeftBeforeBeat = periodInSamples;
			++currentBeats;
			if (currentBeats >= numOfBeats)
			{
				currentBeats = 0;
				++currentT;
			}
		}
		
		
	}
}

void Metronome::reset()
{
	currentBeats = 0;
	samplesLeftBeforeBeat = periodInSamples;
	currentT = 0;
}

int Metronome::getNumSamplesToNextBeat()
{
	return samplesLeftBeforeBeat; // modulo pas performant
}

int Metronome::getCurrentBeat()
{
	return currentBeats;
}

void Metronome::setAudioParameter(double m_sampleRate, int m_BPM)
{
	BPM = m_BPM;
	sampleRate = m_sampleRate;

	periodInSamples = (int)round((60.0 * sampleRate) / (double)BPM);
	samplesLeftBeforeBeat = periodInSamples;
}



int Metronome::getPeriodInSamples()
{
	// retourne la periode d'un carré donnant le tempo de m_bpm :
	//	m_bpm = 1 Tic tous les sampleRate * 60 / BPM
	//  1 carré = 4 Tic = 4 * sampleRate * 60 / BPM
	return periodInSamples;
}

int Metronome::getCurrentT()
{
	return currentT;
}
