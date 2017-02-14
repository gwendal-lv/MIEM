/*
  ==============================================================================

    Amusquare.h
    Created: 14 Feb 2017 1:46:58pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef AMUSQUARE_H_INCLUDED
#define AMUSQUARE_H_INCLUDED

class AmuSquare
{
public:
	AmuSquare(double m_frequency, double m_amplitude, double m_sampleRate, int m_bufferSize);
	~AmuSquare();

	double getNextSample();

private:
	double frequency;
	double amplitude;
	double sampleRate;
	double bufferSize;

	double angle;
	double angleDelta;
	double signe;
	int getSigne(double angle);

};





#endif  // AMUSQUARE_H_INCLUDED
