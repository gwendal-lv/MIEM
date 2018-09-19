/*
  ==============================================================================

    Modulator.h
    Created: 2 Jan 2017 11:36:24am
    Author:  ayup1

  ==============================================================================
*/

#ifndef MODULATOR_H_INCLUDED
#define MODULATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Amusinus.h"
#include "FourierSignal.h"


//==============================================================================
/*
*/
class Modulator    : public AudioSource
{
public:
    Modulator(FourierSignal *m_carrier, FourierSignal *m_mod);
    ~Modulator();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void setMf(double m_mf);
	void setMod(FourierSignal* m_mod);
	void setCarrier(FourierSignal* m_carrier);

private:
	double mf;
	FourierSignal *carrier;
	FourierSignal *mod;


};


#endif  // MODULATOR_H_INCLUDED
