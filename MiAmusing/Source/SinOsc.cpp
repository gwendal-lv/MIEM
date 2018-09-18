/*
  ==============================================================================

    SinOsc.cpp
    Created: 2 Jan 2017 1:24:42am
    Author:  ayup1

  ==============================================================================
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

