/*
  ==============================================================================

    SquareSignal.cpp
    Created: 15 Dec 2016 11:54:24am
    Author:  ayup1

  ==============================================================================
*/

#include "SquareSignal.h"
#include "Amusinus.h"

SquareSignal::SquareSignal(double m_amplitude, double m_frequency, int m_numHarmonics, int m_ID) :
	AudioSource(), frequency(m_frequency), numHarmonics(m_numHarmonics), amplitude(m_amplitude),
	phase(0), state(Stopped), ID(m_ID)
{
	//computeFrequencies();
	DBG("SquareSignal Constructor");
	DBG("f = " + (String)frequency);
}

SquareSignal::~SquareSignal()
{
	DBG("SquareSignal Destructor");
	//delete S;
}

void SquareSignal::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	DBG("SquareSignal::prepareToPlay");
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];
	 
	for (int i = 0; i < numHarmonics;++i) // trouver un moyen de generaliser ca pour pouvoir fusionner triangle et square
	{
		int n = 2 * i + 1; // on prend que les impaires
		harmonicsN[i] = n;
		harmonicsA[i] = amplitude / n;//(2 * amplitude / (n * PI)) * (pow(-1, (n - 1) / 2));
		harmonics[i] = new AmuSinus(harmonicsA[i], n*frequency, 0, sampleRate, samplesPerBlockExpected);
		//harmonics[i] = new AmuSinus(amplitude/n, n*frequency, 0, sampleRate, samplesPerBlockExpected);
		
	} 
	
}
void SquareSignal::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	if (state == Playing || state == Starting)
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

void SquareSignal::releaseResources()
{
	DBG("SquareSignal::releaseResources");
	
	for (int i = 0; i < numHarmonics; ++i)
		delete harmonics[i];
			
	delete[] harmonics;
	delete[] harmonicsN;
	delete[] harmonicsA;
}

void SquareSignal::setFrequency(double newFrequency)
{
	frequency = newFrequency;
	if (harmonics != nullptr)
	{
		for(int i=0;i<numHarmonics;++i)
			harmonics[i]->setFrequency(harmonicsN[i] * newFrequency);
	}
}

void SquareSignal::setAmplitude(double newAmplitude)
{
	amplitude = newAmplitude;
	if (harmonics != nullptr)
		for (int i = 0; i < numHarmonics; ++i)
			harmonics[i]->setAmplitude(harmonicsA[i] * newAmplitude);
}

void SquareSignal::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;
		switch (state)
		{
		case Stopping:
		case Stopped: // arrete de jouer et remet les harmoniques a leurs phases initaiales
			if (harmonics != nullptr)
				for (int i = 0; i < numHarmonics; ++i)
					harmonics[i]->resetPhase();
			break;
		case Starting:
			state = Playing;
			break;
		case Playing:
			break;
		case Pausing: 
		case Paused: // arrete de jouer mais laisse la phase la ou elle en etait
			state = Paused;
			break;
		default:
			DBG("probleme avec state");
			break;
		}
	}
}

int SquareSignal::getID()
{
	return ID;
}