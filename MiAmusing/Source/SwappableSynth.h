/*
  ==============================================================================

    SwappableSynth.h
    Created: 23 Nov 2017 11:40:28am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include <thread>
#include <atomic>



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
	void addVoice(juce::SynthesiserVoice* newVoice);

	void clearSounds();
	void setSound(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData);
	void setSound(String soundPath);
	void renderNextBlock(AudioSampleBuffer &outputBuffer, const MidiBuffer &inputBuffer, int startSample, int numSample);

private:
	// setState return true if the transition is possible, false otherwise
	bool setState(SwappableSynthState newState);

	Synthesiser synthA, synthB;
	ScopedPointer<AudioFormatReader> audioReader;
	SamplerSound* soundA;
	SamplerSound* soundB;
	AudioFormatManager audioFormatManager;

	void addSoundOnThread(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData);
	void addSoundFromExternalFileOnThread(String soundPath);

	std::atomic_bool synthAPlaying;
	std::atomic<SwappableSynthState> state;
	
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

