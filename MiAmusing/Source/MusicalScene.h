/*
  ==============================================================================

    MusicalScene.h
    Created: 19 Dec 2016 8:22:55am
    Author:  ayup1

  ==============================================================================
*/

#ifndef MUSICALSCENE_H_INCLUDED
#define MUSICALSCENE_H_INCLUDED

#include"JuceHeader.h"


class MusicalScene
{
public:
	MusicalScene(String m_name);
	~MusicalScene();

	void addSource(int ID, int sourceType);

	void setFrequency(int ID, double frequency);
	void setAmplitude(int ID, double amplitude);
	void setPath(int ID, String path);
	void setState(int ID, TransportState transportState); // gere l'etat de la source : play, start, stop
	void setVolume(double amplitude);
	void setReverse(bool enable);

private:
	String name;
	
	OwnedArray<AudioSource> trackArray;
	
};




#endif  // MUSICALSCENE_H_INCLUDED
