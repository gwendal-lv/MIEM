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

#ifndef FOURIERSIGNAL_H_INCLUDED
#define FOURIERSIGNAL_H_INCLUDED

#include "AmuSignal.h"
#include "Amusinus.h"

class FourierSignal : public AmuSignal
{
public:
	FourierSignal(double m_amplitude, double m_frequency, int m_numHarmonics);
	~FourierSignal();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void updateSample();
	double getNextSample();

	void setFrequency(double newFrequency) override;
	void setAmplitude(double newAmplitude) override;
	void setPhase(double newPhase);
	
	double getAmplitude();
	double getFrequency();

	void changeState(TransportState newState) override;

	virtual void computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate) = 0;
	AmuSinus** getHarmonics(); 
	AmuSinus* getHarmonic(int idx); 

	int getNumHarmonics();
	int getHarmonicsN(int idx);
	double getHarmonicsA(int idx);

	void shiftPhase(double shift);
	void resetPhase(double newPhase);
	double getPhase();

protected:
	AmuSinus **harmonics = 0;
	int *harmonicsN = 0; // ordre des harmoniques
	double *harmonicsA = 0; // amplitude des harmoniques

	double phase;
	int numHarmonics; // nombre d'harmoniques que l'on veut utiliser

	double sample;
	
};





#endif  // FOURIERSIGNAL_H_INCLUDED
