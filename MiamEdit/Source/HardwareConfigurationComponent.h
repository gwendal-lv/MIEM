/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

namespace Miam {

    // Forward declarations
    class SettingsManager;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

 // \brief An important component that corresponds to an Miam::AppMode of the
 // Miam::Presenter.
 //
 // An auto-generated component, created by the Projucer.
 //

                                                                    //[/Comments]
*/
class HardwareConfigurationComponent  : public Component,
                                        public TextEditorListener,
                                        public SliderListener,
                                        public ButtonListener
{
public:
    //==============================================================================
    HardwareConfigurationComponent ();
    ~HardwareConfigurationComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void CompleteInitialization(SettingsManager* _settingsManager);

    int GetUdpPort();
    void SetUdpPort(int udpPort);

    virtual void textEditorTextChanged(TextEditor& editorThatHasChanged) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SettingsManager* settingsManager = 0;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> inputsCountSlider;
    ScopedPointer<Slider> outputsCountSlider;
    ScopedPointer<Label> inputsCountLabel;
    ScopedPointer<Label> outputsCountLabel;
    ScopedPointer<ToggleButton> oscPluginToggleButton;
    ScopedPointer<Label> udpPortLabel;
    ScopedPointer<TextEditor> udpPortTextEditor;
    ScopedPointer<ToggleButton> keyboardButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HardwareConfigurationComponent)
};

//[EndFile] You can add extra defines here...

} // namespace Miam
//[/EndFile]
