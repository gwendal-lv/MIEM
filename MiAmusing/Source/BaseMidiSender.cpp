/*
  ==============================================================================

    BaseMidiSender.cpp
    Created: 18 May 2017 11:13:19am
    Author:  ayup1

  ==============================================================================
*/

#include "BaseMidiSender.h"
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioManager.h"

using namespace Amusing;
using namespace std;

TimeLine::TimeLine()
{
	midiTimesSize = 0;
	midiOfftimesSize = 0;

	
	channel = 1;
	duration = 0.05;//roundToInt( 0.5 * 10000.0);

	continuous = false;
	

	lastNote = 0;
	t0 = 0;
	position = 0;

	offset = 0;

	chordSize = 0;

	//newSound = nullptr;
	filterFrequencyToReach = 400.0;
	currentFilterFrequency = 400.0;
	deltaF = 0;
	filterActive = false;
}

TimeLine::~TimeLine()
{

}

void TimeLine::setAudioManager(AudioManager* m_audioManager)
{
	audioManager = m_audioManager;
	midiCollector.reset(audioManager->getCurrentSampleRate());
	swappableSynth.setCurrentPlaybackSampleRate(audioManager->getCurrentSampleRate());
	swappableSynth.setBuffersSize(2, audioManager->getCurrentSamplesBlock());
	//synth.setCurrentPlaybackSampleRate(audioManager->getCurrentSampleRate());
	for (int i = 0; i < 16; i++)
	{
		//synth.addVoice(new SamplerVoice);
		swappableSynth.addVoice();//new SamplerVoice);
	}
	duplicatedFilter.prepare({ audioManager->getCurrentSampleRate(),(uint32)audioManager->getCurrentSamplesBlock(),2 });
	duplicatedFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
	duplicatedFilter.state->setCutOffFrequency(audioManager->getCurrentSampleRate(), 20000);
}

void TimeLine::setMidiTime(int idx, double newTime, int m_noteNumber,float m_velocity)
{
	while (newTime >= 1.0)
		newTime -= 1.0;
	if (idx < maxSize)
	{
		//DBG("<");
		if (idx < midiTimesSize) 
		{
			midiTimes[idx] = newTime;
			if (newTime + duration > 1.0) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = newTime + duration - 1.0;
			else
				midiOffTimes[idx] = newTime + duration;
			notes[idx] = m_noteNumber;
			velocity[idx] = (int)m_velocity;
		}
		else
		{
			//DBG(">");
			for (int i = 0; i < idx - midiTimesSize - 1; ++i)
			{
				midiTimes[midiTimesSize + i] = 0;
				midiOffTimes[midiTimesSize + i] = 0;
				notes[midiTimesSize + i] = 0;
				velocity[midiTimesSize + i] = 0;
				++midiTimesSize;
				++midiOfftimesSize;
			}
			midiTimes[idx] = newTime;
			if (newTime + duration > 1.0) // verif si on depasse pas le temps de la periode !
				midiOffTimes[idx] = (newTime + duration - 1.0);
			else
				midiOffTimes[idx] = (newTime + duration);
			notes[idx] = m_noteNumber;
			velocity[idx] = (int)m_velocity;
			++midiTimesSize;
			++midiOfftimesSize;
		}
		//DBG("create note : " + (String)notes[idx]);
		//DBG("BMS : number of corners is now : " + (String)midiTimesSize);
	}
	testMidi();
}

void TimeLine::setMidiChannel(int m_chan)
{
	channel = m_chan;
}

void TimeLine::setId(int m_Id)
{
	Id = m_Id;
}

void TimeLine::setAllVelocities(float m_velocity)
{
	for (int i = 0; i < midiTimesSize; ++i)
	{
		velocity[i] = m_velocity;
	}
}

int TimeLine::getId()
{
	return Id;
}

bool TimeLine::isNoteOnTime(int m_position, int i, int period, bool &m_end, int &m_channel, int &m_note, uint8 &m_velocity)
{
	int numPassage(0), blop(26);
	while (m_position >= period)
	{
		m_position -= period;
		numPassage++;
		if (numPassage >= 50)
			blop = 12;
		else if (numPassage >= 30)
			blop = 22;
		else if (numPassage >= 10)
			blop = 24;
	}
	if (i < midiTimesSize)
	{
		m_end = false; // on a pas encore atteint la fin de la liste de notes (au cas où il y en a plusieurs à jouer au même moment)
		if (m_position == roundToInt(midiTimes[i] * period))
		{
			if (velocity[i] != 0)
			{
				m_channel = channel;
				m_note = notes[i];
				m_velocity = (uint8)velocity[i];
				return true;
			}
			else // si velocité == 0, pas besoin d'envoyer de note vu qu'on l'entend pas...
				return false;


		}
		else
			return false;

	}
	else
	{
		m_end = true;
		return false;
	}
	
}

