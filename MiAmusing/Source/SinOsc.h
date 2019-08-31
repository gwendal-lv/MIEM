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

#ifndef SINOSC_H_INCLUDED
#define SINOSC_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmuSignal.h"
#include "Amusinus.h"

//==============================================================================
/*
*/
class SinOsc    : public AudioSource
{
public:
    SinOsc(AmuSinus *m_carrier, AmuSinus *m_mod);
    ~SinOsc();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void setMf(double m_mf);
	void nextSample();
	double getSample();

	double getCurrentAngle();
	double getCurrentAmplitude();
	double getCurrentModulation();

private:
	// carrier
	AmuSinus *carrier;
	// modulator
	AmuSinus *mod;

	// indice de modulation
	double mf;

	// valeur de l'integrale du signal modulant
	double Imod;

	double currentAngle;
	double currentAmplitude;
	double currentModulation;

};


#endif  // SINOSC_H_INCLUDED
