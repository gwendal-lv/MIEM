/*
  ==============================================================================

    ReadingHead.cpp
    Created: 21 Aug 2017 3:58:39pm
    Author:  ayup1

  ==============================================================================
*/

#include "ReadingHead.h"
#include "AudioManager.h"

PlayHead::PlayHead(Metronome* m_metronome) : speed(1.0), speedToReach(1.0), position(0), state(PlayHeadState::Stop), transitionPosition(0), metronome(m_metronome)
{
	position2 = 0;
	numT = 0;
	currentBeats = 0;
	numOfBeats;
	//periodInSamples = metronome->getPeriodInSamples();
	plus = 0;
	incPlus = 0;
	oldPeriod = 0.0;
}

PlayHead::~PlayHead()
{
}

void PlayHead::setId(int _id)
{
	Id = _id;
	
}

int PlayHead::getId()
{
	return Id;
}

void PlayHead::setAudioManager(AudioManager* m_audioManager)
{
	audioManager = m_audioManager;
	numOfBeats = audioManager->getNumOfBeats();
}

void PlayHead::LinkTo(TimeLine* m_timeLine)
{
	timeLine = m_timeLine;
}

void PlayHead::setSpeed(double m_speed)
{

	rest = 0;
	speedToReach = m_speed;
	speed = m_speed;
	rest = speedToReach - floor(speedToReach);
	double delta = 0.001;
	for (int i = 2; i < 5; ++i)
	{
		if (speedToReach >= 1.0 / (double)i - delta / 2.0 && speedToReach <= 1.0 / (double)i + delta / 2.0)
			numT = i;
	}
	int tmpT = metronome->getCurrentT();
	if (numT != 0)
	{
		while (tmpT > numT)
			tmpT -= numT;
	}

	// plus tient compte du d�calage qu'il y aura par rapport � v = 1
	plus = tmpT * (1.0 / (double)numT) * numOfBeats * metronome->getPeriodInSamples(); //position + 1.0;


	
}

void PlayHead::changeState()
{
	switch (state)
	{
	case PlayHeadState::Play:
		state = PlayHeadState::Pause;
		break;
	case PlayHeadState::Pause:
		state = PlayHeadState::Play;
		break;
	case PlayHeadState::Stop:
		break;
	default:
		break;
	}
}

void PlayHead::setReadingPosition(double p)
{
	if (p > 1)
		p -= 1;
	position =  numOfBeats * metronome->getPeriodInSamples() * p;//(double)currentPeriod * p;
}

double PlayHead::getReadingPosition()
{
	return position / double(numOfBeats * metronome->getPeriodInSamples()); 
}

int PlayHead::getTimeLineId()
{
	if (timeLine != 0)
		return timeLine->getId();
	else
		return 0;
}

void PlayHead::setState(PlayHeadState m_state)
{
	state = m_state;
	switch (m_state)
	{
	case PlayHeadState::Play:
		if (timeLine != 0)
			timeLine->setSynthPlaying(true);
		break;
	case PlayHeadState::Pause:
		break;
	case PlayHeadState::Stop:
		position = 0;
		if (timeLine != 0)
			timeLine->setSynthPlaying(false);
		break;
	default:
		break;
	}
}

