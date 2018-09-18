/*
  ==============================================================================

    SinOsc.h
    Created: 2 Jan 2017 1:24:42am
    Author:  ayup1

  ==============================================================================
*/

#ifndef SINOSC_H_INCLUDED
#define SINOSC_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmuSignal.h"
#include "Amusinus.h"

//==============================================================================
/*
*/
class SinOsc    : public AudioSource
{
public:
    SinOsc(AmuSinus *m_carrier, AmuSinus *m_mod);
    ~SinOsc();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void setMf(double m_mf);
	void nextSample();
	double getSample();

	double getCurrentAngle();
	double getCurrentAmplitude();
	double getCurrentModulation();

private:
	// carrier
	AmuSinus *carrier;
	// modulator
	AmuSinus *mod;

	// indice de modulation
	double mf;

	// valeur de l'integrale du signal modulant
	double Imod;

	double currentAngle;
	double currentAmplitude;
	double currentModulation;

};


#endif  // SINOSC_H_INCLUDED
