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

#include "TriangleSignal.h"
#include "Amusinus.h"
#include <cmath>

TriangleSignal::TriangleSignal(double m_amplitude, double m_frequency, int m_numHarmonics) : 
	FourierSignal(m_amplitude, m_frequency, m_numHarmonics)
{
	//computeFrequencies();
	DBG("TriangleSignal Constructor");
	DBG("f = " + (String)frequency);
}

TriangleSignal::~TriangleSignal()
{
	DBG("TriangleSignal Destructor");
	//delete S;
}

void TriangleSignal::computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate)
{
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];

	for (int i = 0; i < numHarmonics; ++i)
	{
		int n = 2 * i + 1;
		harmonicsN[i] = n;
		harmonicsA[i] = (4 * amplitude / pow(n * PI, 2)) * (1 - pow(-1, n));
		harmonics[i] = new AmuSinus(harmonicsA[i], n*frequency, PI / 2, sampleRate, samplesPerBlockExpected);
	}
}