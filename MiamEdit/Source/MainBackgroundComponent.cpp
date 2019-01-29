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

    speakersTextButton.reset (new TextButton ("Speakers text button"));
    addAndMakeVisible (speakersTextButton.get());
    speakersTextButton->setButtonText (TRANS("Speakers"));
    speakersTextButton->setConnectedEdges (Button::ConnectedOnRight);
    speakersTextButton->addListener (this);
    speakersTextButton->setColour (TextButton::buttonColourId, Colour (0x55ffffff));
    speakersTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    speakersTextButton->setColour (TextButton::textColourOffId, Colours::black);

    statesTextButton.reset (new TextButton ("States text button"));
    addAndMakeVisible (statesTextButton.get());
    statesTextButton->setButtonText (TRANS("Control States"));
    statesTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    statesTextButton->addListener (this);
    statesTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    statesTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    statesTextButton->setColour (TextButton::textColourOffId, Colours::black);

    statesTextButton->setBounds (200, 8, 120, 24);

    mainInfoLabel.reset (new Label ("Main info label",
                                    TRANS("...")));
    addAndMakeVisible (mainInfoLabel.get());
    mainInfoLabel->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    mainInfoLabel->setJustificationType (Justification::centredRight);
    mainInfoLabel->setEditable (false, false, false);
    mainInfoLabel->setColour (Label::backgroundColourId, Colour (0x56ffffff));
    mainInfoLabel->setColour (Label::outlineColourId, Colour (0x84ffffff));
    mainInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    mainInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    scenesTextButton.reset (new TextButton ("Scenes text button"));
    addAndMakeVisible (scenesTextButton.get());
    scenesTextButton->setButtonText (TRANS("Geometric Scenes"));
    scenesTextButton->setConnectedEdges (Button::ConnectedOnRight);
    scenesTextButton->addListener (this);
    scenesTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    scenesTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    scenesTextButton->setColour (TextButton::textColourOffId, Colours::black);

    scenesTextButton->setBounds (80, 8, 120, 24);

    hardwareConfTextButton.reset (new TextButton ("Hardware Configuration text button"));
    addAndMakeVisible (hardwareConfTextButton.get());
    hardwareConfTextButton->setButtonText (TRANS("Configuration"));
    hardwareConfTextButton->addListener (this);
    hardwareConfTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    hardwareConfTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    hardwareConfTextButton->setColour (TextButton::textColourOffId, Colours::black);

    hardwareConfTextButton->setBounds (328, 8, 112, 24);

    startTextButton.reset (new TextButton ("Start text button"));
    addAndMakeVisible (startTextButton.get());
    startTextButton->setButtonText (TRANS("Start MiamSpat"));
    startTextButton->setColour (TextButton::buttonColourId, Colour (0xa0ffffff));
    startTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    startTextButton->setColour (TextButton::textColourOffId, Colours::black);

    fileTextButton.reset (new TextButton ("File text button"));
    addAndMakeVisible (fileTextButton.get());
    fileTextButton->setButtonText (TRANS("File"));
    fileTextButton->addListener (this);
    fileTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    fileTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    fileTextButton->setColour (TextButton::textColourOffId, Colours::black);

    fileTextButton->setBounds (8, 8, 64, 24);


    //[UserPreSize]
    genericScenesText = scenesTextButton->getButtonText();
    genericStatesText = statesTextButton->getButtonText();
    spatScenesText = TRANS("Spat Scenes");
    spatStatesText = TRANS("Routing Matrices");
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
    statesTextButton = nullptr;
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
    mainInfoLabel->setBounds (getWidth() - 8 - (getWidth() - 456), 8, getWidth() - 456, 24);
    startTextButton->setBounds (328 + 112 - -16, 8, 8, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainBackgroundComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]

    // Button click retransmitted to the view
    view->ButtonClicked(buttonThatWasClicked->getName());

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == speakersTextButton.get())
    {
        //[UserButtonCode_speakersTextButton] -- add your button handler code here..
        //[/UserButtonCode_speakersTextButton]
    }
    else if (buttonThatWasClicked == statesTextButton.get())
    {
        //[UserButtonCode_statesTextButton] -- add your button handler code here..
        //[/UserButtonCode_statesTextButton]
    }
    else if (buttonThatWasClicked == scenesTextButton.get())
    {
        //[UserButtonCode_scenesTextButton] -- add your button handler code here..
        //[/UserButtonCode_scenesTextButton]
    }
    else if (buttonThatWasClicked == hardwareConfTextButton.get())
    {
        //[UserButtonCode_hardwareConfTextButton] -- add your button handler code here..
        //[/UserButtonCode_hardwareConfTextButton]
    }
    else if (buttonThatWasClicked == fileTextButton.get())
    {
        //[UserButtonCode_fileTextButton] -- add your button handler code here..
        fileMenu->ShowMenuAndSendUserAnswer();
        //[/UserButtonCode_fileTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void MainBackgroundComponent::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    switch(view->GetSessionPurpose())
    {
        case AppPurpose::Spatialisation :
            scenesTextButton->setButtonText(spatScenesText);
            statesTextButton->setButtonText(spatStatesText);
            break;

        case AppPurpose::GenericController :
            scenesTextButton->setButtonText(genericScenesText);
            statesTextButton->setButtonText(genericStatesText);
            break;

        default :
            break;
    }
    //[/UserCode_visibilityChanged]
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
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.33"
                 fixedSize="1" initialWidth="1024" initialHeight="600">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff707070"/>
  <TEXTBUTTON name="Speakers text button" id="2752e5f61c280c43" memberName="speakersTextButton"
              virtualName="" explicitFocusOrder="0" pos="0R 8 8 24" posRelativeX="dcc32a783566df37"
              bgColOff="55ffffff" bgColOn="ffffffff" textCol="ff000000" buttonText="Speakers"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="States text button" id="8bdb167a1cca5b0b" memberName="statesTextButton"
              virtualName="" explicitFocusOrder="0" pos="200 8 120 24" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Control States"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Main info label" id="d52b689341b86690" memberName="mainInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="8Rr 8 456M 24" bkgCol="56ffffff"
         outlineCol="84ffffff" edTextCol="ff000000" edBkgCol="0" labelText="..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <TEXTBUTTON name="Scenes text button" id="9994cb0e99bfd3ca" memberName="scenesTextButton"
              virtualName="" explicitFocusOrder="0" pos="80 8 120 24" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Geometric Scenes"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Hardware Configuration text button" id="1dad683cba944341"
              memberName="hardwareConfTextButton" virtualName="" explicitFocusOrder="0"
              pos="328 8 112 24" bgColOff="ffbfbfbf" bgColOn="ffffffff" textCol="ff000000"
              buttonText="Configuration" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
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
