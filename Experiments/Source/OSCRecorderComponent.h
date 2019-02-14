/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "ExperimentState.h"

class OSCRecorder;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OSCRecorderComponent  : public Component,
                              public Button::Listener
{
public:
    //==============================================================================
    OSCRecorderComponent ();
    ~OSCRecorderComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void SetRecorderManager(OSCRecorder* _recorder) {recorderManager = _recorder;}

    void DisplayNewState(ExperimentState newState, int presetStep, size_t presetsCount);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    OSCRecorder* recorderManager = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> startButton;
    std::unique_ptr<TextButton> listenButton;
    std::unique_ptr<TextButton> finishedButton;
    std::unique_ptr<Label> countLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCRecorderComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

