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

#include "Metronome.h"

#include<cmath>

Metronome::Metronome() : BPM(120), samplesTime(0), periodInSamples(0), samplesLeftBeforeBeat(0)
{
	numOfBeats = 4;
	currentBeats = 0;
	currentT = 0;
}

Metronome::~Metronome()
{
}

void Metronome::update()
{
	// la mise � jour du metronome est s�par�e en deux parties : 
	// 1) modification du BPM si besoin
	// 2) le calcul de la position dans le temps : nombre d'�chantillons restant avant le prochain temps, nombre de temps pass�s, et nombre de tour effectu�s

	if (BPM != BPM_ToReach)
	{
		++transitionPosition;
		if (transitionPosition != transitionTime)
		{
			BPM += incBPM;
			int newPeriodInSamples = (int)round((60.0 * sampleRate) / (double)BPM);
			samplesLeftBeforeBeat *= roundToInt((double)newPeriodInSamples / (double)periodInSamples);
			periodInSamples = newPeriodInSamples;
		}
		else
		{
			BPM = BPM_ToReach;
			int newPeriodInSamples = (int)round((60.0 * sampleRate) / (double)BPM);
			samplesLeftBeforeBeat *= roundToInt((double)newPeriodInSamples / (double)periodInSamples);
			periodInSamples = (int)round((60.0 * sampleRate) / (double)BPM);
			transitionPosition = 0;
		}
	}

	// le nombre d'�chantillons restant avant le prochain temps diminue � chaque update
	// une fois qu'il atteint 0, on passe au temps suivant
	// au bout de "numOfBeats" temps, le nombre de tour augmente de 1
	if (periodInSamples != 0)
	{
		--samplesLeftBeforeBeat;
		if (samplesLeftBeforeBeat < 0)
		{
			samplesLeftBeforeBeat = periodInSamples;
			++currentBeats;
			if (currentBeats >= numOfBeats)
			{
				currentBeats = 0;
				++currentT;
			}
		}
		
		
	}
}

void Metronome::reset()
{
	currentBeats = 0;
	samplesLeftBeforeBeat = periodInSamples;
	currentT = 0;
}

int Metronome::getNumSamplesToNextBeat()
{
	return samplesLeftBeforeBeat; // modulo pas performant
}

int Metronome::getCurrentBeat()
{
	return currentBeats;
}

void Metronome::setAudioParameter(double m_sampleRate, int m_BPM)
{
	if (periodInSamples == 0) // on a pas encore configur� une premi�re fois
	{
		BPM = m_BPM;
		BPM_ToReach = m_BPM;
		sampleRate = m_sampleRate;
		periodInSamples = (int)round((60.0 * sampleRate) / (double)BPM);
		samplesLeftBeforeBeat = periodInSamples;
	}
	else if (BPM != m_BPM || sampleRate != m_sampleRate) // si on change une des options
	{
		BPM_ToReach = m_BPM;
		transitionTime = roundToInt(5.0*sampleRate / 1000.0); // temps de transition = 5ms
		incBPM = double(BPM_ToReach - BPM) / (double)transitionTime;
		transitionPosition = 0;
	}
}



int Metronome::getPeriodInSamples()
{
	// retourne la periode d'un carr� donnant le tempo de m_bpm :
	//	m_bpm = 1 Tic tous les sampleRate * 60 / BPM
	//  1 carr� = 4 Tic = 4 * sampleRate * 60 / BPM

	return periodInSamples;
}

int Metronome::getCurrentT()
{
	return currentT;
}
