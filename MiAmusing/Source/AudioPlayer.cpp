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

void AudioPlayer::setFrequency(String sceneName, int ID, double frequency)
{
	DBG("setFrequency");
	if (sinus == nullptr)
		DBG("Error no sinus");
	else
		sinus->setFrequency(frequency);
}

void AudioPlayer::setAmplitude(String sceneName, int ID, double amplitude)
{
	DBG("setAmplitude");
	if (sinus == nullptr)
		DBG("Error no sinus");
	else
		sinus->setAmplitude(amplitude);
}

void AudioPlayer::setPath(String sceneName, int ID, String path)
{
	DBG("setPath");
}

void AudioPlayer::setState(String sceneName, int ID, TransportState transportState)
{
	DBG("setState");
}

void AudioPlayer::setVolume(String sceneName, double amplitude)
{
	DBG("setVolume");
}

void AudioPlayer::setReverse(String sceneName, int ID, bool enable)
{
	DBG("setReverse");
}

void AudioPlayer::addSource(String sceneName, int ID, int areaId)
{
	sinus = new ToneGeneratorAudioSource();
	sinus->setAmplitude(0.0f);
	sinus->setFrequency(440);
	
}