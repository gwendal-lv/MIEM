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
#include "SpatFileChoosers.h"
//[/Headers]

#include "StartupComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#include "Presenter.h"
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
StartupComponent::StartupComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (loadTextButton = new TextButton ("Load text button"));
    loadTextButton->setButtonText (TRANS("Load existing session"));
    loadTextButton->addListener (this);
    loadTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    loadTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    loadTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (createEmptyTextButton = new TextButton ("Create Empty text button"));
    createEmptyTextButton->setButtonText (TRANS("Create empty session"));
    createEmptyTextButton->addListener (this);
    createEmptyTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    createEmptyTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    createEmptyTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (createDefaultTextButton = new TextButton ("Create Default text button"));
    createDefaultTextButton->setButtonText (TRANS("Create default session"));
    createDefaultTextButton->addListener (this);
    createDefaultTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    createDefaultTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    createDefaultTextButton->setColour (TextButton::textColourOffId, Colours::black);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    
    //bouton désactivé pour l'instant...
    createDefaultTextButton->setEnabled(false);
    
    //[/Constructor]
}

StartupComponent::~StartupComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    loadTextButton = nullptr;
    createEmptyTextButton = nullptr;
    createDefaultTextButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void StartupComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff707070));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StartupComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    loadTextButton->setBounds ((getWidth() / 2) - (168 / 2), (getHeight() / 2) + -48, 168, 24);
    createEmptyTextButton->setBounds ((getWidth() / 2) - (168 / 2), (getHeight() / 2), 168, 24);
    createDefaultTextButton->setBounds ((getWidth() / 2) - (168 / 2), (getHeight() / 2) + 28, 168, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void StartupComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadTextButton)
    {
        //[UserButtonCode_loadTextButton] -- add your button handler code here..
        LoadFileChooser fileChooser;
        if ( fileChooser.browseForFileToOpen() )
        {
            File resultFile = fileChooser.getResult();
            presenter->LoadSession(resultFile.getFullPathName().toStdString());
        }
        //[/UserButtonCode_loadTextButton]
    }
    else if (buttonThatWasClicked == createEmptyTextButton)
    {
        //[UserButtonCode_createEmptyTextButton] -- add your button handler code here..
        SaveFileChooser fileChooser;
        if ( fileChooser.browseForFileToSave(true) )
        {
            File resultFile = fileChooser.getResult();
            presenter->CreateSession(resultFile.getFullPathName().toStdString(), true);
        }
        //[/UserButtonCode_createEmptyTextButton]
    }
    else if (buttonThatWasClicked == createDefaultTextButton)
    {
        //[UserButtonCode_createDefaultTextButton] -- add your button handler code here..
        //[/UserButtonCode_createDefaultTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StartupComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff707070"/>
  <TEXTBUTTON name="Load text button" id="dcc32a783566df37" memberName="loadTextButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc -48C 168 24" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Load existing session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Create Empty text button" id="714ffb446833c5ce" memberName="createEmptyTextButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 0C 168 24" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Create empty session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Create Default text button" id="ac4f9aa6f5974fbc" memberName="createDefaultTextButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 28C 168 24" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Create default session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
