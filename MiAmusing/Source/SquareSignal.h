/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
