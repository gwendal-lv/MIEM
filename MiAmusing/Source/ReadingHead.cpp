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
	periodInSamples = metronome->getPeriodInSamples();
	plus = 0;
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
	currentPeriod = m_timeLine->getPeriod() / speed;
}

void PlayHead::setSpeed(double m_speed)
{
	/*
	if (speedToReach != m_speed)
	{
		speedToReach = m_speed;
		transitionTime =  timeLine->getPeriod() / 2.0; // une demi période pour retrouver la bonne position
		double currentPeriodePercentage = (double)position / (double)timeLine->getPeriod(); // pourcentage de la période où l'on se trouve
		double newPeriodePercentage =  currentPeriodePercentage + ((double)speedToReach / (double)speed) * 0.5; // pourcentage de la période où on devrait se trouver + demi-tour (car on prend un temps de transition T/2)
		//speed = (newPeriodePercentage - currentPeriodePercentage) * (double)timeLine->getPeriod() / (double)transitionTime; // transition speed
		
		speed = (speedToReach * (double)((((double)position + numT * (double)timeLine->getPeriod())/speed) + transitionTime) - (double)position) / (double)transitionTime;
		transitionTime *= speed; // pour garder le nombre de "click"
		// numT /= speedToReach; // trouver la transformation de numbre de tour

		if(timeLine != nullptr)
			currentPeriod = timeLine->getPeriod() / speed; // peut poser problème...
	}
	*/
	//timeLine->setSpeed(m_speed);
	rest = 0;
	if (speedToReach != m_speed)
	{
		switch (state)
		{
			case PlayHeadState::Play :
			case PlayHeadState::Pause:
				
				if (m_speed >= speed) // speedToReach >= speed en fait
				{
					// transition time = temps avant prochain passages par 0, pour pas avoir de problème de continuité de la position
					transitionTime = ((numOfBeats - metronome->getCurrentBeat() - 1) * periodInSamples + metronome->getNumSamplesToNextBeat());//- speedToReach; //(double)numOfBeats * (double)periodInSamples / 2.0;
					speedToReach = m_speed;
					rest = speedToReach - floor(speedToReach);
					
					numT = (rest == 0) ? 1 : ceil(1.0 / rest);
					speedInc = (speedToReach - speed) / transitionTime;
				}
				else
				{
					if (m_speed < 1)
					{
						//m_speed = 0.5;
						speedToReach = m_speed;
						double currentPosition = position;//speed * (((double)metronome->getCurrentBeat() + 1.0) * (double)periodInSamples - metronome->getNumSamplesToNextBeat());
														  // retenir le nombre de tour 
						rest = speedToReach - floor(speedToReach);
						//currentT = 0;
						int addT;
						if (rest != 0)
						{
							numT = (rest == 0) ? 1 : ceil(1.0 / rest);
							addT = metronome->getCurrentT();
							while (addT > numT)
								addT -= numT;
						}
						else
							addT = 0;
						double positionToReach = numOfBeats * periodInSamples + addT * rest * numOfBeats * periodInSamples;
						double currentPositionAtSpeedToReach = speedToReach * (((double)metronome->getCurrentBeat() + 1.0) * (double)periodInSamples - metronome->getNumSamplesToNextBeat()) + addT * rest * numOfBeats * periodInSamples;;
						transitionTime = (positionToReach - currentPositionAtSpeedToReach) / speedToReach;
						transitionSpeed = (positionToReach - currentPosition) / transitionTime;
					}
					else
					{
						speedToReach = m_speed;
						double currentPosition = position;
														 
						// transition time = temps avant prochain passages par 0, pour pas avoir de problème de continuité de la position
						transitionTime = ((numOfBeats - metronome->getCurrentBeat() - 1) * periodInSamples + metronome->getNumSamplesToNextBeat());// -speedToReach;

						// position to reach = endroit où on se trouvera avant le passage en 0
						double positionToReach = (numOfBeats * periodInSamples) * speedToReach;// (numOfBeats * periodInSamples - speedToReach) * speedToReach;
						transitionSpeed = (positionToReach - currentPosition) / transitionTime;
					}
					
				}
				break;
			case PlayHeadState::Stop :
				speed = m_speed;
				/*if (speed < 1)
					speed = 0.5;*/
				speedToReach = speed;
				rest = speedToReach - floor(speedToReach);
				
				numT = (rest == 0) ? 1 : ceil(1.0 / rest);
				break;
		}
	}
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
	position = (double)timeLine->getPeriod() * p;//(double)currentPeriod * p;
}

