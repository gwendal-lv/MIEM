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
