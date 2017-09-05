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
	

	//initialise(0, 2, nullptr, true);
	//addAudioCallback(this);
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
		playHeads[i] = 0;
		playHeadsKnown[i] = 0;
	}
}

AudioManager::~AudioManager()
{
	if(midiOuput != nullptr)
		for(int i=1; i< 17; i++)
			midiOuput->sendMessageNow(MidiMessage::allNotesOff(i));
	DBG("audioManager destructor");
	
	runThread = false;
	T.join();
	setSource(nullptr);
	
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

	
	model->removeDeviceManagerFromOptionWindow();
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
	
	periode = metronome.BPMtoPeriodInSample(50);//timeToSample(4000);
	position = 0;

	/*AudioDeviceSetup currentAudioSetup;
	this->getAudioDeviceSetup(currentAudioSetup);

	DBG("default midi output : " + (String)this->getDefaultMidiOutputName());
	midiOuput = this->getDefaultMidiOutput();*/

	midiOuput = model->getMidiOutput();
	
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
	for (int j = 0; j < maxSize; j++)
	{
		if (timeLinesKnown[j] != 0)
			timeLinesKnown[j] = 0;

	}

	/*runThread = false;
	T.join();*/
	
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
	//DBG(String(position));
	getParameters();
	getNewTimeLines();
	getNewPlayHeads();
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
					if (playHeadsKnown[j] != 0)
						playHeadsKnown[j]->process();


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
		if(ptr != 0)
			timeLinesKnown[ptr->getId()] = ptr;
	}
}

