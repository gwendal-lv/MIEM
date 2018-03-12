/*
  ==============================================================================

    ColorArray.h
    Created: 30 Nov 2017 11:54:40am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AreaOptions.h"

class AreaOptions;

//==============================================================================
/*
	This component contains de avalaible color for the Polygons.
	Each color is associated with a path to a sample by the SoundFileManager
*/
class ColorArray    : public Component, public ButtonListener
{
public:
    ColorArray(int size = 4);
    ~ColorArray();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button*) override;

	void completeInitialisation(AreaOptions *m_areaOption);
	void setSamplesColor(int Nsamples, Colour colorCode[]);
	void addColourSample(int index, Colour colour);
	void setCurrentColorSelected(int idx);
	//void addColor(Colour newColour);

private:
	int currentColor;
	bool wasClicked;
	OwnedArray<TextButton> buttonArray;

	AreaOptions *areaOptions;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorArray)
};
