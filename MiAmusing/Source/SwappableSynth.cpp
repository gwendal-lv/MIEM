/*
  ==============================================================================

    SwappableSynth.cpp
    Created: 23 Nov 2017 11:40:28am
    Author:  ayup1

  ==============================================================================
*/

#include "SwappableSynth.h"


SwappableSynth::SwappableSynth()
{
	//soundA = nullptr;
	//soundB = nullptr;
	synthAPlaying = false;
	G_Off = 1.0;
	G_On = 0.0;
	audioFormatManager.registerBasicFormats();
}

SwappableSynth::~SwappableSynth()
{
}

void SwappableSynth::setBuffersSize(int numChannels, int numSamples)
{
	rampBuffer_On.setSize(numChannels, numSamples);
	rampBuffer_off.setSize(numChannels, numSamples);
}

void SwappableSynth::setCurrentPlaybackSampleRate(double m_sampleRate)
{
	synthA.setCurrentPlaybackSampleRate(m_sampleRate);
	synthB.setCurrentPlaybackSampleRate(m_sampleRate);

}

void SwappableSynth::addVoice()
{
	synthA.addVoice(new SamplerVoice());
	synthB.addVoice(new SamplerVoice());
}

void SwappableSynth::clearSounds()
{
	/*if(synthA.getNumSounds()>0)
		synthA.clearSounds();
	if(synthB.getNumSounds()>0)
		synthB.clearSounds();*/


	//if (soundA != nullptr)
	//	delete soundA; // faudra mettre dans 2 synthé différents
	//if (soundB != nullptr)
	//	delete soundB;
}


void SwappableSynth::setSound(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData)
{
	state = SwappableSynthState::Loading;
	std::thread (&SwappableSynth::addSoundOnThread, this, srcData, srcDataSize, keepInternalCopyOfData).detach();
	

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
	//synthA.addSound(newSound);
	
}

void SwappableSynth::setSound(String soundPath)
{
	if (soundPath.isEmpty())
		DBG("wtf");
	else
	{
		
		std::thread(&SwappableSynth::addSoundFromExternalFileOnThread, this, soundPath).detach();
	}
}

void SwappableSynth::renderNextBlock(AudioSampleBuffer & outputBuffer, const MidiBuffer & inputBuffer, int startSample, int numSample)
{
	float startGain = 0.0f;
	float endGain = 0.0f;
	switch (state)
	{
	case SwappableSynthState::Loading :
		if (G_Off >= 0)
		{
			if (synthAPlaying)
			{
				synthA.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
				//synthB.renderNextBlock(rampBuffer_On, inputBuffer, 0.0f, 0.0f); // juste test...
			}
			else
			{
				synthB.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
				//synthA.renderNextBlock(rampBuffer_On, inputBuffer, 0.0f, 0.0f); // juste test...
			}
			startGain = G_Off;
			endGain = getNextGainOff(numSample);
			for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
				outputBuffer.addFromWithRamp(ch, startSample, rampBuffer_off.getWritePointer(ch), numSample, startGain, endGain);
		}
		break;
	case SwappableSynthState::Playing :
		if (synthAPlaying)
			synthA.renderNextBlock(outputBuffer, inputBuffer, startSample, numSample);
		else
			synthB.renderNextBlock(outputBuffer, inputBuffer, startSample, numSample);
		break;
	case SwappableSynthState::Swapping :
		if (synthAPlaying)
		{
			synthA.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
			synthB.renderNextBlock(rampBuffer_On, inputBuffer, startSample, numSample);
		}
		else
		{
			synthB.renderNextBlock(rampBuffer_off, inputBuffer, startSample, numSample);
			synthA.renderNextBlock(rampBuffer_On, inputBuffer, startSample, numSample);
		}

		if (G_Off != 0.0) // si le synthé précédent n'est pas encore arrêté, continuer de le diminuer
		{
			startGain = G_Off;
			endGain = getNextGainOff(numSample);
			for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
				outputBuffer.addFromWithRamp(ch, startSample, rampBuffer_off.getWritePointer(ch), numSample, startGain, G_Off);
		}
		
		startGain = G_On;
		endGain = getNextGainOn(numSample);
		for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
			outputBuffer.addFromWithRamp(ch, startSample, rampBuffer_On.getWritePointer(ch), numSample, startGain, G_On);
		
		if (G_On == 1.0 && G_Off == 0)
		{
			G_On = 0.0;
			G_Off = 1.0;
			state = SwappableSynthState::Playing;
			/////////////////////
			//synthMtx.unlock();/// unlock the mutex -> the internal synths are once again accessible
			/////////////////////
			synthAPlaying = !synthAPlaying;
		}

		break;
	default:
		break;
	}
	//synthA.renderNextBlock(outputBuffer, inputBuffer, startSample, numSample);
	
}

bool SwappableSynth::setState(SwappableSynthState newState)
{

	return false;
}

