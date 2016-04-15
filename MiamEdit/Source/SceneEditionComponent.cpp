/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SceneEditionManager.h"
//[/Headers]

#include "SceneEditionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
SceneEditionComponent::SceneEditionComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    addAndMakeVisible(sceneCanvasComponent = new SceneCanvasComponent());
    //[/Constructor_pre]

    addAndMakeVisible (groupComponent = new GroupComponent ("new group",
                                                            TRANS("Area edition")));

    addAndMakeVisible (groupComponent2 = new GroupComponent ("new group",
                                                             TRANS("Spatialization effect")));

    addAndMakeVisible (addPointTextButton = new TextButton ("Add Point text button"));
    addPointTextButton->setButtonText (TRANS("Add Point"));
    addPointTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addPointTextButton->addListener (this);
    addPointTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addPointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (deletePointTextButton = new TextButton ("Delete Point text button"));
    deletePointTextButton->setButtonText (TRANS("Delete Point"));
    deletePointTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deletePointTextButton->addListener (this);
    deletePointTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deletePointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (copyTextButton = new TextButton ("Copy Text Button"));
    copyTextButton->setButtonText (TRANS("Copy"));
    copyTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    copyTextButton->addListener (this);
    copyTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    copyTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (pasteTextButton = new TextButton ("Delete Point"));
    pasteTextButton->setButtonText (TRANS("Paste"));
    pasteTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    pasteTextButton->addListener (this);
    pasteTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    pasteTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (addAreaTextButton = new TextButton ("Add Area text button"));
    addAreaTextButton->setButtonText (TRANS("Add Area"));
    addAreaTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    addAreaTextButton->addListener (this);
    addAreaTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (deleteAreaTextButton = new TextButton ("Delete Point text button"));
    deleteAreaTextButton->setButtonText (TRANS("Delete Area"));
    deleteAreaTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    deleteAreaTextButton->addListener (this);
    deleteAreaTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (labelR = new Label ("new label",
                                           TRANS("R")));
    labelR->setFont (Font (15.00f, Font::plain));
    labelR->setJustificationType (Justification::centredLeft);
    labelR->setEditable (false, false, false);
    labelR->setColour (TextEditor::textColourId, Colours::black);
    labelR->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderR = new Slider ("Slider R"));
    sliderR->setRange (0, 255, 1);
    sliderR->setSliderStyle (Slider::LinearHorizontal);
    sliderR->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderR->addListener (this);

    addAndMakeVisible (labelG = new Label ("new label",
                                           TRANS("G")));
    labelG->setFont (Font (15.00f, Font::plain));
    labelG->setJustificationType (Justification::centredLeft);
    labelG->setEditable (false, false, false);
    labelG->setColour (TextEditor::textColourId, Colours::black);
    labelG->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderG = new Slider ("SliderG"));
    sliderG->setRange (0, 255, 1);
    sliderG->setSliderStyle (Slider::LinearHorizontal);
    sliderG->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderG->addListener (this);

    addAndMakeVisible (labelB = new Label ("new label",
                                           TRANS("B")));
    labelB->setFont (Font (15.00f, Font::plain));
    labelB->setJustificationType (Justification::centredLeft);
    labelB->setEditable (false, false, false);
    labelB->setColour (TextEditor::textColourId, Colours::black);
    labelB->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderB = new Slider ("Slider B"));
    sliderB->setRange (0, 255, 1);
    sliderB->setSliderStyle (Slider::LinearHorizontal);
    sliderB->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderB->addListener (this);

    addAndMakeVisible (sendBackwardTextButton = new TextButton ("Send backward text button"));
    sendBackwardTextButton->setButtonText (TRANS("Send backward"));
    sendBackwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    sendBackwardTextButton->addListener (this);
    sendBackwardTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sendBackwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (bringForwardTextButton = new TextButton ("Bring forward text button"));
    bringForwardTextButton->setButtonText (TRANS("Bring forward"));
    bringForwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    bringForwardTextButton->addListener (this);
    bringForwardTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    bringForwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (bringToFrontTextButton = new TextButton ("Bring to front text button"));
    bringToFrontTextButton->setButtonText (TRANS("to front"));
    bringToFrontTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    bringToFrontTextButton->addListener (this);
    bringToFrontTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    bringToFrontTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (sendToBackTextButton = new TextButton ("Send to back text button"));
    sendToBackTextButton->setButtonText (TRANS("to back"));
    sendToBackTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    sendToBackTextButton->addListener (this);
    sendToBackTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sendToBackTextButton->setColour (TextButton::buttonOnColourId, Colours::white);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1024, 560);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SceneEditionComponent::~SceneEditionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    delete sceneCanvasComponent;
    //[/Destructor_pre]

    groupComponent = nullptr;
    groupComponent2 = nullptr;
    addPointTextButton = nullptr;
    deletePointTextButton = nullptr;
    copyTextButton = nullptr;
    pasteTextButton = nullptr;
    addAreaTextButton = nullptr;
    deleteAreaTextButton = nullptr;
    labelR = nullptr;
    sliderR = nullptr;
    labelG = nullptr;
    sliderG = nullptr;
    labelB = nullptr;
    sliderB = nullptr;
    sendBackwardTextButton = nullptr;
    bringForwardTextButton = nullptr;
    bringToFrontTextButton = nullptr;
    sendToBackTextButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SceneEditionComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffd9d9d9));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SceneEditionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    // Canvas bounds
    sceneCanvasComponent->setBounds(208, 8, getWidth()-216, getHeight()-16);

    //[/UserPreResize]

    groupComponent->setBounds (8, 6, 192, 258);
    groupComponent2->setBounds (8, proportionOfHeight (0.9893f) - proportionOfHeight (0.4804f), 192, proportionOfHeight (0.4804f));
    addPointTextButton->setBounds (16, 224, 88, 24);
    deletePointTextButton->setBounds (104, 224, 88, 24);
    copyTextButton->setBounds (16, 56, 88, 24);
    pasteTextButton->setBounds (104, 56, 88, 24);
    addAreaTextButton->setBounds (16, 32, 88, 24);
    deleteAreaTextButton->setBounds (104, 32, 88, 24);
    labelR->setBounds (16, 144, 24, 24);
    sliderR->setBounds (40, 144, 150, 24);
    labelG->setBounds (16, 168, 24, 24);
    sliderG->setBounds (40, 168, 150, 24);
    labelB->setBounds (16, 192, 24, 24);
    sliderB->setBounds (40, 192, 150, 24);
    sendBackwardTextButton->setBounds (16, 112, 104, 24);
    bringForwardTextButton->setBounds (16, 88, 104, 24);
    bringToFrontTextButton->setBounds (120, 88, 72, 24);
    sendToBackTextButton->setBounds (120, 112, 72, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SceneEditionComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == addPointTextButton)
    {
        //[UserButtonCode_addPointTextButton] -- add your button handler code here..
        sceneEditionManager->OnAddPoint();
        //[/UserButtonCode_addPointTextButton]
    }
    else if (buttonThatWasClicked == deletePointTextButton)
    {
        //[UserButtonCode_deletePointTextButton] -- add your button handler code here..
        sceneEditionManager->OnDeletePoint();
        //[/UserButtonCode_deletePointTextButton]
    }
    else if (buttonThatWasClicked == copyTextButton)
    {
        //[UserButtonCode_copyTextButton] -- add your button handler code here..
        sceneEditionManager->OnCopyArea();
        //[/UserButtonCode_copyTextButton]
    }
    else if (buttonThatWasClicked == pasteTextButton)
    {
        //[UserButtonCode_pasteTextButton] -- add your button handler code here..
        sceneEditionManager->OnPasteArea();
        //[/UserButtonCode_pasteTextButton]
    }
    else if (buttonThatWasClicked == addAreaTextButton)
    {
        //[UserButtonCode_addAreaTextButton] -- add your button handler code here..
        sceneEditionManager->OnAddArea();
        //[/UserButtonCode_addAreaTextButton]
    }
    else if (buttonThatWasClicked == deleteAreaTextButton)
    {
        //[UserButtonCode_deleteAreaTextButton] -- add your button handler code here..
        sceneEditionManager->OnDeleteArea();
        //[/UserButtonCode_deleteAreaTextButton]
    }
    else if (buttonThatWasClicked == sendBackwardTextButton)
    {
        //[UserButtonCode_sendBackwardTextButton] -- add your button handler code here..
        sceneEditionManager->OnSendBackward();
        //[/UserButtonCode_sendBackwardTextButton]
    }
    else if (buttonThatWasClicked == bringForwardTextButton)
    {
        //[UserButtonCode_bringForwardTextButton] -- add your button handler code here..
        sceneEditionManager->OnBringForward();
        //[/UserButtonCode_bringForwardTextButton]
    }
    else if (buttonThatWasClicked == bringToFrontTextButton)
    {
        //[UserButtonCode_bringToFrontTextButton] -- add your button handler code here..
        sceneEditionManager->OnBringToFront();
        //[/UserButtonCode_bringToFrontTextButton]
    }
    else if (buttonThatWasClicked == sendToBackTextButton)
    {
        //[UserButtonCode_sendToBackTextButton] -- add your button handler code here..
        sceneEditionManager->OnSendToBack();
        //[/UserButtonCode_sendToBackTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SceneEditionComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderR)
    {
        //[UserSliderCode_sliderR] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderR]
    }
    else if (sliderThatWasMoved == sliderG)
    {
        //[UserSliderCode_sliderG] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderG]
    }
    else if (sliderThatWasMoved == sliderB)
    {
        //[UserSliderCode_sliderB] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderB]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// CONSTRUCTION HELPERS
void SceneEditionComponent::CompleteInitialization(SceneEditionManager* _sceneEditionManager)
{
    sceneEditionManager = _sceneEditionManager;
    sceneCanvasComponent->CompleteInitialization(sceneEditionManager);
}







// ========== SETTERS AND GETTERS ==========





// ========== ORDERS FROM PRESENTER ==========
void SceneEditionComponent::enablePolygonEditionControls()
{
    addAreaTextButton->setEnabled(false);
    deleteAreaTextButton->setEnabled(true);
    copyTextButton->setEnabled(true);
    pasteTextButton->setEnabled(true);

    setVisibleSelectedAreaControls(true);
    setEnabledSelectedAreaControls(true);
}
void SceneEditionComponent::disablePolygonEditionControls()
{
    addAreaTextButton->setEnabled(true);
    deleteAreaTextButton->setEnabled(false);
    copyTextButton->setEnabled(false);

    setVisibleSelectedAreaControls(false);
}
/* Disables all buttons but the one that launched an action currently running
 */
void SceneEditionComponent::disableAllButtonsBut(juce::String onlyActiveButtonName)
{
    setEnabledAllControls(false);

    if (onlyActiveButtonName == "Add Point text button")
        addPointTextButton->setEnabled(true);
    else if (onlyActiveButtonName == "Delete Point text button")
        deletePointTextButton->setEnabled(true);
}

void SceneEditionComponent::setEnabledAllControls(bool areEnabled)
{
    // For any area
    addAreaTextButton->setEnabled(areEnabled);
    deleteAreaTextButton->setEnabled(areEnabled);
    copyTextButton->setEnabled(areEnabled);
    pasteTextButton->setEnabled(areEnabled);

    setEnabledSelectedAreaControls(areEnabled);
}
void SceneEditionComponent::setVisibleSelectedAreaControls(bool areVisible)
{
    // For a selected area
    bringForwardTextButton->setVisible(areVisible);
    bringToFrontTextButton->setVisible(areVisible);
    sendBackwardTextButton->setVisible(areVisible);
    sendToBackTextButton->setVisible(areVisible);
    labelR->setVisible(areVisible);
    labelG->setVisible(areVisible);
    labelB->setVisible(areVisible);
    sliderR->setVisible(areVisible);
    sliderG->setVisible(areVisible);
    sliderB->setVisible(areVisible);

    // Polygon
    addPointTextButton->setVisible(areVisible);
    deletePointTextButton->setVisible(areVisible);
}
void SceneEditionComponent::setEnabledSelectedAreaControls(bool areEnabled)
{
    // For a selected area
    bringForwardTextButton->setEnabled(areEnabled);
    bringToFrontTextButton->setEnabled(areEnabled);
    sendBackwardTextButton->setEnabled(areEnabled);
    sendToBackTextButton->setEnabled(areEnabled);
    labelR->setEnabled(areEnabled);
    labelG->setEnabled(areEnabled);
    labelB->setEnabled(areEnabled);
    sliderR->setEnabled(areEnabled);
    sliderG->setEnabled(areEnabled);
    sliderB->setEnabled(areEnabled);

    // Polygon
    addPointTextButton->setEnabled(areEnabled);
    deletePointTextButton->setEnabled(areEnabled);
}




void SceneEditionComponent::SetAreaColourValue(juce::Colour colour)
{
    sliderR->setValue(colour.getRed());
    sliderG->setValue(colour.getGreen());
    sliderB->setValue(colour.getBlue());
}
void SceneEditionComponent::colourSliderMoved()
{
    Colour newColour = Colour(sliderR->getValue(), sliderG->getValue(), sliderB->getValue());
    sceneEditionManager->OnNewColour(newColour);
}




//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SceneEditionComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1024" initialHeight="560">
  <BACKGROUND backgroundColour="ffd9d9d9"/>
  <GROUPCOMPONENT name="new group" id="87d416270d41f58c" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 6 192 258" title="Area edition"/>
  <GROUPCOMPONENT name="new group" id="90b16e3024c520fd" memberName="groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="8 98.929%r 192 48.036%"
                  title="Spatialization effect"/>
  <TEXTBUTTON name="Add Point text button" id="71769222a7765795" memberName="addPointTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 224 88 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Add Point" connectedEdges="2"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point text button" id="2bdfdc3999886d02" memberName="deletePointTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 224 88 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Delete Point" connectedEdges="1"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Copy Text Button" id="33b309fbb149d4ee" memberName="copyTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 56 88 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Copy" connectedEdges="6" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point" id="653c7d46ffba1120" memberName="pasteTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 56 88 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Paste" connectedEdges="5" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Add Area text button" id="d377c545746b9e8d" memberName="addAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 32 88 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Add Area" connectedEdges="10"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point text button" id="3c0e2321516c0809" memberName="deleteAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 32 88 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Delete Area" connectedEdges="9"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="893f3de478724dd" memberName="labelR" virtualName=""
         explicitFocusOrder="0" pos="16 144 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="R" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="Slider R" id="c86e71b3772d49e9" memberName="sliderR" virtualName=""
          explicitFocusOrder="0" pos="40 144 150 24" min="0" max="255"
          int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="d31faee34d487318" memberName="labelG" virtualName=""
         explicitFocusOrder="0" pos="16 168 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="G" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="SliderG" id="8455ecc13f47ec08" memberName="sliderG" virtualName=""
          explicitFocusOrder="0" pos="40 168 150 24" min="0" max="255"
          int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="ae5abc145d9184cf" memberName="labelB" virtualName=""
         explicitFocusOrder="0" pos="16 192 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="B" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="Slider B" id="15aa4b94476e3563" memberName="sliderB" virtualName=""
          explicitFocusOrder="0" pos="40 192 150 24" min="0" max="255"
          int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTBUTTON name="Send backward text button" id="a356ce3575b4d98b" memberName="sendBackwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 112 104 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Send backward" connectedEdges="6"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring forward text button" id="d626373b1db0a49" memberName="bringForwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 88 104 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Bring forward" connectedEdges="10"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring to front text button" id="43d8b27a93068fcf" memberName="bringToFrontTextButton"
              virtualName="" explicitFocusOrder="0" pos="120 88 72 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="to front" connectedEdges="9" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Send to back text button" id="1fe791d6bf4489ca" memberName="sendToBackTextButton"
              virtualName="" explicitFocusOrder="0" pos="120 112 72 24" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="to back" connectedEdges="5" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
