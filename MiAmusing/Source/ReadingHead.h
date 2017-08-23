/*
  ==============================================================================

    ReadingHead.h
    Created: 21 Aug 2017 3:58:39pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include<memory>
#include "BaseMidiSender.h"
class ReadingHead
{
public:
	ReadingHead();
	~ReadingHead();

	void setAudioManager(Amusing::AudioManager* m_audioManager);
	void LinkTo(std::shared_ptr<TimeLine> timeLine);
	void setSpeed(double m_speed);
	void setReadingPotition(double p);
	int getReadingPosition();
	

	void process(); 
	

private:
	// members of ReadingHead
	double speed;
	int currentPeriod; // period taking speed into accound
	int position;

	// references to other objects
	std::shared_ptr<TimeLine> timeLine; // reference to the associated timeLine
	Amusing::AudioManager* audioManager;// reference to the audioManager to send the MIDI
};

