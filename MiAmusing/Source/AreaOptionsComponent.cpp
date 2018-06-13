/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "MultiSceneCanvasComponentAmusing.h"
//[/Headers]

#include "AreaOptionsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AreaOptionsComponent::AreaOptionsComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    volumeButton.reset (new TextButton ("new button"));
    addAndMakeVisible (volumeButton.get());
    volumeButton->setButtonText (TRANS("Volume"));
    volumeButton->addListener (this);

    speedButton.reset (new TextButton ("new button"));
    addAndMakeVisible (speedButton.get());
    speedButton->setButtonText (TRANS("Speed"));
    speedButton->addListener (this);

    sampleButton.reset (new TextButton ("new button"));
    addAndMakeVisible (sampleButton.get());
    sampleButton->setButtonText (TRANS("Sample"));
    sampleButton->addListener (this);

    octaveButton.reset (new TextButton ("new button"));
    addAndMakeVisible (octaveButton.get());
    octaveButton->setButtonText (TRANS("Octave"));
    octaveButton->addListener (this);

    rhythmButton.reset (new TextButton ("new button"));
    addAndMakeVisible (rhythmButton.get());
    rhythmButton->setButtonText (TRANS("Rhythm"));
    rhythmButton->addListener (this);

    closeOptionsButton.reset (new TextButton ("new button"));
    addAndMakeVisible (closeOptionsButton.get());
    closeOptionsButton->setButtonText (TRANS("X"));
    closeOptionsButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

AreaOptionsComponent::~AreaOptionsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    volumeButton = nullptr;
    speedButton = nullptr;
    sampleButton = nullptr;
    octaveButton = nullptr;
    rhythmButton = nullptr;
    closeOptionsButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AreaOptionsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AreaOptionsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    volumeButton->setBounds (proportionOfWidth (0.0200f), 0, proportionOfWidth (0.9001f), proportionOfHeight (0.1500f));
    speedButton->setBounds (proportionOfWidth (0.0200f), 0 + proportionOfHeight (0.1500f) - -2, proportionOfWidth (0.9001f), roundToInt (proportionOfHeight (0.1500f) * 1.0000f));
    sampleButton->setBounds (proportionOfWidth (0.0200f), (0 + proportionOfHeight (0.1500f) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2, proportionOfWidth (0.9001f), roundToInt (proportionOfHeight (0.1500f) * 1.0000f));
    octaveButton->setBounds (proportionOfWidth (0.0200f), ((0 + proportionOfHeight (0.1500f) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2, proportionOfWidth (0.9001f), roundToInt (proportionOfHeight (0.1500f) * 1.0000f));
    rhythmButton->setBounds (proportionOfWidth (0.0200f), (((0 + proportionOfHeight (0.1500f) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2, proportionOfWidth (0.9001f), roundToInt (proportionOfHeight (0.1500f) * 1.0000f));
    closeOptionsButton->setBounds (proportionOfWidth (0.0203f), ((((0 + proportionOfHeight (0.1500f) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1500f) * 1.0000f)) - -2, proportionOfWidth (0.9001f), roundToInt (proportionOfHeight (0.1500f) * 1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AreaOptionsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == volumeButton.get())
    {
        //[UserButtonCode_volumeButton] -- add your button handler code here..
		canvasComponent->optionButtonClicked(OptionButtonClicked::Volume);
        //[/UserButtonCode_volumeButton]
    }
    else if (buttonThatWasClicked == speedButton.get())
    {
        //[UserButtonCode_speedButton] -- add your button handler code here..
		canvasComponent->optionButtonClicked(OptionButtonClicked::Speed);
        //[/UserButtonCode_speedButton]
    }
    else if (buttonThatWasClicked == sampleButton.get())
    {
        //[UserButtonCode_sampleButton] -- add your button handler code here..
		canvasComponent->optionButtonClicked(OptionButtonClicked::Sample);
        //[/UserButtonCode_sampleButton]
    }
    else if (buttonThatWasClicked == octaveButton.get())
    {
        //[UserButtonCode_octaveButton] -- add your button handler code here..
		canvasComponent->optionButtonClicked(OptionButtonClicked::Octave);
        //[/UserButtonCode_octaveButton]
    }
    else if (buttonThatWasClicked == rhythmButton.get())
    {
        //[UserButtonCode_rhythmButton] -- add your button handler code here..
		canvasComponent->optionButtonClicked(OptionButtonClicked::Rhythm);
        //[/UserButtonCode_rhythmButton]
    }
    else if (buttonThatWasClicked == closeOptionsButton.get())
    {
        //[UserButtonCode_closeOptionsButton] -- add your button handler code here..
		setVisible(false);
		canvasComponent->optionButtonClicked(OptionButtonClicked::Closed);
        //[/UserButtonCode_closeOptionsButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AreaOptionsComponent::CompleteInitialisation(MultiSceneCanvasComponentAmusing * _canvasComponent)
{
	canvasComponent = _canvasComponent;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AreaOptionsComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="new button" id="5c62636afaa99798" memberName="volumeButton"
              virtualName="" explicitFocusOrder="0" pos="2.029% 0 90.008% 15.036%"
              buttonText="Volume" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="482bb152c7d9cc28" memberName="speedButton"
              virtualName="" explicitFocusOrder="0" pos="2.029% -2R 90.008% 100%"
              posRelativeY="5c62636afaa99798" posRelativeH="5c62636afaa99798"
              buttonText="Speed" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e95fad03e7a3c441" memberName="sampleButton"
              virtualName="" explicitFocusOrder="0" pos="2.029% -2R 90.008% 100%"
              posRelativeY="482bb152c7d9cc28" posRelativeH="5c62636afaa99798"
              buttonText="Sample" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5960e66316e7ae3d" memberName="octaveButton"
              virtualName="" explicitFocusOrder="0" pos="2.029% -2R 90.008% 100%"
              posRelativeY="e95fad03e7a3c441" posRelativeH="5c62636afaa99798"
              buttonText="Octave" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b237be36e238aabb" memberName="rhythmButton"
              virtualName="" explicitFocusOrder="0" pos="2.029% -2R 90.008% 100%"
              posRelativeY="5960e66316e7ae3d" posRelativeH="5c62636afaa99798"
              buttonText="Rhythm" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d76c1069cbfb80a3" memberName="closeOptionsButton"
              virtualName="" explicitFocusOrder="0" pos="2.029% -2R 90.008% 100%"
              posRelativeY="b237be36e238aabb" posRelativeH="5c62636afaa99798"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
