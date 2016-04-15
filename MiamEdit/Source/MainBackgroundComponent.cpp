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
#include "View.h"
//[/Headers]

#include "MainBackgroundComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainBackgroundComponent::MainBackgroundComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (textButtonSpeakers = new TextButton ("textButtonSpeakers"));
    textButtonSpeakers->setButtonText (TRANS("Speakers"));
    textButtonSpeakers->setConnectedEdges (Button::ConnectedOnRight);
    textButtonSpeakers->addListener (this);
    textButtonSpeakers->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    textButtonSpeakers->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (textButtonSpeakersGroups = new TextButton ("textButtonSpeakersGroups"));
    textButtonSpeakersGroups->setButtonText (TRANS("Speakers Groups"));
    textButtonSpeakersGroups->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    textButtonSpeakersGroups->addListener (this);
    textButtonSpeakersGroups->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    textButtonSpeakersGroups->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (mainInfoLabel = new Label ("Main info label",
                                                  TRANS("...")));
    mainInfoLabel->setFont (Font (15.00f, Font::plain));
    mainInfoLabel->setJustificationType (Justification::centredRight);
    mainInfoLabel->setEditable (false, false, false);
    mainInfoLabel->setColour (Label::backgroundColourId, Colour (0x56ffffff));
    mainInfoLabel->setColour (Label::outlineColourId, Colour (0x84000000));
    mainInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    mainInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textButtonScenes = new TextButton ("textButtonScenes"));
    textButtonScenes->setButtonText (TRANS("Spat Scenes"));
    textButtonScenes->setConnectedEdges (Button::ConnectedOnLeft);
    textButtonScenes->addListener (this);
    textButtonScenes->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    textButtonScenes->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (textButtonHardware = new TextButton ("textButtonHardware"));
    textButtonHardware->setButtonText (TRANS("Hardware configuration"));
    textButtonHardware->addListener (this);
    textButtonHardware->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    textButtonHardware->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (textButtonStartSpat = new TextButton ("textButtonStartSpat"));
    textButtonStartSpat->setButtonText (TRANS("Start"));
    textButtonStartSpat->setColour (TextButton::buttonColourId, Colour (0xa0ffffff));
    textButtonStartSpat->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Loading...")));
    label->setFont (Font (15.00f, Font::italic));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1024, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MainBackgroundComponent::~MainBackgroundComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textButtonSpeakers = nullptr;
    textButtonSpeakersGroups = nullptr;
    mainInfoLabel = nullptr;
    textButtonScenes = nullptr;
    textButtonHardware = nullptr;
    textButtonStartSpat = nullptr;
    label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainBackgroundComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff707070));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainBackgroundComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textButtonSpeakers->setBounds (8, 8, 120, 24);
    textButtonSpeakersGroups->setBounds (128, 8, 120, 24);
    mainInfoLabel->setBounds (getWidth() - 8 - (getWidth() - 720), 8, getWidth() - 720, 24);
    textButtonScenes->setBounds (248, 8, 120, 24);
    textButtonHardware->setBounds (376, 8, 176, 24);
    textButtonStartSpat->setBounds (584, 8, 96, 24);
    label->setBounds (proportionOfWidth (0.5000f) - (150 / 2), proportionOfHeight (0.5000f) - (24 / 2), 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainBackgroundComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    miamView->ButtonClicked(buttonThatWasClicked->getName());
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButtonSpeakers)
    {
        //[UserButtonCode_textButtonSpeakers] -- add your button handler code here..
        //[/UserButtonCode_textButtonSpeakers]
    }
    else if (buttonThatWasClicked == textButtonSpeakersGroups)
    {
        //[UserButtonCode_textButtonSpeakersGroups] -- add your button handler code here..
        //[/UserButtonCode_textButtonSpeakersGroups]
    }
    else if (buttonThatWasClicked == textButtonScenes)
    {
        //[UserButtonCode_textButtonScenes] -- add your button handler code here..
        //[/UserButtonCode_textButtonScenes]
    }
    else if (buttonThatWasClicked == textButtonHardware)
    {
        //[UserButtonCode_textButtonHardware] -- add your button handler code here..
        //[/UserButtonCode_textButtonHardware]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainBackgroundComponent::CompleteInitialization()
{
    // Scenes component should be shown on opening
    // (default scene always loaded)
    textButtonScenes->triggerClick();
}

void MainBackgroundComponent::DisplayInfo(const String& message)
{
    mainInfoLabel->setText(message, NotificationType::sendNotificationAsync);
    clearLabelTimer.startTimer(mainInfoLabel.get()); // will clear it after a precise time
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainBackgroundComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="1024" initialHeight="600">
  <BACKGROUND backgroundColour="ff707070"/>
  <TEXTBUTTON name="textButtonSpeakers" id="2752e5f61c280c43" memberName="textButtonSpeakers"
              virtualName="" explicitFocusOrder="0" pos="8 8 120 24" bgColOff="55ffffff"
              bgColOn="ffffffff" buttonText="Speakers" connectedEdges="2" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="textButtonSpeakersGroups" id="8bdb167a1cca5b0b" memberName="textButtonSpeakersGroups"
              virtualName="" explicitFocusOrder="0" pos="128 8 120 24" bgColOff="55ffffff"
              bgColOn="ffffffff" buttonText="Speakers Groups" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="Main info label" id="d52b689341b86690" memberName="mainInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="8Rr 8 720M 24" bkgCol="56ffffff"
         outlineCol="84000000" edTextCol="ff000000" edBkgCol="0" labelText="..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="textButtonScenes" id="9994cb0e99bfd3ca" memberName="textButtonScenes"
              virtualName="" explicitFocusOrder="0" pos="248 8 120 24" bgColOff="55ffffff"
              bgColOn="ffffffff" buttonText="Spat Scenes" connectedEdges="1"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="textButtonHardware" id="1dad683cba944341" memberName="textButtonHardware"
              virtualName="" explicitFocusOrder="0" pos="376 8 176 24" bgColOff="55ffffff"
              bgColOn="ffffffff" buttonText="Hardware configuration" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="textButtonStartSpat" id="cecb4b1d1a8f7c2d" memberName="textButtonStartSpat"
              virtualName="" explicitFocusOrder="0" pos="584 8 96 24" bgColOff="a0ffffff"
              bgColOn="ffffffff" buttonText="Start" connectedEdges="0" needsCallback="0"
              radioGroupId="0"/>
  <LABEL name="new label" id="f13afc15bc19a998" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="50%c 50%c 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Loading..." editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="1" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
