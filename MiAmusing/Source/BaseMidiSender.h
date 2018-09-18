/*
  ==============================================================================

    BaseMidiSender.h
    Created: 18 May 2017 11:13:19am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include<vector>
//#include "AudioManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "SwappableSynth.h"
namespace Amusing
{
	class AudioManager;
}

enum ChordType
{
	MajorThird,
	MinorThird,
	AugmentedQuart,
	PerfectChord,
};

enum FilterType
{
	LowPass,
	HighPass,
};

class TimeLine
{
public:
	TimeLine();
	~TimeLine();

	void setAudioManager(Amusing::AudioManager* m_audioManager);
	void setMidiTime(int idx, double newTime, int m_noteNumber, float m_velocity);
	void setMidiChannel(int m_chan);
	void setId(int m_Id);
	void setAllVelocities(float m_velocity);
	int getId();

	bool isNoteOnTime(int m_position, int i, int period, bool &end, int &channel, int &note, uint8 &m_velocity);
	bool isNoteOffTime(int m_position, int i, int period, bool &end, int &channel, int &note);
	bool isChordOnTime(int m_position, int i, int period, bool &end, int & m_channel, int &noteToPlay, uint8 & m_velocity);
	bool isChordOffTime(int m_position, int period, int i, bool &end, int& m_channel, int &m_chordToPlay);


	double getRelativePosition();

	void addChord(TimeLine* otherTimeLine, double chordTime);
	void createChord(ChordType m_chordType, double m_chordTime, int baseNote1, int baseNote2);
	void createPerfectChord(double chordTime, int currentNote);
	void resetAllChords();

	int getRandomNote();

	void addMessageToQueue(MidiMessage msg);
	void removeNextBlockOfMessages(MidiBuffer & incomingMidi, int numSamples);

	void setFilterFrequency(double frequency);
	void setSynthPlaying(bool m_shouldPlay);

	void renderNextBlock(AudioSampleBuffer &outputAudio, const MidiBuffer &incomingMidi, int startSample, int numSamples);
	void clearSounds();
	//void addSound(const SynthesiserSound::Ptr& newSound);
	void addSound(const void* srcData, size_t srcDataSize, bool keepInternalCopyOfData);
	void addSound(String soundPath);

private:
	static const int maxSize = 128;
	double midiTimes[maxSize]; // times to send MIDI note On
	double midiOffTimes[maxSize]; // times to send MIDI note Off
	int notes[maxSize];
	int midiTimesSize;
	int midiOfftimesSize;
	int velocity[maxSize];
	int offset;

	double chordTimesOn[maxSize];
	double chordTimesOff[maxSize];
	int chordNotesOn[maxSize];
	int chordNotesOff[maxSize];
	int chordSize;
	
	int lastNotePosition;
	int t0;
	int position;

	int Id; // to make link between the timeLine and the graphic object that it represents

	// parameter of the notes to send
	int channel;
	double duration;
	int lastNote; // last note played -> we have to send the noteOff msg when the object is deleted
	bool continuous; // if true, we send the noteOn for all the notes

	int numOfReaders;

	void testMidi();
	MidiMessageCollector midiCollector;

	//dsp::StateVariableFilter::Filter<float> filterDSP;
	dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> duplicatedFilter;
	//Synthesiser synth;
	SwappableSynth swappableSynth;
	//ScopedPointer<AudioFormatReader> audioReader;
	//SamplerSound* newSound;
	double currentFilterFrequency;
	double deltaF;
	double filterFrequencyToReach;
	FilterType filterType;
	void updateFilter();
	bool filterActive;

	// reference to the audioManager to send the MIDI
	Amusing::AudioManager* audioManager;
	JUCE_LEAK_DETECTOR(TimeLine);
};