bool TimeLine::isNoteOffTime(int m_position, int i, int period, bool &m_end, int &m_channel, int &m_note)
{
	if (i < midiOfftimesSize)
	{
		m_end = false;
		if (m_position == roundToInt(midiOffTimes[i] * period))
		{

			if (velocity[i] != 0)
			{
				m_channel = channel;
				m_note = notes[i];
				return true;
			}
			else // si velocité == 0, pas besoin d'envoyer de note vu qu'on l'entend pas...
				return false;
		}
		else
			return false;

	}
	else
	{
		m_end = true;
		return false;
	}
}

bool TimeLine::isChordOnTime(int m_position, int period, int &m_channel, int *m_chordToPlay, uint8 &m_velocity)
{
	int num = 0;
	for (int i = 0; i < chordSize; ++i)
	{
		if (chordTimesOn[i] * period == m_position)
		{
			m_chordToPlay[num] = chordNotesOn[i];
			num++;
		}
	}

	if (num != 0)
	{
		m_channel = channel;
		m_velocity = velocity[0];
		return true;
	}
	else
		return false;
}

bool TimeLine::isChordOffTime(int m_position, int period, int &m_channel, int m_chordToPlay[])
{
	int num = 0;
	for (int i = 0; i < chordSize; ++i)
	{
		if (chordTimesOff[i] * period == m_position)
		{
			m_chordToPlay[num] = chordNotesOff[i];
			num++;
		}
	}

	if (num != 0)
	{
		m_channel = channel;
		return true;
	}
	else
		return false;
}


double TimeLine::getRelativePosition()
{
	return ( (double)position);
}


void TimeLine::addChord(TimeLine * otherTimeLine, double chordTime)
{
	int currentNote = 0;

	// recherche si on jouait déjà une note à l'instant chordTime
	for (int i = 0; i < midiTimesSize; i++)
	{
		if (midiTimes[i] == chordTime)
		{
			currentNote = notes[i];
			break;
		}
	}
	
	if (currentNote != 0)// si une note était jouée à cette instant elle devient une des notes de l'accord
	{
		int otherChordNote;
		if (otherTimeLine->isNoteAvailable(ChordType::MajorThird, currentNote, otherChordNote))
			createChord(ChordType::MajorThird, chordTime, currentNote, otherChordNote);
		else if (otherTimeLine->isNoteAvailable(ChordType::MinorThird, currentNote, otherChordNote))
			createChord(ChordType::MinorThird, chordTime, currentNote, otherChordNote);
		else if (otherTimeLine->isNoteAvailable(ChordType::MinorThird, currentNote, otherChordNote))
			createChord(ChordType::AugmentedQuart, chordTime, currentNote, otherChordNote);
		else
			createPerfectChord(chordTime, currentNote);

	} 
	else // sinon on sélectionnera une des autres notes comme base
	{
		/*for (int j = 0; j < midiTimesSize; j++)
		{
			currentNote = notes[j];
			int otherChordNote;
			if (otherTimeLine->isNoteAvailable(ChordType::MajorThird, currentNote, otherChordNote))
			{
				createChord(ChordType::MajorThird, chordTime, currentNote, otherChordNote);
				break;
			}
			else if (otherTimeLine->isNoteAvailable(ChordType::MinorThird, currentNote, otherChordNote))
			{
				createChord(ChordType::MinorThird, chordTime, currentNote, otherChordNote);
				break;
			}
			else if (otherTimeLine->isNoteAvailable(ChordType::MinorThird, currentNote, otherChordNote))
			{
				createChord(ChordType::AugmentedQuart, chordTime, currentNote, otherChordNote);
				break;
			}
			else
				currentNote = 0;
		}*/
	}
	//if(currentNote == 0) // aucune note n'était jouée et aucun accord trouvé entre les 2 aires --> accord parfait avec juste la note comme base

}

bool TimeLine::isNoteAvailable(ChordType m_chordType, int baseNote1, int &otherChordNote)
{
	int difference = 13; // impossible
	int baseNote2 = 0;
	switch (m_chordType)
	{
	case MajorThird:
		for (int i = 0; i < midiTimesSize; i++)
		{
			difference = abs(notes[i] - baseNote1);
			if (difference > 11)
			{
				// baseNote2 = octaveNotes1 + position note dans son octave
				baseNote2 = (baseNote1 / 12) + notes[i] % 12;
				difference = abs(notes[i] - baseNote1);
			}
			else
				baseNote2 = notes[i];

			if (difference != 1 || difference != 2 || difference != 6 || difference != 10 || difference != 11)
			{
				otherChordNote = baseNote2;
				return true;
			}
			else
				return false;
		}
		break;
	case MinorThird:

		break;
	case AugmentedQuart:
		break;
	case PerfectChord:
		break;
	default:
		break;
	}
	return false;
}

