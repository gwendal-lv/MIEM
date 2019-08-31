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

#ifndef RYTHME_H_INCLUDED
#define RYTHME_H_INCLUDED

#include "JuceHeader.h"
#include <vector>

struct On
{
	int duration;
	bool on; // play or not
};

class Rythme : public Component, // voir s'il y a une classe pour recuperer des notes midi, ...
	public Timer//,
			  // public Thread // plutot un timer?
{
public:
	Rythme(Component* m_mother, int m_bpm, int duration);
	~Rythme();

	void mouseDown(const MouseEvent &event) override; // si les clics sont "prioritaires" sur le thread audio, peut etre simplement faire source audio (= retour) et recuperer bufferToFill.buffer, stocker tout dans + gd buffer
	void mouseUp(const MouseEvent & event) override;

	//void run() override;
	void resized() override;
	void paint(Graphics&) override;

	void timerCallback() override;

	int* getBuffer();
	int getNextSample();
	void startRecord();
	void stopRecord();
	void setBPM(int newBPM);
	void setMaxT(int newDuration);
	void setSampleRate(double m_sampleRate);

private:
	//std::vector<int> rythmeToSend; // rythme enregistre
	double sampleRate; // echantillonnage utilise par les sources audio
	int bpm; // tempo impose
	//double minT; // duree minimal d'une note/rythme
	int maxT; // temps max d'une note = duree max enregistree (en periode du timer)
	//int sleepTime; // temps entre les execution du thread >= 1/sampleRate (etre sur d'avoir assez d'echantillon et de pas manquer une tape de l'utilisateur)
	int Ttimer;

	int t;
	bool record;
	On currentOn;
	std::vector<On> rythme;
	Component* mother;

	int *buffer;
	int L; //bufferSize
	void fillBuffer();

	int position;
	
};





#endif  // RYTHME_H_INCLUDED
