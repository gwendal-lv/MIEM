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
#include "Metronome.h"

enum PlayHeadState
{
	Play,
	Pause,
	Stop
};

class PlayHead
{
public:
	PlayHead(Metronome* m_metronome);
	~PlayHead();

	void setAudioManager(Amusing::AudioManager* m_audioManager);
	void LinkTo(TimeLine* _timeLine);
	void setSpeed(double m_speed);
	void changeState();
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
	double speedToReach; // vitesse � atteindre lors du changement de vitesse
	double transitionPosition;
	double transitionTime; // periode pendant laquelle la t�te de lecture doit r�duire ou accumuler du retard
	double speedInc;
	int currentPeriod; // period taking speed into account
	double position;
	int numT;
	
	double rest;
	double plus;
	PlayHeadState state;

	double position2;
	int numOfBeats;
	int currentBeats;

	double transitionSpeed;
	double incPlus;
	double oldPeriod; // each time, we search from oldPosition to position if there's a note to play
					  // thus we always need oldPosition < position -> if the BPM change, we need to applay the same transform to the oldPositon...
					  // !! oldPeriod = period pour un beat et pas pour un tour !!

	Metronome *metronome; // used to be synchronized with all the reading head
	void testPosition(int P); // look in the associate timeLine if there is a MIDI msg to send

	static const int chordSize = 3;
	int chordToPlay[chordSize];

	// references to other objects
	TimeLine* timeLine; // reference to the associated timeLine
	Amusing::AudioManager* audioManager;// reference to the audioManager to send the MIDI

	JUCE_LEAK_DETECTOR(PlayHead);
};

