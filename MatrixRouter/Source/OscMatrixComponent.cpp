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
#include "Presenter.h"
#include "MatrixComponent.h"
//[/Headers]

#include "OscMatrixComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
OscMatrixComponent::OscMatrixComponent (Presenter* _presenter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    presenter = _presenter;
    //[/Constructor_pre]

    addAndMakeVisible (oscGroupComponent = new GroupComponent ("OSC Group Component",
                                                               TRANS("OSC configuration")));
    oscGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    oscGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (udpPortTextEditor = new TextEditor ("UDP Port Text Editor"));
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setText (TRANS("-1"));

    addAndMakeVisible (udpPortLabel = new Label ("UPD Port Label",
                                                 TRANS("UDP port")));
    udpPortLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredLeft);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (Label::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (udpStatusLabel = new Label ("UPD Status Label",
                                                   TRANS("Status : ...")));
    udpStatusLabel->setFont (Font (15.00f, Font::italic));
    udpStatusLabel->setJustificationType (Justification::centredLeft);
    udpStatusLabel->setEditable (false, false, false);
    udpStatusLabel->setColour (Label::textColourId, Colours::black);
    udpStatusLabel->setColour (TextEditor::textColourId, Colours::black);
    udpStatusLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slidersMatrix = new Miam::LabelledMatrixComponent (this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels));
    slidersMatrix->setName ("Labelled Matrix of Sliders");

    addAndMakeVisible (preferencesComponent = new GroupComponent ("Preferences Group Component",
                                                                  TRANS("Preferences")));
    preferencesComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    preferencesComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (keyboardButton = new ToggleButton ("keyboard button"));
    keyboardButton->setButtonText (TRANS("Keyboard editing mode"));
    keyboardButton->addListener (this);
    keyboardButton->setColour (ToggleButton::textColourId, Colours::black);


    //[UserPreSize]

    // Not doable from the projucer
    udpPortTextEditor->addListener(this);

    // Black button tick (the contour will stay white)
    keyboardButton->setColour (ToggleButton::tickColourId, Colours::black);

    //[/UserPreSize]

    setSize (400, 300);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

OscMatrixComponent::~OscMatrixComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    oscGroupComponent = nullptr;
    udpPortTextEditor = nullptr;
    udpPortLabel = nullptr;
    udpStatusLabel = nullptr;
    slidersMatrix = nullptr;
    preferencesComponent = nullptr;
    keyboardButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OscMatrixComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffcacaca));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OscMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    oscGroupComponent->setBounds (8, 0, getWidth() - 264, 56);
    udpPortTextEditor->setBounds (88, 24, 64, 24);
    udpPortLabel->setBounds (16, 24, 72, 24);
    udpStatusLabel->setBounds (88 + 64 - -8, 24, (getWidth() - 264) - 160, 24);
    slidersMatrix->setBounds (8, 64, getWidth() - 16, getHeight() - 72);
    preferencesComponent->setBounds (getWidth() - 8 - 240, 0, 240, 56);
    keyboardButton->setBounds ((getWidth() - 8 - 240) + 8, 24, 240 - 16, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OscMatrixComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == keyboardButton)
    {
        //[UserButtonCode_keyboardButton] -- add your button handler code here..
        slidersMatrix->GetMatrixComponent()->SetSlidersTextBoxesAreEditable(keyboardButton->getToggleState());
        //[/UserButtonCode_keyboardButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OscMatrixComponent::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    // Comparaison avec l'adresse de l'objet référencé (et pas l'adresse de la
    // référence elle-même)
    if (&textEditor == udpPortTextEditor)
    {
        std::string udpText(textEditor.getText().toStdString());
        int parsedValue = -10;
        try
        {
            parsedValue = std::stoi(udpText);
        }
        catch (std::exception) // std::invalid_argument or std::out_of_range
        {
            textEditor.clear();
            return;
        }
        // At this point, the parse to int was successful.
        if (0 < parsedValue && parsedValue < 32768)
        {
            presenter->OnUdpPortChanged(parsedValue);
        }
    }
}

void OscMatrixComponent::OnSliderValueChanged(int row, int col, double value)
{
    presenter->OnSliderValueChanged(row, col, value);
}
void OscMatrixComponent::SetSliderValue(int row, int col, double value)
{
    slidersMatrix->GetMatrixComponent()->SetSliderValue(row, col, value);
}
void OscMatrixComponent::SetUdpPortAndMessage(int udpPort, bool isConnected)
{
    udpPortTextEditor->setText(std::to_string(udpPort), NotificationType::sendNotification);
    if (isConnected)
    {
        udpStatusLabel->setText("Connected on port " + std::to_string(udpPort) + ".", NotificationType::dontSendNotification);
        udpStatusLabel->setColour(Label::ColourIds::textColourId, Colours::black);
    }
    else
    {
        udpStatusLabel->setText("Cannot connect on port " + std::to_string(udpPort) + "!", NotificationType::dontSendNotification);
        udpStatusLabel->setColour(Label::ColourIds::textColourId, Colours::darkred);
    }
}
void OscMatrixComponent::SetActiveSliders(int inputsCount, int outputsCount)
{
    slidersMatrix->GetMatrixComponent()->SetActiveSliders(inputsCount, outputsCount);
    __DisplayDebugMsg("in=" + std::to_string(inputsCount) + " out=" + std::to_string(outputsCount));
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OscMatrixComponent" componentName=""
                 parentClasses="public Component, public ISlidersMatrixListener, public TextEditor::Listener"
                 constructorParams="Presenter* _presenter" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="300">
  <BACKGROUND backgroundColour="ffcacaca"/>
  <GROUPCOMPONENT name="OSC Group Component" id="dfbb24a51fa3d6c0" memberName="oscGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 0 264M 56" outlinecol="ff454545"
                  textcol="ff000000" title="OSC configuration"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="88 24 64 24" initialText="-1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="16 24 72 24" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="UDP port" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="UPD Status Label" id="3fd4c40c4d48dfec" memberName="udpStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="-8R 24 160M 24" posRelativeX="e4ef4437203ce19e"
         posRelativeW="dfbb24a51fa3d6c0" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Status : ..." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="1" justification="33"
         typefaceStyle="Italic"/>
  <GENERICCOMPONENT name="Labelled Matrix of Sliders" id="216c3aa5980dcb58" memberName="slidersMatrix"
                    virtualName="" explicitFocusOrder="0" pos="8 64 16M 72M" class="Miam::LabelledMatrixComponent"
                    params="this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels"/>
  <GROUPCOMPONENT name="Preferences Group Component" id="bb46950e139db507" memberName="preferencesComponent"
                  virtualName="" explicitFocusOrder="0" pos="8Rr 0 240 56" outlinecol="ff454545"
                  textcol="ff000000" title="Preferences"/>
  <TOGGLEBUTTON name="keyboard button" id="8c809b7ecaa8a037" memberName="keyboardButton"
                virtualName="" explicitFocusOrder="0" pos="8 24 16M 24" posRelativeX="bb46950e139db507"
                posRelativeW="bb46950e139db507" txtcol="ff000000" buttonText="Keyboard editing mode"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
