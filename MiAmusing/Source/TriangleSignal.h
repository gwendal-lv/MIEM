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

enum TransportStateTr
{
	StoppedTr,
	StartingTr,
	PlayingTr,
	PausingTr,
	PausedTr,
	StoppingTr
};

class TriangleSignal : public AudioSource
{
public:
	TriangleSignal(double m_amplitude, double m_frequency, int m_numHarmonics, int m_ID);
	~TriangleSignal();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

	void setFrequency(double newFrequency);
	void setAmplitude(double newAmplitude);
	

	void changeState(TransportStateTr newState);

	int getID();

private:
	AmuSinus **harmonics;
	int *harmonicsN; // ordre des harmoniques
	double *harmonicsA; // amplitude des harmoniques

	int ID;

	double amplitude;
	double frequency;
	double phase;
	int numHarmonics; // nombre d'harmoniques que l'on veut utiliser

	TransportStateTr state;
};



#endif  // TRIANGLESIGNAL_H_INCLUDED
