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
#include "FourierSignal.h"




// faire architecture des sources  : mySources(tout generic avec les etats) ---> shapeSources (generique pour triangle, square, ...)
//                                                                           \--> soundSources (utilisation des fichiers) 

class SquareSignal : public FourierSignal // ajouter les etats comme dans Sound, pour pourvoir utiliser facilement les deux de la mm facon.	
{
public:
	SquareSignal (double m_amplitude, double m_frequency, int m_numHarmonics);
	~SquareSignal ();

	void computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate) override;

/*
private:
	
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
	
	AmuSinus **harmonics;
	int *harmonicsN; // ordre des harmoniques
	double *harmonicsA; // amplitude des harmoniques

	double phase;
	int numHarmonics; // nombre d'harmoniques que l'on veut utiliser*/



};





#endif  // SQUARESIGNAL_H_INCLUDED
