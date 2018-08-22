/*
  ==============================================================================

    SawSignal.cpp
    Created: 11 Apr 2017 2:31:13pm
    Author:  ayup1

  ==============================================================================
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