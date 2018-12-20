/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Presenter.h"
#include "MatrixComponent.h"


#ifdef __MIAM_DEBUG
#include "PluginProcessor.h" // pour le débuggueur OSC
#endif


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

    matrixGroupComponent.reset (new GroupComponent ("Matrix Group Component",
                                                    TRANS("Routing matrix")));
    addAndMakeVisible (matrixGroupComponent.get());
    matrixGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    matrixGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    slidersMatrix.reset (new Miam::LabelledMatrixComponent (this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels));
    addAndMakeVisible (slidersMatrix.get());
    slidersMatrix->setName ("Labelled Matrix of Sliders");

    preferencesGroupComponent.reset (new GroupComponent ("Preferences Group Component",
                                                         TRANS("Preferences")));
    addAndMakeVisible (preferencesGroupComponent.get());
    preferencesGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    preferencesGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    udpPortTextEditor.reset (new TextEditor ("UDP Port Text Editor"));
    addAndMakeVisible (udpPortTextEditor.get());
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setText (TRANS("-1"));

    udpPortLabel.reset (new Label ("UPD Port Label",
                                   TRANS("Listen to OSC on UDP port:")));
    addAndMakeVisible (udpPortLabel.get());
    udpPortLabel->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredLeft);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (Label::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    udpPortLabel->setBounds (216 + 8, 16, 184, 24);

    udpStatusLabel.reset (new Label ("UPD Status Label",
                                     TRANS("Status : ...")));
    addAndMakeVisible (udpStatusLabel.get());
    udpStatusLabel->setFont (Font (15.0f, Font::italic));
    udpStatusLabel->setJustificationType (Justification::centredLeft);
    udpStatusLabel->setEditable (false, false, false);
    udpStatusLabel->setColour (Label::textColourId, Colours::black);
    udpStatusLabel->setColour (TextEditor::textColourId, Colours::black);
    udpStatusLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    audioConfigComponent.reset (new GroupComponent ("Audio Config Group Component",
                                                    TRANS("Audio dynamics")));
    addAndMakeVisible (audioConfigComponent.get());
    audioConfigComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    audioConfigComponent->setColour (GroupComponent::textColourId, Colours::black);

    audioConfigComponent->setBounds (0, 0, 208, 72);

    keyboardButton.reset (new ToggleButton ("keyboard button"));
    addAndMakeVisible (keyboardButton.get());
    keyboardButton->setButtonText (TRANS("Keyboard editing mode"));
    keyboardButton->addListener (this);
    keyboardButton->setColour (ToggleButton::textColourId, Colours::black);

    attackSlider.reset (new Slider ("Attack Slider"));
    addAndMakeVisible (attackSlider.get());
    attackSlider->setRange (1, 100, 1);
    attackSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    attackSlider->setTextBoxStyle (Slider::TextBoxRight, false, 60, 20);
    attackSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    attackSlider->addListener (this);

    attackLabel.reset (new Label ("Attack Label",
                                  TRANS("Attack")));
    addAndMakeVisible (attackLabel.get());
    attackLabel->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    attackLabel->setJustificationType (Justification::centredLeft);
    attackLabel->setEditable (false, false, false);
    attackLabel->setColour (Label::textColourId, Colours::black);
    attackLabel->setColour (TextEditor::textColourId, Colours::black);
    attackLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    attackLabel->setBounds (0 + 8, 28, 56, 24);

    attackUnitLabel.reset (new Label ("Attack unit Label",
                                      TRANS("ms")));
    addAndMakeVisible (attackUnitLabel.get());
    attackUnitLabel->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    attackUnitLabel->setJustificationType (Justification::centredLeft);
    attackUnitLabel->setEditable (false, false, false);
    attackUnitLabel->setColour (Label::textColourId, Colours::black);
    attackUnitLabel->setColour (TextEditor::textColourId, Colours::black);
    attackUnitLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    helpTextButton.reset (new TextButton ("Help button"));
    addAndMakeVisible (helpTextButton.get());
    helpTextButton->setButtonText (TRANS("?"));
    helpTextButton->addListener (this);
    helpTextButton->setColour (TextButton::buttonColourId, Colour (0x3e000000));
    helpTextButton->setColour (TextButton::textColourOffId, Colours::black);

    connectTextButton.reset (new TextButton ("Connect text button"));
    addAndMakeVisible (connectTextButton.get());
    connectTextButton->setButtonText (TRANS("Connect"));
    connectTextButton->addListener (this);
    connectTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    connectTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    connectTextButton->setColour (TextButton::textColourOffId, Colours::black);

    connectTextButton->setBounds (456, 16, 88, 24);


    //[UserPreSize]

    // Not doable from the projucer
    udpPortTextEditor->setInputRestrictions(5, "0123456789"); // le '-' n'est pas compris....
    udpPortTextEditor->addListener(this);

    // Black button tick (the contour will stay white)
    keyboardButton->setColour (ToggleButton::tickColourId, Colours::black);

    //[/UserPreSize]

    setSize (400, 300);


    //[Constructor] You can add your own custom stuff here..

    //Initially disabled (will be re-enabled on timeout from Model)
    //setEnabled(false);

    // Matrice de sliders : pour l'instant, type matriciel avec noms de E/S caché
    slidersMatrix->SetDisplayPurpose(App::GetPurpose());
    slidersMatrix->SetInputNamesVisible(false);
    slidersMatrix->SetOutputNamesVisible(false);

    //[/Constructor]
}

