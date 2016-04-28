/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

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

    addAndMakeVisible (speakersTextButton = new TextButton ("Speakers text button"));
    speakersTextButton->setButtonText (TRANS("Speakers"));
    speakersTextButton->setConnectedEdges (Button::ConnectedOnRight);
    speakersTextButton->addListener (this);
    speakersTextButton->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    speakersTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (speakersGroupsTextButton = new TextButton ("Speakers Groups text button"));
    speakersGroupsTextButton->setButtonText (TRANS("Speakers Groups"));
    speakersGroupsTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    speakersGroupsTextButton->addListener (this);
    speakersGroupsTextButton->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    speakersGroupsTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (mainInfoLabel = new Label ("Main info label",
                                                  TRANS("...")));
    mainInfoLabel->setFont (Font (15.00f, Font::plain));
    mainInfoLabel->setJustificationType (Justification::centredRight);
    mainInfoLabel->setEditable (false, false, false);
    mainInfoLabel->setColour (Label::backgroundColourId, Colour (0x56ffffff));
    mainInfoLabel->setColour (Label::outlineColourId, Colour (0x84000000));
    mainInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    mainInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (scenesTextButton = new TextButton ("Scenes text button"));
    scenesTextButton->setButtonText (TRANS("Spat Scenes"));
    scenesTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    scenesTextButton->addListener (this);
    scenesTextButton->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    scenesTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (hardwareConfTextButton = new TextButton ("Hardware Configuration text button"));
    hardwareConfTextButton->setButtonText (TRANS("Hardware Configuration"));
    hardwareConfTextButton->addListener (this);
    hardwareConfTextButton->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    hardwareConfTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (startTextButton = new TextButton ("Start text button"));
    startTextButton->setButtonText (TRANS("Start"));
    startTextButton->setColour (TextButton::buttonColourId, Colour (0xa0ffffff));
    startTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

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

    // !!! By default, "start" button deactivated !!!
    //startTextButton->setEnabled(false);

    //[/Constructor]
}

MainBackgroundComponent::~MainBackgroundComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    speakersTextButton = nullptr;
    speakersGroupsTextButton = nullptr;
    mainInfoLabel = nullptr;
    scenesTextButton = nullptr;
    hardwareConfTextButton = nullptr;
    startTextButton = nullptr;
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

    speakersTextButton->setBounds (8, 8, 120, 24);
    speakersGroupsTextButton->setBounds (128, 8, 120, 24);
    mainInfoLabel->setBounds (getWidth() - 8 - (getWidth() - 720), 8, getWidth() - 720, 24);
    scenesTextButton->setBounds (248, 8, 120, 24);
    hardwareConfTextButton->setBounds (376, 8, 176, 24);
    startTextButton->setBounds (584, 8, 96, 24);
    label->setBounds (proportionOfWidth (0.5000f) - (150 / 2), proportionOfHeight (0.5000f) - (24 / 2), 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainBackgroundComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]

    // Button click retransmitted to the view
    miamView->ButtonClicked(buttonThatWasClicked->getName());

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == speakersTextButton)
    {
        //[UserButtonCode_speakersTextButton] -- add your button handler code here..
        //[/UserButtonCode_speakersTextButton]
    }
    else if (buttonThatWasClicked == speakersGroupsTextButton)
    {
        //[UserButtonCode_speakersGroupsTextButton] -- add your button handler code here..
        //[/UserButtonCode_speakersGroupsTextButton]
    }
    else if (buttonThatWasClicked == scenesTextButton)
    {
        //[UserButtonCode_scenesTextButton] -- add your button handler code here..
        //[/UserButtonCode_scenesTextButton]
    }
    else if (buttonThatWasClicked == hardwareConfTextButton)
    {
        //[UserButtonCode_hardwareConfTextButton] -- add your button handler code here..
        //[/UserButtonCode_hardwareConfTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainBackgroundComponent::CompleteInitialization()
{
}

void MainBackgroundComponent::DisplayInfo(const String& message)
{
    mainInfoLabel->setText(message, NotificationType::sendNotificationAsync);
    clearLabelTimer.StartTimer(mainInfoLabel.get()); // will clear it after a precise time
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
  <TEXTBUTTON name="Speakers text button" id="2752e5f61c280c43" memberName="speakersTextButton"
              virtualName="" explicitFocusOrder="0" pos="8 8 120 24" bgColOff="55ffffff"
              bgColOn="ffffffff" buttonText="Speakers" connectedEdges="2" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Speakers Groups text button" id="8bdb167a1cca5b0b" memberName="speakersGroupsTextButton"
              virtualName="" explicitFocusOrder="0" pos="128 8 120 24" bgColOff="55ffffff"
              bgColOn="ffffffff" buttonText="Speakers Groups" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="Main info label" id="d52b689341b86690" memberName="mainInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="8Rr 8 720M 24" bkgCol="56ffffff"
         outlineCol="84000000" edTextCol="ff000000" edBkgCol="0" labelText="..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="Scenes text button" id="9994cb0e99bfd3ca" memberName="scenesTextButton"
              virtualName="" explicitFocusOrder="0" pos="248 8 120 24" bgColOff="55ffffff"
              bgColOn="ffffffff" buttonText="Spat Scenes" connectedEdges="1"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Hardware Configuration text button" id="1dad683cba944341"
              memberName="hardwareConfTextButton" virtualName="" explicitFocusOrder="0"
              pos="376 8 176 24" bgColOff="55ffffff" bgColOn="ffffffff" buttonText="Hardware Configuration"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Start text button" id="cecb4b1d1a8f7c2d" memberName="startTextButton"
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
