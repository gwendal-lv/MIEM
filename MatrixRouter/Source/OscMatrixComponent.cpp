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

    addAndMakeVisible (matrixGroupComponent = new GroupComponent ("Matrix Group Component",
                                                                  TRANS("Routing matrix")));
    matrixGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    matrixGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (preferencesGroupComponent = new GroupComponent ("Preferences Group Component",
                                                                       TRANS("Preferences")));
    preferencesGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    preferencesGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (udpPortTextEditor = new TextEditor ("UDP Port Text Editor"));
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setText (TRANS("-1"));

    addAndMakeVisible (udpPortLabel = new Label ("UPD Port Label",
                                                 TRANS("Listen to OSC on UDP port :")));
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

    addAndMakeVisible (audioConfigComponent = new GroupComponent ("Audio Config Group Component",
                                                                  TRANS("Audio dynamics")));
    audioConfigComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    audioConfigComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (keyboardButton = new ToggleButton ("keyboard button"));
    keyboardButton->setButtonText (TRANS("Keyboard editing mode"));
    keyboardButton->addListener (this);
    keyboardButton->setColour (ToggleButton::textColourId, Colours::black);

    addAndMakeVisible (slidersMatrix = new Miam::LabelledMatrixComponent (this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels));
    slidersMatrix->setName ("Labelled Matrix of Sliders");

    addAndMakeVisible (attackSlider = new Slider ("Attack Slider"));
    attackSlider->setRange (0.1, 100, 0.1);
    attackSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    attackSlider->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    attackSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    attackSlider->addListener (this);

    addAndMakeVisible (attackLabel = new Label ("Attack Label",
                                                TRANS("Attack")));
    attackLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    attackLabel->setJustificationType (Justification::centredLeft);
    attackLabel->setEditable (false, false, false);
    attackLabel->setColour (Label::textColourId, Colours::black);
    attackLabel->setColour (TextEditor::textColourId, Colours::black);
    attackLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (attackUnitLabel = new Label ("Attack unit Label",
                                                    TRANS("milliseconds")));
    attackUnitLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    attackUnitLabel->setJustificationType (Justification::centredLeft);
    attackUnitLabel->setEditable (false, false, false);
    attackUnitLabel->setColour (Label::textColourId, Colours::black);
    attackUnitLabel->setColour (TextEditor::textColourId, Colours::black);
    attackUnitLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


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

    matrixGroupComponent = nullptr;
    preferencesGroupComponent = nullptr;
    udpPortTextEditor = nullptr;
    udpPortLabel = nullptr;
    udpStatusLabel = nullptr;
    audioConfigComponent = nullptr;
    keyboardButton = nullptr;
    slidersMatrix = nullptr;
    attackSlider = nullptr;
    attackLabel = nullptr;
    attackUnitLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OscMatrixComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::silver);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OscMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    matrixGroupComponent->setBounds (0, 0 + 72, getWidth() - 0, getHeight() - 72);
    preferencesGroupComponent->setBounds (256, 0, getWidth() - 256, roundFloatToInt (72 * 1.0000f));
    udpPortTextEditor->setBounds ((256 + 8) + 184 - -8, 16, 64, 24);
    udpPortLabel->setBounds (256 + 8, 16, 184, 24);
    udpStatusLabel->setBounds (((256 + 8) + 184 - -8) + 64 - -8, 16, (getWidth() - 256) - 160, 24);
    audioConfigComponent->setBounds (0, 0, 248, 72);
    keyboardButton->setBounds (256 + 8, 40, (getWidth() - 256) - 24, 24);
    slidersMatrix->setBounds (0 + 8, (0 + 72) + 16, (getWidth() - 0) - 16, (getHeight() - 72) - 24);
    attackSlider->setBounds ((0 + 8) + 56 - 16, 16, 108, 48);
    attackLabel->setBounds (0 + 8, 28, 56, 24);
    attackUnitLabel->setBounds (((0 + 8) + 56 - 16) + 108, 28, 88, 24);
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

void OscMatrixComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == attackSlider)
    {
        //[UserSliderCode_attackSlider] -- add your slider handling code here..
        presenter->OnAttackDurationChanged(attackSlider->getValue());
        //[/UserSliderCode_attackSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
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
  <BACKGROUND backgroundColour="ffc0c0c0"/>
  <GROUPCOMPONENT name="Matrix Group Component" id="19b69873bd3945f7" memberName="matrixGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0R 0M 72M" posRelativeY="bb46950e139db507"
                  outlinecol="ff454545" textcol="ff000000" title="Routing matrix"/>
  <GROUPCOMPONENT name="Preferences Group Component" id="dfbb24a51fa3d6c0" memberName="preferencesGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="256 0 256M 100%" posRelativeH="bb46950e139db507"
                  outlinecol="ff454545" textcol="ff000000" title="Preferences"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="-8R 16 64 24" posRelativeX="8d369e08975b779c"
              initialText="-1" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="8 16 184 24" posRelativeX="dfbb24a51fa3d6c0"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Listen to OSC on UDP port :"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <LABEL name="UPD Status Label" id="3fd4c40c4d48dfec" memberName="udpStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="-8R 16 160M 24" posRelativeX="e4ef4437203ce19e"
         posRelativeW="dfbb24a51fa3d6c0" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Status : ..." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="1" justification="33"
         typefaceStyle="Italic"/>
  <GROUPCOMPONENT name="Audio Config Group Component" id="bb46950e139db507" memberName="audioConfigComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0 248 72" outlinecol="ff454545"
                  textcol="ff000000" title="Audio dynamics"/>
  <TOGGLEBUTTON name="keyboard button" id="8c809b7ecaa8a037" memberName="keyboardButton"
                virtualName="" explicitFocusOrder="0" pos="8 40 24M 24" posRelativeX="dfbb24a51fa3d6c0"
                posRelativeW="dfbb24a51fa3d6c0" txtcol="ff000000" buttonText="Keyboard editing mode"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="Labelled Matrix of Sliders" id="216c3aa5980dcb58" memberName="slidersMatrix"
                    virtualName="" explicitFocusOrder="0" pos="8 16 16M 24M" posRelativeX="19b69873bd3945f7"
                    posRelativeY="19b69873bd3945f7" posRelativeW="19b69873bd3945f7"
                    posRelativeH="19b69873bd3945f7" class="Miam::LabelledMatrixComponent"
                    params="this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels"/>
  <SLIDER name="Attack Slider" id="b96b5d59dd56bbaa" memberName="attackSlider"
          virtualName="" explicitFocusOrder="0" pos="16R 16 108 48" posRelativeX="19707b3f29742e60"
          textboxtext="ff000000" min="0.10000000000000000555" max="100"
          int="0.10000000000000000555" style="RotaryVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Attack Label" id="19707b3f29742e60" memberName="attackLabel"
         virtualName="" explicitFocusOrder="0" pos="8 28 56 24" posRelativeX="bb46950e139db507"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Attack"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <LABEL name="Attack unit Label" id="c34a141f13007692" memberName="attackUnitLabel"
         virtualName="" explicitFocusOrder="0" pos="0R 28 88 24" posRelativeX="b96b5d59dd56bbaa"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="milliseconds"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
