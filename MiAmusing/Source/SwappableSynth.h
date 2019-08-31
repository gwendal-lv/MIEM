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

#pragma once

#include "JuceHeader.h"
#include <thread>
#include <atomic>
#include <mutex>


/*
	Synthesiser allowing to change base Sound.
	This class is used like the juce::Synth
	To be able to avoid lock, it contains two Synthesiser : one playing at the time, while the other will perform the modification.
	When the modification is finished, the two synth will swap so the new sound will be playing
*/
class SwappableSynth
{
	enum SwappableSynthState
	{
		Playing,
		Loading,
		Swapping,
	};

public:
	SwappableSynth();
	~SwappableSynth();

	void setBuffersSize(int numChannels, int numSamples);
	void setCurrentPlaybackSampleRate(double m_sampleRate);
	void addVoice();

	void clearSounds();
	void setSound(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData);
	void setSound(String soundPath);
	void renderNextBlock(AudioSampleBuffer &outputBuffer, const MidiBuffer &inputBuffer, int startSample, int numSample);

	void skipSwapping(bool m_shouldSkip);

private:
	// setState return true if the transition is possible, false otherwise
	bool setState(SwappableSynthState newState);
	bool skipMutex; // if true, unlock the mutex directly after the loading
					// else unlock the mutex after the swapping

	Synthesiser synthA, synthB;
	//ScopedPointer<AudioFormatReader> audioReader;
	//SamplerSound* soundA;
	//SamplerSound* soundB;
	AudioFormatManager audioFormatManager;

	String currentPathA;
	String currentPathB;

	void addSoundOnThread(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData);
	void addSoundFromExternalFileOnThread(String soundPath);

	std::atomic_bool synthAPlaying;
	std::atomic<SwappableSynthState> state;

	std::mutex synthMtx;
	std::atomic<bool> waitForUnlock;
	
	AudioSampleBuffer rampBuffer_off;
	AudioSampleBuffer rampBuffer_On;
	const int T_On = 100; // 100ms to end a sound
	const int T_Off = 100; // 100ms to begin playing the new sound
	float DG_On;
	float DG_Off;
	float G_Off;
	float G_On;
	float getNextGainOff(int numSamples);
	float getNextGainOn(int numSamples);
};

