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

//[Headers] You can add your own extra header files here...
#include <cmath>

#include "SettingsManager.h"
#include "AudioDefines.h"
#include "InterpolationTypes.h"

#include "XmlUtils.h"
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

    inputsCountSlider.reset (new Slider ("Inputs Count slider"));
    addAndMakeVisible (inputsCountSlider.get());
    inputsCountSlider->setTooltip (TRANS("Number of lines of the matrix"));
    inputsCountSlider->setRange (1, 1024, 1);
    inputsCountSlider->setSliderStyle (Slider::IncDecButtons);
    inputsCountSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    inputsCountSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    inputsCountSlider->addListener (this);

    outputsCountSlider.reset (new Slider ("Outputs Count slider"));
    addAndMakeVisible (outputsCountSlider.get());
    outputsCountSlider->setTooltip (TRANS("Number of columns of the matrix"));
    outputsCountSlider->setRange (1, 1024, 1);
    outputsCountSlider->setSliderStyle (Slider::IncDecButtons);
    outputsCountSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    outputsCountSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    outputsCountSlider->addListener (this);

    inputsCountLabel.reset (new Label ("Inputs Count label",
                                       TRANS("Number of input channels:")));
    addAndMakeVisible (inputsCountLabel.get());
    inputsCountLabel->setTooltip (TRANS("Number of lines of the matrix"));
    inputsCountLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    inputsCountLabel->setJustificationType (Justification::centredRight);
    inputsCountLabel->setEditable (false, false, false);
    inputsCountLabel->setColour (Label::textColourId, Colours::black);
    inputsCountLabel->setColour (TextEditor::textColourId, Colours::black);
    inputsCountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    outputsCountLabel.reset (new Label ("outputs Count label",
                                        TRANS("Number of output channels:")));
    addAndMakeVisible (outputsCountLabel.get());
    outputsCountLabel->setTooltip (TRANS("Number of columns of the matrix"));
    outputsCountLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    outputsCountLabel->setJustificationType (Justification::centredRight);
    outputsCountLabel->setEditable (false, false, false);
    outputsCountLabel->setColour (Label::textColourId, Colours::black);
    outputsCountLabel->setColour (TextEditor::textColourId, Colours::black);
    outputsCountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    oscPluginToggleButton.reset (new ToggleButton ("OSC Plugin toggle button"));
    addAndMakeVisible (oscPluginToggleButton.get());
    oscPluginToggleButton->setButtonText (TRANS("Send OSC to MIEM Matrix Router remote plug-in"));
    oscPluginToggleButton->setToggleState (true, dontSendNotification);
    oscPluginToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    udpPortLabel.reset (new Label ("UPD Port Label",
                                   TRANS("Plug-in listening on UDP port:")));
    addAndMakeVisible (udpPortLabel.get());
    udpPortLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredRight);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (Label::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    udpPortTextEditor.reset (new TextEditor ("UDP Port Text Editor"));
    addAndMakeVisible (udpPortTextEditor.get());
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setText (TRANS("8001"));

    keyboardToggleButton.reset (new ToggleButton ("Keyboard toggle button"));
    addAndMakeVisible (keyboardToggleButton.get());
    keyboardToggleButton->setButtonText (TRANS("Edit routing matrices from keyboard"));
    keyboardToggleButton->addListener (this);
    keyboardToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    ipAddressLabel.reset (new Label ("Ip Address Label",
                                     TRANS("Plug-in host IP address:")));
    addAndMakeVisible (ipAddressLabel.get());
    ipAddressLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    ipAddressLabel->setJustificationType (Justification::centredRight);
    ipAddressLabel->setEditable (false, false, false);
    ipAddressLabel->setColour (Label::textColourId, Colours::black);
    ipAddressLabel->setColour (TextEditor::textColourId, Colours::black);
    ipAddressLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ipAddressTextEditor.reset (new TextEditor ("IP Address Text Editor"));
    addAndMakeVisible (ipAddressTextEditor.get());
    ipAddressTextEditor->setMultiLine (false);
    ipAddressTextEditor->setReturnKeyStartsNewLine (false);
    ipAddressTextEditor->setReadOnly (false);
    ipAddressTextEditor->setScrollbarsShown (true);
    ipAddressTextEditor->setCaretVisible (true);
    ipAddressTextEditor->setPopupMenuEnabled (true);
    ipAddressTextEditor->setText (TRANS("127.0.0.1"));

    inputNamesToggleButton.reset (new ToggleButton ("Input Names toggle button"));
    addAndMakeVisible (inputNamesToggleButton.get());
    inputNamesToggleButton->setButtonText (TRANS("Display names"));
    inputNamesToggleButton->addListener (this);
    inputNamesToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    outputNamesToggleButton.reset (new ToggleButton ("Output Names toggle button"));
    addAndMakeVisible (outputNamesToggleButton.get());
    outputNamesToggleButton->setButtonText (TRANS("Display names"));
    outputNamesToggleButton->addListener (this);
    outputNamesToggleButton->setColour (ToggleButton::textColourId, Colours::black);

    interpolationTypeComboBox.reset (new ComboBox ("Interpolation Type combo box"));
    addAndMakeVisible (interpolationTypeComboBox.get());
    interpolationTypeComboBox->setEditableText (false);
    interpolationTypeComboBox->setJustificationType (Justification::centredLeft);
    interpolationTypeComboBox->setTextWhenNothingSelected (String());
    interpolationTypeComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    interpolationTypeComboBox->addListener (this);

    interpolationTypeLabel.reset (new Label ("Interpolation Type label",
                                             TRANS("Type of interpolation:")));
    addAndMakeVisible (interpolationTypeLabel.get());
    interpolationTypeLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    interpolationTypeLabel->setJustificationType (Justification::centredRight);
    interpolationTypeLabel->setEditable (false, false, false);
    interpolationTypeLabel->setColour (Label::textColourId, Colours::black);
    interpolationTypeLabel->setColour (TextEditor::textColourId, Colours::black);
    interpolationTypeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    infoHyperlinkButton.reset (new HyperlinkButton (TRANS("To get help and more information, please visit http://miem.laras.be"),
                                                    URL ("http://miem.laras.be")));
    addAndMakeVisible (infoHyperlinkButton.get());
    infoHyperlinkButton->setTooltip (TRANS("http://miem.laras.be"));
    infoHyperlinkButton->setButtonText (TRANS("To get help and more information, please visit http://miem.laras.be"));
    infoHyperlinkButton->setColour (HyperlinkButton::textColourId, Colours::white);

    enableMasterVolumeToggleButton.reset (new ToggleButton ("Enable Master Volume toggle button"));
    addAndMakeVisible (enableMasterVolumeToggleButton.get());
    enableMasterVolumeToggleButton->setTooltip (TRANS("In MIEM Spat, the Master Volume can be applied to the whole routing matrix for a global control."));
    enableMasterVolumeToggleButton->setButtonText (TRANS("Enable Master Volume fader in MIEM Spat"));
    enableMasterVolumeToggleButton->addListener (this);
    enableMasterVolumeToggleButton->setColour (ToggleButton::textColourId, Colours::black);


    //[UserPreSize]

    // Sauvegarde des valeurs SPAT par défaut pour l'instant dans le Projucer
    inputLabelSpatText = inputsCountLabel->getText();
    inputLabelGenericText = TRANS("Number of parameters:");
    oscTargetSpatText = oscPluginToggleButton->getButtonText();
    oscTargetGenericText = TRANS("Send OSC control data to target device");

    // Sliders max values from defines
    inputsCountSlider->setRange (1, Miam_MaxNumInputs, 1);
    outputsCountSlider->setRange (1, Miam_MaxNumOutputs, 1);

    // Remplissage de la combobox interpolation, sans choix par défaut
    // Le zéro doit être "aucun interpolateur" pour correspondre aux indices des combobox....
    assert((int)(InterpolationType::None) == 0);
    for (int i=1 ; i < (int)InterpolationType::InterpolationTypesCount ; i++)
        interpolationTypeComboBox->addItem(InterpolationTypes::GetInterpolationName((InterpolationType)i),
                                           i);

    // OSC plugin control is the only choice for now (defaultly activated)
    oscPluginToggleButton->setEnabled(false);

    // listeners
    udpPortTextEditor->addListener(this);
    ipAddressTextEditor->addListener(this);

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
    keyboardToggleButton = nullptr;
    ipAddressLabel = nullptr;
    ipAddressTextEditor = nullptr;
    inputNamesToggleButton = nullptr;
    outputNamesToggleButton = nullptr;
    interpolationTypeComboBox = nullptr;
    interpolationTypeLabel = nullptr;
    infoHyperlinkButton = nullptr;
    enableMasterVolumeToggleButton = nullptr;


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

    inputsCountSlider->setBounds ((getWidth() / 2) + -8 - 150, 16, 150, 24);
    outputsCountSlider->setBounds ((getWidth() / 2) + -8 - 150, 48, 150, 24);
    inputsCountLabel->setBounds ((getWidth() / 2) + -156 - 208, 16 + 0, 208, 24);
    outputsCountLabel->setBounds ((getWidth() / 2) + -156 - 208, 48 + 0, 208, 24);
    oscPluginToggleButton->setBounds ((getWidth() / 2) - (400 / 2), 200, 400, 24);
    udpPortLabel->setBounds ((getWidth() / 2) - 336, 264 + 0, 336, 24);
    udpPortTextEditor->setBounds ((getWidth() / 2) + 8, 264, 64, 24);
    keyboardToggleButton->setBounds ((getWidth() / 2) - (400 / 2), getHeight() - 120, 400, 24);
    ipAddressLabel->setBounds ((getWidth() / 2) - 336, 232 + 0, 336, 24);
    ipAddressTextEditor->setBounds ((getWidth() / 2) + 8, 232, 120, 24);
    inputNamesToggleButton->setBounds ((getWidth() / 2) + 8, 16, 400, 24);
    outputNamesToggleButton->setBounds ((getWidth() / 2) + 8, 48, 400, 24);
    interpolationTypeComboBox->setBounds ((getWidth() / 2) + 8, 104, 320, 24);
    interpolationTypeLabel->setBounds ((getWidth() / 2) - 336, 104 + 0, 336, 24);
    infoHyperlinkButton->setBounds ((getWidth() / 2) - ((getWidth() - 16) / 2), getHeight() - 32, getWidth() - 16, 24);
    enableMasterVolumeToggleButton->setBounds ((getWidth() / 2) - (400 / 2), 150, 400, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void HardwareConfigurationComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == inputsCountSlider.get())
    {
        //[UserSliderCode_inputsCountSlider] -- add your slider handling code here..
        settingsManager->OnInOutChannelsCountChanged((int) std::round(inputsCountSlider->getValue()), (int) std::round(outputsCountSlider->getValue()));
        //[/UserSliderCode_inputsCountSlider]
    }
    else if (sliderThatWasMoved == outputsCountSlider.get())
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

    if (buttonThatWasClicked == keyboardToggleButton.get())
    {
        //[UserButtonCode_keyboardToggleButton] -- add your button handler code here..
        settingsManager->OnAllowKeyboardEdition(keyboardToggleButton->getToggleState());
        //[/UserButtonCode_keyboardToggleButton]
    }
    else if (buttonThatWasClicked == inputNamesToggleButton.get())
    {
        //[UserButtonCode_inputNamesToggleButton] -- add your button handler code here..
        settingsManager->OnInOutNamesDisplayedChanged(inputNamesToggleButton->getToggleState(),
                                                      outputNamesToggleButton->getToggleState());
        //[/UserButtonCode_inputNamesToggleButton]
    }
    else if (buttonThatWasClicked == outputNamesToggleButton.get())
    {
        //[UserButtonCode_outputNamesToggleButton] -- add your button handler code here..
        settingsManager->OnInOutNamesDisplayedChanged(inputNamesToggleButton->getToggleState(),
                                                      outputNamesToggleButton->getToggleState());
        //[/UserButtonCode_outputNamesToggleButton]
    }
    else if (buttonThatWasClicked == enableMasterVolumeToggleButton.get())
    {
        //[UserButtonCode_enableMasterVolumeToggleButton] -- add your button handler code here..
        settingsManager->OnMasterGainEnabledChanged(enableMasterVolumeToggleButton->getToggleState());
        //[/UserButtonCode_enableMasterVolumeToggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void HardwareConfigurationComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == interpolationTypeComboBox.get())
    {
        //[UserComboBoxCode_interpolationTypeComboBox] -- add your combo box handling code here..
        int chosenId = interpolationTypeComboBox->getSelectedId();
        if (0 < chosenId && chosenId < (int)InterpolationType::InterpolationTypesCount)
            settingsManager->OnInterpolationTypeChanged((InterpolationType)chosenId);
        //[/UserComboBoxCode_interpolationTypeComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void HardwareConfigurationComponent::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...

    // Selon le mode : on affiche ou non les paramètres d'E/S
    if (settingsManager)
    {
        switch (settingsManager->GetSessionPurpose())
        {
            case AppPurpose::Spatialisation :
                inputsCountLabel->setText(inputLabelSpatText, NotificationType::sendNotification);
                oscPluginToggleButton->setButtonText(oscTargetSpatText);
                outputsCountLabel->setVisible(true);
                outputsCountSlider->setVisible(true);
                outputNamesToggleButton->setVisible(true);
                // input name toggle :  coché, mais garde la valeur d'activation précédente
                inputNamesToggleButton->setEnabled(true);
                break;

            case AppPurpose::GenericController :
                inputsCountLabel->setText(inputLabelGenericText, NotificationType::sendNotification);
                oscPluginToggleButton->setButtonText(oscTargetGenericText);
                outputsCountLabel->setVisible(false);
                outputsCountSlider->setVisible(false);
                outputNamesToggleButton->setVisible(false);
                // input name toggle : coché mais désactivé
                inputNamesToggleButton->setToggleState(true, dontSendNotification);
                inputNamesToggleButton->setEnabled(false);
                break;

            default :
                break;
        }
    }

    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void HardwareConfigurationComponent::textEditorTextChanged(TextEditor& editorThatHasChanged)
{
    if (&editorThatHasChanged == ipAddressTextEditor.get())
    {
        TryParseIpAddress();
    }
    else if (&editorThatHasChanged == udpPortTextEditor.get())
    {
        TryParseUdpPort();
    }
}
std::string HardwareConfigurationComponent::TryParseIpAddress()
{
    std::string ipAddress = ipAddressTextEditor->getText().toStdString();
    if ( XmlUtils::IsIpv4AddressValid(ipAddress) ) // en gras
    {
        ipAddressTextEditor->applyFontToAllText(Font().boldened());
        //ipAddressTextEditor->setColour(TextEditor::ColourIds::textColourId, Colours::palegreen);
        return ipAddress;
    }
    else
    {
        ipAddressTextEditor->applyFontToAllText(Font()); // font par défaut si c'est pas bon
        //ipAddressTextEditor->setColour(TextEditor::ColourIds::textColourId, Colours::white);
        return "";
    }
}
int HardwareConfigurationComponent::TryParseUdpPort()
{
    bool enteredValueIsCorrect = true;
    int parsedValue = -1;
    try {
        parsedValue = std::stoi(udpPortTextEditor->getText().toStdString());
    } catch (std::exception) {
        enteredValueIsCorrect = false;
    }
    if (parsedValue <= 0 || 65535 < parsedValue)
        enteredValueIsCorrect = false;

    if ( enteredValueIsCorrect ) // en gras
    {
        udpPortTextEditor->applyFontToAllText(Font().boldened());
        return parsedValue;
    }
    else
    {
        udpPortTextEditor->applyFontToAllText(Font()); // font par défaut si c'est pas bon
        return -1;
    }
}
void HardwareConfigurationComponent::SetAvailableInterpolations(std::initializer_list<InterpolationType> interpolationTypeArgs)
{
    // Par défaut, tout est considéré désactivé
    std::vector<bool> isInterpolationAvailable;
    isInterpolationAvailable.resize((size_t)InterpolationType::InterpolationTypesCount, false);
    // Recherche des types à activer
    // (le ::None est le numéro zéro)
    for (auto interpolationType : interpolationTypeArgs)
    {
        int typeAsInteger = (int)interpolationType;
        if (interpolationType == InterpolationType::Any)
        {
            for (size_t i=0; i<isInterpolationAvailable.size() ; i++)
                isInterpolationAvailable[i] = true;
            break;
        }
        else if (1 <= typeAsInteger && typeAsInteger < (int)InterpolationType::InterpolationTypesCount)
            isInterpolationAvailable[typeAsInteger] = true;
    }
    // Activation des bons éléments de la liste
    // (le ::None est le numéro zéro)
    for (int i=1 ; i<(int)isInterpolationAvailable.size() ; i++)
        interpolationTypeComboBox->setItemEnabled(i, isInterpolationAvailable[i]);
}
void HardwareConfigurationComponent::SetInterpolationChoiceVisible(bool shouldBeVisible)
{
    interpolationTypeComboBox->setVisible(shouldBeVisible);
    interpolationTypeLabel->setVisible(shouldBeVisible);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HardwareConfigurationComponent"
                 componentName="" parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="1024"
                 initialHeight="600">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffafafaf"/>
  <SLIDER name="Inputs Count slider" id="77ed5b9e29dce02e" memberName="inputsCountSlider"
          virtualName="" explicitFocusOrder="0" pos="-8Cr 16 150 24" tooltip="Number of lines of the matrix"
          textboxtext="ff000000" min="1" max="1.024e3" int="1" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="Outputs Count slider" id="ca10a4b3f2104985" memberName="outputsCountSlider"
          virtualName="" explicitFocusOrder="0" pos="-8Cr 48 150 24" tooltip="Number of columns of the matrix"
          textboxtext="ff000000" min="1" max="1.024e3" int="1" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="Inputs Count label" id="5edb179087fb7973" memberName="inputsCountLabel"
         virtualName="" explicitFocusOrder="0" pos="-156Cr 0 208 24" posRelativeY="77ed5b9e29dce02e"
         tooltip="Number of lines of the matrix" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Number of input channels:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="34"/>
  <LABEL name="outputs Count label" id="5b4dd76a50f5f0d4" memberName="outputsCountLabel"
         virtualName="" explicitFocusOrder="0" pos="-156Cr 0 208 24" posRelativeY="ca10a4b3f2104985"
         tooltip="Number of columns of the matrix" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Number of output channels:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="34"/>
  <TOGGLEBUTTON name="OSC Plugin toggle button" id="74b5dae6c2ea74a2" memberName="oscPluginToggleButton"
                virtualName="" explicitFocusOrder="0" pos="0Cc 200 400 24" txtcol="ff000000"
                buttonText="Send OSC to MIEM Matrix Router remote plug-in" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="1"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 336 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="e4ef4437203ce19e" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Plug-in listening on UDP port:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="8C 264 64 24" initialText="8001"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="Keyboard toggle button" id="8c809b7ecaa8a037" memberName="keyboardToggleButton"
                virtualName="" explicitFocusOrder="0" pos="0Cc 120R 400 24" posRelativeX="dfbb24a51fa3d6c0"
                posRelativeW="dfbb24a51fa3d6c0" txtcol="ff000000" buttonText="Edit routing matrices from keyboard"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="Ip Address Label" id="2066f0f6ef12dcf0" memberName="ipAddressLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 336 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="6997b5b4dc28675a" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Plug-in host IP address:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="IP Address Text Editor" id="6997b5b4dc28675a" memberName="ipAddressTextEditor"
              virtualName="" explicitFocusOrder="0" pos="8C 232 120 24" initialText="127.0.0.1"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="Input Names toggle button" id="b7d5b33136328768" memberName="inputNamesToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8C 16 400 24" txtcol="ff000000"
                buttonText="Display names" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="Output Names toggle button" id="ba9f950d6ef902d9" memberName="outputNamesToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8C 48 400 24" txtcol="ff000000"
                buttonText="Display names" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <COMBOBOX name="Interpolation Type combo box" id="900a9258dc365ad0" memberName="interpolationTypeComboBox"
            virtualName="" explicitFocusOrder="0" pos="8C 104 320 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="Interpolation Type label" id="184dbfa5837bdd41" memberName="interpolationTypeLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 0 336 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="900a9258dc365ad0" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Type of interpolation:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="34"/>
  <HYPERLINKBUTTON name="Info hyperlink button" id="fa3a8802c3b7c7f0" memberName="infoHyperlinkButton"
                   virtualName="" explicitFocusOrder="0" pos="0.5Cc 32R 16M 24"
                   tooltip="http://miem.laras.be" textCol="ffffffff" buttonText="To get help and more information, please visit http://miem.laras.be"
                   connectedEdges="0" needsCallback="0" radioGroupId="0" url="http://miem.laras.be"/>
  <TOGGLEBUTTON name="Enable Master Volume toggle button" id="b650c1c657fd4f2e"
                memberName="enableMasterVolumeToggleButton" virtualName="" explicitFocusOrder="0"
                pos="0Cc 150 400 24" posRelativeX="dfbb24a51fa3d6c0" posRelativeW="dfbb24a51fa3d6c0"
                tooltip="In MIEM Spat, the Master Volume can be applied to the whole routing matrix for a global control."
                txtcol="ff000000" buttonText="Enable Master Volume fader in MIEM Spat"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

