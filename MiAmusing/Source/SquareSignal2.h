/*
  ==============================================================================

    SquareSignal2.h
    Created: 14 Feb 2017 2:02:10pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef SQUARESIGNAL2_H_INCLUDED
#define SQUARESIGNAL2_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class AmuSquare;
class SquareSignal2    : public AudioSource
{
public:
    SquareSignal2(double m_frequency, double m_amplitude, int m_time2play);
    ~SquareSignal2();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources() override;

private:
	double amplitude, frequency;
	int time2play;
	int N;
	int pos;
	AmuSquare *amusquare;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SquareSignal2)
};


#endif  // SQUARESIGNAL2_H_INCLUDED
