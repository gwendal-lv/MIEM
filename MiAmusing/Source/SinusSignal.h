/*
  ==============================================================================

    SinusSignal.h
    Created: 4 Jan 2017 4:41:10pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef SINUSSIGNAL_H_INCLUDED
#define SINUSSIGNAL_H_INCLUDED

#include "JuceHeader.h"
#include "Amusinus.h"
#include "FourierSignal.h"


class SinusSignal : public FourierSignal
{
public:
	SinusSignal(double m_amplitude, double m_frequency, int m_numHarmonics);
	~SinusSignal();

	void computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate);

};



#endif  // SINUSSIGNAL_H_INCLUDED