void SwappableSynth::addSoundOnThread(const void * srcData, size_t srcDataSize, bool keepInternalCopyOfData)
{

	if (!synthAPlaying)
	{
		synthA.clearSounds();
	}
	else
		synthB.clearSounds();

	BigInteger allNotes;
	WavAudioFormat wavFormat;
	ScopedPointer<AudioFormatReader> audioReader = wavFormat.createReaderFor(new MemoryInputStream(srcData,
		srcDataSize,
		keepInternalCopyOfData),
		true);
	allNotes.setRange(0, 128, true);
	//if (!synthAPlaying)
	//{
	//	//if(soundA != nullptr)
	//	//	delete soundA;
	//	soundA = new SamplerSound("demo sound",
	//		*audioReader,
	//		allNotes,
	//		74,   // root midi note
	//		0.1,  // attack time
	//		0.1,  // release time
	//		10.0  // maximum sample length
	//	);
	//}
	//else if (synthAPlaying)
	//{
	//	//if(soundB != nullptr)
	//	//	delete soundB;
	//	soundB = new SamplerSound("demo sound",
	//		*audioReader,
	//		allNotes,
	//		74,   // root midi note
	//		0.1,  // attack time
	//		0.1,  // release time
	//		10.0  // maximum sample length
	//	);
	//}
	

	if (!synthAPlaying)
		synthA.addSound(new SamplerSound("demo sound",
			*audioReader,
			allNotes,
			74,   // root midi note
			0.1,  // attack time
			0.1,  // release time
			10.0  // maximum sample length
		));
	else
		synthB.addSound(new SamplerSound("demo sound",
			*audioReader,
			allNotes,
			74,   // root midi note
			0.1,  // attack time
			0.1,  // release time
			10.0  // maximum sample length
		));

	state = SwappableSynthState::Swapping;
	//DBG("thread finished : "+ (String)synthA.getNumSounds() + " " + (String)synthB.getNumSounds());
}

void SwappableSynth::addSoundFromExternalFileOnThread(String soundPath)
{
	///////////////////
	synthMtx.lock();///	Mutex to prevent access to one of the internal synth from loading to playing state
					/////////////////// Mutex is release in the renderNextBlock when reaching the Playing state

	// avoid to load two times the same sample
	//if (synthAPlaying)
	//{
	//	if (soundPath == currentPathA) // A joue et c'est déjà le bon son 
	//		return;
	//	else
	//		currentPathA = soundPath;  // A joue mais ce n'est pas le bon son
	//}
	//else // idem avec B
	//{
	//	if (soundPath == currentPathB)
	//		return;
	//	else
	//		currentPathB = soundPath;
	//}

	

	//if (synthAPlaying)
	//{
	//	currentPathA = soundPath;
	//}
	//else
	//{
	//	currentPathB = soundPath;
	//}

	state = SwappableSynth::Loading;


	// si le nom du fichier est un des fichiers embarqués -> chargement différent
	bool isBinary = false;
	for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
	{
		if (soundPath == BinaryData::namedResourceList[i])
		{
			isBinary = true;
			int dataSize = 0;
			const void * srcData = BinaryData::getNamedResource(BinaryData::namedResourceList[i], dataSize);
			addSoundOnThread(srcData, dataSize, false);
			DBG("thread finished : " + soundPath);
		}
	}

	if (!isBinary)
	{
		if (!synthAPlaying)
		{
			synthA.clearSounds();
		}
		else
			synthB.clearSounds();


		//const File file("C:\\Users\\ayup1\\Documents\\Juce Test Recording 0.wav"); // Downloads\\Bass-Drum-1.wav");
		const File file(soundPath);
		ScopedPointer<AudioFormatReader> audioReader = audioFormatManager.createReaderFor(file);

		BigInteger allNotes;

		allNotes.setRange(0, 128, true);

		if (!synthAPlaying)
		{
			/*soundA = new SamplerSound("demo sound",
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			);*/


			synthA.addSound(new SamplerSound(soundPath,
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			));
		}
		else
		{
			/*soundB = new SamplerSound("demo sound",
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			);*/
			synthB.addSound(new SamplerSound(soundPath,
				*audioReader,
				allNotes,
				74,   // root midi note
				0.1,  // attack time
				0.1,  // release time
				10.0  // maximum sample length
			));
		}

		state = SwappableSynthState::Swapping;

		

		DBG("thread finished : " + soundPath);//(String)synthA.getNumSounds() + " " + (String)synthB.getNumSounds());
	
	}

	/////////////////////
	synthMtx.unlock();/// unlock the mutex -> the internal synths are once again accessible
	/////////////////////
	
}

float SwappableSynth::getNextGainOff(int numSamples)
{
	G_Off += (float)( ((double)numSamples / synthA.getSampleRate()) * (0.0 - 1.0) / ((double)T_Off / 1000.0));
	if (G_Off < 0.0f)
		G_Off = 0.0f;
	return G_Off;
}

float SwappableSynth::getNextGainOn(int numSamples)
{
	G_On += (float)(((double)numSamples / synthA.getSampleRate()) * (1.0 - 0.0) / ((double)T_On / 1000.0));
	if (G_On > 1.0f)
		G_On = 1.0f;
	return G_On;
}