double PlayHead::getReadingPosition()
{
	return position / (double)timeLine->getPeriod();//(double)currentPeriod;
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
		break;
	case PlayHeadState::Pause:
		break;
	case PlayHeadState::Stop:
		position = 0;
		break;
	default:
		break;
	}
}

void PlayHead::process()
{
	double r = 0;
	int a;
	//int plus = 0;
	switch (state)
	{
	case PlayHeadState::Play:
		
		if (speed != speedToReach)
		{
			++transitionPosition; // avance au rythme du metronome (vitesse = 1)
			if (speedToReach >= 1)
			{
				if (speedToReach > speed)
				{
					speed += speedInc;

					if (transitionPosition >= transitionTime)
					{
						transitionPosition = 0;
						speed = speedToReach;
						numT = 0;

					}
					position = speed * (((double)metronome->getCurrentBeat() + 1.0) * (double)periodInSamples - metronome->getNumSamplesToNextBeat());
				}
				else
				{
					position += transitionSpeed;
					if (transitionPosition >= transitionTime)
					{
						transitionPosition = 0;
						speed = speedToReach;
					}
				}
			}
			else
			{
				position += transitionSpeed;
				if (transitionPosition >= transitionTime)
				{
					transitionPosition = 0;
					speed = speedToReach;
					rest = speedToReach - floor(speedToReach);
					
					numT = (rest == 0) ? 1 : ceil(1.0 / rest);
				}
			}
		}
		else
		{
			if (speed < 1)
			{
				if (metronome->getCurrentBeat() == 0 && metronome->getNumSamplesToNextBeat() == periodInSamples && speed == speedToReach)
				{ // ça marche mais il faut trouver comment gérer ça lors de la transition...
					
					if (numT != 0)
					{
						int tmpT = metronome->getCurrentT();
						while (tmpT > numT)
							tmpT -= numT;
						plus = position + 1.0;
						if (tmpT == numT) //&& (position == 0 || position == speed * numOfBeats * periodInSamples))
						{
							//currentT = 0;
							plus = 0;
						}
					}
				}
			}
			else
			{
				plus = 0;
				
			}

			position = speed * (((double)metronome->getCurrentBeat() + 1.0) * (double)periodInSamples - metronome->getNumSamplesToNextBeat());
			position += plus;
		}
		
		

		while(position > numOfBeats * periodInSamples)
			position -= (numOfBeats * periodInSamples);

		
		int sub, up; // interval to test for midi event

		// check if speed is an integer
		r = speed - ceil(speed);
		if (r != 0) // not an integer -> find sub and up integer
		{
			sub = (int)ceil(position);
			up = (int)ceil(position + speed);
		}
		else
		{
			sub = (int)position;
			up = (int)position + (int)speed;
		}

		//position2 = periodInSamples - metronome->getNumSamplesToNextBeat(); //periodInSamples - metronome->getNumSamplesToNextBeat();

		for (int i = sub; i < up; i++)
		{
			testPosition(i);
		}

		
		

		//position = periodInSamples - metronome->getNumSamplesToNextBeat(); //periodInSamples - metronome->getNumSamplesToNextBeat();
		//testPosition(roundToInt(position));
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
		if (timeLine->isNoteOnTime(P, i, m_end, m_channel, m_note, m_velocity))
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
		if (timeLine->isNoteOffTime(P, i, m_end, m_channel, m_note))
		{
			MidiMessage midiMsgOff = MidiMessage::noteOff(m_channel, m_note);
			audioManager->sendMidiMessage(midiMsgOff,this);
		}
		i++;
	}

	if (timeLine->isChordOnTime(P, m_channel, chordToPlay, m_velocity))
	{
		for (int j = 0; j < chordSize; j++)
		{
			if (chordToPlay[j] > 0)
			{
				MidiMessage midiMsg = MidiMessage::noteOn(m_channel, chordToPlay[j], m_velocity);
				audioManager->sendMidiMessage(midiMsg,this);
			}
		}
	}

	if (timeLine->isChordOffTime(P, m_channel, chordToPlay))
	{
		for (int j = 0; j < chordSize; j++)
		{
			if (chordToPlay[j] > 0)
			{
				MidiMessage midiMsgOff = MidiMessage::noteOff(m_channel, chordToPlay[j]);
				audioManager->sendMidiMessage(midiMsgOff,this);
			}
		}
	}

}