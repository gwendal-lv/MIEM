/*
  ==============================================================================

    Amusinus.h
    Created: 15 Dec 2016 10:18:05am
    Author:  ayup1

  ==============================================================================
*/

#ifndef AMUSINUS_H_INCLUDED
#define AMUSINUS_H_INCLUDED

#define PI 3.14159265359

struct AmuSinusParameter 
{
	double m_amplitude;
	double m_frequency;
	double m_phase;
	double m_sampleRate;
	int m_bufferSize;
};

class AmuSinus
{
public:
	AmuSinus(double m_amplitude, double m_frequency, double m_phase, double m_sampleRate, int m_bufferSize);
	AmuSinus(AmuSinusParameter parameters);
	~AmuSinus();

	// ----- setters and getters -------
	void setFrequency(double newFrequency);
	void setAmplitude(double newAmplitude);

	//to change phase
	void setPhase(double newPhase);
	void resetPhase();
	void resetPhase(double newPhase);

	// get output
	double getSample();

	double getFrequency();
	double getAmplitude();
	double getPhase();

private:
	// parameters
	double amplitude;
	double frequency;
	double phase;
	double sampleRate;
	double bufferSize;

	// intern variable
	double angle;

	// to update angle
	double angleDelta;
	void updateAngleDelta();
	
	// to change frequency
	double targetFrequency;
	void updateFrequency();

	// to change amplitude
	double targetAmplitude;
	void updateAmplitude();


};




#endif  // AMUSINUS_H_INCLUDED
