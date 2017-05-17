/*
  ==============================================================================

    AudioManager.cpp
    Created: 19 Jan 2017 10:18:34am
    Author:  ayup1

  ==============================================================================
*/
//..\..\..\..\boost_1_63_0\geometry\algorithms
#include<thread>
#include<cmath>
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
	testPos = 0;

	count = 0;

	trackVector.reserve(Nmax);
	activeVector.reserve(Nmax);
	mixer = new MixerAudioSource();
	//setAudioChannels(0, 2);
	initialise(0, 2, nullptr, true);
	addAudioCallback(this);
	setSource(this);
}

AudioManager::~AudioManager()
{
	DBG("audioManager destructor");
	//shutdownAudio();
	setSource(nullptr);
	removeAudioCallback(this);
	closeAudioDevice();
	DBG("audioManager destructor fin");

	DBG("AudioManager::releaseResources");
	delete mixer;
	DBG("AudioManager::releaseResources fin");
}
/*
void AudioManager::paint (Graphics& g)
{


   
}
*/
/*
void AudioManager::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
*/

void AudioManager::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	DBG("AudioManager::prepareToPlay");
	currentSamplesPerBlock = samplesPerBlockExpected;
	currentSampleRate = sampleRate;
	div = ((sampleRate / (double)samplesPerBlockExpected))/50;
	DBG((String)sampleRate);
	DBG((String)samplesPerBlockExpected);
	DBG("div = " + (String)div);
	metronome.setAudioParameter(samplesPerBlockExpected, sampleRate);
	
	

	AudioDeviceSetup currentAudioSetup;
	this->getAudioDeviceSetup(currentAudioSetup);

	DBG("default midi output : " + (String)this->getDefaultMidiOutputName());
	midiOuput = this->getDefaultMidiOutput();
}
void AudioManager::releaseResources()
{
	

	
}
void AudioManager::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	metronome.update();
	MidiMessage msg = MidiMessage::noteOn(10, 36, (uint8)100);
	MidiMessage msg2 = MidiMessage::noteOff(10, 36);
	if (midiOuput != nullptr)
	{
		midiOuput->sendMessageNow(msg);
		//midiOuput->sendMessageNow(msg2);
	}
	//midiBuffer.addEvent(metronome.getNextMidiMsg(), 4);
	askParameter();
	if (state != Stop)
	{
		if (Nsources > 0)
		{
			verifyAllSource();
			mixer->getNextAudioBlock(bufferToFill);
		}
		//trackVector[0]->getNextAudioBlock(bufferToFill);
	//HandleEvent();
	}
	else
		bufferToFill.clearActiveBufferRegion();

	if (fmod(count, div) == 0)
		sendPosition();
	++count;
	bool testComplete = true;
	if (testComplete)
	{
		if (state == Play)
		{
			//DBG("testPos = " + (String)(testPos*1000));
			testPos = (testPos + 0.001);
			if (testPos > 1)
				testPos = 0;
			AsyncParamChange param;
			param.Type = AsyncParamChange::ParamType::Position;
			param.Id1 = 0;
			param.DoubleValue = testPos;
			model->SendParamChange(param);
		}
	}
	
}


void AudioManager::sendPosition()
{
	/*
	AsyncParamChange param;
	for (int i = 0; i < Nsources; ++i)
	{
		param.Type = AsyncParamChange::ParamType::Position;
		if (trackVector[i]->isPlaying())
			if (auto currentADSR = std::dynamic_pointer_cast<ADSRSignal>(trackVector[i]))
			{
				param.Id1 = i;
				param.DoubleValue = (double)currentADSR->getPosition() / (double)currentADSR->getLength();
				model->SendParamChange(param);
			}
	}
	*/
	AsyncParamChange param;
	for (int i = 0; i < Nsources; ++i)
	{
		if (i > 1) // pour le moment, pour pas envoyer a un Follower inexistant et verifier qu'on detecte bien la collision
			break;
		param.Type = AsyncParamChange::ParamType::Position;
		if (trackVector[i]->isPlaying())
			if (auto currentADSR = std::dynamic_pointer_cast<ADSRSignal>(trackVector[i]))
			{
				param.Id1 = i;
				param.DoubleValue = (double)currentADSR->getPosition() / (double)currentADSR->getLength();
				model->SendParamChange(param);
			}
	}
}

