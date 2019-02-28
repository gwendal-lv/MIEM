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

class UserQuestionsManager;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UserFinalQuestions  : public Component,
                            public Button::Listener
{
public:
    //==============================================================================
    UserFinalQuestions ();
    ~UserFinalQuestions();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void SetUserQuestionsManager(UserQuestionsManager* _manager)
    { userQuestionsManager = _manager; }

    std::shared_ptr<bptree::ptree> GetQuestionsBPTree();

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void mouseUp (const MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    UserQuestionsManager* userQuestionsManager = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ToggleButton> similarInterfaceToggleButton;
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> similarInterfaceQuestionLabel;
    std::unique_ptr<TextEditor> similarInterfaceTextEditor;
    std::unique_ptr<ToggleButton> similarExperimentToggleButton;
    std::unique_ptr<Label> similarExpeQuestionLabel;
    std::unique_ptr<TextEditor> similarExpeTextEditor;
    std::unique_ptr<TextButton> finishedButton;
    std::unique_ptr<Label> selfExpertiseLevelLabel;
    std::unique_ptr<Slider> expertiseSlider;
    std::unique_ptr<Label> expertiseLevelLabel;
    std::unique_ptr<Label> expertiseLevelLabel2;
    std::unique_ptr<Label> expertiseLevelLabel3;
    std::unique_ptr<Label> expertiseLevelLabel4;
    std::unique_ptr<Label> expertiseLevelLabel5;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserFinalQuestions)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