void AudioManager::getNewPlayHeads()
{
	PlayHead* ptr;
	while (playHeadsToAudio.pop(ptr))
	{
		if (ptr != 0)
			playHeadsKnown[ptr->getId()] = ptr;
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
	

	

	for (int j = 0; j < maxSize; j++)
	{
		if (playHeadsKnown[j] != 0)
		{
			param.Id1 = j;
			param.DoubleValue = playHeadsKnown[j]->getReadingPosition();
			model->SendParamChange(param);
		}
	}
	if (param.Id1 != 0)
		DBG("2eme tete de lecture");
	//param.Id1 = 0;
	//param.DoubleValue = (double)position / (double)periode; //+ 1.0/8.0;
	//model->SendParamChange(param);
	
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
			if (param.Id2 == 1024) // crée ou supprime une timeLine
			{
				switch (param.IntegerValue)
				{
				case 1:

					paramToAllocationThread.push(param);
					break;
				case 0:

					timeLinesKnown[param.Id1] = 0; // so we won't access to the element anymore, we forget it
					paramToAllocationThread.push(param); // we ask to the allocation thread to delete it
					break;
				default:
					DBG("IMPOSSIBLE : 1 to activate, 0 to desactivate");
					break;
				}
			}
			else if (param.Id1 == 1024) // crée ou supprime une tête de lectire
			{
				switch (param.IntegerValue)
				{
				case 1: // création
					if (playHeadsKnown[param.Id2] == 0)
						paramToAllocationThread.push(param);
					else
						playHeadsKnown[param.Id2]->setSpeed(param.DoubleValue);
					break;
				case 0: // suppression
					playHeadsKnown[param.Id2] = 0;
					paramToAllocationThread.push(param);
					break;
				default:
					break;
				}
			}
			else // (crée et) associe une tête de lecture à une timeLine 
			{
				if (timeLinesKnown[param.Id1] != 0) // pour s'assurer que la timeLines à associer existe
				{
					if (playHeadsKnown[param.Id2] == 0) // la tête de lecture n'existe pas encore -> demander création + association
					{
						paramToAllocationThread.push(param);
					}
					else // faire juste l'association car la tête de lectire existe déjà
					{
						playHeadsKnown[param.Id2]->LinkTo(timeLinesKnown[param.Id1]);
						playHeadsKnown[param.Id2]->setSpeed(param.DoubleValue);
					}
				}
				else
					paramToAllocationThread.push(param);
			}
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			if (timeLinesKnown[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLinesKnown[param.Id1]->setMidiTime(param.Id2, roundToInt(param.DoubleValue * (double)periode), param.IntegerValue,param.FloatValue);
			else // si == 0, on ne sait pas s'il n'existe pas ou si le thread est encore en train de le creer -> envoyer au thread pour verifier et faire le necessaire
				paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			DBG("state = Play;");
			state = Play;
			//DBG("send masterVolume midi : " + (String)roundToInt(param.FloatValue*127.0f));
			//sendMidiMessage(juce::MidiMessage::controllerEvent(1,7,roundToInt(param.FloatValue*127.0f)));//juce::MidiMessage::masterVolume(param.FloatValue));
			//sendMidiMessage(juce::MidiMessage::masterVolume(param.FloatValue));
			//param.IntegerValue = metronome.BPMtoPeriodInSample(param.IntegerValue);//timeToSample(param.IntegerValue);
			//paramToAllocationThread.push(param);
			playHeadsKnown[param.Id1]->setSpeed(param.DoubleValue);
			break;
		case Miam::AsyncParamChange::ParamType::Pause :
			DBG("state = Pause;");
			state = Pause;
			paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::ParamType::Stop :
			DBG("state = Stop;");
			state = Stop;
			paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::Update :
			DBG("Updtae received");
			if (playHeadsKnown[param.Id2] != 0 && timeLinesKnown[param.Id1] != 0)
			{
				playHeadsKnown[param.Id2]->LinkTo(timeLinesKnown[param.Id1]);
				playHeadsKnown[param.Id2]->setSpeed(param.DoubleValue);
				playHeadsKnown[param.Id2]->setReadingPosition(param.FloatValue);
			}
			else
				paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::Duration :
			state = Play;
			sendMidiMessage(juce::MidiMessage::controllerEvent(1, 7, roundToInt(param.FloatValue*127.0f)));
			//juce::MidiMessage::masterVolume(param.FloatValue));
			sendMidiMessage(juce::MidiMessage::masterVolume(param.FloatValue));
			param.IntegerValue = metronome.BPMtoPeriodInSample(param.IntegerValue);//timeToSample(param.IntegerValue);
			paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::UdpPort:
			if (timeLinesKnown[param.Id1] != 0)
				timeLinesKnown[param.Id1]->setMidiChannel(param.IntegerValue);
			else
				paramToAllocationThread.push(param);
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
			if (param.Id2 == 1024)
			{
				switch (param.IntegerValue)
				{
				case 0:
					DBG("desactivate source : " + (String)param.Id1);
					--midiSenderSize;
					delete timeLines[param.Id1];
					timeLines[param.Id1] = 0;
					DBG("source : " + (String)param.Id1 + "deleted");
					//midiSenderVector.erase(midiSenderVector.begin() + param.Id1);
					break;
				case 1:
					//DBG("AM : I construct a new polygon with ID : " + (String)param.Id1);
					//midiSenderVector[param.Id1] = std::shared_ptr<TimeLine>(new TimeLine());
					DBG("activate source    : " + (String)param.Id1);
					if (timeLines[param.Id1] == 0)
						timeLines[param.Id1] = new TimeLine();

					timeLines[param.Id1]->setPeriod(periode);
					if (param.FloatValue != 0)
						timeLines[param.Id1]->setSpeed(param.FloatValue);
					else
						timeLines[param.Id1]->playNoteContinuously();

					timeLines[param.Id1]->setAudioManager(this);
					//DBG("midiChannel : " + (String)param.IntegerValue);
					if (param.IntegerValue != 0)
						timeLines[param.Id1]->setMidiChannel(param.IntegerValue);
					//timeLines[param.Id1]->setSpeed(param.FloatValue);
					timeLines[param.Id1]->setId(param.Id1);
					++midiSenderSize;
					timeLinesToAudio.push(timeLines[param.Id1]);
					break;
				default:
					DBG("IMPOSSIBLE");
				}
			}
			else if (param.Id1 == 1024)
			{
				switch (param.IntegerValue)
				{
				case 0:
					delete playHeads[param.Id2];
					playHeads[param.Id2] = 0;
					break;
				case 1:
					if (playHeads[param.Id2] == 0)
						playHeads[param.Id2] = new PlayHead();
					playHeads[param.Id2]->setId(param.Id2);
					playHeads[param.Id2]->setSpeed(param.DoubleValue);
					playHeads[param.Id2]->setAudioManager(this);
					playHeadsToAudio.push(playHeads[param.Id2]);
					break;
				default:
					break;
				}
			}
			else
			{
				playHeads[param.Id2] = new PlayHead(); // first create the PlayHead
				playHeads[param.Id2]->setId(param.Id2);
				playHeads[param.Id2]->LinkTo(timeLines[param.Id1]);
				playHeads[param.Id2]->setSpeed(param.DoubleValue);
				playHeads[param.Id2]->setAudioManager(this);
				playHeadsToAudio.push(playHeads[param.Id2]);
			}
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			//DBG("new note received : " + (String)param.IntegerValue);
			if (timeLines[param.Id1] != 0)
				timeLines[param.Id1]->setMidiTime(param.Id2, roundToInt(param.DoubleValue * (double)periode), param.IntegerValue,param.FloatValue);
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			
			break;
		case Miam::AsyncParamChange::Update:
			DBG("Updtae received");
			if (playHeads[param.Id2] != 0 && timeLines[param.Id1] != 0)
			{
				playHeads[param.Id2]->LinkTo(timeLines[param.Id1]);
				playHeads[param.Id2]->setSpeed(param.DoubleValue);
				playHeads[param.Id2]->setReadingPosition(param.FloatValue);
			}
			break;
		case Miam::AsyncParamChange::Duration:
			for (int i = 0; i < maxSize; ++i)
			{
				if (timeLines[i] != 0)
					timeLines[i]->setPeriod(param.IntegerValue);
			}
			for (int i = 0; i < maxSize; ++i)
			{
				if (playHeads[i] != 0)
					playHeads[i]->setState(PlayHeadState::Play);
			}
			position = round((double)position * (double)param.IntegerValue / (double)periode);
			periode = param.IntegerValue;
			break;
		case Miam::AsyncParamChange::Pause:
			for (int i = 0; i < maxSize; ++i)
			{
				if (playHeads[i] != 0)
					playHeads[i]->setState(PlayHeadState::Pause);
			}
			break;
		case Miam::AsyncParamChange::Stop:
			for (int i = 0; i < maxSize; ++i)
			{
				if (playHeads[i] != 0)
					playHeads[i]->setState(PlayHeadState::Stop);
			}
			break;
		case Miam::AsyncParamChange::UdpPort:
			if (timeLines[param.Id1] != 0)
				timeLines[param.Id1]->setMidiChannel(param.IntegerValue);
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
	DBG("delete all the playheads");
	for (int i = 0; i < maxSize; ++i)
	{
		if (playHeads[i] != nullptr)
			delete playHeads[i];
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

//AudioDeviceManager& AudioManager::getAudioDeviceManager()
//{
//	return *this;
//}