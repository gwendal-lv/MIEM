/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

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
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
MainBackgroundComponent::MainBackgroundComponent ()
    : clearLabelTimer(currentDisplayedInfoPriority)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (speakersTextButton = new TextButton ("Speakers text button"));
    speakersTextButton->setButtonText (TRANS("Speakers"));
    speakersTextButton->setConnectedEdges (Button::ConnectedOnRight);
    speakersTextButton->addListener (this);
    speakersTextButton->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    speakersTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    speakersTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (spatStatesTextButtn = new TextButton ("Spatialization States text button"));
    spatStatesTextButtn->setButtonText (TRANS("Routing Matrices"));
    spatStatesTextButtn->setConnectedEdges (Button::ConnectedOnRight);
    spatStatesTextButtn->addListener (this);
    spatStatesTextButtn->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    spatStatesTextButtn->setColour (TextButton::buttonOnColourId, Colours::white);
    spatStatesTextButtn->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (mainInfoLabel = new Label ("Main info label",
                                                  TRANS("...")));
    mainInfoLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    mainInfoLabel->setJustificationType (Justification::centredRight);
    mainInfoLabel->setEditable (false, false, false);
    mainInfoLabel->setColour (Label::backgroundColourId, Colour (0x56ffffff));
    mainInfoLabel->setColour (Label::outlineColourId, Colour (0x84ffffff));
    mainInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    mainInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (scenesTextButton = new TextButton ("Scenes text button"));
    scenesTextButton->setButtonText (TRANS("Spat Scenes"));
    scenesTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    scenesTextButton->addListener (this);
    scenesTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    scenesTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    scenesTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (hardwareConfTextButton = new TextButton ("Hardware Configuration text button"));
    hardwareConfTextButton->setButtonText (TRANS("Configuration"));
    hardwareConfTextButton->addListener (this);
    hardwareConfTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    hardwareConfTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    hardwareConfTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (startTextButton = new TextButton ("Start text button"));
    startTextButton->setButtonText (TRANS("Start MiamSpat"));
    startTextButton->setColour (TextButton::buttonColourId, Colour (0xa0ffffff));
    startTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    startTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (fileTextButton = new TextButton ("File text button"));
    fileTextButton->setButtonText (TRANS("File"));
    fileTextButton->addListener (this);
    fileTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    fileTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    fileTextButton->setColour (TextButton::textColourOffId, Colours::black);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1024, 600);


    //[Constructor] You can add your own custom stuff here..

    // Buttons still deactivated for now
    startTextButton->setEnabled(false);
    speakersTextButton->setEnabled(false);

    // On les rend carrément invisibles aussi
    startTextButton->setVisible(false);
    speakersTextButton->setVisible(false);

    // ClearLabelTimer + priorités des infos affichées
    clearLabelTimer.SetLabelToClearAfterTimeout(mainInfoLabel.get());
    currentDisplayedInfoPriority = -1; // plus petit que tout...

    //[/Constructor]
}

MainBackgroundComponent::~MainBackgroundComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    speakersTextButton = nullptr;
    spatStatesTextButtn = nullptr;
    mainInfoLabel = nullptr;
    scenesTextButton = nullptr;
    hardwareConfTextButton = nullptr;
    startTextButton = nullptr;
    fileTextButton = nullptr;


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

    speakersTextButton->setBounds (8 + 64, 8, 8, 24);
    spatStatesTextButtn->setBounds ((8 + 64) + 8, 8, 112, 24);
    mainInfoLabel->setBounds (getWidth() - 8 - (getWidth() - 456), 8, getWidth() - 456, 24);
    scenesTextButton->setBounds (((8 + 64) + 8) + 112, 8, 112, 24);
    hardwareConfTextButton->setBounds (((8 + 64) + 8) + 112 - -120, 8, 112, 24);
    startTextButton->setBounds ((((8 + 64) + 8) + 112 - -120) + 112 - -16, 8, 8, 24);
    fileTextButton->setBounds (8, 8, 64, 24);
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
    else if (buttonThatWasClicked == spatStatesTextButtn)
    {
        //[UserButtonCode_spatStatesTextButtn] -- add your button handler code here..
        //[/UserButtonCode_spatStatesTextButtn]
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
    else if (buttonThatWasClicked == fileTextButton)
    {
        //[UserButtonCode_fileTextButton] -- add your button handler code here..
        fileMenu->ShowMenuAndSendUserAnswer();
        //[/UserButtonCode_fileTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainBackgroundComponent::CompleteInitialization(Presenter* _presenter)
{
    fileMenu.reset( new FileMenu(_presenter) );
}

void MainBackgroundComponent::DisplayInfo(const String& message, int priority)
{
    // On n'affiche que les priorités supérieures (si égale : on affiche la + fraîche)
    if (priority >= currentDisplayedInfoPriority)
    {
        mainInfoLabel->setText(message, NotificationType::sendNotificationAsync);
        currentDisplayedInfoPriority = priority;
        
        clearLabelTimer.StartTimer(); // will clear it after a precise time
    }
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
                 variableInitialisers="clearLabelTimer(currentDisplayedInfoPriority)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1024" initialHeight="600">
  <BACKGROUND backgroundColour="ff707070"/>
  <TEXTBUTTON name="Speakers text button" id="2752e5f61c280c43" memberName="speakersTextButton"
              virtualName="" explicitFocusOrder="0" pos="0R 8 8 24" posRelativeX="dcc32a783566df37"
              bgColOff="55ffffff" bgColOn="ffffffff" textCol="ff000000" buttonText="Speakers"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Spatialization States text button" id="8bdb167a1cca5b0b"
              memberName="spatStatesTextButtn" virtualName="" explicitFocusOrder="0"
              pos="0R 8 112 24" posRelativeX="2752e5f61c280c43" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Routing Matrices"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Main info label" id="d52b689341b86690" memberName="mainInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="8Rr 8 456M 24" bkgCol="56ffffff"
         outlineCol="84ffffff" edTextCol="ff000000" edBkgCol="0" labelText="..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="34"/>
  <TEXTBUTTON name="Scenes text button" id="9994cb0e99bfd3ca" memberName="scenesTextButton"
              virtualName="" explicitFocusOrder="0" pos="0R 8 112 24" posRelativeX="8bdb167a1cca5b0b"
              bgColOff="ffbfbfbf" bgColOn="ffffffff" textCol="ff000000" buttonText="Spat Scenes"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Hardware Configuration text button" id="1dad683cba944341"
              memberName="hardwareConfTextButton" virtualName="" explicitFocusOrder="0"
              pos="-120R 8 112 24" posRelativeX="8bdb167a1cca5b0b" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Configuration"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Start text button" id="cecb4b1d1a8f7c2d" memberName="startTextButton"
              virtualName="" explicitFocusOrder="0" pos="-16R 8 8 24" posRelativeX="1dad683cba944341"
              bgColOff="a0ffffff" bgColOn="ffffffff" textCol="ff000000" buttonText="Start MiamSpat"
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="File text button" id="dcc32a783566df37" memberName="fileTextButton"
              virtualName="" explicitFocusOrder="0" pos="8 8 64 24" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="File" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
