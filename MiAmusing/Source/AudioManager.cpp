/*
  ==============================================================================

    AudioManager.cpp
    Created: 19 Jan 2017 10:18:34am
    Author:  ayup1

  ==============================================================================
*/
//..\..\..\..\boost_1_63_0\geometry\algorithms
#include<thread>
#include<boost\geometry.hpp>
#include<boost\lockfree\queue.hpp>
#include<boost\lockfree\spsc_queue.hpp>



#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioManager.h"
#include "AsyncParamChange.h"
#include "ADSRSignal.h"
#include "AmuSignal.h"
#include "SquareSignal.h"
#include "TriangleSignal.h"
#include "SinusSignal.h"
using namespace Amusing;
//==============================================================================
AudioManager::AudioManager(AmusingModel *m_model) : model(m_model), Nsources(0), state(Stop)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	DBG("AudioManager::AudioManager");


	/////////////////////
	useADSR = 1;////////
	////////////////////

	trackVector.reserve(Nmax);
	activeVector.reserve(Nmax);
	mixer = new MixerAudioSource();
	setAudioChannels(0, 2);
}

AudioManager::~AudioManager()
{
	DBG("audioManager destructor");
	shutdownAudio();
	DBG("audioManager destructor fin");
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
	DBG("AudioManager::prepareToPlay");
	currentSamplesPerBlock = samplesPerBlockExpected;
	currentSampleRate = sampleRate;
}
void AudioManager::releaseResources()
{
	DBG("AudioManager::releaseResources");
	delete mixer;
	DBG("AudioManager::releaseResources fin");
}
void AudioManager::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	
	askParameter();
	if (Nsources > 0)
		mixer->getNextAudioBlock(bufferToFill);
		//trackVector[0]->getNextAudioBlock(bufferToFill);
	
		
	//bufferToFill.clearActiveBufferRegion();
}

void AudioManager::chooseAudioType(int position, int type)
{
	DBG("BORDEEL : " + (String)type);
	if (position = 1025)
	{
		switch (type)
		{
		case 3:
			trackVector.push_back(std::shared_ptr<TriangleSignal>(new TriangleSignal(0.5, 100, 15)));
			break;
		case 4:
			trackVector.push_back(std::shared_ptr<SquareSignal>(new SquareSignal(0.5, 100, 15)));
			break;
		case 20:
			trackVector.push_back(std::shared_ptr<SinusSignal>(new SinusSignal(0.5, 100, 15)));
			break;
		default:
			break;
		}
		trackVector.end()->get()->prepareToPlay(currentSamplesPerBlock, currentSampleRate);
		
	}
	else
	{
		switch (type)
		{
		case 3:
			trackVector[position] = (std::shared_ptr<TriangleSignal>(new TriangleSignal(0.5, 100, 15)));
			break;
		case 4:
			trackVector[position] = (std::shared_ptr<SquareSignal>(new SquareSignal(0.5, 100, 15)));
			break;
		case 20:
			trackVector[position] = (std::shared_ptr<SinusSignal>(new SinusSignal(0.5, 100, 15)));
			break;
		default:
			break;
		}
		trackVector[position]->prepareToPlay(currentSamplesPerBlock, currentSampleRate);
		
	}
	
	++Nsources;
	DBG("sources actives = " + (String)Nsources);
}

void AudioManager::AncienchooseAudioType(int type, double duration)
{
	DBG("AncienChooseAudioType");
	switch (type)
	{
	case 3:
		if(useADSR == 0)
			trackVector.push_back(std::shared_ptr<TriangleSignal>(new TriangleSignal(0.5, 100, 15)));
		else if (useADSR == 1)
		{
			//std::shared_ptr<ADSRSignal> P(new ADSRSignal(std::shared_ptr<TriangleSignal>(new TriangleSignal(0.5, 100, 15)).get(), duration));
			//trackVector.push_back(P);
			trackVector.push_back(std::shared_ptr<ADSRSignal>(new ADSRSignal(3, duration)));
		}
		break;
	case 4:
		if(useADSR == 0)
			trackVector.push_back(std::shared_ptr<SquareSignal>(new SquareSignal(0.5, 100, 15)));
		else if (useADSR == 1)
		{
			DBG("avant alloc");
			//std::shared_ptr<ADSRSignal> P(new ADSRSignal(std::shared_ptr<SquareSignal>(new SquareSignal(0.5, 100, 15)).get(), duration));
			//P->isEmpty();
			trackVector.push_back(std::shared_ptr<ADSRSignal>(new ADSRSignal(4, duration)));
			DBG("apres alloc");
			//trackVector.push_back(P);
			
			DBG("apres push");
		}
		break;
	case 20:
		if(useADSR == 0)
			trackVector.push_back(std::shared_ptr<SinusSignal>(new SinusSignal(0.5, 100, 15)));
		else if (useADSR == 1)
		{
			//std::shared_ptr<ADSRSignal> P(new ADSRSignal(std::shared_ptr<SinusSignal>(new SinusSignal(0.5, 100, 15)).get(), duration));
			trackVector.push_back(std::shared_ptr<ADSRSignal>( new ADSRSignal(20,duration)));
		}
		break;
	default:
		break;
	}
	//trackVector.end()->get()->prepareToPlay(currentSamplesPerBlock, currentSampleRate);
	DBG("LA");
	
	
	if (auto ad = std::dynamic_pointer_cast<ADSRSignal> (trackVector[Nsources]))
		ad->isEmpty();

	trackVector[Nsources]->prepareToPlay(currentSamplesPerBlock, currentSampleRate);
	
	
	mixer->addInputSource(trackVector[Nsources].get(), false);
	trackVector[Nsources]->changeState(Stopped);
	++Nsources;
}

