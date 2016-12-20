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

enum TransportState // ajouter pause (pausing + paused)
    {
        Stopped,
        Starting,
        Playing,
		Pausing,
		Paused,
        Stopping
    };

class Sound : public ChangeListener
{
    public :
        String name;
    
        ScopedPointer<AudioFormatReaderSource> readerSource;
        AudioTransportSource *transportSource;
        TransportState state;
        ResamplingAudioSource *output;
        
        AudioFormatReader* reader; // nouveau
        AudioSubsectionReader* subsectionReader; //nouveau
    
        void changeState(TransportState newState);
        void changeListenerCallback (ChangeBroadcaster* source) override;
        
        void setSound(AudioFormatReader *reader,int samplesExpected,double currentSampleRate);
        void setSubsection(int start, int end,int samplesExpected,double currentSampleRate);
        
        Sound();
        ~Sound();
        
};



#endif  // SOUND_H_INCLUDED
