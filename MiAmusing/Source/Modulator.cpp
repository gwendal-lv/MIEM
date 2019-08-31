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

#include "../JuceLibraryCode/JuceHeader.h"
#include "Modulator.h"
#include <cmath>

//==============================================================================
Modulator::Modulator(FourierSignal *m_carrier, FourierSignal *m_mod) :
	carrier(m_carrier), mod(m_mod), mf(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	// shift mod's phase to integrate
	// mettre un setMod 
	
}

Modulator::~Modulator()
{
}

void Modulator::setMf(double m_mf)
{
	mf = m_mf;
}

void Modulator::setCarrier(FourierSignal* m_carrier)
{
	carrier = m_carrier;
}

void Modulator::setMod(FourierSignal *m_mod)
{

	mod = m_mod;
	mod->setPhase(carrier->getPhase());

	DBG("Phase : " + (String)mod->getPhase() + " " + (String)carrier->getPhase());

}

void Modulator::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	
	
}

void Modulator::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	float* const buffer0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	float* const buffer1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
	
	for (int i = 0; i < bufferToFill.numSamples; ++i)
	{

		carrier->updateSample();
		mod->updateSample();
		

		buffer0[i] = 0;
		buffer1[i] = 0;

		mf = 1;

		
		double modTot = 0;
		for (int j = 0; j < mod->getNumHarmonics(); ++j)
			modTot += mod->getHarmonic(j)->getAmplitude() * std::sin(mod->getHarmonic(j)->getCurrentAngle() + 3*PI/2) / mod->getHarmonic(j)->getFrequency();
		
		
		for (int j = 0; j < carrier->getNumHarmonics(); ++j)
		{
			
			buffer0[i] += (float)(carrier->getHarmonic(j)->getAmplitude() * std::sin(carrier->getHarmonic(j)->getCurrentAngle() +  modTot));
			buffer1[i] = (float)modTot; // pour vérifier la modulation
		}
		
	}

	
}

void Modulator::releaseResources()
{

}


