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
	void setPeriod(int m_period);
	void setSpeed(float newSpeed);
	void setMidiTime(int idx, int newTime, int m_noteNumber, float m_velocity);
	void setMidiChannel(int m_chan);
	void setId(int m_Id);
	void setAllVelocities(float m_velocity);
	int getId();
	float getSpeed();
	int getPeriod();

	bool isNoteOnTime(int m_position, int i, bool &end, int &channel, int &note, uint8 &m_velocity);
	bool isNoteOffTime(int m_position, int i, bool &end, int &channel, int &note);
	bool isChordOnTime(int m_position, int & m_channel, int *m_chordToPlay, uint8 & m_velocity);
	bool isChordOffTime(int m_position, int & m_channel, int m_chordToPlay[]);
	//void process(int time);
	//void playNoteContinuously();

	double getRelativePosition();
	void alignWith(TimeLine *ref, double phase);

	void addChord(TimeLine* otherTimeLine, int chordTime);
	bool isNoteAvailable(ChordType m_chordType, int baseNote1, int &otherChordNote);
	void createChord(ChordType m_chordType, int m_chordTime, int baseNote1, int baseNote2);
	void createPerfectChord(int chordTime, int currentNote);
	void resetAllChords();

	void addMessageToQueue(MidiMessage msg);
	void removeNextBlockOfMessages(MidiBuffer & incomingMidi, int numSamples);

	void setFilterFrequency(double frequency);
	IIRFilter* getFilter();


	void renderNextBlock(AudioSampleBuffer &outputAudio, const MidiBuffer &incomingMidi, int startSample, int numSamples);
	void clearSounds();
	void addSound(const SynthesiserSound::Ptr& newSound);
	void addSound(const void* srcData, size_t srcDataSize, bool keepInternalCopyOfData);

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
	float speed;
	int duration;
	int period; // period, to be sure we don't set a noteOff signal after the end of the period...
	int currentPeriod; // period taking speed into accound
	int lastNote; // last note played -> we have to send the noteOff msg when the object is deleted
	bool continuous; // if true, we send the noteOn for all the notes

	void applyOffSet(int offset);

	void testMidi();
	MidiMessageCollector midiCollector;

	IIRFilter* filter;
	Synthesiser synth;
	ScopedPointer<AudioFormatReader> audioReader;
	SamplerSound* newSound;
	double currentFilterFrequency;
	double deltaF;
	double filterFrequencyToReach;
	FilterType filterType;
	void updateFilter();

	// reference to the audioManager to send the MIDI
	Amusing::AudioManager* audioManager;
	JUCE_LEAK_DETECTOR(TimeLine);
};

