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

    baseNote.reset (new Slider ("new slider"));
    addAndMakeVisible (baseNote.get());
    baseNote->setRange (0, 10, 0);
    baseNote->setSliderStyle (Slider::LinearVertical);
    baseNote->setTextBoxStyle (Slider::TextBoxBelow, false, 24, 20);
    baseNote->setColour (Slider::textBoxTextColourId, Colours::white);
    baseNote->addListener (this);

    baseNote->setBounds (6, 4, 31, 176);

    speed.reset (new Slider ("new slider"));
    addAndMakeVisible (speed.get());
    speed->setRange (0, 6, 1);
    speed->setSliderStyle (Slider::LinearHorizontal);
    speed->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    speed->setColour (Slider::backgroundColourId, Colour (0x00263238));
    speed->setColour (Slider::textBoxTextColourId, Colours::black);
    speed->addListener (this);

    speed->setBounds (46, 164, 160, 24);

    speedLabel.reset (new Label ("new label",
                                 TRANS("speed")));
    addAndMakeVisible (speedLabel.get());
    speedLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    speedLabel->setJustificationType (Justification::centredLeft);
    speedLabel->setEditable (false, false, false);
    speedLabel->setColour (TextEditor::textColourId, Colours::black);
    speedLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    speedLabel->setBounds (206, 164, 50, 24);

    velocitySlider.reset (new Slider ("new slider"));
    addAndMakeVisible (velocitySlider.get());
    velocitySlider->setRange (0, 127, 0);
    velocitySlider->setSliderStyle (Slider::Rotary);
    velocitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    velocitySlider->addListener (this);

    velocitySlider->setBounds (223, 19, 71, 56);

    colorArray.reset (new ColorArray());
    addAndMakeVisible (colorArray.get());
    colorArray->setName ("new component");

    colorArray->setBounds (48, 16, 150, 24);


    //[UserPreSize]
	speed->setValue(1, NotificationType::dontSendNotification);
    //[/UserPreSize]

    setSize (200, 200);


    //[Constructor] You can add your own custom stuff here..
	speed->setSkewFactor(1);
	customLook.setScaleMarking(speed->getMinimum(), speed->getMaximum(),speed->getSkewFactor());
	speed->setLookAndFeel(&customLook);
	colorArray->completeInitialisation(this);
    //[/Constructor]
}

AreaOptions::~AreaOptions()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    baseNote = nullptr;
    speed = nullptr;
    speedLabel = nullptr;
    velocitySlider = nullptr;
    colorArray = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AreaOptions::paint (Graphics& /*g*/)
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

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AreaOptions::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == baseNote.get())
    {
        //[UserSliderCode_baseNote] -- add your slider handling code here..
		//DBG("ici");
		int octave = roundToInt(baseNote->getValue());
		baseNote->setValue(octave, NotificationType::dontSendNotification);
		amusingSceneComponent->OnBaseNoteChanged(octave);
        //[/UserSliderCode_baseNote]
    }
    else if (sliderThatWasMoved == speed.get())
    {
        //[UserSliderCode_speed] -- add your slider handling code here..
		double value = speed->getValue();
		DBG("speed = " + (String)value);
		double valueInt = round(value);
		double tol = 0.2;
		if (abs(value - valueInt) < tol)
		{
			value = valueInt;
			speed->setValue(value, NotificationType::dontSendNotification);
		}
		value = speedTable[(int)valueInt];
		amusingSceneComponent->OnSpeedChanged(value);


        //[/UserSliderCode_speed]
    }
    else if (sliderThatWasMoved == velocitySlider.get())
    {
        //[UserSliderCode_velocitySlider] -- add your slider handling code here..
		amusingSceneComponent->OnVelocityChanged(velocitySlider->getValue());
        //[/UserSliderCode_velocitySlider]
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

void AreaOptions::setSpeedSliderValue(double _speed)
{
	for (int i = 0; i < 7; ++i)
	{
		if(_speed == speedTable[i])
			speed->setValue(i,NotificationType::dontSendNotification);
	}
}

void AreaOptions::setOctaveSlider(int currentOctave)
{
	baseNote->setValue(currentOctave, NotificationType::dontSendNotification);
}

void AreaOptions::setVelocitySliderValue(double _velocity)
{
	velocitySlider->setValue(_velocity, NotificationType::dontSendNotification);
}

void AreaOptions::OnColorChanged(Colour newColour, int colourIdx)
{
	amusingSceneComponent->OnColourChanged(newColour,colourIdx);
}

void AreaOptions::setCurrentColorSelected(int idx)
{
	colorArray->setCurrentColorSelected(idx);
}

void AreaOptions::setSamplesColor(int Nsamples, Colour colorCode[])
{
	colorArray->setSamplesColor(Nsamples, colorCode);
}

void AreaOptions::addColourSample(int index, Colour colour)
{
	colorArray->addColourSample(index, colour);
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
          min="0.00000000000000000000" max="10.00000000000000000000" int="0.00000000000000000000"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="24" textBoxHeight="20" skewFactor="1.00000000000000000000"
          needsCallback="1"/>
  <SLIDER name="new slider" id="a3ec7f9759661bec" memberName="speed" virtualName=""
          explicitFocusOrder="0" pos="46 164 160 24" bkgcol="263238" textboxtext="ff000000"
          min="0.00000000000000000000" max="6.00000000000000000000" int="1.00000000000000000000"
          style="LinearHorizontal" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.00000000000000000000"
          needsCallback="1"/>
  <LABEL name="new label" id="6adeb9d99d4ed76a" memberName="speedLabel"
         virtualName="" explicitFocusOrder="0" pos="206 164 50 24" edTextCol="ff000000"
         edBkgCol="0" labelText="speed" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="50d98b58b91eff3b" memberName="velocitySlider"
          virtualName="" explicitFocusOrder="0" pos="223 19 71 56" min="0.00000000000000000000"
          max="127.00000000000000000000" int="0.00000000000000000000" style="Rotary"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <GENERICCOMPONENT name="new component" id="deb5bed8d718a864" memberName="colorArray"
                    virtualName="ColorArray" explicitFocusOrder="0" pos="48 16 150 24"
                    class="Component" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
