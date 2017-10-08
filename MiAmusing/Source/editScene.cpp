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
#include "GraphicSessionManager.h"
#include "ControlEvent.h"
using namespace Amusing;
//[/Headers]

#include "editScene.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EditScene::EditScene ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (playButton = new TextButton ("new button"));
    playButton->setButtonText (TRANS("Play"));
    playButton->addListener (this);

    addAndMakeVisible (pauseButton = new TextButton ("new button"));
    pauseButton->setButtonText (TRANS("Pause"));
    pauseButton->addListener (this);

    addAndMakeVisible (stopButton = new TextButton ("new button"));
    stopButton->setButtonText (TRANS("Stop"));
    stopButton->addListener (this);

    addAndMakeVisible (addCarreButton = new TextButton ("new button"));
    addCarreButton->setButtonText (TRANS("Square"));
    addCarreButton->addListener (this);

    addAndMakeVisible (addTriangleButton = new TextButton ("new button"));
    addTriangleButton->setButtonText (TRANS("Triangle"));
    addTriangleButton->addListener (this);

    addAndMakeVisible (addTrueCircleButton = new TextButton ("new button"));
    addTrueCircleButton->setButtonText (TRANS("Circle"));
    addTrueCircleButton->addListener (this);

    addAndMakeVisible (deleteButton = new TextButton ("new button"));
    deleteButton->setButtonText (TRANS("Delete"));
    deleteButton->addListener (this);

    addAndMakeVisible (completeButton = new TextButton ("new button"));
    completeButton->setButtonText (TRANS("Hexagone"));
    completeButton->addListener (this);

    addAndMakeVisible (optionButton = new TextButton ("new button"));
    optionButton->setButtonText (TRANS("Options"));
    optionButton->addListener (this);

    addAndMakeVisible (comboBoxMidi = new ComboBox ("midiChannel"));
    comboBoxMidi->setEditableText (false);
    comboBoxMidi->setJustificationType (Justification::centredLeft);
    comboBoxMidi->setTextWhenNothingSelected (String());
    comboBoxMidi->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxMidi->addListener (this);

    addAndMakeVisible (timeSlider = new Slider ("new slider"));
    timeSlider->setRange (50, 200, 1);
    timeSlider->setSliderStyle (Slider::IncDecButtons);
    timeSlider->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    timeSlider->setColour (Slider::textBoxTextColourId, Colours::white);
    timeSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00152f3c));
    timeSlider->addListener (this);

    addAndMakeVisible (volumeSlider = new Slider ("new slider"));
    volumeSlider->setRange (0, 1, 0);
    volumeSlider->setSliderStyle (Slider::Rotary);
    volumeSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volumeSlider->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	timeSlider->setValue(4);
	for (int i = 0; i < 16;i++)
		comboBoxMidi->addItem("ch " + (String)(i+1), i+1);
    //[/Constructor]
}

