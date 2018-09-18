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


using namespace Amusing;
//==============================================================================
AudioManager::AudioManager(AmusingModel *m_model) : model(m_model), state(Stop), playInternalSynth(false), timeStamp(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	DBG("AudioManager::AudioManager");

	beatsByTimeLine = 4;

	setSource(this);
	runThread = true;
	//T = std::thread(&AudioManager::threadFunc, this);
	
	metronome = new Metronome();

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
	
	//runThread = false;
	std::thread Tdelete (&AudioManager::clearAudioObjectsOnThread, this);
	Tdelete.join();

	//T.join();
	setSource(nullptr);
	
	DBG("audioManager destructor fin");

	
	if (midiOuput == nullptr)
	{
		DBG("midiOuput == nullptr !!!!!!!!!!!");
	}
	else
		DBG("still exist");

	model->removeDeviceManagerFromOptionWindow();
	//delete midiOuput;
	delete metronome;
}


void AudioManager::prepareToPlay(int samplesPerBlockExpected, double _sampleRate)
{
	DBG("AudioManager::prepareToPlay");
	currentSamplesPerBlock = samplesPerBlockExpected;
	currentSampleRate = _sampleRate;

	/*midiCollector.reset(_sampleRate);
	synth.setCurrentPlaybackSampleRate(_sampleRate);
	int numVoice = 10;
	for (int i = 0; i < numVoice; i++)
	{
		synth.addVoice(new SamplerVoice);
	}*/
	
	metronome->setAudioParameter(_sampleRate,50);
	
	position = 0;

	

	midiOuput = model->getMidiOutput();
	if (midiOuput == nullptr)
		setUsingSampledSound();
	else
	{
		playInternalSynth = false;
		//audioFormatManager.clearFormats();
	}
	
}
void AudioManager::releaseResources()
{
	DBG("AudioManager::releaseResources");
	/*for (int j = 0; j < maxSize; j++)
	{
		if (timeLinesKnown[j] != 0)
			timeLinesKnown[j] = 0;

	}*/

	/*runThread = false;
	T.join();*/

	for (int i = 0; i < maxSize; ++i)
	{
		if (timeLines[i] != nullptr)
			timeLines[i]->clearSounds();
	}
	DBG("all sounds cleared");
	
	if (midiOuput == nullptr)
	{
		DBG("midiOuput == nullptr !!!!!!!!!!!");
	}
	else
	{
		
		if (midiOuput == nullptr)
			DBG("bien detruit ! ");
		else
			DBG("pas detruit");
	}
}
void AudioManager::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	bufferToFill.clearActiveBufferRegion(); // buffer is the same for input and output -> need to clear the buffer before creating the output.
											// otherwise, the input will be added to the output

	getParameters();
	getNewTimeLines();
	getNewPlayHeads();
	switch (state)
	{
	case Amusing::Play:
		sendPosition();
		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{
			timeStamp++;
			
			for (int j = 0; j < maxSize; j++)
			{
				// les t�tes de lecture d�terminent s'il faut envoyer une note Midi
				// celles-ci seront jou�es par le synth� contenu par la timeLine
				if (playHeadsKnown[j] != 0)
					playHeadsKnown[j]->process();
			}
			metronome->update();
			++position;
			if (position == beatsByTimeLine * metronome->getPeriodInSamples())
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
		metronome->reset();
		break;
	default:
		break;
	}
	
	if (playInternalSynth)
	{
		for (int i = 0; i < maxSize; ++i)
		{
			if (timeLinesKnown[i] != 0)
			{
				// les notes Midi devant �tre jou�es sont r�cup�r�es dans le buffer incomingMidi
				MidiBuffer incomingMidi;
				timeLinesKnown[i]->removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);

				// le synth� contenu par la timeLine joue les notes midi qui ont �t� r�cup�r�es
				timeLinesKnown[i]->renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);


			}
		}
	}
	else
		bufferToFill.clearActiveBufferRegion();
	
}

