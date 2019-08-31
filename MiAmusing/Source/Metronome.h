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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/// \brief Time reference for all the audio track
class Metronome
{
public:
	Metronome();
	~Metronome();

	void setAudioParameter(double m_sampleRate, int m_BPM);
	void update();
	void reset();
	int getNumSamplesToNextBeat();
	int getCurrentBeat();
	int getPeriodInSamples();
	int getCurrentT();

private:
	int BPM_ToReach;
	int transitionPosition;
	int transitionTime; // 5ms en samples
	double incBPM;
	double BPM; // format double pour pouvoir faire la transition d'un BPM � un autre, utilis� en int le reste du temps
	int samplesTime;
	
	double sampleRate;

	int periodInSamples;
	double nextSample;

	int currentT;

	int timeStamp;

	int samplesLeftBeforeBeat;
	int currentBeats;
	int numOfBeats;
};

