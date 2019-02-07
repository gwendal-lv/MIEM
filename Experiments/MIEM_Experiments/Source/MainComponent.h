/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OSCRecorderComponent.h"
#include "OSCRecorder.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component
{
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