void AudioManager::getNewTimeLines()
{
	TimeLine* ptr;
	ptr = nullptr;
	while (timeLinesToAudio.pop(ptr))
	{
		if(ptr != 0)
			timeLinesKnown[ptr->getId()] = ptr;
	}
}

void AudioManager::getNewPlayHeads()
{
	PlayHead* ptr;
	ptr = nullptr;
	while (playHeadsToAudio.pop(ptr))
	{
		if (ptr != 0)
		{
			playHeadsKnown[ptr->getId()] = ptr;
			playHeadsKnown[ptr->getId()]->setReadingPosition((double)position / (double)(beatsByTimeLine * metronome->getPeriodInSamples()));
		}
	}
}

void AudioManager::sendMidiMessage(MidiMessage msg, PlayHead* sender)
{
	if (playInternalSynth)
	{
		double s = (double)timeStamp / currentSampleRate;
		if(s== 0.0)
			s = (double)(timeStamp+1) / currentSampleRate;
		//double ms = s * 1000;
		msg.setTimeStamp(s);
		//midiCollector.addMessageToQueue(msg);
		if(sender != nullptr && sender != 0)
			if(timeLinesKnown[sender->getTimeLineId()] != 0)
				timeLinesKnown[sender->getTimeLineId()]->addMessageToQueue(msg);
	}
	else
	{
		if (midiOuput != nullptr)
		{
			midiOuput->sendMessageNow(msg);
		}
	}
}

double AudioManager::getCurrentSampleRate()
{
	return currentSampleRate;
}

int AudioManager::getCurrentSamplesBlock()
{
	return currentSamplesPerBlock;
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
			param.Id2 = playHeadsKnown[j]->getTimeLineId();
			param.DoubleValue = playHeadsKnown[j]->getReadingPosition();
			model->SendParamChange(param);
		}
	}

	//param.Id1 = 0;
	//param.DoubleValue = (double)position / (double)periode; //+ 1.0/8.0;
	//model->SendParamChange(param);
	
}

