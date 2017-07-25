/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "AmusingSceneComponent.h"
//[/Headers]

#include "AreaOptions.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
AreaOptions::AreaOptions ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (baseNote = new Slider ("new slider"));
    baseNote->setRange (0, 10, 0);
    baseNote->setSliderStyle (Slider::LinearVertical);
    baseNote->setTextBoxStyle (Slider::TextBoxBelow, false, 24, 20);
    baseNote->setColour (Slider::textBoxTextColourId, Colours::white);
    baseNote->addListener (this);

    addAndMakeVisible (velocity = new Slider ("new slider"));
    velocity->setRange (0, 10, 0);
    velocity->setSliderStyle (Slider::LinearHorizontal);
    velocity->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    velocity->setColour (Slider::textBoxTextColourId, Colours::black);
    velocity->addListener (this);

    addAndMakeVisible (velocityLabel = new Label ("new label",
                                                  TRANS("speed")));
    velocityLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    velocityLabel->setJustificationType (Justification::centredLeft);
    velocityLabel->setEditable (false, false, false);
    velocityLabel->setColour (TextEditor::textColourId, Colours::black);
    velocityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
	velocity->setValue(1);
    //[/UserPreSize]

    setSize (200, 200);


    //[Constructor] You can add your own custom stuff here..
	velocity->setSkewFactor(1);
	customLook.setScaleMarking(velocity->getMinimum(), velocity->getMaximum(),velocity->getSkewFactor());
	velocity->setLookAndFeel(&customLook);
    //[/Constructor]
}

AreaOptions::~AreaOptions()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    baseNote = nullptr;
    velocity = nullptr;
    velocityLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AreaOptions::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AreaOptions::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    baseNote->setBounds (6, 4, 31, 176);
    velocity->setBounds (46, 156, 160, 24);
    velocityLabel->setBounds (206, 156, 50, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AreaOptions::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == baseNote)
    {
        //[UserSliderCode_baseNote] -- add your slider handling code here..
		DBG("ici");
		amusingSceneComponent->OnBaseNoteChanged(baseNote->getValue());
        //[/UserSliderCode_baseNote]
    }
    else if (sliderThatWasMoved == velocity)
    {
        //[UserSliderCode_velocity] -- add your slider handling code here..
		
		double value = velocity->getValue();
		DBG("speed = " + (String)value);
		double valueInt = round(value);
		double tol = 0.2;
		if (abs(value - valueInt) < tol)
		{
			value = valueInt;
			velocity->setValue(value);
		}
		amusingSceneComponent->OnSpeedChanged(value);
        //[/UserSliderCode_velocity]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void AreaOptions::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
	amusingSceneComponent->mouseDown(e.getEventRelativeTo(amusingSceneComponent));
    //[/UserCode_mouseDown]
}

void AreaOptions::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
	amusingSceneComponent->mouseDrag(e.getEventRelativeTo(amusingSceneComponent));
    //[/UserCode_mouseDrag]
}

void AreaOptions::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
	amusingSceneComponent->mouseUp(e.getEventRelativeTo(amusingSceneComponent));
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AreaOptions::CompleteInitialization(AmusingSceneComponent* m_amusingSceneComponent)
{
	DBG("initialization areaOptions");
	amusingSceneComponent = m_amusingSceneComponent;
}

void AreaOptions::setSpeedSliderValue(double speed)
{
	velocity->setValue(speed);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AreaOptions" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="200" initialHeight="200">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff"/>
  <SLIDER name="new slider" id="9e11577c496b9715" memberName="baseNote"
          virtualName="" explicitFocusOrder="0" pos="6 4 31 176" textboxtext="ffffffff"
          min="0" max="10" int="0" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="24" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="new slider" id="a3ec7f9759661bec" memberName="velocity"
          virtualName="" explicitFocusOrder="0" pos="46 156 160 24" textboxtext="ff000000"
          min="0" max="10" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="new label" id="6adeb9d99d4ed76a" memberName="velocityLabel"
         virtualName="" explicitFocusOrder="0" pos="206 156 50 24" edTextCol="ff000000"
         edBkgCol="0" labelText="speed" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
