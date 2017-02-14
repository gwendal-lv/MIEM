/*
  ==============================================================================

    Amusquare.cpp
    Created: 14 Feb 2017 1:46:58pm
    Author:  ayup1

  ==============================================================================
*/

#include "Amusquare.h"
#include <cmath>
#include "../JuceLibraryCode/JuceHeader.h" // juste pour le DBG

#define PI 3.14159265359

AmuSquare::AmuSquare(double m_frequency, double m_amplitude, double m_sampleRate, int m_bufferSize) : 
	angle(0), amplitude(m_amplitude), frequency(m_frequency), sampleRate(m_sampleRate), bufferSize(m_bufferSize)
{
	angleDelta = (2 * PI*frequency / sampleRate);
}

AmuSquare::~AmuSquare()
{
}

double AmuSquare::getNextSample()
{
	double sample = 0;
	sample = amplitude * getSigne(angle);
	//DBG((String)sample);

	angle += angleDelta;
	if (angle >= 2*PI)
		angle = 0;

	return sample;
}

int AmuSquare::getSigne(double angle)
{
	if (0 <= angle && angle < PI)
		return 1;
	else
		return -1;
}