OscMatrixComponent::~OscMatrixComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    matrixGroupComponent = nullptr;
    slidersMatrix = nullptr;
    preferencesGroupComponent = nullptr;
    udpPortTextEditor = nullptr;
    udpPortLabel = nullptr;
    udpStatusLabel = nullptr;
    audioConfigComponent = nullptr;
    keyboardButton = nullptr;
    attackSlider = nullptr;
    attackLabel = nullptr;
    attackUnitLabel = nullptr;
    helpTextButton = nullptr;
    connectTextButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OscMatrixComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff909090));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OscMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    matrixGroupComponent->setBounds (0, 0 + 72, getWidth() - 0, getHeight() - 72);
    slidersMatrix->setBounds (0 + 8, (0 + 72) + 16, (getWidth() - 0) - 16, (getHeight() - 72) - 24);
    preferencesGroupComponent->setBounds (216, 0, getWidth() - 251, 72);
    udpPortTextEditor->setBounds ((216 + 8) + 184 - 8, 16, 48, 24);
    udpStatusLabel->setBounds (((216 + 8) + 184 - 8) + 48 - -104, 16, (getWidth() - 251) - 357, 24);
    keyboardButton->setBounds (216 + 8, 40, (getWidth() - 251) - 24, 24);
    attackSlider->setBounds ((0 + 8) + 56 - 8, 16, 112, 48);
    attackUnitLabel->setBounds (((0 + 8) + 56 - 8) + 112, 28, 88, 24);
    helpTextButton->setBounds (getWidth() - 4 - 24, 8, 24, 60);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OscMatrixComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == keyboardButton.get())
    {
        //[UserButtonCode_keyboardButton] -- add your button handler code here..
        slidersMatrix->GetMatrixComponent()->SetSlidersTextBoxesAreEditable(keyboardButton->getToggleState());
        //[/UserButtonCode_keyboardButton]
    }
    else if (buttonThatWasClicked == helpTextButton.get())
    {
        //[UserButtonCode_helpTextButton] -- add your button handler code here..
        PopupMenu menu = createHelpPopup();
        menu.show();
        //[/UserButtonCode_helpTextButton]
    }
    else if (buttonThatWasClicked == connectTextButton.get())
    {
        //[UserButtonCode_connectTextButton] -- add your button handler code here..
        parseUdpPortAndSendEvent();
        //[/UserButtonCode_connectTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void OscMatrixComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == attackSlider.get())
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
    if (&textEditor == udpPortTextEditor.get())
    {
        parseUdpPortAndSendEvent();
    }
}
void OscMatrixComponent::textEditorTextChanged(TextEditor& textEditor)
{
    // Comparaison avec l'adresse de l'objet référencé (et pas l'adresse de la
    // référence elle-même)
    if (&textEditor == udpPortTextEditor.get())
    {        // SI le texte a changé : on autorise une future connection (bouton apparent)
        connectTextButton->setEnabled(true);
    }
}
void OscMatrixComponent::parseUdpPortAndSendEvent()
{
    std::string udpText(udpPortTextEditor->getText().toStdString());
    int parsedValue = -10;
    try {
        parsedValue = std::stoi(udpText);
    }
    catch (std::exception) { // std::invalid_argument or std::out_of_range
        udpPortTextEditor->clear();
        return;
    }
    // At this point, the parse to int was successful.
    if (0 < parsedValue && parsedValue < 32768)
    {
        presenter->OnUdpPortChanged(parsedValue);
        // On en désactive pas tout de suite le bouton( on attend le retour de presenter)
    }
}

