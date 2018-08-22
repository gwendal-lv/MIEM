/*
  ==============================================================================

    ADSRSignal.h
    Created: 9 Jan 2017 5:34:02pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef ADSRSIGNAL_H_INCLUDED
#define ADSRSIGNAL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FourierSignal.h"

enum ADSR_State
{
	Attack,
	Decay,
	Sustain,
	Release,
	Silence
};

//==============================================================================
/*
*/
class ADSRSignal    : public AmuSignal//AudioSource // peut etre Positionable
{
public:
    ADSRSignal(FourierSignal *m_signal);
	ADSRSignal(FourierSignal *m_signal, bool m_stopSustain);
	ADSRSignal(FourierSignal *m_signal, double m_duration);
	ADSRSignal(int type, double duration, bool m_loop);
    ~ADSRSignal();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;
    
	void setFrequency(double newFrequency) override;
	void setAmplitude(double newAmplitude) override;
	void changeState(TransportState newState) override;

	void playNote();
	void releaseNote();

	void setDuration(double duration);

	int getLength();

	void isEmpty();
	
	bool isLooping();

	int getPosition();

private:
	FourierSignal *signal;
	void changeState(ADSR_State newState);

	// internal attribut
	ADSR_State ADSR_state;
	int position;
	double currentGain;
	bool stopSustain; // indique si tient la note le temps prevu ou si on la maintient jusque nouvel ordre, stopSustain = true : arrete apres le temps prevu
	bool loop;
	bool erase;

	// parameter of ADSR envelop
	double attackT;
	double attackLvl;
	double decay;
	double sustainT;
	double sustainLvl;
	double release;

	// durations of ADSR envelop in sample
	int endAttackP;
	int endDecayP;
	int endSustainP;
	int endReleaseP;

	int currentSamplesPerBlock;
	double currentSampleRate;
};


#endif  // ADSRSIGNAL_H_INCLUDED
