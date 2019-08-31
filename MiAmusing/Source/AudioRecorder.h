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

class AudioRecorder : public AudioIODeviceCallback
{
public:
	AudioRecorder();

	~AudioRecorder();

	//==============================================================================
	void startRecording(const File& file);

	void stop();

	bool isRecording() const;

	//==============================================================================
	void audioDeviceAboutToStart(AudioIODevice* device);

	void audioDeviceStopped();

	void audioDeviceIOCallback(const float** inputChannelData, int /*numInputChannels*/,
		float** outputChannelData, int numOutputChannels,
		int numSamples) override;

private:

	TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
	ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
	double sampleRate;
	int64 nextSampleNum;

	CriticalSection writerLock;
	AudioFormatWriter::ThreadedWriter* volatile activeWriter;
};