void AudioManager::chooseAudioType(int position, int type)
{
	DBG("BORDEEL : " + (String)type);
	if (position == 1025)
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
		DBG("Triangle");
		if(useADSR == 0)
			trackVector.push_back(std::shared_ptr<TriangleSignal>(new TriangleSignal(0.5, 100, 15)));
		else if (useADSR == 1)
		{
			//std::shared_ptr<ADSRSignal> P(new ADSRSignal(std::shared_ptr<TriangleSignal>(new TriangleSignal(0.5, 100, 15)).get(), duration));
			//trackVector.push_back(P);
			trackVector.push_back(std::shared_ptr<ADSRSignal>(new ADSRSignal(3, duration,false)));
		}
		break;
	case 4:
		DBG("Carre");
		if(useADSR == 0)
			trackVector.push_back(std::shared_ptr<SquareSignal>(new SquareSignal(0.5, 100, 15)));
		else if (useADSR == 1)
		{
			DBG("avant alloc");
			//std::shared_ptr<ADSRSignal> P(new ADSRSignal(std::shared_ptr<SquareSignal>(new SquareSignal(0.5, 100, 15)).get(), duration));
			//P->isEmpty();
			trackVector.push_back(std::shared_ptr<ADSRSignal>(new ADSRSignal(4, duration,false)));
			DBG("apres alloc");
			//trackVector.push_back(P);
			
			DBG("apres push");
		}
		break;
	case 20:
		DBG("Cercle");
		if(useADSR == 0)
			trackVector.push_back(std::shared_ptr<SinusSignal>(new SinusSignal(0.5, 100, 15)));
		else if (useADSR == 1)
		{
			//std::shared_ptr<ADSRSignal> P(new ADSRSignal(std::shared_ptr<SinusSignal>(new SinusSignal(0.5, 100, 15)).get(), duration));
			trackVector.push_back(std::shared_ptr<ADSRSignal>( new ADSRSignal(20,duration,false)));
		}
		break;
	default:
		break;
	}
	//trackVector.end()->get()->prepareToPlay(currentSamplesPerBlock, currentSampleRate);
	DBG("LA : " +(String)Nsources);
	
	
	if (auto ad = std::dynamic_pointer_cast<ADSRSignal> (trackVector[Nsources]))
		ad->isEmpty();
	//DBG("apres cast");
	trackVector[Nsources]->prepareToPlay(currentSamplesPerBlock, currentSampleRate);
	//DBG("apres prepare to play");
	
	mixer->addInputSource(trackVector[Nsources].get(), false);
	//DBG("apres add to mixer");
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
		//DBG("param type receive = " + (String)param.Type + " " + (String)param.Id2);
		switch (param.Type)
		{
		case Miam::AsyncParamChange::ParamType::None :
			//DBG("None");
			break;
		case Miam::AsyncParamChange::ParamType::Activate :
			//DBG("Activate");
			AncienchooseAudioType(param.Id2,param.DoubleValue);
			//DBG("continue");
			
			break;
		case Miam::AsyncParamChange::ParamType::Frequency :
			//DBG("Frequency" + (String)param.Id1 + " a " + (String)param.DoubleValue);
			if (param.Id1 > (int)trackVector.size() - 1 || param.Id1 > Nsources - 1)
			{
				DBG("Stop !!! : " + (String)param.Id1 + " > "+ (String)(Nsources-1)  );
				break;
			}
			//trackVector[param.Id1]->setFrequency(param.DoubleValue); // enlever pour adsr, remettre pour normaux
			//DBG("Source " + (String)param.Id1 + " f = " + (String)trackVector[param.Id1]->getFrequency() + " v = " + (String)trackVector[param.Id1]->getAmplitude());
			break;
		case Miam::AsyncParamChange::ParamType::Volume :
			if (param.DoubleValue == 0)
				--Nsources;
			else
			{
				//DBG("Volume" + (String)param.Id1 + " a " + (String)param.DoubleValue);
				if (param.Id1 > (int)trackVector.size()-1)
				{
					//DBG("Stop !!!");
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
			if (param.Id1 == 255)
				changeState(Stop);
			else
				trackVector[param.Id1]->changeState(TransportState::Stopped);
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			sourceControled.push_back(param.Id2);
			trackVector[param.Id2]->changeState(TransportState::Starting);
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
		{
			DBG("ddd == true");
			ad->setDuration(param.DoubleValue);
			if (ad->isStopped())
				ad->changeState(Starting);
		}
		
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
			//playAllSources();
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

void AudioManager::verifyAllSource()
{
	for (int i = 0; i < Nsources; ++i)
		if (trackVector[i]->isStopped())
		{
			Miam::AsyncParamChange param;
			param.Type = Miam::AsyncParamChange::ParamType::Activate;
			param.Id1 = i;
			model->SendParamChange(param);
			//trackVector[i]->changeState(Starting);
		}
}

void AudioManager::playAllSources()
{
	for (int i = 0; i < Nsources; ++i)
		trackVector[sourceControled[i]]->changeState(TransportState::Starting);
}

void AudioManager::playAllControledSources()
{
	for (int i = 0; i < (int)sourceControled.size(); ++i)
		trackVector[i]->changeState(TransportState::Starting);
}

void AudioManager::stopAllSources()
{
	for (int i = 0; i < Nsources; ++i)
		trackVector[i]->changeState(TransportState::Stopping);
}

void AudioManager::HandleEvent()
{
	//DBG("handleEvent");
	Miam::AsyncParamChange param;
	param.Id1 = 0;
	param.Type = Miam::AsyncParamChange::ParamType::Activate;
	model->SendParamChange(param);
}

AudioDeviceManager& AudioManager::getAudioDeviceManager()
{
	return *this;
}