void PlayHead::process()
{
	if(oldPeriod == 0.0)
		oldPeriod = metronome->getPeriodInSamples();
	double oldPosition = position * metronome->getPeriodInSamples() / oldPeriod;
	oldPeriod = metronome->getPeriodInSamples();
	
	switch (state)
	{
	case PlayHeadState::Play:
		
		if (speed != speedToReach)
		{
			++transitionPosition; // avance au rythme du metronome (vitesse = 1)
			
			if (speedToReach > speed)
			{
				speed += speedInc;
				//position = speed * (((double)metronome->getCurrentBeat() + 1.0) * (double)periodInSamples - metronome->getNumSamplesToNextBeat());
				plus += incPlus;

				if (transitionPosition >= transitionTime)
				{
					if (speedToReach - 2 * speedInc < speed && speed < speedToReach + 2 * speedInc) // on est assez proche de la vitesse voulue
					{
						transitionPosition = 0;
						speed = speedToReach;
						double delta = 0.001; // on pourrait faire numT = ceil(1.0 / rest), mais avec les arrondi on peut arriver � un mauvais r�sultat (1/3 = 0.33333000 -> numT = 4 !!! donc faux !)
						for (int i = 2; i < 5; ++i)
						{
							if (speedToReach >= 1.0 / (double)i - delta / 2.0 && speedToReach <= 1.0 / (double)i + delta / 2.0)
								numT = i;
						}

						int tmpT = metronome->getCurrentT();
						while (tmpT > numT)
							tmpT -= numT;

						// plus tient compte du d�calage qu'il y aura par rapport � v = 1
						plus = tmpT * (1.0 / (double)numT) * numOfBeats * metronome->getPeriodInSamples(); //position + 1.0;

					}
					else // si on n'est pas encore assez proche de la vitesse � atteindre -> refaire une transition
					{
						transitionPosition = 0;
						transitionTime = numOfBeats * metronome->getPeriodInSamples(); // on ralonge la transition de 1 tour
						speedInc = (speedToReach - speed) / transitionTime;

						// calculer les d�calages de chacune des vitesse par rapport � la t�te de lecture unitaire quand elle recommence la lecture
						double newPlus = 0;
						int newNumT(1), newTmpT(0);

						double delta = 0.001; // on pourrait faire numT = ceil(1.0 / rest), mais avec les arrondi on peut arriver � un mauvais r�sultat (1/3 = 0.33333000 -> numT = 4 !!! donc faux !)
						for (int i = 2; i < 5; ++i)
						{
							if (speedToReach >= 1.0 / (double)i - delta / 2.0 && speedToReach <= 1.0 / (double)i + delta / 2.0)
								newNumT = i;
						}

						newTmpT = metronome->getCurrentT();
						while (newTmpT > newNumT)
							newTmpT -= newNumT;




						newPlus = newTmpT * (1.0 / (double)newNumT) * numOfBeats * metronome->getPeriodInSamples(); //position + 1.0;
						if (newPlus < plus)
							newPlus += numOfBeats * metronome->getPeriodInSamples();

						incPlus = (newPlus - plus) / transitionTime;
					}
				}
				position = speed * (((double)metronome->getCurrentBeat() + 1.0) * (double)metronome->getPeriodInSamples() - metronome->getNumSamplesToNextBeat());
				position += plus;
			}
			else
			{
				position += transitionSpeed;
				if (transitionPosition >= transitionTime)
				{
					transitionPosition = 0;
					speed = speedToReach;
					rest = speedToReach - floor(speedToReach);

					int tmpT = metronome->getCurrentT();
					while (tmpT > numT)
						tmpT -= numT;
					plus = tmpT * (1.0 / (double)numT) * numOfBeats * metronome->getPeriodInSamples();
					//numT = (rest == 0) ? 1 : ceil(1.0 / rest);
				}
			}
			
		}
		else
		{
			if (speed < 1)
			{
				if (metronome->getCurrentBeat() == 0 && metronome->getNumSamplesToNextBeat() == metronome->getPeriodInSamples() && speed == speedToReach)
				{
					if (numT != 0)
					{
						int tmpT = metronome->getCurrentT();
						while (tmpT > numT)
							tmpT -= numT;
						plus = position + 1.0;
						if (tmpT == numT) 
						{
							plus = 0;
						}
					}
				}
			}
			else
			{
				plus = 0;
				
			}

			position = speed * (((double)metronome->getCurrentBeat() + 1.0) * (double)metronome->getPeriodInSamples() - metronome->getNumSamplesToNextBeat());
			position += plus;
		}
		
		

		/*while(position > numOfBeats * metronome->getPeriodInSamples())
			position -= (numOfBeats * metronome->getPeriodInSamples());*/

		
		int sub, up; // interval to test for midi event

		// check if speed is an integer
		//r = speed - ceil(speed);
		//if (r != 0) // not an integer -> find sub and up integer
		//{
		//	sub = (int)ceil(position);
		//	up = (int)ceil(position + speed);
		//}
		//else
		//{
		//	sub = (int)position;
		//	up = (int)position + (int)speed;
		//}

		// peut-�tre v�rifier que oldPosition est plus petite que newPosition?
		sub = (int)ceil(oldPosition);
		up = (int)ceil(position);

		//if (sub > up) // passe dedans si : BPM change -> rien changer
		//{			  // ou si on passe par 0 (entre fin de cycle et nouveau cycle
		//	
		//	test = sub;//up += (numOfBeats * metronome->getPeriodInSamples());*/
		//	sub = up;
		//}
		for (int i = sub; i < up; i++)
		{
			testPosition(i);
		}

		break;
	case PlayHeadState::Pause:
		break;
	case PlayHeadState::Stop:

		break;
	default:
		break;
	}
	
	
}

void PlayHead::testPosition(int P)
{

	int m_channel, m_note;
	uint8 m_velocity;

	bool m_end = false;
	int i = 0;
	while (m_end == false)
	{
		if (timeLine->isNoteOnTime(P, i, numOfBeats * metronome->getPeriodInSamples(), m_end, m_channel, m_note, m_velocity))
		{
			//DBG((String)i + " : " + (String)position + " " + (String)(P / (double)timeLine->getPeriod()));
			MidiMessage midiMsg = MidiMessage::noteOn(m_channel, m_note, m_velocity);
			audioManager->sendMidiMessage(midiMsg,this);
		}
		i++;
		
	}
	

	m_end = false;
	i = 0;
	while (m_end == false)
	{
		if (timeLine->isNoteOffTime(P, i, numOfBeats * metronome->getPeriodInSamples(), m_end, m_channel, m_note))
		{
			MidiMessage midiMsgOff = MidiMessage::noteOff(m_channel, m_note);
			audioManager->sendMidiMessage(midiMsgOff,this);
		}
		i++;
	}

	m_end = false;
	i = 0;
	while (m_end == false)
	{
		if (timeLine->isChordOnTime(P, i, numOfBeats * metronome->getPeriodInSamples(), m_end, m_channel, m_note, m_velocity))
		{
			MidiMessage midiMsg = MidiMessage::noteOn(m_channel, m_note, m_velocity);
			audioManager->sendMidiMessage(midiMsg, this);
		}
		++i;
	}

	m_end = false;
	i = 0;
	while (m_end == false)
	{
		if (timeLine->isChordOffTime(P, i, numOfBeats * metronome->getPeriodInSamples(), m_end, m_channel, m_note))
		{
			MidiMessage midiMsg = MidiMessage::noteOff(m_channel, m_note);
			audioManager->sendMidiMessage(midiMsg, this);
		}
		++i;
	}
}
