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
	AudioSource(), signal(m_signal), state(Silence),position(0), currentGain(0), // initialisatio
	attackT(2), attackLvl(1.25), decay(1), sustainT(10), sustainLvl(1.00), release(2)  // parametres de l'ADSR
{
   // si on a pas d'info sur le temps de maintien, il faudra le placer en l'infini 
	// --> il faudra attendre un changeState(Release) venu de l'exterieur pour passer a l'etat suivant
}

ADSRSignal::ADSRSignal(FourierSignal *m_signal, bool m_stopSustain) :
	AudioSource(), signal(m_signal), state(Attack), position(0), currentGain(0), // initialisatio
	attackT(0.1), attackLvl(0.5), decay(0.1), sustainT(0.3), sustainLvl(0.2), release(0.5),  // parametres de l'ADSR
	stopSustain(m_stopSustain)
{
	// si on a pas d'info sur le temps de maintien, il faudra le placer en l'infini 
	// --> il faudra attendre un changeState(Release) venu de l'exterieur pour passer a l'etat suivant
}



ADSRSignal::~ADSRSignal()
{
}

void ADSRSignal::changeState(ADSR_State newState)
{
	
	if (state != newState)
	{
		DBG("state : " + (String)newState);
		state = newState;
		switch (state)
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
	float* const buffer0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	float* const buffer1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

	//int startSampleAttack = 0;
	//int numSampleAttack = 0;
	//double startAttackGain(currentGain), endAttackGain(currentGain);
	//int startSampleDecay = 0;
	//int numSampleDecay = 0;
	//double startDecayGain(currentGain), endDecayGain(currentGain);
	//int startSampleSustain = 0;
	//int numSampleSustain = 0;
	//double startSustainGain(currentGain), endSustainGain(currentGain);
	//int startSampleRelease = 0;
	//int numSampleRelease = 0;
	//double startReleaseGain(currentGain), endReleaseGain(currentGain);
	//DBG("position = " + (String)position);
	for (int i = 0; i < bufferToFill.numSamples; ++i)
	{
		signal->updateSample();
		double sample = signal->getNextSample();
		//DBG("sample = "+ (String)sample);
		

			// ce qui se passe si on est entre deux "deadlines" : switch(state)
			switch (state)
			{
			case Attack:
				//numSampleAttack = i - startSampleAttack;
				currentGain += (attackLvl - 0) / (endAttackP - 0); // fixee avec les parametres ADSR
				//endAttackGain = currentGain;
				if (position == endAttackP)
				{
					//startAttackGain = currentGain; deja fait au debut
					//endAttackGain = attackLvl;
					//startSampleDecay = i;
					changeState(Decay);
				}
				break;
			case Decay:
				//numSampleDecay = i - startSampleDecay;
				currentGain += (sustainLvl - attackLvl) / (endDecayP - endAttackP);
				//endDecayGain = currentGain;
				if (position == endDecayP)
				{
					//startDecayGain = currentGain;
					//endDecayGain = sustainLvl;
					//startSampleSustain = i;
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
						//endSustainGain = currentGain;
						//startSampleRelease = i;
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
	/*for (int i = 0; i < bufferToFill.numSamples; ++i)
	{
		signal->updateSample();
		double sample = signal->getNextSample();
		//DBG("sample = "+ (String)sample);
		buffer0[i] = sample;
		buffer1[i] = sample;
	}*/

	//DBG((String)startSampleAttack + " " + (String)numSampleAttack + " " + (String)startAttackGain + " " + (String)endAttackGain);
	//bufferToFill.buffer->applyGainRamp(startSampleAttack, numSampleAttack, startAttackGain, endAttackGain);
	//bufferToFill.buffer->applyGainRamp(startSampleDecay, numSampleDecay, startDecayGain, endDecayGain);
	//bufferToFill.buffer->applyGainRamp(startSampleSustain, numSampleSustain, startSustainGain, endSustainGain);
	//bufferToFill.buffer->applyGainRamp(startSampleRelease, numSampleRelease, startReleaseGain, endReleaseGain);
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
