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
#include "SinOsc.h"
#include <cmath>

//==============================================================================
SinOsc::SinOsc(AmuSinus *m_carrier, AmuSinus *m_mod) : 
	carrier(m_carrier), mod(m_mod)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	currentAmplitude = carrier->getAmplitude();
}

SinOsc::~SinOsc()
{
}

void SinOsc::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// "integrate" the sinus  : Int(sinus) = -cosinus = sinus(alpha+ 3*pi/2)
	mod->shiftPhase(3 * PI / 2);
	mod->resetAmplitude(1); // mettra ici mf peut etre
}

void SinOsc::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	float* const buffer0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	float* const buffer1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
	
	for (int i = 0; i < bufferToFill.numSamples; ++i)
	{
		//carrier->getSample(); // deplace la porteuse et la modulante
		//Imod = mod->getSample();
		
		
		buffer0[i] = (float)getSample(); // calcul la valeur actuelle en fonction de la valeur actuelle de la porteuse et de la modulante
		buffer1[i] = (float)getSample();
	}
}

double SinOsc::getSample()
{
	carrier->getSample();
	Imod = mod->getSample();

	currentModulation = mf*Imod;
	currentAngle = carrier->getCurrentAngle() + currentModulation;
	return carrier->getCurrentAmplitude()*std::sin(currentAngle);
}


void SinOsc::releaseResources()
{

}

void SinOsc::setMf(double m_mf)
{
	mf = m_mf;
}


double SinOsc::getCurrentAngle()
{
	return currentAngle;
}

double SinOsc::getCurrentAmplitude()
{
	return currentAmplitude;
}

double SinOsc::getCurrentModulation()
{
	return currentModulation;
}

