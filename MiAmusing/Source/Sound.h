/*
  ==============================================================================

    Sound.h
    Created: 29 Nov 2016 3:55:55pm
    Author:  amusing

  ==============================================================================
*/

#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmuSignal.h"
#include "AmusingFormatReaderSource.h"

class Sound : public AmuSignal, ChangeListener
{
    public :
    

    
        
       
        
        
        
        
       
       

public :
	Sound(AudioFormatManager *m_formatManager);
	~Sound();

	void setPath(String m_path);
	String getPath();
	
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void setFrequency(double newFrequency) override;
	void setAmplitude(double newAmplitude) override;

	void changeState(TransportState newState) override;

private :

	void changeListenerCallback(ChangeBroadcaster* source) override;
	void setSubsection(int start, int end);

	AudioFormatManager *formatManager;
	String path;

	//ScopedPointer<AudioFormatReaderSource> readerSource;
	ScopedPointer<AmusingFormatReaderSource> readerSource;
	AudioTransportSource *transportSource;
	TransportState state;

	AudioSubsectionReader *subsectionReader;
	ResamplingAudioSource *resampling;

	int samplesPerBlock;
	double currentSampleRate;

	double targetAmplitude;
	
	double targetFrequency;
	void updateFrequency();

	int64 position;
	

	AudioFormatReader* reader; // nouveau
	

	
		

        
};



#endif  // SOUND_H_INCLUDED
