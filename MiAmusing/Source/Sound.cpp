/*
  ==============================================================================

    Sound.cpp
    Created: 29 Nov 2016 3:55:55pm
    Author:  amusing

  ==============================================================================
*/

#include "Sound.h"
#include <cmath>

Sound::Sound(AudioFormatManager *m_formatManager) : 
	state(Stopped), AmuSignal(0.125,1), formatManager(m_formatManager), 
	transportSource(nullptr)
{
	DBG("Sound constructor");
}

Sound::~Sound()
{
	DBG("Sound::~Sound");

	delete resampling;

}

void Sound::changeState (TransportState newState)
{
        if (state != newState)
        {
            state = newState;
            
            switch (state)
            {
                case Stopped:
                    transportSource->setPosition (0.0);
                    break;
                    
                case Starting:
					DBG("Starting");
                    transportSource->start();
                    break;
                
                case Playing:
                    break;

				case Pausing:
					transportSource->stop();
					break;

				case Paused :
					break;
                
                case Stopping:
                    transportSource->stop();
                    break;
            }
        }
}


void Sound::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == transportSource)
	{
		if (transportSource->isPlaying())
			changeState(Playing);
		else if ((state == Stopping) || (state == Playing))
			changeState(Stopped);
		else if (Pausing == state)
			changeState(Paused);
	}
}
    
//------------------------------------------------------------------

void Sound::setPath(String m_path)
{
	path = m_path;

	transportSource = new AudioTransportSource();
	transportSource->addChangeListener(this);

	if (path.isNotEmpty())
	{
		const File file(path);
		reader = formatManager->createReaderFor(file);
		if (reader != nullptr)
		{
			subsectionReader = new AudioSubsectionReader(reader, (int64)0, (int64)reader->lengthInSamples, true);
			ScopedPointer<AmusingFormatReaderSource> newSource = new AmusingFormatReaderSource(subsectionReader, true);//true
			transportSource->setSource(newSource, 0, nullptr, reader->sampleRate);

			readerSource = newSource.release();
			if (readerSource == nullptr)
				DBG("setPath : readerSource == nullptr");
			readerSource->setLooping(true);

			resampling = new ResamplingAudioSource(transportSource, true, 2);
			resampling->setResamplingRatio(1.0);

		}
		else
			DBG("reader is null!!!");
	}
	else
		DBG("Path is empty !!!");
}

String Sound::getPath()
{
	return path;
}

void Sound::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	//transportSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampling->prepareToPlay(samplesPerBlockExpected, sampleRate);
	samplesPerBlock = samplesPerBlockExpected;
	currentSampleRate = sampleRate;
}

void Sound::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	if (readerSource == nullptr)
		bufferToFill.clearActiveBufferRegion();
	else
	{
		//DBG("position = " + (String)transportSource->getCurrentPosition());
		//transportSource->getNextAudioBlock(bufferToFill);
		
		int f = 5;
		if (f < 0)
		{
			
			//int64 currentPosition = (int64)(transportSource->getCurrentPosition() * currentSampleRate);
			//int64 nextPosition = transportSource->getNextReadPosition();
			//position = position - samplesPerBlock;
			position = position - bufferToFill.numSamples;
			if (position < 0)
				position = transportSource->getTotalLength() + position;
			transportSource->setPosition(position/currentSampleRate);
			//resampling->setResamplingRatio(1.0f);
			resampling->getNextAudioBlock(bufferToFill);
			bufferToFill.buffer->reverse(bufferToFill.startSample, bufferToFill.numSamples);
			//currentPosition = (int64)(transportSource->getCurrentPosition() * currentSampleRate);
			//nextPosition = transportSource->getNextReadPosition();
			
		}
		else
		{
			resampling->getNextAudioBlock(bufferToFill);
			position = transportSource->getNextReadPosition();
		}

		if (amplitude != targetAmplitude) // update amplitude
		{
			bufferToFill.buffer->applyGainRamp(bufferToFill.startSample, bufferToFill.startSample + bufferToFill.numSamples,
				(float)amplitude, (float)targetAmplitude);
			amplitude = targetAmplitude;
		}
		else
			bufferToFill.buffer->applyGain((float)amplitude);

		
			
	}
}



void Sound::setFrequency(double newFrequency)
{
	DBG((String)newFrequency);
	DBG((String)(abs(newFrequency)));
	resampling->setResamplingRatio(abs(newFrequency));
	frequency = newFrequency;
	if (frequency < 0)
		readerSource->setReverse(true);
	else
		readerSource->setReverse(false);
	//targetFrequency = newFrequency;
}

void Sound::updateFrequency()
{

}

void Sound::setAmplitude(double newAmplitude)
{
	//transportSource->setGain(newAmplitude);
	targetAmplitude = newAmplitude;
}



void Sound::releaseResources()
{
	//transportSource->releaseResources();
	resampling->releaseResources();
}

void Sound::setSubsection(int start, int end)
{
	if (subsectionReader != nullptr)
	{
		changeState(Stopping);
		delete subsectionReader;
		subsectionReader = new AudioSubsectionReader(reader, (int64)start, (int64)end, true);
		ScopedPointer<AmusingFormatReaderSource> newSource = new AmusingFormatReaderSource(subsectionReader, true);
		transportSource->setSource(newSource, 0, nullptr, reader->sampleRate);
		readerSource = newSource.release();
	}
}