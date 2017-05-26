/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include <cmath>

#include "SettingsManager.h"
#include "AudioDefines.h"
//[/Headers]

#include "HardwareConfigurationComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
HardwareConfigurationComponent::HardwareConfigurationComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (inputsCountSlider = new Slider ("Inputs Count slider"));
    inputsCountSlider->setRange (1, 1024, 1);
    inputsCountSlider->setSliderStyle (Slider::IncDecButtons);
    inputsCountSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    inputsCountSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    inputsCountSlider->addListener (this);

    addAndMakeVisible (outputsCountSlider = new Slider ("Outputs Count slider"));
    outputsCountSlider->setRange (1, 1024, 1);
    outputsCountSlider->setSliderStyle (Slider::IncDecButtons);
    outputsCountSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    outputsCountSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    outputsCountSlider->addListener (this);

    addAndMakeVisible (inputsCountLabel = new Label ("Inputs Count label",
                                                     TRANS("Number of input channels:")));
    inputsCountLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    inputsCountLabel->setJustificationType (Justification::centredRight);
    inputsCountLabel->setEditable (false, false, false);
    inputsCountLabel->setColour (Label::textColourId, Colours::black);
    inputsCountLabel->setColour (TextEditor::textColourId, Colours::black);
    inputsCountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (outputsCountLabel = new Label ("outputs Count label",
                                                      TRANS("Number of output channels:")));
    outputsCountLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    outputsCountLabel->setJustificationType (Justification::centredRight);
    outputsCountLabel->setEditable (false, false, false);
    outputsCountLabel->setColour (Label::textColourId, Colours::black);
    outputsCountLabel->setColour (TextEditor::textColourId, Colours::black);
    outputsCountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (oscPluginToggleButton = new ToggleButton ("OSC Plugin toggle button"));
    oscPluginToggleButton->setButtonText (TRANS("Send OSC to Miam Matrix Router plug-in"));
    oscPluginToggleButton->addListener (this);
    oscPluginToggleButton->setToggleState (true, dontSendNotification);
    oscPluginToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    addAndMakeVisible (udpPortLabel = new Label ("UPD Port Label",
                                                 TRANS("Remote plug-in listening on UDP port:")));
    udpPortLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredRight);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (Label::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (udpPortTextEditor = new TextEditor ("UDP Port Text Editor"));
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setText (TRANS("-1"));

    addAndMakeVisible (keyboardButton = new ToggleButton ("keyboard button"));
    keyboardButton->setButtonText (TRANS("Edit routing matrices from keyboard"));
    keyboardButton->addListener (this);
    keyboardButton->setColour (ToggleButton::textColourId, Colours::black);


    //[UserPreSize]

    // Sliders max values from defines
    inputsCountSlider->setRange (1, Miam_MaxNumInputs, 1);
    outputsCountSlider->setRange (1, Miam_MaxNumOutputs, 1);
    
    // OSC plugin control is the only choice for now (defaultly activated)
    oscPluginToggleButton->setEnabled(false);

    //[/UserPreSize]

    setSize (1024, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

HardwareConfigurationComponent::~HardwareConfigurationComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    inputsCountSlider = nullptr;
    outputsCountSlider = nullptr;
    inputsCountLabel = nullptr;
    outputsCountLabel = nullptr;
    oscPluginToggleButton = nullptr;
    udpPortLabel = nullptr;
    udpPortTextEditor = nullptr;
    keyboardButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void HardwareConfigurationComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffafafaf));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void HardwareConfigurationComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    inputsCountSlider->setBounds ((getWidth() / 2) + 8, 16, 150, 24);
    outputsCountSlider->setBounds ((getWidth() / 2) + 8, 48, 150, 24);
    inputsCountLabel->setBounds ((getWidth() / 2) - 208, 16 + 0, 208, 24);
    outputsCountLabel->setBounds ((getWidth() / 2) - 208, 48 + 0, 208, 24);
    oscPluginToggleButton->setBounds ((getWidth() / 2) - (400 / 2), 96, 400, 24);
    udpPortLabel->setBounds ((getWidth() / 2) - 336, 120 + 0, 336, 24);
    udpPortTextEditor->setBounds ((getWidth() / 2) + 8, 120, 64, 24);
    keyboardButton->setBounds ((getWidth() / 2) - (400 / 2), getHeight() - 48, 400, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void HardwareConfigurationComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == inputsCountSlider)
    {
        //[UserSliderCode_inputsCountSlider] -- add your slider handling code here..
        settingsManager->OnInOutChannelsCountChanged((int) std::round(inputsCountSlider->getValue()), (int) std::round(outputsCountSlider->getValue()));
        //[/UserSliderCode_inputsCountSlider]
    }
    else if (sliderThatWasMoved == outputsCountSlider)
    {
        //[UserSliderCode_outputsCountSlider] -- add your slider handling code here..
        settingsManager->OnInOutChannelsCountChanged((int) std::round(inputsCountSlider->getValue()), (int) std::round(outputsCountSlider->getValue()));
        //[/UserSliderCode_outputsCountSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void HardwareConfigurationComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == oscPluginToggleButton)
    {
        //[UserButtonCode_oscPluginToggleButton] -- add your button handler code here..
        //[/UserButtonCode_oscPluginToggleButton]
    }
    else if (buttonThatWasClicked == keyboardButton)
    {
        //[UserButtonCode_keyboardButton] -- add your button handler code here..
        //[/UserButtonCode_keyboardButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void HardwareConfigurationComponent::CompleteInitialization(SettingsManager* _settingsManager)
{
    settingsManager = _settingsManager;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HardwareConfigurationComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="1024" initialHeight="600">
  <BACKGROUND backgroundColour="ffafafaf"/>
  <SLIDER name="Inputs Count slider" id="77ed5b9e29dce02e" memberName="inputsCountSlider"
          virtualName="" explicitFocusOrder="0" pos="8C 16 150 24" textboxtext="ff000000"
          min="1" max="1024" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="Outputs Count slider" id="ca10a4b3f2104985" memberName="outputsCountSlider"
          virtualName="" explicitFocusOrder="0" pos="8C 48 150 24" textboxtext="ff000000"
          min="1" max="1024" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Inputs Count label" id="5edb179087fb7973" memberName="inputsCountLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 208 24" posRelativeY="77ed5b9e29dce02e"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Number of input channels:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="34"/>
  <LABEL name="outputs Count label" id="5b4dd76a50f5f0d4" memberName="outputsCountLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 208 24" posRelativeY="ca10a4b3f2104985"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Number of output channels:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="34"/>
  <TOGGLEBUTTON name="OSC Plugin toggle button" id="74b5dae6c2ea74a2" memberName="oscPluginToggleButton"
                virtualName="" explicitFocusOrder="0" pos="0Cc 96 400 24" txtcol="ff000000"
                buttonText="Send OSC to Miam Matrix Router plug-in" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 336 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="e4ef4437203ce19e" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Remote plug-in listening on UDP port:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="34"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="8C 120 64 24" initialText="-1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="keyboard button" id="8c809b7ecaa8a037" memberName="keyboardButton"
                virtualName="" explicitFocusOrder="0" pos="0Cc 48R 400 24" posRelativeX="dfbb24a51fa3d6c0"
                posRelativeW="dfbb24a51fa3d6c0" txtcol="ff000000" buttonText="Edit routing matrices from keyboard"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
