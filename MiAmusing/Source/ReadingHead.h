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

enum PlayHeadState
{
	Play,
	Pause,
	Stop
};

class PlayHead
{
public:
	PlayHead();
	~PlayHead();

	void setAudioManager(Amusing::AudioManager* m_audioManager);
	void LinkTo(TimeLine* _timeLine);
	void setSpeed(double m_speed);
	void setReadingPosition(double p);
	double getReadingPosition();
	int getTimeLineId();
	void setId(int _id);
	int getId();
	void setState(PlayHeadState m_state);

	

	void process(); 
	

private:
	// members of ReadingHead
	int Id;
	double speed;
	int currentPeriod; // period taking speed into accound
	double position;
	PlayHeadState state;

	void testPosition(int P); // look in the associate timeLine if there is a MIDI msg to send

	// references to other objects
	TimeLine* timeLine; // reference to the associated timeLine
	Amusing::AudioManager* audioManager;// reference to the audioManager to send the MIDI
};

