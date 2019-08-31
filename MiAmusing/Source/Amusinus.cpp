/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
	//DBG("AmuSinus Constructor");
	//DBG("frequency sinus = " + (String)frequency);
	updateAngleDelta();
}

AmuSinus::AmuSinus(AmuSinusParameter parameters) :
	amplitude(parameters.m_amplitude), frequency(parameters.m_frequency), targetFrequency(parameters.m_frequency), phase(parameters.m_phase), angle(parameters.m_phase),
	sampleRate(parameters.m_sampleRate), angleDelta(0), bufferSize(parameters.m_bufferSize)
{
	updateAngleDelta();
}

AmuSinus::AmuSinus(AmuSinus *toCopy) :
	amplitude(toCopy->getAmplitude()), targetAmplitude(toCopy->getAmplitude()),
	frequency(toCopy->getFrequency()), targetFrequency(toCopy->getFrequency()),
	phase(toCopy->getPhase()), angle(toCopy->getPhase()),
	sampleRate(toCopy->getSampleRate()), angleDelta(0), bufferSize(toCopy->getBufferSize())
{
	updateAngleDelta();
}

AmuSinus::~AmuSinus()
{
	//DBG("AmuSinus Destructor");
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

double AmuSinus::getCurrentSample()
{
	return amplitude * std::sin(angle);
}

void AmuSinus::setAmplitude(double newAmplitude)
{
	targetAmplitude = newAmplitude;
	updateAmplitude();
}

void AmuSinus::resetAmplitude(double newAmplitude)
{
	amplitude = newAmplitude;
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

void AmuSinus::shiftPhase(double shift)
{
	angle += shift;
	phase += shift;
}

double AmuSinus::getPhase()
{
	return phase;
}

double AmuSinus::getSampleRate()
{
	return sampleRate;
}

int AmuSinus::getBufferSize()
{
	return (int)bufferSize;
}

double AmuSinus::getCurrentAmplitude()
{
	return amplitude;
}

double AmuSinus::getCurrentAngle()
{
	return angle;
}