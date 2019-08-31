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

#ifndef MODULATOR_H_INCLUDED
#define MODULATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Amusinus.h"
#include "FourierSignal.h"


//==============================================================================
/*
*/
class Modulator    : public AudioSource
{
public:
    Modulator(FourierSignal *m_carrier, FourierSignal *m_mod);
    ~Modulator();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void setMf(double m_mf);
	void setMod(FourierSignal* m_mod);
	void setCarrier(FourierSignal* m_carrier);

private:
	double mf;
	FourierSignal *carrier;
	FourierSignal *mod;


};


#endif  // MODULATOR_H_INCLUDED
