/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OSCRecorderComponent.h"
#include "OSCRecorderIntroComponent.h"
#include "OSCRecorder.h"

#include "ExperimentsLookAndFeel.h"

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
    
    /// \brief If nullptr, all GUI components will be hidden. Own labels, etc, will then
    /// be displayed
    void SetOneGuiComponentVisible(Component* component);

private:
    //==============================================================================
    // Your private member variables go here...
    
    
    // Self-managed (for visibilty, etc...)
    std::unique_ptr<OSCRecorderComponent> oscRecorderComponent;
    
    
    std::unique_ptr<OSCRecorderIntroComponent> oscRecorderIntroComponent;
    std::unique_ptr<UserQuestions> firstUserQuestionsComponent;
    std::unique_ptr<UserFinalQuestions> finalUserQuestionsComponent;
    // stores all the previous "big GUI" components pointers
    std::vector<Component*> bigGuiComponents;
    
    
    // Own small components
    std::unique_ptr<Label> backLabel;
    std::unique_ptr<Label> expeLabel;
    std::unique_ptr<Label> expeLabel2;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
