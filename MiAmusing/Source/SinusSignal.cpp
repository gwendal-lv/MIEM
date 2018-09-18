/*
  ==============================================================================

    SinusSignal.cpp
    Created: 4 Jan 2017 4:41:10pm
    Author:  ayup1

  ==============================================================================
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