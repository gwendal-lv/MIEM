/*
  ==============================================================================

    AudioManager.h
    Created: 19 Jan 2017 10:18:34am
    Author:  ayup1

  ==============================================================================
*/

#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#include <thread>
#include <vector>

//#include "../JuceLibraryCode/JuceHeader.h"
#include "JuceHeader.h"
#include "AmusingModel.h"
#include "AmuSignal.h"

#include "Metronome.h"
#include "BaseMidiSender.h"
#include "ReadingHead.h"

#include "AsyncParamChange.h"

// Pre-declaration for pointer members
namespace Amusing {

	class AmusingModel;//AudioManager;

	enum AudioManagerState
	{
		Play,
		Pause,
		Stop
	};


//==============================================================================
/*
*/
	class AudioManager : public AudioSource,
						 public AudioSourcePlayer
	{
	public:
		AudioManager(AmusingModel *m_mode);
		~AudioManager();
		
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void releaseResources() override;
		void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

		void sendMidiMessage(MidiMessage midiMsg, PlayHead* sender); // send Midi message to external synth

		double getCurrentSampleRate();
		int getCurrentSamplesBlock();

		int getNumOfBeats() { return beatsByTimeLine; }

	private:

		AmusingModel *model; // reference to the parent

		void sendPosition(); // send the readingHeads position (in percent) to the presenter (via model)

		AudioManagerState state; // Play, Pause, ...
		
		// audio thread parameters
		int currentSamplesPerBlock;
		double currentSampleRate;

		// midi message to send to the internal synth
		MidiMessageCollector midiCollector; 

		bool playInternalSynth;
		void setUsingSampledSound();
		int timeStamp;

		int beatsByTimeLine;
		int position;
		Metronome *metronome;
		
		MidiBuffer midiBuffer;
		MidiOutput *midiOuput;

		
		void getParameters(); // function of the audio thread to handle parameters from the Presenter

		std::thread T; // allocation thread
		void threadFunc(); // function of the allocation thread
		void clearAudioObjectsOnThread();
		std::mutex allocationThreadsMutex;
		bool runThread;	
		void getAudioThreadMsg(); // function of the allocation thread to handle parameters from the audio thread

		/////////////
		// timeLines is the array of the timeLines, the allocation is manage by the thread
		// timeLinesToAudio is the lockfree queue used to send to the audio a pointer to the TimeLines objects
		// timeLinesCopy belong to the audio thread and is the copy of the real TimeLinesObject used by the object
		// 
		// the audioThread keep checking for new parameter/msg sent by the presenter,
		// if the parameter needs an allocation, a msg is sent to a separate thread.
		// when it receives the order to create a new TimeLine, the thread creates it,
		// when it's done, a pointer on this object is sent to the audio thread 
		// thus the audioThread knows only the ready objects.
		//
		// when the audioThread receives order to delete a TimeLine, it relay the msg to the separate thread
		// the audioThread continue by setting his pointer to the object to null
		// while the allocation thread delete the object

		static const int maxSize = 1024;
		TimeLine* timeLines[maxSize];
		boost::lockfree::spsc_queue<TimeLine*, boost::lockfree::capacity<(1 << 17)>> timeLinesToAudio;
		TimeLine* timeLinesKnown[maxSize];
		void getNewTimeLines();
		boost::lockfree::spsc_queue<Miam::AsyncParamChange, boost::lockfree::capacity<(1 << 17)>> paramToAllocationThread;
		
		PlayHead* playHeads[maxSize];
		boost::lockfree::spsc_queue<PlayHead*, boost::lockfree::capacity<(1 << 17)>> playHeadsToAudio;
		PlayHead* playHeadsKnown[maxSize];
		void getNewPlayHeads();
	};
}

#endif  // AUDIOMANAGER_H_INCLUDED
