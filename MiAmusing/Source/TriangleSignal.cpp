/*
  ==============================================================================

    TriangleSignal.cpp
    Created: 16 Dec 2016 9:51:18am
    Author:  ayup1

  ==============================================================================
*/

#include "TriangleSignal.h"
#include "Amusinus.h"
#include <cmath>

TriangleSignal::TriangleSignal(double m_amplitude, double m_frequency, int m_numHarmonics, int m_ID) : 
	AudioSource(), frequency(m_frequency), numHarmonics(m_numHarmonics), amplitude(m_amplitude),
	phase(0), state(StoppedTr), ID(m_ID)
{
	//computeFrequencies();
	DBG("TriangleSignal Constructor");
	DBG("f = " + (String)frequency);
}

TriangleSignal::~TriangleSignal()
{
	DBG("TriangleSignal Destructor");
	//delete S;
}

void TriangleSignal::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	//DBG("TriangleSignal::prepareToPlay");
	
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];

	for(int i=0; i < numHarmonics; ++i)
	{
		int n = 2 * i + 1;
		harmonicsN[i] = n;
		harmonicsA[i] = (4 * amplitude / pow(n * PI, 2)) * (1-pow(-1,n));
		//DBG("n = " + (String)n + " a" + (String)n + " = " + (String)A);
		//double A = amplitude / (n ^ 2);
		harmonics[i] = new AmuSinus(harmonicsA[i], n*frequency, PI/2, sampleRate, samplesPerBlockExpected);
	}
}
void TriangleSignal::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	if (state == PlayingTr || state == StartingTr)
	{
		float* const buffer0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
		float* const buffer1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{
			buffer0[i] = 0;
			buffer0[i] = 0;
			for (int j = 0; j < numHarmonics; ++j)
			{
				buffer0[i] += (float)harmonics[j]->getSample();
				buffer1[i] += (float)harmonics[j]->getSample();
			}
		}
	}
	else
		bufferToFill.clearActiveBufferRegion();
}

void TriangleSignal::releaseResources()
{
	DBG("TriangleSignal::releaseResources");
	
	for (int i = 0; i < numHarmonics; ++i)
		delete harmonics[i];
	

	delete[] harmonics;
	delete[] harmonicsN;
	delete[] harmonicsA;
}

void TriangleSignal::setFrequency(double newFrequency)
{
	frequency = newFrequency;
	if (harmonics != nullptr)
	{
		for (int i = 0; i<numHarmonics; ++i)
			harmonics[i]->setFrequency(harmonicsN[i] * newFrequency);
	}
}

void TriangleSignal::setAmplitude(double newAmplitude)
{
	amplitude = newAmplitude;
	if (harmonics != nullptr)
		for (int i = 0; i < numHarmonics; ++i)
			harmonics[i]->setAmplitude(harmonicsA[i] * newAmplitude);
}

void TriangleSignal::changeState(TransportStateTr newState)
{
	if (state != newState)
	{
		state = newState;
		switch (state)
		{
		case StoppingTr:
		case StoppedTr: // arrete de jouer et remet les harmoniques a leurs phases initaiales
			if (harmonics != nullptr)
				for (int i = 0; i < numHarmonics; ++i)
					harmonics[i]->resetPhase();
			break;
		case StartingTr:
			state = PlayingTr;
			break;
		case PlayingTr:
			break;
		case PausingTr:
		case PausedTr: // arrete de jouer mais laisse la phase la ou elle en etait
			state = PausedTr;
			break;
		default:
			DBG("probleme avec state");
			break;
		}
	}
}

int TriangleSignal::getID()
{
	return ID;
}

