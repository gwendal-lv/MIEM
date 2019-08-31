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

#ifndef ADSRSIGNAL_H_INCLUDED
#define ADSRSIGNAL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FourierSignal.h"

enum ADSR_State
{
	Attack,
	Decay,
	Sustain,
	Release,
	Silence
};

//==============================================================================
/*
*/
class ADSRSignal    : public AmuSignal//AudioSource // peut etre Positionable
{
public:
    ADSRSignal(FourierSignal *m_signal);
	ADSRSignal(FourierSignal *m_signal, bool m_stopSustain);
	ADSRSignal(FourierSignal *m_signal, double m_duration);
	ADSRSignal(int type, double duration, bool m_loop);
    ~ADSRSignal();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;
    
	void setFrequency(double newFrequency) override;
	void setAmplitude(double newAmplitude) override;
	void changeState(TransportState newState) override;

	void playNote();
	void releaseNote();

	void setDuration(double duration);

	int getLength();

	void isEmpty();
	
	bool isLooping();

	int getPosition();

private:
	FourierSignal *signal;
	void changeState(ADSR_State newState);

	// internal attribut
	ADSR_State ADSR_state;
	int position;
	double currentGain;
	bool stopSustain; // indique si tient la note le temps prevu ou si on la maintient jusque nouvel ordre, stopSustain = true : arrete apres le temps prevu
	bool loop;
	bool erase;

	// parameter of ADSR envelop
	double attackT;
	double attackLvl;
	double decay;
	double sustainT;
	double sustainLvl;
	double release;

	// durations of ADSR envelop in sample
	int endAttackP;
	int endDecayP;
	int endSustainP;
	int endReleaseP;

	int currentSamplesPerBlock;
	double currentSampleRate;
};


#endif  // ADSRSIGNAL_H_INCLUDED
