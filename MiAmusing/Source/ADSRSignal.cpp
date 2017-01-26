/*
  ==============================================================================

    ADSRSignal.cpp
    Created: 9 Jan 2017 5:34:02pm
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSRSignal.h"

//==============================================================================
ADSRSignal::ADSRSignal(FourierSignal *m_signal) : 
	AmuSignal(m_signal->getAmplitude(), m_signal->getFrequency()), signal(m_signal), ADSR_state(Silence),position(0), currentGain(0), // initialisatio
	attackT(2), attackLvl(1.25), decay(1), sustainT(10), sustainLvl(1.00), release(2)  // parametres de l'ADSR
{
   // si on a pas d'info sur le temps de maintien, il faudra le placer en l'infini 
	// --> il faudra attendre un changeState(Release) venu de l'exterieur pour passer a l'etat suivant
}

ADSRSignal::ADSRSignal(FourierSignal *m_signal, bool m_stopSustain) :
	AmuSignal(m_signal->getAmplitude(), m_signal->getFrequency()), signal(m_signal), ADSR_state(Attack), position(0), currentGain(0), // initialisatio
	attackT(0.1), attackLvl(0.5), decay(0.1), sustainT(0.3), sustainLvl(0.2), release(0.5),  // parametres de l'ADSR
	stopSustain(m_stopSustain)
{
	// si on a pas d'info sur le temps de maintien, il faudra le placer en l'infini 
	// --> il faudra attendre un changeState(Release) venu de l'exterieur pour passer a l'etat suivant
}

ADSRSignal::ADSRSignal(FourierSignal *m_signal, double duration) :
	AmuSignal(m_signal->getAmplitude(), m_signal->getFrequency()), signal(m_signal), ADSR_state(Attack), position(0), currentGain(0), // initialisatio
	attackT(0.1), attackLvl(0.5), decay(0.1), sustainLvl(0.2), release(0.5),  // parametres de l'ADSR
	stopSustain(false)
{
	// duree de la note specifiee
	sustainT = duration - (attackT + decay + release);
}

ADSRSignal::~ADSRSignal()
{
}

void ADSRSignal::changeState(ADSR_State newState)
{
	
	if (ADSR_state != newState)
	{
		DBG("state : " + (String)newState);
		ADSR_state = newState;
		switch (ADSR_state)
		{
		case Attack: // garder la focntion comme ça si on jamais on associe un filtre
			break;
		case Decay:
			break;
		case Sustain:
			break;
		case Release:
			break;
		case Silence : // ne fais rien, ne joue rien
		default:
			break;
		}
	}
}

void ADSRSignal::playNote()
{
	DBG("playNote");
	changeState(Attack);
	position = 0;
}

void ADSRSignal::releaseNote()
{
	changeState(Release);
	position = endSustainP + 1;
}

void ADSRSignal::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	currentSamplesPerBlock = samplesPerBlockExpected;
	currentSampleRate = sampleRate;
	signal->prepareToPlay(samplesPerBlockExpected, sampleRate);
	signal->resetPhase(0);
	endAttackP  = round(attackT  * sampleRate);
	endDecayP   = endAttackP + round(decay    * sampleRate);
	endSustainP = endDecayP + round(sustainT * sampleRate);
	endReleaseP = endSustainP + round(release  * sampleRate);
	DBG((String)endAttackP + " "+ (String)endDecayP + " " + (String)endSustainP + " " + (String)endReleaseP);
}

void ADSRSignal::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
	if (state == Starting || state == Playing || state == Stopping || state == Pausing)
	{
		float* const buffer0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
		float* const buffer1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);


		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{
			signal->updateSample();
			double sample = signal->getNextSample();
			//DBG("sample = "+ (String)sample);


				// ce qui se passe si on est entre deux "deadlines" : switch(state)
			switch (state)
			{
			case Attack:
				currentGain += (attackLvl - 0) / (endAttackP - 0); // fixee avec les parametres ADSR
				if (position == endAttackP)
				{

					changeState(Decay);
				}
				break;
			case Decay:
				//numSampleDecay = i - startSampleDecay;
				currentGain += (sustainLvl - attackLvl) / (endDecayP - endAttackP);
				//endDecayGain = currentGain;
				if (position == endDecayP)
				{
					changeState(Sustain);
				}
				break;
			case Sustain:
				//numSampleSustain = i - startSampleSustain;
				currentGain = sustainLvl;
				//endSustainGain = currentGain;
				if (position == endSustainP)
				{
					//startSustainGain = currentGain;
					if (stopSustain)
					{
						changeState(Release);
					}
					else
					{
						position = endDecayP; // continue de boucler la periode sustain
					}
				}
				break;
			case Release:
				//numSampleRelease = i - startSampleRelease;
				currentGain += (0 - sustainLvl) / (endReleaseP - endSustainP);
				//endReleaseGain = currentGain;
				if (position == endReleaseP)
				{
					//startReleaseGain = currentGain;
					//endReleaseGain = 0;
					changeState(Silence);
				}
				break;
			case Silence:
			default:
				currentGain = 0;

				break;
			}

			buffer0[i] = currentGain * sample;
			buffer1[i] = currentGain * sample;

			++position;
		}
	}
	else if (state == Paused || state == Stopped) // Paused, et Stopped ne doivent rien renvoyer la seule difference est la remise a zero fait par stopped (dans changeState)
	{
		bufferToFill.clearActiveBufferRegion();
	}

	if (state == Pausing) // on recupère ce qui devait etre joue et on diminue le volume, puis passe a l'etat paused
	{
		bufferToFill.buffer->applyGainRamp(bufferToFill.startSample, bufferToFill.numSamples, currentGain, 0);
		changeState(Paused);
	}

}

void ADSRSignal::releaseResources()
{
	//signal->releaseResources();
}

int ADSRSignal::getLength()
{
	if (stopSustain)
		return endReleaseP;
	else
		return 0;
}

void ADSRSignal::setFrequency(double newFrequency)
{
	signal->setFrequency(newFrequency);
}

void ADSRSignal::setAmplitude(double newAmplitude)
{
	signal->setAmplitude(newAmplitude);
}

void ADSRSignal::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;
		switch (newState)
		{
		case Stopped:  // remise a zero

			position = 0;
			break;
		case Starting:
			playNote();
			state = Playing;
			break;
		case Playing:
			break;
		case Pausing:
			//faire une rampe vers 0 pour la pause
			break;
		case Paused:
			break;
		case Stopping:
			releaseNote(); // rampe vers 0
			break;
		default:
			break;
		}
	}
}

void ADSRSignal::setDuration(double newDuration)
{
	if ((attackT + decay + sustainT + release) != newDuration)
	{
		endAttackP = round(attackT  * currentSampleRate);
		endDecayP = endAttackP + round(decay    * currentSampleRate);
		endSustainP = endDecayP + round(sustainT * currentSampleRate);
		endReleaseP = endSustainP + round(release  * currentSampleRate);
		DBG((String)endAttackP + " " + (String)endDecayP + " " + (String)endSustainP + " " + (String)endReleaseP);
	}
}