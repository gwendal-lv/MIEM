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

#include "UserQuestionsManager.h"

// forward declarations
class UserQuestionsManager;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UserQuestions  : public Component,
                       public Button::Listener
{
public:
    //==============================================================================
    UserQuestions ();
    ~UserQuestions();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void SetUserQuestionsManager(UserQuestionsManager* _manager)
    { userQuestionsManager = _manager; }

    std::shared_ptr<bptree::ptree> GetQuestionsBPTree();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    UserQuestionsManager* userQuestionsManager = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> label;
    std::unique_ptr<TextButton> finishedButton;
    std::unique_ptr<ToggleButton> allowDataToggleButton;
    std::unique_ptr<Label> label2;
    std::unique_ptr<ToggleButton> visionDisorderToggleButton;
    std::unique_ptr<Label> label3;
    std::unique_ptr<TextEditor> visionDisorderTextEditor;
    std::unique_ptr<ToggleButton> physicalDisorderToggleButton;
    std::unique_ptr<Label> label4;
    std::unique_ptr<TextEditor> physicalDisorderTextEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserQuestions)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

