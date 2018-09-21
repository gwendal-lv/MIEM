/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
class MultiSceneCanvasComponentAmusing;

enum OptionButtonClicked
{
	Volume,
	Speed,
	Rhythm,
	Sample,
	Octave,
	Closed,
};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AreaOptionsComponent  : public Component,
                              public Button::Listener,
                              public Slider::Listener
{
public:
    //==============================================================================
    AreaOptionsComponent ();
    ~AreaOptionsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void CompleteInitialisation(MultiSceneCanvasComponentAmusing* _canvasComponent);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	MultiSceneCanvasComponentAmusing * canvasComponent;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> volumeButton;
    std::unique_ptr<TextButton> speedButton;
    std::unique_ptr<TextButton> sampleButton;
    std::unique_ptr<TextButton> octaveButton;
    std::unique_ptr<TextButton> rhythmButton;
    std::unique_ptr<TextButton> closeOptionsButton;
    std::unique_ptr<Slider> soloSlider;
    std::unique_ptr<Label> soloLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AreaOptionsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
