/*
  ==============================================================================

    Amusinus.cpp
    Created: 15 Dec 2016 10:18:05am
    Author:  ayup1

  ==============================================================================
*/

#include "Amusinus.h"
#include <cmath>

#include "../JuceLibraryCode/JuceHeader.h" // juste pour le DBG

AmuSinus::AmuSinus(double m_amplitude, double m_frequency, double m_phase, double m_sampleRate, int m_bufferSize) :
	amplitude(m_amplitude), targetAmplitude(m_amplitude), 
	frequency(m_frequency), targetFrequency(m_frequency),
	phase(m_phase), angle(m_phase),
	sampleRate(m_sampleRate), angleDelta(0), bufferSize(m_bufferSize)
{
	DBG("AmuSinus Constructor");
	DBG("frequency sinus = " + (String)frequency);
	updateAngleDelta();
}

AmuSinus::AmuSinus(AmuSinusParameter parameters) :
	amplitude(parameters.m_amplitude), frequency(parameters.m_frequency), targetFrequency(parameters.m_frequency), phase(parameters.m_phase), angle(parameters.m_phase),
	sampleRate(parameters.m_sampleRate), angleDelta(0), bufferSize(parameters.m_bufferSize)
{
	updateAngleDelta();
}

AmuSinus::~AmuSinus()
{
	DBG("AmuSinus Destructor");
}

void AmuSinus::updateAngleDelta()
{
	angleDelta = (2 * PI * frequency / sampleRate); // 2*pi*f*Dt
	//DBG("(f,sampleRate) =  (" + (String)frequency + " , " + (String)sampleRate + ")");
}

void AmuSinus::setFrequency(double newFrequency)
{
	//DBG("target = " + (String)targetFrequency);
	targetFrequency = newFrequency;
	updateFrequency();
}

void AmuSinus::updateFrequency()
{
	if (targetFrequency != frequency)
	{
		//DBG("freq avant : " + (String)frequency);
		frequency = frequency + (targetFrequency - frequency) / bufferSize;
		//DBG("freq apres : " + (String)frequency);
	}
}

double AmuSinus::getFrequency()
{
	return frequency;
}

double AmuSinus::getSample()
{
	
	double sample = amplitude * std::sin(angle);

	
	updateAngleDelta();
	angle += angleDelta;

	updateFrequency();
	updateAmplitude();
	return sample;
}

void AmuSinus::setAmplitude(double newAmplitude)
{
	targetAmplitude = newAmplitude;
	updateAmplitude();
}

void AmuSinus::updateAmplitude()
{
	if (targetAmplitude != amplitude)
		amplitude = amplitude + (targetAmplitude - amplitude) / bufferSize;
}

double AmuSinus::getAmplitude()
{
	return amplitude;
}

void AmuSinus::setPhase(double newPhase)
{
	if (newPhase != phase)
	{
		angle += (newPhase - phase);
		phase = newPhase;
	}
}

void AmuSinus::resetPhase()
{
	angle = phase;
}

void AmuSinus::resetPhase(double newPhase)
{
	angle = newPhase;
	phase = newPhase;
}

double AmuSinus::getPhase()
{
	return phase;
}