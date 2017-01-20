/*
  ==============================================================================

    AmuSignal.cpp
    Created: 19 Dec 2016 9:57:50am
    Author:  ayup1

  ==============================================================================
*/

#include "AmuSignal.h"

AmuSignal::AmuSignal(double m_amplitude, double m_frequency) : 
	AudioSource(), amplitude(m_amplitude), frequency(m_frequency), state(Stopped)
{
}

AmuSignal::~AmuSignal()
{
}

