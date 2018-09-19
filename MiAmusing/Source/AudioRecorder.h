/*
  ==============================================================================

    AudioRecorder.h
    Created: 13 Oct 2017 2:04:18pm
    Author:  ayup1

  ==============================================================================
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