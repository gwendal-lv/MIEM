/*
  ==============================================================================

    SawSignal.h
    Created: 11 Apr 2017 2:31:13pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef SAWSIGNAL_H_INCLUDED
#define SAWSIGNAL_H_INCLUDED

#include "JuceHeader.h"
#include "Amusinus.h"
#include "FourierSignal.h"

class SawSignal : public FourierSignal
{
public:
	SawSignal(double m_amplitude, double m_frequency, int m_numHarmonics);
	~SawSignal();

private:
	void computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate) override;
};





#endif  // SAWSIGNAL_H_INCLUDED
