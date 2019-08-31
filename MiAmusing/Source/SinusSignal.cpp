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

#include "SinusSignal.h"
#include "Amusinus.h"
#include <cmath>

SinusSignal::SinusSignal(double m_amplitude, double m_frequency, int m_numHarmonics) : 
	FourierSignal(m_amplitude, m_frequency, 1)
{
	
}

SinusSignal::~SinusSignal()
{

}

void SinusSignal::computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate)
{
	harmonics = new AmuSinus*[numHarmonics];
	harmonicsN = new int[numHarmonics];
	harmonicsA = new double[numHarmonics];

	int n = 1;
	harmonicsN[0] = 1;
	harmonicsA[0] = amplitude;
	harmonics[0] = new AmuSinus(harmonicsA[0], n*frequency, 0, sampleRate, samplesPerBlockExpected);

}