EditScene::~EditScene()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    playButton = nullptr;
    pauseButton = nullptr;
    stopButton = nullptr;
    addCarreButton = nullptr;
    addTriangleButton = nullptr;
    addTrueCircleButton = nullptr;
    deleteButton = nullptr;
    completeButton = nullptr;
    optionButton = nullptr;
    comboBoxMidi = nullptr;
    timeSlider = nullptr;
    volumeSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EditScene::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EditScene::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    playButton->setBounds (8, 93, 50, 24);
    pauseButton->setBounds (8, 117, 50, 24);
    stopButton->setBounds (8, 141, 50, 24);
    addCarreButton->setBounds (8, 173, 50, 24);
    addTriangleButton->setBounds (8, 197, 50, 24);
    addTrueCircleButton->setBounds (8, 221, 50, 24);
    deleteButton->setBounds (8, 284, 50, 24);
    completeButton->setBounds (8, 245, 50, 24);
    optionButton->setBounds (8, 15, 50, 24);
    comboBoxMidi->setBounds (4, 318, 60, 24);
    timeSlider->setBounds (4, 352, 45, 40);
    volumeSlider->setBounds (0, 40, 64, 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EditScene::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == playButton)
    {
        //[UserButtonCode_playButton] -- add your button handler code here..

		graphicSessionManager->HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
        //[/UserButtonCode_playButton]
    }
    else if (buttonThatWasClicked == pauseButton)
    {
        //[UserButtonCode_pauseButton] -- add your button handler code here..
		graphicSessionManager->HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Pause)));
        //[/UserButtonCode_pauseButton]
    }
    else if (buttonThatWasClicked == stopButton)
    {
        //[UserButtonCode_stopButton] -- add your button handler code here..
		graphicSessionManager->HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Stop)));
        //[/UserButtonCode_stopButton]
    }
    else if (buttonThatWasClicked == addCarreButton)
    {
        //[UserButtonCode_addCarreButton] -- add your button handler code here..
        graphicSessionManager->OnAddSquare();
        //[/UserButtonCode_addCarreButton]
    }
    else if (buttonThatWasClicked == addTriangleButton)
    {
        //[UserButtonCode_addTriangleButton] -- add your button handler code here..
		graphicSessionManager->OnAddTriangle();
        //[/UserButtonCode_addTriangleButton]
    }
    else if (buttonThatWasClicked == addTrueCircleButton)
    {
        //[UserButtonCode_addTrueCircleButton] -- add your button handler code here..
		graphicSessionManager->OnAddTrueCircle();
        //[/UserButtonCode_addTrueCircleButton]
    }
    else if (buttonThatWasClicked == deleteButton)
    {
        //[UserButtonCode_deleteButton] -- add your button handler code here..
		//setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
		graphicSessionManager->OnDelete();
        //[/UserButtonCode_deleteButton]
    }
    else if (buttonThatWasClicked == completeButton)
    {
        //[UserButtonCode_completeButton] -- add your button handler code here..
		graphicSessionManager->OnAddComplete();
        //[/UserButtonCode_completeButton]
    }
    else if (buttonThatWasClicked == optionButton)
    {
        //[UserButtonCode_optionButton] -- add your button handler code here..
		graphicSessionManager->OnDeviceOptionsClicked();
        //[/UserButtonCode_optionButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void EditScene::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxMidi)
    {
        //[UserComboBoxCode_comboBoxMidi] -- add your combo box handling code here..
		//DBG("now select : " + comboBoxMidi->getText() + " with Id : " + (String)comboBoxMidi->getSelectedId());
		graphicSessionManager->SetMidiChannel(comboBoxMidi->getSelectedId());
        //[/UserComboBoxCode_comboBoxMidi]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void EditScene::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == timeSlider)
    {
        //[UserSliderCode_timeSlider] -- add your slider handling code here..
		graphicSessionManager->OnTempoChanged((int)timeSlider->getValue());//*1000);
        //[/UserSliderCode_timeSlider]
    }
    else if (sliderThatWasMoved == volumeSlider)
    {
        //[UserSliderCode_volumeSlider] -- add your slider handling code here..
		DBG("volumeSlider : " + (String)volumeSlider->getValue());
		graphicSessionManager->OnMasterVolumeChanged((float)volumeSlider->getValue());
        //[/UserSliderCode_volumeSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void EditScene::CompleteInitialization(GraphicSessionManager* _graphicSessionManager)//, MultiCanvasComponent* _multiCanvasComponent)
{
	DBG("Dans EditScene::CompleteInitialization");
    graphicSessionManager = _graphicSessionManager;
	DBG("Apres");
    //multiCanvasComponent = _multiCanvasComponent;
    //addAndMakeVisible(multiCanvasComponent);
}

void EditScene::setMidiChannel(int chan)
{
	comboBoxMidi->setSelectedId(chan);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EditScene" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="new button" id="98a87a5f1f2e9c34" memberName="playButton"
              virtualName="" explicitFocusOrder="0" pos="8 93 50 24" buttonText="Play"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5bbb70de73e3d61c" memberName="pauseButton"
              virtualName="" explicitFocusOrder="0" pos="8 117 50 24" buttonText="Pause"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c78ba22b761d0a8a" memberName="stopButton"
              virtualName="" explicitFocusOrder="0" pos="8 141 50 24" buttonText="Stop"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="571676e166394c53" memberName="addCarreButton"
              virtualName="" explicitFocusOrder="0" pos="8 173 50 24" buttonText="Square"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="50fbe27b27abfd3" memberName="addTriangleButton"
              virtualName="" explicitFocusOrder="0" pos="8 197 50 24" buttonText="Triangle"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="9ac75bac4695259b" memberName="addTrueCircleButton"
              virtualName="" explicitFocusOrder="0" pos="8 221 50 24" buttonText="Circle"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="bbaaf855224933f0" memberName="deleteButton"
              virtualName="" explicitFocusOrder="0" pos="8 284 50 24" buttonText="Delete"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3ae91a7e9eec6859" memberName="completeButton"
              virtualName="" explicitFocusOrder="0" pos="8 245 50 24" buttonText="Hexagone"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="753688072def86b" memberName="optionButton"
              virtualName="" explicitFocusOrder="0" pos="8 15 50 24" buttonText="Options"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="midiChannel" id="15ec0fa26eb54f8b" memberName="comboBoxMidi"
            virtualName="" explicitFocusOrder="0" pos="4 318 60 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="31e65db12379ed8f" memberName="timeSlider"
          virtualName="" explicitFocusOrder="0" pos="4 352 45 40" textboxtext="ffffffff"
          textboxbkgd="152f3c" min="50" max="200" int="1" style="IncDecButtons"
          textBoxPos="TextBoxAbove" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="new slider" id="8da9b88bff0269e8" memberName="volumeSlider"
          virtualName="" explicitFocusOrder="0" pos="0 40 64 48" min="0"
          max="1" int="0" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
