/*
  ==============================================================================

    FourierSignal.h
    Created: 19 Dec 2016 4:57:45pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef FOURIERSIGNAL_H_INCLUDED
#define FOURIERSIGNAL_H_INCLUDED

#include "AmuSignal.h"
#include "Amusinus.h"

class FourierSignal : public AmuSignal
{
public:
	FourierSignal(double m_amplitude, double m_frequency, int m_numHarmonics);
	~FourierSignal();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void updateSample();
	double getNextSample();

	void setFrequency(double newFrequency) override;
	void setAmplitude(double newAmplitude) override;
	void setPhase(double newPhase);
	
	double getAmplitude();
	double getFrequency();

	void changeState(TransportState newState) override;

	virtual void computeHarmonicsParameters(int samplesPerBlockExpected, double sampleRate) = 0;
	AmuSinus** getHarmonics(); 
	AmuSinus* getHarmonic(int idx); 

	int getNumHarmonics();
	int getHarmonicsN(int idx);
	double getHarmonicsA(int idx);

	void shiftPhase(double shift);
	void resetPhase(double newPhase);
	double getPhase();

protected:
	AmuSinus **harmonics = 0;
	int *harmonicsN = 0; // ordre des harmoniques
	double *harmonicsA = 0; // amplitude des harmoniques

	double phase;
	int numHarmonics; // nombre d'harmoniques que l'on veut utiliser

	double sample;
	
};





#endif  // FOURIERSIGNAL_H_INCLUDED
