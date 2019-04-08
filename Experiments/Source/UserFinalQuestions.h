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
    private :
    std::string sliderValueToMethodName(double sliderValue);
    public :

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void mouseUp (const MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    UserQuestionsManager* userQuestionsManager = 0;

    bool questionsAnswered = false;
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
    std::unique_ptr<Label> remarkLabel;
    std::unique_ptr<TextEditor> remarkTextEditor;
    std::unique_ptr<Label> fastLabel;
    std::unique_ptr<ImageButton> imageButton;
    std::unique_ptr<Label> preferLabel;
    std::unique_ptr<Slider> fastSlider;
    std::unique_ptr<Slider> intuitiveSlider;
    std::unique_ptr<Slider> preferSlider;
    std::unique_ptr<ImageButton> imageButton2;
    std::unique_ptr<Label> precisionLabel2;
    std::unique_ptr<Label> precisionLabel;
    std::unique_ptr<Slider> preciseSlider;
    std::unique_ptr<Label> intuitiveLabel;
    std::unique_ptr<GroupComponent> separationBar2;
    std::unique_ptr<GroupComponent> separationBar3;
    std::unique_ptr<GroupComponent> separationBar4;
    std::unique_ptr<GroupComponent> separationBar5;
    std::unique_ptr<Label> labelQ1;
    std::unique_ptr<Label> selfExpertiseLevelLabel2;
    std::unique_ptr<Label> labelQ2;
    std::unique_ptr<Label> labelQ3;
    std::unique_ptr<Label> labelQ4;
    std::unique_ptr<Label> labelQ5;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserFinalQuestions)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