void OscMatrixComponent::OnSliderValueChanged(int row, int col, double value)
{
    presenter->OnSliderValueChanged(row, col, value);
}
void OscMatrixComponent::OnMatrixZeroed()
{
    presenter->OnMatrixZeroed();
}
void OscMatrixComponent::SetSliderValue(int row, int col, double value)
{
    slidersMatrix->GetMatrixComponent()->SetSliderValue(row, col, value);
}
void OscMatrixComponent::SetAttackSliderValue(double value)
{
    attackSlider->setValue(value, NotificationType::dontSendNotification);
}
void OscMatrixComponent::SetUdpPortAndMessage(int udpPort, bool isConnected, std::string& oscAddress)
{
    udpPortTextEditor->setText(std::to_string(udpPort), NotificationType::dontSendNotification);
    if (isConnected)
    {
        udpStatusLabel->setText("Listening to " + oscAddress + " messages on port " + std::to_string(udpPort) + ".", NotificationType::sendNotification);
        udpStatusLabel->setColour(Label::ColourIds::textColourId, Colours::black);
        connectTextButton->setEnabled(false);
    }
    else
    {
        udpStatusLabel->setText("Cannot connect on port " + std::to_string(udpPort) + "!", NotificationType::sendNotification);
        udpStatusLabel->setColour(Label::ColourIds::textColourId, Colours::darkred);
        connectTextButton->setEnabled(true);
    }
}
void OscMatrixComponent::SetActiveSliders(int inputsCount, int outputsCount)
{
    slidersMatrix->SetActiveSliders(inputsCount, outputsCount);
}
PopupMenu OscMatrixComponent::createHelpPopup()
{
    PopupMenu menu;
    int lastItemId = 0;
    // Direct interaction at first
    menu.addItem (++lastItemId, "Right-click on matrix to show reset options.", false);
    // OSC commands available then
    menu.addSeparator();
    for (size_t i=0 ; i<networkHelpContent.size() ; i++)
        menu.addItem (++lastItemId, networkHelpContent[i].c_str(), false);
    // Automation advice
    menu.addSeparator();
    menu.addItem(++lastItemId, "DAW-specific instructions : please read PDF manual.", false);
    menu.addSeparator();
    menu.addItem(++lastItemId, String("MIEM Matrix Router v") + JucePlugin_VersionString, false);
    menu.addItem(++lastItemId, String("Gwendal Le Vaillant, ") + JucePlugin_ManufacturerEmail + ", 2018.", false);


    return menu;
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
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.33"
                 fixedSize="0" initialWidth="400" initialHeight="300">
  <BACKGROUND backgroundColour="ff909090"/>
  <GROUPCOMPONENT name="Matrix Group Component" id="19b69873bd3945f7" memberName="matrixGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0R 0M 72M" posRelativeY="bb46950e139db507"
                  outlinecol="ff454545" textcol="ff000000" title="Routing matrix"/>
  <GENERICCOMPONENT name="Labelled Matrix of Sliders" id="216c3aa5980dcb58" memberName="slidersMatrix"
                    virtualName="" explicitFocusOrder="0" pos="8 16 16M 24M" posRelativeX="19b69873bd3945f7"
                    posRelativeY="19b69873bd3945f7" posRelativeW="19b69873bd3945f7"
                    posRelativeH="19b69873bd3945f7" class="Miam::LabelledMatrixComponent"
                    params="this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels"/>
  <GROUPCOMPONENT name="Preferences Group Component" id="dfbb24a51fa3d6c0" memberName="preferencesGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="216 0 251M 72" posRelativeH="bb46950e139db507"
                  outlinecol="ff454545" textcol="ff000000" title="Preferences"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="8R 16 48 24" posRelativeX="8d369e08975b779c"
              initialText="-1" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="8 16 184 24" posRelativeX="dfbb24a51fa3d6c0"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Listen to OSC on UDP port:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="UPD Status Label" id="3fd4c40c4d48dfec" memberName="udpStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="-104R 16 357M 24"
         posRelativeX="e4ef4437203ce19e" posRelativeW="dfbb24a51fa3d6c0"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Status : ..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <GROUPCOMPONENT name="Audio Config Group Component" id="bb46950e139db507" memberName="audioConfigComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0 208 72" outlinecol="ff454545"
                  textcol="ff000000" title="Audio dynamics"/>
  <TOGGLEBUTTON name="keyboard button" id="8c809b7ecaa8a037" memberName="keyboardButton"
                virtualName="" explicitFocusOrder="0" pos="8 40 24M 24" posRelativeX="dfbb24a51fa3d6c0"
                posRelativeW="dfbb24a51fa3d6c0" txtcol="ff000000" buttonText="Keyboard editing mode"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="Attack Slider" id="b96b5d59dd56bbaa" memberName="attackSlider"
          virtualName="" explicitFocusOrder="0" pos="8R 16 112 48" posRelativeX="19707b3f29742e60"
          textboxtext="ff000000" min="1.0" max="100.0" int="1.0" style="RotaryVerticalDrag"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="60"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Attack Label" id="19707b3f29742e60" memberName="attackLabel"
         virtualName="" explicitFocusOrder="0" pos="8 28 56 24" posRelativeX="bb46950e139db507"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Attack"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="Attack unit Label" id="c34a141f13007692" memberName="attackUnitLabel"
         virtualName="" explicitFocusOrder="0" pos="0R 28 88 24" posRelativeX="b96b5d59dd56bbaa"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="ms"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="Help button" id="99be38dc78387f16" memberName="helpTextButton"
              virtualName="" explicitFocusOrder="0" pos="4Rr 8 24 60" posRelativeX="922404df7bcd082e"
              posRelativeY="922404df7bcd082e" posRelativeW="922404df7bcd082e"
              posRelativeH="922404df7bcd082e" bgColOff="3e000000" textCol="ff000000"
              buttonText="?" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Connect text button" id="8304a80afe002d99" memberName="connectTextButton"
              virtualName="" explicitFocusOrder="0" pos="456 16 88 24" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Connect" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