void AudioManager::trackVectorHandler(bool activation,int type)
{
	int position = 1025;
	if (activation) // creation de la source
	{
		mixer->removeInputSource(trackVector[position].get());
		activeVector[position] = false; // audioManager ne voit plus la source
		chooseAudioType(position, type);
		activeVector[position] = true;
		mixer->addInputSource(trackVector[position].get(), false);
	}
}

void AudioManager::askParameter()
{
	bool ddd = false;
	//DBG("Dans askparam");
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
			DBG("Activate");
			AncienchooseAudioType(param.Id2,param.DoubleValue);
			DBG("continue");
			
			break;
		case Miam::AsyncParamChange::ParamType::Frequency :
			//DBG("Frequency" + (String)param.Id1 + " a " + (String)param.DoubleValue);
			if (param.Id1 > trackVector.size() - 1 || param.Id1 > Nsources - 1)
			{
				DBG("Stop !!! : " + (String)param.Id1 + " > "+ (String)(Nsources-1)  );
				break;
			}
			trackVector[param.Id1]->setFrequency(param.DoubleValue);
			DBG("Source " + (String)param.Id1 + " f = " + (String)trackVector[param.Id1]->getFrequency() + " v = " + (String)trackVector[param.Id1]->getAmplitude());
			break;
		case Miam::AsyncParamChange::ParamType::Volume :
			if (param.DoubleValue == 0)
				--Nsources;
			else
			{
				DBG("Volume" + (String)param.Id1 + " a " + (String)param.DoubleValue);
				if (param.Id1 > trackVector.size()-1)
				{
					DBG("Stop !!!");
					break;
				}
				trackVector[param.Id1]->setAmplitude(param.DoubleValue);
				
			}
			break;
		case Miam::AsyncParamChange::ParamType::Duration:
			//if (auto ad = std::dynamic_pointer_cast<ADSRSignal>(trackVector[param.Id1])
			//{
				//ad->setDuration(param.DoubleValue);
			//}
			//DBG("Duration received");
			ddd = true;
			break;
		case Miam::AsyncParamChange::ParamType::Play:
			DBG("Play control received");
			changeState(Play);
			break;
		case Miam::AsyncParamChange::ParamType::Pause:
			DBG("Pause control received");
			break;
		case Miam::AsyncParamChange::ParamType::Stop:
			DBG("Stop control received");
			changeState(Stop);
			break;
		default:
			break;
		}
	}
	else
	{
		//DBG("No param");
	}

	if (ddd == true)
	{
		if (auto ad = std::dynamic_pointer_cast<ADSRSignal>(trackVector[param.Id1]))
			ad->setDuration(param.DoubleValue);
	}
}

void AudioManager::changeState(AudioManagerState newState)
{
	if (newState != state)
	{
		state = newState;
		switch (state)
		{
		case Amusing::Play:
			playAllSources();
			break;
		case Amusing::Pause:
			break;
		case Amusing::Stop:
			stopAllSources();
			break;
		default:
			break;
		}
	}
}

void AudioManager::playAllSources()
{
	for (int i = 0; i < Nsources; ++i)
		trackVector[i]->changeState(TransportState::Starting);
}
void AudioManager::stopAllSources()
{
	for (int i = 0; i < Nsources; ++i)
		trackVector[i]->changeState(TransportState::Stopping);
}
