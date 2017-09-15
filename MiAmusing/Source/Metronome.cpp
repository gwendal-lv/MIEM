/*
  ==============================================================================

    Metronome.cpp
    Created: 17 May 2017 10:05:38am
    Author:  ayup1

  ==============================================================================
*/

#include "Metronome.h"

#include<cmath>

Metronome::Metronome() : BPM(120), samplesTime(0), samplePeriod(0)
{
}

Metronome::~Metronome()
{
}

void Metronome::update()
{

	/////
	if (samplePeriod != 0)
	{
		++samplesTime;//samplesTime += samplesPerBlock;
		if (samplesTime%samplePeriod == 0)
		{
			nextSample = 1;
			timeStamp = samplesTime;
		}
		else
			nextSample = 0;
	}
}

int Metronome::getSamplesToNextBeat()
{
	return samplePeriod - samplesTime%samplePeriod; // modulo pas performant
}

void Metronome::setAudioParameter(int m_samplesPerBlock, double m_sampleRate)
{
	samplesPerBlock = m_samplesPerBlock;
	sampleRate = m_sampleRate;

	samplePeriod = (int)round((60.0 * sampleRate) / (double)BPM);
}

double Metronome::getNextSample()
{
	return nextSample;
}

MidiMessage Metronome::getNextMidiMsg()
{
	int midiChannel = 10;
	if (nextSample)
	{
		MidiMessage msg = MidiMessage::noteOn(midiChannel, 36, (uint8)100);
		msg.setTimeStamp(timeStamp);
		return msg;
	}
	else
		return MidiMessage();
}

int Metronome::timeToSample(double ms)
{
	return (int)round(ms * sampleRate / 1000);
}

int Metronome::BPMtoPeriodInSample(int m_bpm)
{
	return (int)round(sampleRate * 60.0 * 4.0 / (double)m_bpm);
}