/*
  ==============================================================================

    AudioManager.cpp
    Created: 19 Jan 2017 10:18:34am
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioManager.h"
#include "AsyncParamChange.h"
#include "ADSRSignal.h"
#include "SquareSignal.h"
using namespace Amusing;
//==============================================================================
AudioManager::AudioManager(AmusingModel *m_model) : model(m_model), Nsources(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	trackVector.reserve(Nmax);
	setAudioChannels(0, 2);
}

AudioManager::~AudioManager()
{
	shutdownAudio();
}

void AudioManager::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

   
}

void AudioManager::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void AudioManager::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

}
void AudioManager::releaseResources()
{

}
void AudioManager::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	askParameter();
	bufferToFill.clearActiveBufferRegion();
}

void AudioManager::askParameter()
{
	Miam::AsyncParamChange param;
	if (model->lookForParameter(param))
	{
		//DBG("receive parameter");
		//DBG("param type receive = " + (String)param.Type);
		switch (param.Type)
		{
		case Miam::AsyncParamChange::ParamType::None :
			//DBG("None");
			break;
		case Miam::AsyncParamChange::ParamType::Activate :
			switch (param.Id2)
			{
			case 0 :
				trackVector.push_back(std::shared_ptr<SquareSignal>( new SquareSignal(0.5, 100, 15)));
				break;
			default:
				break;
			}

			++Nsources;
			DBG("sources actives = " + (String)Nsources );
			break;
		case Miam::AsyncParamChange::ParamType::Frequency :
			DBG("Frequency");
			break;
		case Miam::AsyncParamChange::ParamType::Volume :
			if (param.DoubleValue == 0)
				--Nsources;
			DBG("Volume");
			break;
		default:
			break;
		}
	}
}