void TimeLine::createChord(ChordType m_chordType, double m_chordTime, int baseNote1, int baseNote2)
{
	int difference = 13;
	int baseNote3 = 0;
	switch (m_chordType)
	{
	case MajorThird:
		difference = abs(baseNote2 - baseNote1);
		switch (difference)
		{
		case 3 :
			baseNote3 = baseNote1 < baseNote2 ? baseNote1 - 4 : baseNote2 - 4;
			break;
		case 4 :
			baseNote3 = baseNote1 < baseNote2 ? baseNote1 + 7 : baseNote2 + 7;
			break;
		case 5 :
			baseNote3 = baseNote1 < baseNote2 ? baseNote1 -3 : baseNote2 -3;
			break;
		case 7 :
			baseNote3 = baseNote1 > baseNote2 ? baseNote1 - 3 : baseNote2 - 3;
			break;
		case 8 :
			baseNote3 = baseNote1 > baseNote2 ? baseNote1 - 5 : baseNote2 - 5;
			break;
		case 9 :
			baseNote3 = baseNote1 > baseNote2 ? baseNote1 - 4 : baseNote2 - 4;
			break;
		case 0 :
			baseNote2 = baseNote1 + 4;
			baseNote3 = baseNote1 + 7;
		default:
			break;
		}
		break;
	case MinorThird:
		break;
	case AugmentedQuart:
		break;
	case PerfectChord:
		break;
	default:
		break;
	}
	chordTimesOn[chordSize] = m_chordTime;
	chordTimesOn[chordSize+1] = m_chordTime + 1;
	chordTimesOn[chordSize+2] = m_chordTime + 2;
	chordNotesOn[chordSize] = baseNote1;
	chordNotesOn[chordSize + 1] = baseNote2;
	chordNotesOn[chordSize + 2] = baseNote3;
	chordTimesOff[chordSize] = (m_chordTime + duration);
	chordTimesOff[chordSize + 1] = (m_chordTime + 1 + duration);
	chordTimesOff[chordSize + 2] = (m_chordTime + 2 + duration);
	chordNotesOff[chordSize] = baseNote1;
	chordNotesOff[chordSize + 1] = baseNote2;
	chordNotesOff[chordSize + 2] = baseNote3;
	chordSize += 3;
}

void TimeLine::createPerfectChord(double m_chordTime, int baseNote1)
{
	int baseNote2 = baseNote1 + 4;
	int baseNote3 = baseNote1 + 7;
	chordTimesOn[chordSize] = m_chordTime;
	chordTimesOn[chordSize + 1] = m_chordTime + 1;
	chordTimesOn[chordSize + 2] = m_chordTime + 2;
	chordNotesOn[chordSize] = baseNote1;
	chordNotesOn[chordSize + 1] = baseNote2;
	chordNotesOn[chordSize + 2] = baseNote3;
	chordTimesOff[chordSize] = (m_chordTime + duration);
	chordTimesOff[chordSize + 1] = (m_chordTime + 1 + duration);
	chordTimesOff[chordSize + 2] = (m_chordTime + 2 + duration);
	chordNotesOff[chordSize] = baseNote1;
	chordNotesOff[chordSize + 1] = baseNote2;
	chordNotesOff[chordSize + 2] = baseNote3;
	chordSize += 3;
}

void TimeLine::resetAllChords()
{
	for (int i = 0; i < chordSize; ++i)
	{
		chordTimesOn[i] = 0;
		
		chordNotesOn[i] = 0;
		
		chordTimesOff[i] = 0;
		
		chordNotesOff[i] = 0;
				
	}
	chordSize = 0;
}

