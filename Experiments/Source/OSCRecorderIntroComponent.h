/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "UserQuestionsManager.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OSCRecorderIntroComponent  : public Component,
                                   public Button::Listener
{
public:
    //==============================================================================
    OSCRecorderIntroComponent ();
    ~OSCRecorderIntroComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    /// \brief Sets the user questions manager - that is actually also used for
    /// listening to the "Intro OK" button click - and sets its own text (because
    /// the presenter is properly set at that time).
    void SetUserQuestionsManager(UserQuestionsManager* _manager);
    
    void SetMainText(const char* mainText);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    UserQuestionsManager* userQuestionsManager = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> finishedButton;
    std::unique_ptr<TextEditor> introTextEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCRecorderIntroComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

