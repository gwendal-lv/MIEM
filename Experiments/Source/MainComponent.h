/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OSCRecorderComponent.h"
#include "OSCRecorder.h"

#include "UserQuestions.h"
#include "UserFinalQuestions.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component
{
    // for easy and direct access from the experiment managers...
    friend class OSCRecorder;
    
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    OSCRecorderComponent& GetOscRecorderComponent() {return *(oscRecorderComponent.get()); }

private:
    //==============================================================================
    // Your private member variables go here...
    
    std::unique_ptr<OSCRecorderComponent> oscRecorderComponent;
    std::unique_ptr<UserQuestions> firstUserQuestionsComponent;
    std::unique_ptr<UserFinalQuestions> finalUserQuestionsComponent;
    
    std::unique_ptr<Label> backLabel;
    std::unique_ptr<Label> expeLabel;
    std::unique_ptr<Label> expeLabel2;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
