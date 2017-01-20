/*
  ==============================================================================

    AudioManager.h
    Created: 19 Jan 2017 10:18:34am
    Author:  ayup1

  ==============================================================================
*/

#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmusingModel.h"
#include "AmuSignal.h"
#include <vector>


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
	class AudioManager : public AudioAppComponent
	{
	public:
		AudioManager(AmusingModel *m_mode);
		~AudioManager();

		void paint(Graphics&) override;
		void resized() override;

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void releaseResources() override;
		void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

		void askParameter();

	private:

		AmusingModel *model;

		std::vector<std::shared_ptr<AmuSignal>> trackVector;
		const int Nmax = 1024;
		int Nsources;

	};
}

#endif  // AUDIOMANAGER_H_INCLUDED
