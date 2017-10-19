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

	model->sharedAudioDeviceManager->addAudioCallback(&recorder);

	setSource(this);
	runThread = true;
	T = std::thread(&AudioManager::threadFunc, this);

	

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

	
	if (midiOuput == nullptr)
	{
		DBG("midiOuput == nullptr !!!!!!!!!!!");
	}
	else
		DBG("still exist");

	model->sharedAudioDeviceManager->removeAudioCallback(&recorder);
	model->removeDeviceManagerFromOptionWindow();
	//delete midiOuput;
}


void AudioManager::prepareToPlay(int samplesPerBlockExpected, double _sampleRate)
{
	DBG("AudioManager::prepareToPlay");
	currentSamplesPerBlock = samplesPerBlockExpected;
	currentSampleRate = _sampleRate;

	midiCollector.reset(_sampleRate);
	synth.setCurrentPlaybackSampleRate(_sampleRate);
	synth.addVoice(new SamplerVoice);
	
	
	metronome.setAudioParameter(samplesPerBlockExpected, _sampleRate);
	
	periode = metronome.BPMtoPeriodInSample(50);//timeToSample(4000);
	position = 0;

	

	midiOuput = model->getMidiOutput();
	if (midiOuput == nullptr)
		setUsingSampledSound();
	
	
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
			//if (midiOuput != nullptr)
			//{
				//midiSender->process(position);
				for (int j = 0; j < maxSize; j++)
				{
					if (playHeadsKnown[j] != 0)
						playHeadsKnown[j]->process();


				}
			//}
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
	
	if (playInternalSynth)
	{
		MidiBuffer incomingMidi;
		midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples); // recupere les messages MIDI à envoyer du midiCollector dans incomingMidi
		synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples); // le synthe interne transforme ces messages MIDI en samples
	}
	else
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
	if (playInternalSynth)
	{
		double s = (double)timeStamp / currentSampleRate;
		if(s== 0.0)
			s = (double)(timeStamp+1) / currentSampleRate;
		//double ms = s * 1000;
		msg.setTimeStamp(s);
		midiCollector.addMessageToQueue(msg);
	}
	else
	{
		if (midiOuput != nullptr)
		{
			midiOuput->sendMessageNow(msg);
		}
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
			param.Id2 = playHeadsKnown[j]->getTimeLineId();
			param.DoubleValue = playHeadsKnown[j]->getReadingPosition();
			model->SendParamChange(param);
		}
	}

	//param.Id1 = 0;
	//param.DoubleValue = (double)position / (double)periode; //+ 1.0/8.0;
	//model->SendParamChange(param);
	
}

void Amusing::AudioManager::startRecording()
{
	if (recorder.isRecording())
	{
		DBG("already recording");
		std::cout << "already recording" << std::endl;
	}
	else
	{
		playInternalSynth = false;
		const File file(File::getSpecialLocation(File::userDocumentsDirectory)
			.getNonexistentChildFile("Juce Test Recording 0", ".wav"));
		recorder.startRecording(file);
	}
}

void Amusing::AudioManager::setUsingSampledSound()
{
	WavAudioFormat wavFormat;
	audioFormatManager.registerBasicFormats();
	//const File file("C:\\Users\\ayup1\\Documents\\Juce Test Recording 0.wav"); // Downloads\\Bass-Drum-1.wav");
	//ScopedPointer<AudioFormatReader> audioReader = audioFormatManager.createReaderFor(file);

	ScopedPointer<AudioFormatReader> audioReader(wavFormat.createReaderFor(new MemoryInputStream(BinaryData::cello_wav,
	BinaryData::cello_wavSize,
	false),
	true));

	BigInteger allNotes;
	allNotes.setRange(0, 128, true);

	synth.clearSounds();
	synth.addSound(new SamplerSound("demo sound",
		*audioReader,
		allNotes,
		74,   // root midi note
		0.1,  // attack time
		0.1,  // release time
		10.0  // maximum sample length
	));

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
			//DBG("Update received");
			if (playHeadsKnown[param.Id2] != 0 && timeLinesKnown[param.Id1] != 0)
			{
				playHeadsKnown[param.Id2]->LinkTo(timeLinesKnown[param.Id1]);
				playHeadsKnown[param.Id2]->setSpeed(param.FloatValue);//param.DoubleValue);
				playHeadsKnown[param.Id2]->setReadingPosition(param.DoubleValue);//param.FloatValue);
				//playHeadsKnown[param.Id2]->setState(PlayHeadState::Pause);
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
		case Miam::AsyncParamChange::Volume :
			if (timeLinesKnown[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLinesKnown[param.Id1]->setAllVelocities(param.FloatValue);
			else // si == 0, on ne sait pas s'il n'existe pas ou si le thread est encore en train de le creer -> envoyer au thread pour verifier et faire le necessaire
				paramToAllocationThread.push(param);
			break;
		case Miam::AsyncParamChange::Position :
			if (timeLinesKnown[param.Id1] != 0 && timeLinesKnown[param.Id2] != 0)
			{
				if (param.Id1 == param.Id2)
					timeLinesKnown[param.Id1]->resetAllChords();
				else
					timeLinesKnown[param.Id1]->addChord(timeLinesKnown[param.Id2], roundToInt(param.DoubleValue * (double)periode));
			}
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
					
					delete timeLines[param.Id1];
					timeLines[param.Id1] = 0;
					DBG("source : " + (String)param.Id1 + "deleted");
					
					break;
				case 1:
					//DBG("AM : I construct a new polygon with ID : " + (String)param.Id1);
					
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
				playHeads[param.Id2]->setSpeed(param.FloatValue);//param.DoubleValue);
				playHeads[param.Id2]->setReadingPosition(param.DoubleValue);//param.FloatValue);
				//playHeadsKnown[param.Id2]->setState(PlayHeadState::Pause);
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
			position = (int)round((double)position * (double)param.IntegerValue / (double)periode);
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
		case Miam::AsyncParamChange::Volume:
			if (timeLines[param.Id1] != 0) // si != 0 : il existe et on peut le modifier
				timeLines[param.Id1]->setAllVelocities(param.FloatValue);

		case Miam::AsyncParamChange::Position:
			if (timeLines[param.Id1] != 0 && timeLines[param.Id2] != 0)
			{
				if (param.Id1 == param.Id2)
					timeLines[param.Id1]->resetAllChords();
				else
					timeLines[param.Id1]->addChord(timeLines[param.Id2], roundToInt(param.DoubleValue * (double)periode));
			}
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
	for (int j = 0; j < maxSize; j++)
	{
		if (timeLinesKnown[j] != 0)
			timeLinesKnown[j] = 0;

	}
	DBG("exit thread");
}
