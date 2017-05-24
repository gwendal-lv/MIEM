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
						 public AudioSourcePlayer,
						 public AudioDeviceManager
	{
	public:
		AudioManager(AmusingModel *m_mode);
		~AudioManager();

		//void paint(Graphics&) override;
		//void resized() override;

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void releaseResources() override;
		void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

		void askParameter();
		void chooseAudioType(int position, int type);
		void AncienchooseAudioType(int type,double duration);

		AudioDeviceManager& getAudioDeviceManager();

		void sendMidiMessage(MidiMessage midiMsg);

	private:

		void trackVectorHandler(bool deactivation, int type);
		void changeState(AudioManagerState nexState);
		void playAllSources();
		void stopAllSources();

		void sendPosition();

		void verifyAllSource();
		void HandleEvent();

		AmusingModel *model;
		MixerAudioSource *mixer;
		//std::thread activationThread;// s[2];

		AudioManagerState state;

		std::vector<int> sourceControled;
		void playAllControledSources();

		int currentSamplesPerBlock;
		double currentSampleRate;

		int useADSR;
		int count;
		double div;

		std::vector<std::shared_ptr<AmuSignal>> trackVector;
		std::vector<bool> activeVector;
		const int Nmax = 1024;
		int Nsources;

		double testPos;

		int periode;
		int position;
		Metronome metronome;
		std::shared_ptr<TimeLine> midiSender;
		std::vector<std::shared_ptr<TimeLine>> midiSenderVector;
		MidiBuffer midiBuffer;
		MidiOutput *midiOuput;
		//ScopedPointer<MidiOutput> midiOuput;
		//std::shared_ptr<MidiOutput> midiOuput;
		void getParameters(); // for MIDI
		void threadFunc();
		std::thread T;
		bool runThread;
		int midiSenderSize;

		/////////////
		// timeLines is the array of the timeLines, the allocation is manage by the thread
		// timeLinesToAudio is the lockfree queue used to send to the audio a pointer to the TimeLines objects
		// timeLinesCopy belong to the audio thread and is the copy of the real TimeLinesObject used by the object
		// 
		// when we receive the order to create a new TimeLine, the thread create it,
		// when it's done, a pointer on this object is sent to the audio thread 
		// thus the audioThread knows only the ready objects.
		static const int maxSize = 1024;
		TimeLine* timeLines[maxSize];
		boost::lockfree::spsc_queue<TimeLine*, boost::lockfree::capacity<(1 << 17)>> timeLinesToAudio;
		TimeLine* timeLinesKnown[maxSize];
		void getNewTimeLines();
		boost::lockfree::spsc_queue<Miam::AsyncParamChange, boost::lockfree::capacity<(1 << 17)>> paramToAudio;
	};
}

#endif  // AUDIOMANAGER_H_INCLUDED
