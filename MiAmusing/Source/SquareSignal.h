/*
  ==============================================================================

    SquareSignal.h
    Created: 15 Dec 2016 11:54:24am
    Author:  ayup1

  ==============================================================================
*/

#ifndef SQUARESIGNAL_H_INCLUDED
#define SQUARESIGNAL_H_INCLUDED

#include "JuceHeader.h"
#include "Amusinus.h"

enum TransportState
{
	Stopped,
	Starting,
	Playing,
	Pausing,
	Paused,
	Stopping
};


// faire architecture des sources  : mySources(tout generic avec les etats) ---> shapeSources (generique pour triangle, square, ...)
//                                                                           \--> soundSources (utilisation des fichiers) 

class SquareSignal : public AudioSource // ajouter les etats comme dans Sound, pour pourvoir utiliser facilement les deux de la mm facon.	
{
public:
	SquareSignal (double m_amplitude, double m_frequency, int m_numHarmonics, int ID);
	~SquareSignal ();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void setFrequency(double newFrequency);
	void setAmplitude(double newAmplitude);

	void changeState(TransportState newState);

	int getID();

	// peut-etre prevoir un changement de phase aussi si on doit recaler les signaux

private:
	/*
	AmuSinus *S;
	AmuSinus *S3;
	AmuSinus *S5;
	AmuSinus *S7;
	AmuSinus *S9;
	AmuSinus *S11;
	AmuSinus *S13;
	AmuSinus *S15;
	AmuSinus *S17;
	AmuSinus *S19;
	AmuSinus *S21;
	AmuSinus *S23;
	AmuSinus *S25;
	AmuSinus *S27;
	AmuSinus *S29;
	AmuSinus *Sopp;
	*/
	int ID;

	AmuSinus **harmonics;
	int *harmonicsN; // ordre des harmoniques
	double *harmonicsA; // amplitude des harmoniques


	double amplitude;
	double frequency;
	double phase;
	int numHarmonics; // nombre d'harmoniques que l'on veut utiliser

	TransportState state;

};





#endif  // SQUARESIGNAL_H_INCLUDED
