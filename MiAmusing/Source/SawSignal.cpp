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

#include "SawSignal.h"

SawSignal::SawSignal(double m_amplitude, double m_frequency, int m_numHarmonics) :
	FourierSignal(m_amplitude, m_frequency, m_numHarmonics)
{
}

SawSignal::~SawSignal()
{
}

void SawSignal::computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate)
{
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];

	for (int i = 0; i < numHarmonics; ++i)
	{
		int n = i + 1; // on prend que les impaires
		harmonicsN[i] = n;//(2 * amplitude / (n * PI)) * (pow(-1, (n - 1) / 2));
		harmonicsA[i] = - 2 * amplitude  * pow(-1,n) / (PI * n);
		harmonics[i] = new AmuSinus(harmonicsA[i], n*frequency, n *phase, sampleRate, samplesPerBlockExpected);
	}
}