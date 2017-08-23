/*
  ==============================================================================

    ReadingHead.cpp
    Created: 21 Aug 2017 3:58:39pm
    Author:  ayup1

  ==============================================================================
*/

#include "ReadingHead.h"
#include "AudioManager.h"

ReadingHead::ReadingHead() : speed(1.0)
{

}

ReadingHead::~ReadingHead()
{
}

void ReadingHead::setAudioManager(AudioManager* m_audioManager)
{
	audioManager = m_audioManager;
}

void ReadingHead::LinkTo(std::shared_ptr<TimeLine> m_timeLine)
{
	timeLine = m_timeLine;
	currentPeriod = m_timeLine->getPeriod() / speed;
}

void ReadingHead::setSpeed(double m_speed)
{
	if (speed != m_speed)
	{
		speed = m_speed;
		if(timeLine != nullptr)
			currentPeriod = timeLine->getPeriod() / speed;
	}
}


void ReadingHead::setReadingPotition(double p)
{
	position = round((double)currentPeriod * p);
}

int ReadingHead::getReadingPosition()
{
	return position;
}

void ReadingHead::process()
{
	position++;
	while (position >= currentPeriod)
		position -= currentPeriod;
}