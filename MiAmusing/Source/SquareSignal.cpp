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

#include "SquareSignal.h"
#include "Amusinus.h"

/*SquareSignal::SquareSignal(double m_amplitude, double m_frequency, int m_numHarmonics) :
	AudioSource(), frequency(m_frequency), numHarmonics(m_numHarmonics), amplitude(m_amplitude),
	phase(0), state(Stopped)
{
	//computeFrequencies();
	DBG("SquareSignal Constructor");
	DBG("f = " + (String)frequency);
}*/

SquareSignal::SquareSignal(double m_amplitude, double m_frequency, int m_numHarmonics) :
	FourierSignal(m_amplitude,m_frequency,m_numHarmonics)
{
	//computeFrequencies();
	DBG("SquareSignal Constructor");
	DBG("f = " + (String)frequency);
}

SquareSignal::~SquareSignal()
{
	DBG("SquareSignal Destructor");
	//delete S;
}

/*void SquareSignal::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	DBG("SquareSignal::prepareToPlay");
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];
	 
	for (int i = 0; i < numHarmonics;++i) // trouver un moyen de generaliser ca pour pouvoir fusionner triangle et square
	{
		int n = 2 * i + 1; // on prend que les impaires
		harmonicsN[i] = n;
		harmonicsA[i] = amplitude / n;//(2 * amplitude / (n * PI)) * (pow(-1, (n - 1) / 2));
		harmonics[i] = new AmuSinus(harmonicsA[i], n*frequency, 0, sampleRate, samplesPerBlockExpected);
		//harmonics[i] = new AmuSinus(amplitude/n, n*frequency, 0, sampleRate, samplesPerBlockExpected);
		
	} 
	
}
*/
void SquareSignal::computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate)
{
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];

	for (int i = 0; i < numHarmonics; ++i)
	{
		int n = 2 * i + 1; // on prend que les impaires
		harmonicsN[i] = n;//(2 * amplitude / (n * PI)) * (pow(-1, (n - 1) / 2));
		harmonicsA[i] = amplitude / n;
		harmonics[i] = new AmuSinus(harmonicsA[i], n*frequency, n *phase, sampleRate, samplesPerBlockExpected);
	}
}