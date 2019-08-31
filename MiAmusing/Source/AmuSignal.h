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

#ifndef AMUSIGNAL_H_INCLUDED
#define AMUSIGNAL_H_INCLUDED

#include "JuceHeader.h"

enum TransportState
{
	Stopped,
	Starting,
	Playing,
	Pausing,
	Paused,
	Stopping
};

class AmuSignal : public AudioSource
{
public:
	AmuSignal(double m_amplitude, double m_frequency);
	~AmuSignal();

	virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override = 0;
	virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override = 0;
	virtual void releaseResources() override = 0;

	virtual void setFrequency(double newFrequency) = 0;
	virtual void setAmplitude(double newAmplitude) = 0;

	virtual void changeState(TransportState newState) = 0;

	double getFrequency()
	{
		return frequency;
	}
	double getAmplitude()
	{
		return amplitude;
	}

	bool isStopped();

	bool isPlaying();

protected:
	double amplitude;
	double frequency;
	TransportState state;
};





#endif  // AMUSIGNAL_H_INCLUDED
