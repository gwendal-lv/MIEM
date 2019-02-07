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
#include "OSCRecorder.h"
//[/Headers]

#include "OSCRecorderComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCRecorderComponent::OSCRecorderComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    startButton.reset (new TextButton ("start button"));
    addAndMakeVisible (startButton.get());
    startButton->setButtonText (TRANS("BEGIN PRESET RESEARCH"));
    startButton->addListener (this);
    startButton->setColour (TextButton::buttonColourId, Colour (0xff3d338d));
    startButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    listenButton.reset (new TextButton ("listen button"));
    addAndMakeVisible (listenButton.get());
    listenButton->setButtonText (TRANS("LISTEN"));
    listenButton->addListener (this);
    listenButton->setColour (TextButton::buttonColourId, Colour (0xff959595));
    listenButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("OK, I FOUND THE PRESET"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff2ac858));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    countLabel.reset (new Label ("count label",
                                 TRANS("Current preset: .../...")));
    addAndMakeVisible (countLabel.get());
    countLabel->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    countLabel->setJustificationType (Justification::centred);
    countLabel->setEditable (false, false, false);
    countLabel->setColour (TextEditor::textColourId, Colours::black);
    countLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

OSCRecorderComponent::~OSCRecorderComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    startButton = nullptr;
    listenButton = nullptr;
    finishedButton = nullptr;
    countLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCRecorderComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff313131));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCRecorderComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    startButton->setBounds ((getWidth() / 2) - (500 / 2), 200, 500, 160);
    listenButton->setBounds ((getWidth() / 2) - (500 / 2), 20, 500, 160);
    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), 380, 500, 160);
    countLabel->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 40, 500, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCRecorderComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    if (recorderManager == 0)
    {
        // manager must be set at this point...
        assert(false);
        return;
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == startButton.get())
    {
        //[UserButtonCode_startButton] -- add your button handler code here..
        recorderManager->OnButtonClicked(ExperimentState::SearchingPreset);
        //[/UserButtonCode_startButton]
    }
    else if (buttonThatWasClicked == listenButton.get())
    {
        //[UserButtonCode_listenButton] -- add your button handler code here..
        recorderManager->OnButtonClicked(ExperimentState::Listening);
        //[/UserButtonCode_listenButton]
    }
    else if (buttonThatWasClicked == finishedButton.get())
    {
        //[UserButtonCode_finishedButton] -- add your button handler code here..
        recorderManager->OnButtonClicked(ExperimentState::FinishedSearchingPreset);
        //[/UserButtonCode_finishedButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
juce::String OSCRecorderComponent::GetExperimentStateName(ExperimentState state)
{
    switch (state)
    {
        case ExperimentState::NotInitialized:
            return "Not initialized";
            
        case ExperimentState::ReadyToListen:
            return "Ready to listen";
            
        case ExperimentState::Listening:
            return "Listening";
            
        case ExperimentState::ReadyToSearchPreset:
            return "Ready to search preset";
            
        case ExperimentState::SearchingPreset :
            return "Searching preset";
            
        case ExperimentState::FinishedSearchingPreset :
            return "Finished searching preset";
            
        case ExperimentState::Finished :
            return "Finished";
    }
}

void OSCRecorderComponent::DisplayNewState(ExperimentState newState, int presetIndex, size_t presetsCount)
{
    // BUTTONS display
    switch(newState)
    {
        case ExperimentState::NotInitialized:
            listenButton->setVisible(false);
            startButton->setVisible(false);
            finishedButton->setVisible(false);
            countLabel->setVisible(false);
            break;
            
        case ExperimentState::ReadyToListen:
            listenButton->setVisible(true);
            startButton->setVisible(false);
            finishedButton->setVisible(false);
            countLabel->setVisible(true);
            break;
            
        case ExperimentState::Listening:
            listenButton->setVisible(false);
            startButton->setVisible(false);
            finishedButton->setVisible(false);
            countLabel->setVisible(true);
            break;
            
        case ExperimentState::ReadyToSearchPreset:
            listenButton->setVisible(false);
            startButton->setVisible(true);
            finishedButton->setVisible(false);
            countLabel->setVisible(true);
            break;
            
        case ExperimentState::SearchingPreset:
            listenButton->setVisible(false);
            startButton->setVisible(false);
            finishedButton->setVisible(true);
            countLabel->setVisible(true);
            break;
            
        case ExperimentState::FinishedSearchingPreset:
            listenButton->setVisible(false);
            startButton->setVisible(false);
            finishedButton->setVisible(false);
            countLabel->setVisible(false);
            break;
            
        case ExperimentState::Finished:
            listenButton->setVisible(false);
            startButton->setVisible(false);
            finishedButton->setVisible(false);
            countLabel->setVisible(true);
            break;
    }
    
    
    
    // COUNT LABEL Display
    if (newState != ExperimentState::Finished)
    {
        int presetNumber = presetIndex + 1;
        
        if (presetIndex >= 0) // actual presets
            countLabel->setText(TRANS("Current preset: ") + String(presetNumber) + String("/") + String(presetsCount), NotificationType::dontSendNotification);
        else if (presetIndex == -2)
            countLabel->setText(TRANS("TRIAL preset 1."), NotificationType::dontSendNotification);
        else if (presetIndex == -1)
            countLabel->setText(TRANS("TRIAL preset 2."), NotificationType::dontSendNotification);
        else
            assert(false); // un planned case
    }
    else
    {
        countLabel->setText(TRANS("Experiment is now finished. Thank you for participating!"), NotificationType::dontSendNotification);
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCRecorderComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.33"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff313131"/>
  <TEXTBUTTON name="start button" id="cb0e665733fa514e" memberName="startButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 200 500 160" bgColOff="ff3d338d"
              bgColOn="ff252525" buttonText="BEGIN PRESET RESEARCH" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="listen button" id="54994c165790472c" memberName="listenButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 20 500 160" bgColOff="ff959595"
              bgColOn="ff252525" buttonText="LISTEN" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 380 500 160" bgColOff="ff2ac858"
              bgColOn="ff252525" buttonText="OK, I FOUND THE PRESET" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="count label" id="6112b78b8bf2731a" memberName="countLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cc 40R 500 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Current preset: .../..." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]