/*
  ==============================================================================

    SquareSignal2.cpp
    Created: 14 Feb 2017 2:02:10pm
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SquareSignal2.h"
#include "Amusquare.h"

//==============================================================================
SquareSignal2::SquareSignal2(double m_frequency, double m_amplitude, int m_time2play) :
	frequency(m_frequency), amplitude(m_amplitude), time2play(m_time2play)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	pos = 0;
}

SquareSignal2::~SquareSignal2()
{
}

void SquareSignal2::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	amusquare = new AmuSquare(frequency, amplitude, sampleRate, samplesPerBlockExpected);
	N = round((double)time2play * sampleRate / 1000.0); //time2Play en millisecondes
	DBG("N  = " + (String)N);
}

void SquareSignal2::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	float* const buffer0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	float* const buffer1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
	for (int i = 0; i < bufferToFill.numSamples; ++i)
	{
		double sample = 0;
		if (pos > N)
			sample = 0;
		else
			sample = amusquare->getNextSample();
		buffer0[i] = (float)sample;
		buffer1[i] = (float)sample;
		++pos;
	}
}

void SquareSignal2::releaseResources()
{
	delete amusquare;
}
