/*
  ==============================================================================

    AudioPlayer.cpp
    Created: 12 Dec 2016 10:46:37am
    Author:  ayup1

  ==============================================================================
*/

#include "AudioPlayer.h"

AudioPlayer::AudioPlayer() : sinus(nullptr)
{
	DBG("set audio");
	setAudioChannels(0, 2);
	DBG("audio set");
}

AudioPlayer::~AudioPlayer()
{
	DBG("AudioPlayer destructor");
	shutdownAudio();
	if (sinus != nullptr)
		delete sinus;
}

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	DBG("PrepareToPlay");
	if (sinus != nullptr)
	{
		DBG("sinus != nullptr");
		sinus->prepareToPlay(samplesPerBlockExpected, sampleRate);
	}
	else
	{
		DBG("sinus == nullptr");
	}
}
void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if (sinus == nullptr)
		bufferToFill.clearActiveBufferRegion();
	else
		sinus->getNextAudioBlock(bufferToFill);
}
void AudioPlayer::releaseResources()
{
	if (sinus != nullptr)
		sinus->releaseResources();
}

void AudioPlayer::setFrequency(int ID, double frequency)
{
	DBG("setFrequency");
	if (sinus == nullptr)
		DBG("Error no sinus");
	else
		sinus->setFrequency(frequency);
}

void AudioPlayer::setAmplitude(int ID, double amplitude)
{
	DBG("setAmplitude");
	if (sinus == nullptr)
		DBG("Error no sinus");
	else
		sinus->setAmplitude(amplitude);
}

void AudioPlayer::setPath(int ID, String path)
{
	DBG("setPath");
}

void AudioPlayer::setState(int ID, TransportState transportState)
{
	DBG("setState");
}

void AudioPlayer::setVolume(double amplitude)
{
	DBG("setVolume");
}

void AudioPlayer::setReverse(bool enable)
{
	DBG("setReverse");
}

int AudioPlayer::addSource()
{
	sinus = new ToneGeneratorAudioSource();
	sinus->setAmplitude(0.0f);
	sinus->setFrequency(440);
	return 0;
}