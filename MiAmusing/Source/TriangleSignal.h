/*
  ==============================================================================

    TriangleSignal.h
    Created: 16 Dec 2016 9:51:18am
    Author:  ayup1

  ==============================================================================
*/

#ifndef TRIANGLESIGNAL_H_INCLUDED
#define TRIANGLESIGNAL_H_INCLUDED

#include "JuceHeader.h"
#include "Amusinus.h"
#include "FourierSignal.h"


class TriangleSignal : public FourierSignal
{
public:
	TriangleSignal(double m_amplitude, double m_frequency, int m_numHarmonics);
	~TriangleSignal();

	void computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate);

};



#endif  // TRIANGLESIGNAL_H_INCLUDED
