/*
  ==============================================================================

    AmuSignal.h
    Created: 19 Dec 2016 9:57:50am
    Author:  ayup1

  ==============================================================================
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