void TimeLine::setFilterFrequency(double frequency)
{
	if (frequency != filterFrequencyToReach)
	{
		//if (frequency < 50.0) // < 50Hz, on garde la frequence de cassure a 50Hz
		//{
		//	//filter->setCoefficients(IIRCoefficients::makeLowPass(audioManager->getCurrentSampleRate(), 50.0));
		//	filterFrequencyToReach = 50.0;
		//	filterType = FilterType::LowPass;
		//	filterActive = true;
		//}
		//else if (frequency > 15000.0) // > 15kHz, on garde la frequence de cassure a 15kHz
		//{
		//	filterFrequencyToReach = 15000.0;
		//	filterType = FilterType::HighPass;
		//	filterActive = true;
		//	//filter->setCoefficients(IIRCoefficients::makeHighPass(audioManager->getCurrentSampleRate(), 15000.0));
		//}
		//else if (frequency > 200 && frequency < 2000) // frequence au milieu -> pas de filtre
		//{
		//	filterActive = false;//filter->makeInactive();
		//}
		//else if (frequency <= 200)
		//{
		//	filterFrequencyToReach = frequency;
		//	filterType = FilterType::LowPass;
		//	filterActive = true;
		//	//filter->setCoefficients(IIRCoefficients::makeLowPass(audioManager->getCurrentSampleRate(), frequency));
		//}
		//else if (frequency >= 2000)
		//{
		//	filterFrequencyToReach = frequency;
		//	filterType = FilterType::HighPass;
		//	filterActive = true;
		//	//filter->setCoefficients(IIRCoefficients::makeHighPass(audioManager->getCurrentSampleRate(), frequency));
		//}
		//else
		//	DBG("probleme si aucun des cas du dessus");
		filterFrequencyToReach = frequency;//50.0;
		filterType = FilterType::LowPass;
		filterActive = true;
	}
	deltaF = (filterFrequencyToReach - currentFilterFrequency) / 1.0; // il faudra 5 buffer avant d'arriver à la frequence desiree
}

void TimeLine::testMidi()
{
	/*for (int i = 0; i < midiTimesSize; i++)
	{
		if (midiTimes[i] > currentPeriod)
			DBG("connard");
	}*/
}

void TimeLine::updateFilter()
{
	if (currentFilterFrequency != filterFrequencyToReach)
	{
		//ScopedLock audioLock(audioCallbackLock);
		if (filterFrequencyToReach - currentFilterFrequency < deltaF)
			currentFilterFrequency = filterFrequencyToReach;
		else
			currentFilterFrequency += deltaF;

		//switch (filterType)
		//{
		//case LowPass:
		//	duplicatedFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
		//	duplicatedFilter.state->setCutOffFrequency(audioManager->getCurrentSampleRate(), currentFilterFrequency);
		//	break;
		//case HighPass:
		//	duplicatedFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;//filterDSP.parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
		//	duplicatedFilter.state->setCutOffFrequency(audioManager->getCurrentSampleRate(), (float)currentFilterFrequency);//filterDSP.parameters->setCutOffFrequency(audioManager->getCurrentSampleRate(), currentFilterFrequency);
		//	break;
		//default:
		//	DBG("prob");
		//	break;
		//}
		duplicatedFilter.state->setCutOffFrequency(audioManager->getCurrentSampleRate(), (float)currentFilterFrequency);
	}
}

void TimeLine::addMessageToQueue(MidiMessage msg)
{
	midiCollector.addMessageToQueue(msg);
}

void TimeLine::removeNextBlockOfMessages(MidiBuffer & incomingMidi, int numSamples)
{
	midiCollector.removeNextBlockOfMessages(incomingMidi, numSamples);
	
}

void TimeLine::renderNextBlock(AudioSampleBuffer & outputAudio, const MidiBuffer & incomingMidi, int startSample, int numSamples)
{
	updateFilter();
	//synth.renderNextBlock(outputAudio, incomingMidi, startSample, numSamples);
	swappableSynth.renderNextBlock(outputAudio, incomingMidi, startSample, numSamples);
	dsp::AudioBlock<float> block(outputAudio,
		(size_t)startSample);
	if(filterActive)
		duplicatedFilter.process(dsp::ProcessContextReplacing<float>(block)); //filter->processSamples(outputAudio.getWritePointer(chan,startSample), numSamples);
}

void TimeLine::clearSounds()
{
	swappableSynth.clearSounds();
}


void TimeLine::addSound(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData)
{
	//BigInteger allNotes;
	//WavAudioFormat wavFormat;
	//audioReader = wavFormat.createReaderFor(new MemoryInputStream(srcData,
	//	srcDataSize,
	//	keepInternalCopyOfData),
	//	true);
	//allNotes.setRange(0, 128, true);
	//if (newSound != nullptr)
	//	delete newSound; // faudra mettre dans 2 synthé différents 
	//newSound = new SamplerSound("demo sound",
	//	*audioReader,
	//	allNotes,
	//	74,   // root midi note
	//	0.1,  // attack time
	//	0.1,  // release time
	//	10.0  // maximum sample length
	//);
	//synth.addSound(newSound);
	swappableSynth.setSound(srcData, srcDataSize, keepInternalCopyOfData);
}

void TimeLine::addSound(String soundPath)
{
	swappableSynth.setSound(soundPath);
}
