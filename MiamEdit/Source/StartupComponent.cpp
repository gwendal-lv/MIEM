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
#include "MiemFileChoosers.h"

#include "AppPurpose.h"
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

    loadTextButton.reset (new TextButton ("Load text button"));
    addAndMakeVisible (loadTextButton.get());
    loadTextButton->setButtonText (TRANS("Load session"));
    loadTextButton->addListener (this);
    loadTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    loadTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    loadTextButton->setColour (TextButton::textColourOffId, Colours::black);

    createSpatTextButton.reset (new TextButton ("Create Spat text button"));
    addAndMakeVisible (createSpatTextButton.get());
    createSpatTextButton->setButtonText (TRANS("Create Spatialisation session"));
    createSpatTextButton->addListener (this);
    createSpatTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    createSpatTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    createSpatTextButton->setColour (TextButton::textColourOffId, Colours::black);

    createDefaultTextButton.reset (new TextButton ("Create Default text button"));
    addAndMakeVisible (createDefaultTextButton.get());
    createDefaultTextButton->setButtonText (TRANS("Create default session"));
    createDefaultTextButton->addListener (this);
    createDefaultTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    createDefaultTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    createDefaultTextButton->setColour (TextButton::textColourOffId, Colours::black);

    createGenericTextButton.reset (new TextButton ("Create Empty text button"));
    addAndMakeVisible (createGenericTextButton.get());
    createGenericTextButton->setButtonText (TRANS("Create Generic OSC Controller session"));
    createGenericTextButton->addListener (this);
    createGenericTextButton->setColour (TextButton::buttonColourId, Colour (0xffbfbfbf));
    createGenericTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    createGenericTextButton->setColour (TextButton::textColourOffId, Colours::black);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    //bouton désactivé pour l'instant...
    createDefaultTextButton->setVisible(false);

    //[/Constructor]
}

StartupComponent::~StartupComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    loadTextButton = nullptr;
    createSpatTextButton = nullptr;
    createDefaultTextButton = nullptr;
    createGenericTextButton = nullptr;


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

    loadTextButton->setBounds ((getWidth() / 2) - (168 / 2), (getHeight() / 2) + -96, 168, 52);
    createSpatTextButton->setBounds ((getWidth() / 2) + 150 - 300, (getHeight() / 2) + 46, 300, 32);
    createDefaultTextButton->setBounds ((getWidth() / 2) + -3 - (168 / 2), (getHeight() / 2) + 350, 168, 24);
    createGenericTextButton->setBounds ((getWidth() / 2) + 150 - 300, (getHeight() / 2), 300, 32);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void StartupComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadTextButton.get())
    {
        //[UserButtonCode_loadTextButton] -- add your button handler code here..
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        LoadFileChooser fileChooser({App::GetPurpose()});
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        // CHANGER LE PURPOSE DU SELECTEUR DE FICHIER
        if ( fileChooser.browseForFileToOpen() )
        {
            File resultFile = fileChooser.getResult();
            presenter->LoadSession(resultFile.getFullPathName().toStdString());
        }
        //[/UserButtonCode_loadTextButton]
    }
    else if (buttonThatWasClicked == createSpatTextButton.get())
    {
        //[UserButtonCode_createSpatTextButton] -- add your button handler code here..
        SaveFileChooser fileChooser({AppPurpose::Spatialisation});
        if ( fileChooser.browseForFileToSave(true) )
        {
            File resultFile = fileChooser.getResult();
            presenter->CreateSession(resultFile.getFullPathName().toStdString(),
                                     AppPurpose::Spatialisation,
                                     true); // yes, is empty
        }
        //[/UserButtonCode_createSpatTextButton]
    }
    else if (buttonThatWasClicked == createDefaultTextButton.get())
    {
        //[UserButtonCode_createDefaultTextButton] -- add your button handler code here..
        //[/UserButtonCode_createDefaultTextButton]
    }
    else if (buttonThatWasClicked == createGenericTextButton.get())
    {
        //[UserButtonCode_createGenericTextButton] -- add your button handler code here..
        SaveFileChooser fileChooser({AppPurpose::GenericController});
        if ( fileChooser.browseForFileToSave(true) )
        {
            File resultFile = fileChooser.getResult();
            presenter->CreateSession(resultFile.getFullPathName().toStdString(),
                                     AppPurpose::GenericController,
                                     true); // yes, is empty
        }
        //[/UserButtonCode_createGenericTextButton]
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
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.33"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff707070"/>
  <TEXTBUTTON name="Load text button" id="dcc32a783566df37" memberName="loadTextButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc -96C 168 52" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Load session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Create Spat text button" id="714ffb446833c5ce" memberName="createSpatTextButton"
              virtualName="" explicitFocusOrder="0" pos="150Cr 46C 300 32"
              bgColOff="ffbfbfbf" bgColOn="ffffffff" textCol="ff000000" buttonText="Create Spatialisation session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Create Default text button" id="ac4f9aa6f5974fbc" memberName="createDefaultTextButton"
              virtualName="" explicitFocusOrder="0" pos="-3Cc 350C 168 24"
              bgColOff="ffbfbfbf" bgColOn="ffffffff" textCol="ff000000" buttonText="Create default session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Create Empty text button" id="fb033c6855f61992" memberName="createGenericTextButton"
              virtualName="" explicitFocusOrder="0" pos="150Cr 0C 300 32" bgColOff="ffbfbfbf"
              bgColOn="ffffffff" textCol="ff000000" buttonText="Create Generic OSC Controller session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
