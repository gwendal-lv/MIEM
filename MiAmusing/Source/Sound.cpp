/*
  ==============================================================================

    Sound.cpp
    Created: 29 Nov 2016 3:55:55pm
    Author:  amusing

  ==============================================================================
*/

#include "Sound.h"

Sound::Sound() : state(Stopped)
{
    transportSource = new AudioTransportSource();
    transportSource->addChangeListener (this);
    output = new ResamplingAudioSource(transportSource,false,2);
    output->setResamplingRatio(1.0f);
}

Sound::~Sound()
{
    //transportSource.removeListener();
    
    
	delete subsectionReader;
	delete transportSource;
    delete output;
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

/*void Sound::changeListenerCallback (ChangeBroadcaster* source)
    {
        if (source == transportSource)
        {
            if (transportSource->isPlaying())
                changeState (Playing);
            else
                changeState (Stopped);
        }
    */

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
    
void Sound::setSound(AudioFormatReader *reader,int samplesExpected,double currentSampleRate)
{
    subsectionReader = new AudioSubsectionReader(reader,(int64)0,(int64)reader->lengthInSamples,true);//false
    
    ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (subsectionReader, true);//true
    transportSource->setSource (newSource, 0, nullptr, reader->sampleRate);
                
     readerSource = newSource.release();
                
     transportSource->prepareToPlay(samplesExpected,currentSampleRate);//
     readerSource->prepareToPlay(samplesExpected,currentSampleRate);
     output->prepareToPlay(samplesExpected,currentSampleRate);
}

void Sound::setSubsection(int start, int end,int samplesExpected,double currentSampleRate)
{

    
    changeState(Stopping);
    delete subsectionReader;
    
    subsectionReader = new AudioSubsectionReader(reader,(int64)start,(int64)end,false);
    
    
    
    ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (subsectionReader, false); // nouveau
    
    transportSource->setSource (newSource, 0, nullptr, reader->sampleRate);
                
     readerSource = newSource.release();
                
     transportSource->prepareToPlay(samplesExpected,currentSampleRate);//
     readerSource->prepareToPlay(samplesExpected,currentSampleRate);
     output->prepareToPlay(samplesExpected,currentSampleRate);
    
}