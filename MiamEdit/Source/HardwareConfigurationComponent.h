/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
                                        public TextEditor::Listener,
                                        public Slider::Listener,
                                        public Button::Listener,
                                        public ComboBox::Listener
{
public:
    //==============================================================================
    HardwareConfigurationComponent ();
    ~HardwareConfigurationComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    friend class SettingsManager;

    virtual void textEditorTextChanged(TextEditor& editorThatHasChanged) override;

    /// \brief Returns the IP address as a string, or an empty string if given character sequence
    /// is not valid.
    std::string TryParseIpAddress();
    /// \brief Returns the UDP port as a positive int, or -1 if user input was not valid.
    int TryParseUdpPort();

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void visibilityChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SettingsManager* settingsManager = 0;

    String inputLabelSpatText;
    String inputLabelGenericText;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> inputsCountSlider;
    ScopedPointer<Slider> outputsCountSlider;
    ScopedPointer<Label> inputsCountLabel;
    ScopedPointer<Label> outputsCountLabel;
    ScopedPointer<ToggleButton> oscPluginToggleButton;
    ScopedPointer<Label> udpPortLabel;
    ScopedPointer<TextEditor> udpPortTextEditor;
    ScopedPointer<ToggleButton> keyboardToggleButton;
    ScopedPointer<Label> ipAddressLabel;
    ScopedPointer<TextEditor> ipAddressTextEditor;
    ScopedPointer<ToggleButton> inputNamesToggleButton;
    ScopedPointer<ToggleButton> outputNamesToggleButton;
    ScopedPointer<ComboBox> interpolationTypeComboBox;
    ScopedPointer<Label> interpolationTypeLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HardwareConfigurationComponent)
};

//[EndFile] You can add extra defines here...

} // namespace Miam
//[/EndFile]