void AudioManager::setUsingSampledSound()
{
	WavAudioFormat wavFormat;
	//audioFormatManager.registerBasicFormats();
	//const File file("C:\\Users\\ayup1\\Documents\\Juce Test Recording 0.wav"); // Downloads\\Bass-Drum-1.wav");
	//ScopedPointer<AudioFormatReader> audioReader = audioFormatManager.createReaderFor(file);
	/*
	ScopedPointer<AudioFormatReader> audioReader(wavFormat.createReaderFor(new MemoryInputStream(BinaryData::cello_wav,
	BinaryData::cello_wavSize,
	false),
	true));

	BigInteger allNotes;
	allNotes.setRange(0, 128, true);
	*/
	//synth.clearSounds();
	//synth.addSound(new SamplerSound("demo sound",
	//	*audioReader,
	//	allNotes,
	//	74,   // root midi note
	//	0.1,  // attack time
	//	0.1,  // release time
	//	10.0  // maximum sample length
	//));

	//auto newSound= new SamplerSound("demo sound",
	//		*audioReader,
	//		allNotes,
	//		74,   // root midi note
	//		0.1,  // attack time
	//		0.1,  // release time
	//		10.0  // maximum sample length
	//	); 
	//for (int i = 0; i < maxSize; ++i)
	//{
	//	if (timeLinesKnown[i] != 0)
	//	{
	//		timeLinesKnown[i]->clearSounds();
	//		timeLinesKnown[i]->addSound(newSound);
	//	}
	//}

	playInternalSynth = true;
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
		case Miam::AsyncParamChange::ParamType::InputsCount:
			// test switch synth
			//timeLines[param.Id1]->addSound(BinaryData::cello_wav, BinaryData::cello_wavSize, false);
			if (timeLinesKnown[param.Id1] != 0)
				timeLinesKnown[param.Id1]->addSound(model->getSoundPath(param.Id2));
			else
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		case Miam::AsyncParamChange::ParamType::Activate:
			if (param.Id2 == 1024) // cr�e ou supprime une timeLine
			{
				switch (param.IntegerValue)
				{
				case 1:

					paramToAllocationThread.push(param);
					std::thread(&AudioManager::threadFunc, this).detach();
					break;
				case 0:

					timeLinesKnown[param.Id1] = 0; // so we won't access to the element anymore, we forget it
					paramToAllocationThread.push(param); // we ask to the allocation thread to delete it
					std::thread(&AudioManager::threadFunc, this).detach();
					break;
				default:
					DBG("IMPOSSIBLE : 1 to activate, 0 to desactivate");
					break;
				}
			}
			else if (param.Id1 == 1024) // cr�e ou supprime une t�te de lectire
			{
				switch (param.IntegerValue)
				{
				case 1: // cr�ation
					if (playHeadsKnown[param.Id2] == 0)
					{
						paramToAllocationThread.push(param);
						std::thread(&AudioManager::threadFunc, this).detach();
					}
					else
						playHeadsKnown[param.Id2]->setSpeed(param.DoubleValue);
					break;
				case 0: // suppression
					playHeadsKnown[param.Id2] = 0;
					paramToAllocationThread.push(param);
					std::thread(&AudioManager::threadFunc, this).detach();
					break;
				default:
					break;
				}
			}
			else // (cr�e et) associe une t�te de lecture � une timeLine 
			{
				if (timeLinesKnown[param.Id1] != 0) // pour s'assurer que la timeLines � associer existe
				{
					if (playHeadsKnown[param.Id2] == 0) // la t�te de lecture n'existe pas encore -> demander cr�ation + association
					{
						paramToAllocationThread.push(param);
						std::thread(&AudioManager::threadFunc, this).detach();
					}
					else // faire juste l'association car la t�te de lectire existe d�j�
					{
						playHeadsKnown[param.Id2]->LinkTo(timeLinesKnown[param.Id1]);
						playHeadsKnown[param.Id2]->setSpeed(param.DoubleValue);
					}
				}
				else
				{
					paramToAllocationThread.push(param);
					std::thread(&AudioManager::threadFunc, this).detach();
				}
			}
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			if (timeLinesKnown[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLinesKnown[param.Id1]->setMidiTime(param.Id2, param.DoubleValue, param.IntegerValue,param.FloatValue);
			else // si == 0, on ne sait pas s'il n'existe pas ou si le thread est encore en train de le creer -> envoyer au thread pour verifier et faire le necessaire
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			DBG("state = Play;");
			//state = Play;
			if (playHeadsKnown[param.Id1] != 0)
			{
				playHeadsKnown[param.Id1]->setSpeed(param.DoubleValue);
				if (param.IntegerValue == 1 && state == Play)
				{

					playHeadsKnown[param.Id1]->changeState();
				}
			}
			else
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		case Miam::AsyncParamChange::ParamType::Pause :
			DBG("state = Pause;");
			state = Pause;
			paramToAllocationThread.push(param);
			std::thread(&AudioManager::threadFunc, this).detach();
			break;
		case Miam::AsyncParamChange::ParamType::Stop :
			DBG("state = Stop;");
			state = Stop;
			paramToAllocationThread.push(param);
			std::thread(&AudioManager::threadFunc, this).detach();
			break;
		case Miam::AsyncParamChange::Update :
			//DBG("Update received");
			if (playHeadsKnown[param.Id2] != 0 && timeLinesKnown[param.Id1] != 0)
			{
				playHeadsKnown[param.Id2]->LinkTo(timeLinesKnown[param.Id1]);
				playHeadsKnown[param.Id2]->setSpeed(param.FloatValue);//param.DoubleValue);
				playHeadsKnown[param.Id2]->setReadingPosition(param.DoubleValue);//param.FloatValue);
				//playHeadsKnown[param.Id2]->setState(PlayHeadState::Pause);
			}
			else
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		case Miam::AsyncParamChange::Duration :
			state = Play;
			//sendMidiMessage(juce::MidiMessage::controllerEvent(1, 7, roundToInt(param.FloatValue*127.0f)),nullptr);
			//juce::MidiMessage::masterVolume(param.FloatValue));
			//sendMidiMessage(juce::MidiMessage::masterVolume(param.FloatValue),nullptr);
			paramToAllocationThread.push(param);
			std::thread(&AudioManager::threadFunc, this).detach();
			break;
		case Miam::AsyncParamChange::UdpPort:
			if (timeLinesKnown[param.Id1] != 0)
				timeLinesKnown[param.Id1]->setMidiChannel(param.IntegerValue);
			else
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		case Miam::AsyncParamChange::Volume :
			if (timeLinesKnown[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLinesKnown[param.Id1]->setAllVelocities(param.FloatValue);
			else // si == 0, on ne sait pas s'il n'existe pas ou si le thread est encore en train de le creer -> envoyer au thread pour verifier et faire le necessaire
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		case Miam::AsyncParamChange::Position :
			if (timeLinesKnown[param.Id1] != 0 && timeLinesKnown[param.Id2] != 0)
			{
				if (param.Id1 == param.Id2)
					timeLinesKnown[param.Id1]->resetAllChords();
				else
					timeLinesKnown[param.Id1]->addChord(timeLinesKnown[param.Id2], param.DoubleValue);
			}
			else
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		case Miam::AsyncParamChange::Frequency :
			if (timeLinesKnown[param.Id1] != 0)
			{
				// verifier si entraine allocation memoire... si oui -> mettre dans thread d'allocation
				timeLinesKnown[param.Id1]->setFilterFrequency(param.DoubleValue);
			}
			else
			{
				paramToAllocationThread.push(param);
				std::thread(&AudioManager::threadFunc, this).detach();
			}
			break;
		default:
			break;
		}
	}
}

void AudioManager::getAudioThreadMsg()
{
	Miam::AsyncParamChange param;

	//BigInteger allNotes;
	//WavAudioFormat wavFormat;
	//ScopedPointer<AudioFormatReader> audioReader(wavFormat.createReaderFor(new MemoryInputStream(BinaryData::cello_wav,
	//	BinaryData::cello_wavSize,
	//	false),
	//	true));
	//allNotes.setRange(0, 128, true);
	//auto newSound = new SamplerSound("demo sound",
	//	*audioReader,
	//	allNotes,
	//	74,   // root midi note
	//	0.1,  // attack time
	//	0.1,  // release time
	//	10.0  // maximum sample length
	//);
	int oldPeriod = 0;

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
					if (timeLines[param.Id1] != 0)
					{
						delete timeLines[param.Id1];
						timeLines[param.Id1] = 0;
					}
					DBG("source : " + (String)param.Id1 + "deleted");
					
					break;
				case 1:
					//DBG("AM : I construct a new polygon with ID : " + (String)param.Id1);
					
					DBG("activate source    : " + (String)param.Id1);
					if (timeLines[param.Id1] == 0)
						timeLines[param.Id1] = new TimeLine();
					else
						DBG("impossible");

					timeLines[param.Id1]->setAudioManager(this);
					//DBG("midiChannel : " + (String)param.IntegerValue);
					/*if (param.IntegerValue != 0)
						timeLines[param.Id1]->setMidiChannel(param.IntegerValue);*/
					//timeLines[param.Id1]->setSpeed(param.FloatValue);
					timeLines[param.Id1]->setId(param.Id1);
					
					// LAAAAAAAAAA
					
					timeLines[param.Id1]->clearSounds();
					//timeLines[param.Id1]->addSound(newSound);
					//timeLines[param.Id1]->addSound(model->getSoundPath(param.Id2));
					timeLines[param.Id1]->addSound(model->getSoundPath((int)param.DoubleValue));//BinaryData::cello_wav,BinaryData::cello_wavSize,false);
					// LAAAAAAAAAAA
					
					timeLinesToAudio.push(timeLines[param.Id1]);
					break;
				default:
					DBG("IMPOSSIBLE");
					break;
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
						playHeads[param.Id2] = new PlayHead(metronome);
					if (state == Play)
						playHeads[param.Id2]->setState(PlayHeadState::Play);
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
				playHeads[param.Id2] = new PlayHead(metronome); // first create the PlayHead
				playHeads[param.Id2]->setId(param.Id2);
				playHeads[param.Id2]->LinkTo(timeLines[param.Id1]);
				playHeads[param.Id2]->setSpeed(param.DoubleValue);
				if (state == Play)
					playHeads[param.Id2]->setState(PlayHeadState::Play);
				playHeads[param.Id2]->setAudioManager(this);
				playHeadsToAudio.push(playHeads[param.Id2]);
			}
			break;
		case Miam::AsyncParamChange::ParamType::Source:
			//DBG("AM : the side " + (String)param.Id2 + " is = " + (String)param.DoubleValue);
			//DBG("new note received : " + (String)param.IntegerValue);
			if (timeLines[param.Id1] != 0)
				timeLines[param.Id1]->setMidiTime(param.Id2, param.DoubleValue, param.IntegerValue,param.FloatValue);
			break;
		case Miam::AsyncParamChange::ParamType::Play :
			if (playHeads[param.Id1] != 0)
			{
				playHeads[param.Id1]->setSpeed(param.DoubleValue);
				if (param.IntegerValue == 1 && state == Play)
				{

					playHeads[param.Id1]->changeState();
				}
			}
			break;
		case Miam::AsyncParamChange::Update:
			DBG("Updtae received");
			if (playHeads[param.Id2] != 0 && timeLines[param.Id1] != 0)
			{
				playHeads[param.Id2]->LinkTo(timeLines[param.Id1]);
				playHeads[param.Id2]->setSpeed(param.FloatValue);//param.DoubleValue);
				playHeads[param.Id2]->setReadingPosition(param.DoubleValue);//param.FloatValue);
				//playHeadsKnown[param.Id2]->setState(PlayHeadState::Pause);
			}
			break;
		case Miam::AsyncParamChange::Duration:
			oldPeriod = metronome->getPeriodInSamples();
			metronome->setAudioParameter(currentSampleRate, param.IntegerValue);
			
			for (int i = 0; i < maxSize; ++i)
			{
				if (playHeads[i] != 0)
					playHeads[i]->setState(PlayHeadState::Play);
			}
			position = (int)round((double)position * (double)(beatsByTimeLine * metronome->getPeriodInSamples()) / (double)(beatsByTimeLine *oldPeriod));
			
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
		case Miam::AsyncParamChange::Volume:
			if (timeLines[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLines[param.Id1]->setAllVelocities(param.FloatValue);

		case Miam::AsyncParamChange::Position:
			if (timeLines[param.Id1] != 0 && timeLines[param.Id2] != 0)
			{
				if (param.Id1 == param.Id2)
					timeLines[param.Id1]->resetAllChords();
				else
					timeLines[param.Id1]->addChord(timeLines[param.Id2], param.DoubleValue);
			}
			break;
		case Miam::AsyncParamChange::ParamType::InputsCount:
			// test switch synth
			//timeLines[param.Id1]->addSound(BinaryData::cello_wav, BinaryData::cello_wavSize, false);
			if (timeLines[param.Id1] != 0)
				timeLines[param.Id1]->addSound(model->getSoundPath(param.Id2));
			else
				DBG("No timeLine " + String(param.Id1) + " created");
			break;
		default:

			break;
		}
	}
	//delete newSound;
}

void AudioManager::threadFunc()
{
	allocationThreadsMutex.lock();
	getAudioThreadMsg();
	allocationThreadsMutex.unlock();
	
}

void AudioManager::clearAudioObjectsOnThread()
{
	allocationThreadsMutex.lock();
	DBG("delete all the timeLines");
	for (int i = 0; i < maxSize; ++i)
	{
		if (timeLines[i] != nullptr)
			timeLines[i]->clearSounds();
		delete timeLines[i];
	}
	DBG("delete all the playheads");
	for (int i = 0; i < maxSize; ++i)
	{
		if (playHeads[i] != nullptr)
			delete playHeads[i];
	}
	for (int j = 0; j < maxSize; j++)
	{
		if (timeLinesKnown[j] != 0)
			timeLinesKnown[j] = 0;

	}
	DBG("exit thread");
	allocationThreadsMutex.unlock();
}
