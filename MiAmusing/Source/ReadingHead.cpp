/*
  ==============================================================================

    ReadingHead.cpp
    Created: 21 Aug 2017 3:58:39pm
    Author:  ayup1

  ==============================================================================
*/

#include "ReadingHead.h"
#include "AudioManager.h"

PlayHead::PlayHead() : speed(1.0), position(0), state(PlayHeadState::Stop)
{

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
}

void PlayHead::LinkTo(TimeLine* m_timeLine)
{
	timeLine = m_timeLine;
	currentPeriod = m_timeLine->getPeriod() / speed;
}

void PlayHead::setSpeed(double m_speed)
{
	if (speed != m_speed)
	{
		speed = m_speed;
		if(timeLine != nullptr)
			currentPeriod = timeLine->getPeriod() / speed;
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
	switch (state)
	{
	case PlayHeadState::Play:
		if (position >= timeLine->getPeriod())
			position = 0;

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

		for (int i = sub; i < up; i++)
		{
			testPosition(i);
		}

		position += speed;
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