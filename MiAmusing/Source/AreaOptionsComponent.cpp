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

    soloSlider.reset (new Slider ("new slider"));
    addAndMakeVisible (soloSlider.get());
    soloSlider->setRange (0, 1, 1);
    soloSlider->setSliderStyle (Slider::LinearHorizontal);
    soloSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    soloSlider->addListener (this);

    soloLabel.reset (new Label ("new label",
                                TRANS("Solo")));
    addAndMakeVisible (soloLabel.get());
    soloLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    soloLabel->setJustificationType (Justification::centred);
    soloLabel->setEditable (false, false, false);
    soloLabel->setColour (TextEditor::textColourId, Colours::black);
    soloLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
	volumeButton->setAlpha(0.5f);
	speedButton->setAlpha(0.5f);
	sampleButton->setAlpha(0.5f);
	octaveButton->setAlpha(0.5f);
	rhythmButton->setAlpha(1.0f);
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
    soloSlider = nullptr;
    soloLabel = nullptr;


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

    volumeButton->setBounds (proportionOfWidth (0.0206f), (0 + 0) + proportionOfHeight (0.1306f), proportionOfWidth (0.9003f), proportionOfHeight (0.1306f));
    speedButton->setBounds (proportionOfWidth (0.0206f), ((0 + 0) + proportionOfHeight (0.1306f)) + proportionOfHeight (0.1306f) - -2, proportionOfWidth (0.9003f), roundToInt (proportionOfHeight (0.1306f) * 1.0000f));
    sampleButton->setBounds (proportionOfWidth (0.0206f), (((0 + 0) + proportionOfHeight (0.1306f)) + proportionOfHeight (0.1306f) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2, proportionOfWidth (0.9003f), roundToInt (proportionOfHeight (0.1306f) * 1.0000f));
    octaveButton->setBounds (proportionOfWidth (0.0206f), ((((0 + 0) + proportionOfHeight (0.1306f)) + proportionOfHeight (0.1306f) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2, proportionOfWidth (0.9003f), roundToInt (proportionOfHeight (0.1306f) * 1.0000f));
    rhythmButton->setBounds (proportionOfWidth (0.0206f), (((((0 + 0) + proportionOfHeight (0.1306f)) + proportionOfHeight (0.1306f) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2, proportionOfWidth (0.9003f), roundToInt (proportionOfHeight (0.1306f) * 1.0000f));
    closeOptionsButton->setBounds (proportionOfWidth (0.0206f), ((((((0 + 0) + proportionOfHeight (0.1306f)) + proportionOfHeight (0.1306f) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2) + (roundToInt (proportionOfHeight (0.1306f) * 1.0000f)) - -2, proportionOfWidth (0.9003f), roundToInt (proportionOfHeight (0.1306f) * 1.0000f));
    soloSlider->setBounds (proportionOfWidth (0.0260f) + proportionOfWidth (0.4496f), 0 + 0, proportionOfWidth (0.4496f), proportionOfHeight (0.1306f));
    soloLabel->setBounds (proportionOfWidth (0.0260f), 0, proportionOfWidth (0.4496f), proportionOfHeight (0.1306f));
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
		volumeButton->setAlpha(1.0f);
		speedButton->setAlpha(0.5f);
		sampleButton->setAlpha(0.5f);
		octaveButton->setAlpha(0.5f);
		rhythmButton->setAlpha(0.5f);
		canvasComponent->optionButtonClicked(OptionButtonClicked::Volume);
        //[/UserButtonCode_volumeButton]
    }
    else if (buttonThatWasClicked == speedButton.get())
    {
        //[UserButtonCode_speedButton] -- add your button handler code here..
		volumeButton->setAlpha(0.5f);
		speedButton->setAlpha(1.0f);
		sampleButton->setAlpha(0.5f);
		octaveButton->setAlpha(0.5f);
		rhythmButton->setAlpha(0.5f);
		canvasComponent->optionButtonClicked(OptionButtonClicked::Speed);
        //[/UserButtonCode_speedButton]
    }
    else if (buttonThatWasClicked == sampleButton.get())
    {
        //[UserButtonCode_sampleButton] -- add your button handler code here..
		volumeButton->setAlpha(0.5f);
		speedButton->setAlpha(0.5f);
		sampleButton->setAlpha(1.0f);
		octaveButton->setAlpha(0.5f);
		rhythmButton->setAlpha(0.5f);
		canvasComponent->optionButtonClicked(OptionButtonClicked::Sample);
        //[/UserButtonCode_sampleButton]
    }
    else if (buttonThatWasClicked == octaveButton.get())
    {
        //[UserButtonCode_octaveButton] -- add your button handler code here..
		volumeButton->setAlpha(0.5f);
		speedButton->setAlpha(0.5f);
		sampleButton->setAlpha(0.5f);
		octaveButton->setAlpha(1.0f);
		rhythmButton->setAlpha(0.5f);
		canvasComponent->optionButtonClicked(OptionButtonClicked::Octave);
        //[/UserButtonCode_octaveButton]
    }
    else if (buttonThatWasClicked == rhythmButton.get())
    {
        //[UserButtonCode_rhythmButton] -- add your button handler code here..
		volumeButton->setAlpha(0.5f);
		speedButton->setAlpha(0.5f);
		sampleButton->setAlpha(0.5f);
		octaveButton->setAlpha(0.5f);
		rhythmButton->setAlpha(1.0f);
		canvasComponent->optionButtonClicked(OptionButtonClicked::Rhythm);
        //[/UserButtonCode_rhythmButton]
    }
    else if (buttonThatWasClicked == closeOptionsButton.get())
    {
        //[UserButtonCode_closeOptionsButton] -- add your button handler code here..
		setVisible(false);
		volumeButton->setAlpha(0.5f);
		speedButton->setAlpha(0.5f);
		sampleButton->setAlpha(0.5f);
		octaveButton->setAlpha(0.5f);
		rhythmButton->setAlpha(1.0f);
		canvasComponent->optionButtonClicked(OptionButtonClicked::Closed);
        //[/UserButtonCode_closeOptionsButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void AreaOptionsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == soloSlider.get())
    {
        //[UserSliderCode_soloSlider] -- add your slider handling code here..
		canvasComponent->muteOtherAreas((int)sliderThatWasMoved->getValue());
        //[/UserSliderCode_soloSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
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
              virtualName="" explicitFocusOrder="0" pos="2.059% 0R 90.033% 13.061%"
              posRelativeY="50de1a8aa8ab7dae" buttonText="Volume" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="482bb152c7d9cc28" memberName="speedButton"
              virtualName="" explicitFocusOrder="0" pos="2.059% -2R 90.033% 100%"
              posRelativeY="5c62636afaa99798" posRelativeH="5c62636afaa99798"
              buttonText="Speed" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e95fad03e7a3c441" memberName="sampleButton"
              virtualName="" explicitFocusOrder="0" pos="2.059% -2R 90.033% 100%"
              posRelativeY="482bb152c7d9cc28" posRelativeH="5c62636afaa99798"
              buttonText="Sample" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5960e66316e7ae3d" memberName="octaveButton"
              virtualName="" explicitFocusOrder="0" pos="2.059% -2R 90.033% 100%"
              posRelativeY="e95fad03e7a3c441" posRelativeH="5c62636afaa99798"
              buttonText="Octave" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b237be36e238aabb" memberName="rhythmButton"
              virtualName="" explicitFocusOrder="0" pos="2.059% -2R 90.033% 100%"
              posRelativeY="5960e66316e7ae3d" posRelativeH="5c62636afaa99798"
              buttonText="Rhythm" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d76c1069cbfb80a3" memberName="closeOptionsButton"
              virtualName="" explicitFocusOrder="0" pos="2.059% -2R 90.033% 100%"
              posRelativeY="b237be36e238aabb" posRelativeH="5c62636afaa99798"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="50de1a8aa8ab7dae" memberName="soloSlider"
          virtualName="" explicitFocusOrder="0" pos="0R 0 44.962% 13.061%"
          posRelativeX="8621ed6c9e1093e6" posRelativeY="8621ed6c9e1093e6"
          min="0.00000000000000000000" max="1.00000000000000000000" int="1.00000000000000000000"
          style="LinearHorizontal" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.00000000000000000000"
          needsCallback="1"/>
  <LABEL name="new label" id="8621ed6c9e1093e6" memberName="soloLabel"
         virtualName="" explicitFocusOrder="0" pos="2.6% 0 44.962% 13.061%"
         edTextCol="ff000000" edBkgCol="0" labelText="Solo" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
