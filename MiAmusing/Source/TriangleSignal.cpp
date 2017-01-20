/*
  ==============================================================================

    TriangleSignal.cpp
    Created: 16 Dec 2016 9:51:18am
    Author:  ayup1

  ==============================================================================
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