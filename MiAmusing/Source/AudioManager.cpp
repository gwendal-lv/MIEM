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
	runThread = true;
	T = std::thread(&AudioManager::threadFunc, this);
	midiSenderSize = 0;
	midiSenderVector.reserve(128);
	midiSenderVector = std::vector<std::shared_ptr<TimeLine>>(128, std::shared_ptr<TimeLine>(new TimeLine));

	for (int i = 0; i < maxSize; i++)
	{
		timeLines[i] = 0;
		timeLinesKnown[i] = 0;
	}
}

AudioManager::~AudioManager()
{
	DBG("audioManager destructor");
	//shutdownAudio();
	runThread = false;
	T.join();
	setSource(nullptr);
	removeAudioCallback(this);
	closeAudioDevice();
	DBG("audioManager destructor fin");

	//DBG("AudioManager::releaseResources");
	delete mixer;
	//DBG("AudioManager::releaseResources fin");
	if (midiOuput == nullptr)
	{
		DBG("midiOuput == nullptr !!!!!!!!!!!");
	}
	else
		DBG("still exist");

	//delete midiOuput;
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
	
	periode = metronome.timeToSample(4000);
	position = 0;

	AudioDeviceSetup currentAudioSetup;
	this->getAudioDeviceSetup(currentAudioSetup);

	DBG("default midi output : " + (String)this->getDefaultMidiOutputName());
	midiOuput = this->getDefaultMidiOutput();
	
	
	/*midiSender = std::shared_ptr<TimeLine>(new TimeLine(periode));
	if (midiOuput != nullptr)
	{
		midiSender->setAudioManager(this);
		midiSender->setMidiTime(0, roundToInt(0 * periode / 8));
		midiSender->setMidiTime(1, roundToInt(2 * periode / 8));
		midiSender->setMidiTime(2, roundToInt(4 * periode / 8));
		midiSender->setMidiTime(3, roundToInt(6 * periode / 8));
	}*/
	
}
void AudioManager::releaseResources()
{
	DBG("AudioManager::releaseResources");
	if (midiOuput == nullptr)
	{
		DBG("midiOuput == nullptr !!!!!!!!!!!");
	}
	else
	{
		//MidiOutput::
		//delete midiOuput;
		if (midiOuput == nullptr)
			DBG("bien detruit ! ");
		else
			DBG("pas detruit");
	}
}
void AudioManager::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	getParameters();
	getNewTimeLines();
	switch (state)
	{
	case Amusing::Play:
		sendPosition();
		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{

			if (midiOuput != nullptr)
			{
				//midiSender->process(position);
				for (int j = 0; j < maxSize; j++)
				{
					if (timeLinesKnown[j] != 0)
						timeLinesKnown[j]->process(position);


				}
			}
			++position;
			if (position == periode)
			{
				//DBG("TOC");
				position = 0;
			}
		}
		break;
	case Amusing::Pause:
		break;
	case Amusing::Stop:
		position = 0;
		break;
	default:
		break;
	}
	
	bufferToFill.clearActiveBufferRegion();
	//metronome.update();
	//midiBuffer.addEvent(metronome.getNextMidiMsg(), 4);
}

void AudioManager::getNewTimeLines()
{
	TimeLine* ptr;
	while (timeLinesToAudio.pop(ptr))
	{
		timeLinesKnown[ptr->getId()] = ptr;
	}
}

void AudioManager::sendMidiMessage(MidiMessage msg)
{
	if (midiOuput != nullptr)
	{
		midiOuput->sendMessageNow(msg);
	}
}

void AudioManager::sendPosition()
{
	
	AsyncParamChange param;
	param.Type = AsyncParamChange::ParamType::Position;
	param.Id1 = 0;
	param.DoubleValue = (double)position / (double)periode; //+ 1.0/8.0;
	model->SendParamChange(param);
	
}

void AudioManager::getParameters()
{
	//DBG("getParameters");
	Miam::AsyncParamChange param;
	while (model->lookForParameter(param))
	{
		//DBG("paramType = " + (String)((int)param.Type));
		switch (param.Type)
		{
		case Miam::AsyncParamChange::ParamType::Activate:
			
			switch (param.Id2)
			{
			case 1:
				DBG("activate source    : " + (String)param.Id1);
				paramToAllocationThread.push(param);
				break;
			case 0:
				DBG("desactivate source : " + (String)param.Id1);
				timeLinesKnown[param.Id1] = 0; // so we won't access to the element anymore, we forget it
				paramToAllocationThread.push(param); // we ask to the allocation thread to delete it
				break;
			default :
				DBG("IMPOSSIBLE : 1 to activate, 0 to desactivate");
				break;
			}

			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			if (timeLines[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLines[param.Id1]->setMidiTime(param.Id2, roundToInt(param.DoubleValue * (double)periode), param.IntegerValue);
			else // si == 0, on ne sait pas s'il n'existe pas ou si le thread est encore en train de le creer -> envoyer au thread pour verifier et faire le necessaire
				paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			DBG("state = Play;");
			state = Play;
			param.IntegerValue = metronome.timeToSample(param.IntegerValue);
			paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::ParamType::Pause :
			DBG("state = Pause;");
			state = Pause;
			break;
		case Miam::AsyncParamChange::ParamType::Stop :
			DBG("state = Stop;");
			state = Stop;
			break;
		default:
			break;
		}
	}
}

void AudioManager::getAudioThreadMsg()
{
	Miam::AsyncParamChange param;
	while (paramToAllocationThread.pop(param))
	{
		switch (param.Type)
		{
		case Miam::AsyncParamChange::ParamType::Activate :
			//DBG("source : " + (String)param.Id1);
			switch (param.Id2)
			{
			case 0 :
				--midiSenderSize;
				delete timeLines[param.Id1];
				timeLines[param.Id1] = 0;
				//midiSenderVector.erase(midiSenderVector.begin() + param.Id1);
				break;
			case 1 :
				//DBG("AM : I construct a new polygon with ID : " + (String)param.Id1);
				//midiSenderVector[param.Id1] = std::shared_ptr<TimeLine>(new TimeLine());
				if(timeLines[param.Id1] == 0)
					timeLines[param.Id1] = new TimeLine();
				timeLines[param.Id1]->setPeriod(periode);
				timeLines[param.Id1]->setAudioManager(this);
				//DBG("midiChannel : " + (String)param.IntegerValue);
				timeLines[param.Id1]->setMidiChannel(param.IntegerValue);
				timeLines[param.Id1]->setId(param.Id1);
				++midiSenderSize;
				timeLinesToAudio.push(timeLines[param.Id1]);
				break;
			default :
				DBG("IMPOSSIBLE");
			}
			
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			//DBG("new note received : " + (String)param.IntegerValue);
			if (timeLines[param.Id1] != 0)
				timeLines[param.Id1]->setMidiTime(param.Id2, roundToInt(param.DoubleValue * (double)periode), param.IntegerValue);
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			for (int i = 0; i < maxSize; ++i)
			{
				if (timeLines[i] != 0)
					timeLines[i]->setPeriod(param.IntegerValue);
			}
			position = round((double)position * (double)param.IntegerValue / (double)periode);
			periode = param.IntegerValue;
			break;
		default:
			break;
		}
	}
}

void AudioManager::threadFunc()
{
	while (runThread)
	{ 
		getAudioThreadMsg();
	}
	DBG("delete all the timeLines");
	for (int i = 0; i < maxSize; ++i)
	{
		if (timeLines[i] != nullptr)
			delete timeLines[i];
	}
	
	DBG("exit thread");
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