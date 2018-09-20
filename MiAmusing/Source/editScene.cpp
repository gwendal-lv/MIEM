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
#include <fstream>
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

    groupComponent.reset (new GroupComponent ("new group",
                                              String()));
    addAndMakeVisible (groupComponent.get());
    groupComponent->setColour (GroupComponent::outlineColourId, Colours::white);

    timeSlider.reset (new Slider ("new slider"));
    addAndMakeVisible (timeSlider.get());
    timeSlider->setRange (50, 200, 1);
    timeSlider->setSliderStyle (Slider::IncDecButtons);
    timeSlider->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    timeSlider->setColour (Slider::textBoxTextColourId, Colours::white);
    timeSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0x00152f3c));
    timeSlider->addListener (this);

    imgPlayButton.reset (new ImageButton ("imgPlaybutton"));
    addAndMakeVisible (imgPlayButton.get());
    imgPlayButton->setButtonText (TRANS("new button"));
    imgPlayButton->addListener (this);

    imgPlayButton->setImages (false, true, true,
                              ImageCache::getFromMemory (lecture_png, lecture_pngSize), 1.000f, Colour (0x00000000),
                              ImageCache::getFromMemory (lectureOn_png, lectureOn_pngSize), 1.000f, Colour (0x00000000),
                              ImageCache::getFromMemory (lectureOn_png, lectureOn_pngSize), 1.000f, Colour (0x00000000));
    imgStopButton.reset (new ImageButton ("imgStopButton"));
    addAndMakeVisible (imgStopButton.get());
    imgStopButton->setButtonText (TRANS("new button"));
    imgStopButton->addListener (this);

    imgStopButton->setImages (false, true, true,
                              ImageCache::getFromMemory (stop_png, stop_pngSize), 1.000f, Colour (0x00000000),
                              ImageCache::getFromMemory (stopOn_png, stopOn_pngSize), 1.000f, Colour (0x00000000),
                              ImageCache::getFromMemory (stopOn_png, stopOn_pngSize), 1.000f, Colour (0x00000000));
    imgPauseButton.reset (new ImageButton ("imgPauseButton"));
    addAndMakeVisible (imgPauseButton.get());
    imgPauseButton->setButtonText (TRANS("new button"));
    imgPauseButton->addListener (this);

    imgPauseButton->setImages (false, true, true,
                               ImageCache::getFromMemory (pause_png, pause_pngSize), 1.000f, Colour (0x00000000),
                               ImageCache::getFromMemory (pauseOn_png, pauseOn_pngSize), 1.000f, Colour (0x00000000),
                               ImageCache::getFromMemory (pauseOn_png, pauseOn_pngSize), 1.000f, Colour (0x00000000));
    imgOptionButton.reset (new ImageButton ("imgOptionButton"));
    addAndMakeVisible (imgOptionButton.get());
    imgOptionButton->setButtonText (TRANS("new button"));
    imgOptionButton->addListener (this);

    imgOptionButton->setImages (false, true, true,
                                ImageCache::getFromMemory (option_png, option_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (optionOn_png, optionOn_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (optionOn_png, optionOn_pngSize), 1.000f, Colour (0x00000000));
    imgDeleteButton.reset (new ImageButton ("imgDeleteButton"));
    addAndMakeVisible (imgDeleteButton.get());
    imgDeleteButton->setButtonText (TRANS("new button"));
    imgDeleteButton->addListener (this);

    imgDeleteButton->setImages (false, true, true,
                                ImageCache::getFromMemory (delete_png, delete_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (deleteOn_png, deleteOn_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (deleteOn_png, deleteOn_pngSize), 1.000f, Colour (0x00000000));
    textButton.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("samples"));
    textButton->addListener (this);

    label.reset (new Label ("new label",
                            TRANS("amusing")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addCarreShapeButton.reset (new ShapeButton ("addCarreShape",Colours::white,Colours::blue,Colours::blue));
    addAndMakeVisible (addCarreShapeButton.get());
    addCarreShapeButton->setName ("new component");

    addTriangleShapeButton2.reset (new ShapeButton ("addCarreShape",Colours::white,Colours::blue,Colours::blue));
    addAndMakeVisible (addTriangleShapeButton2.get());
    addTriangleShapeButton2->setName ("new component");

    addHexaShapeButton.reset (new ShapeButton ("addHexaShape",Colours::white,Colours::blue,Colours::blue));
    addAndMakeVisible (addHexaShapeButton.get());
    addHexaShapeButton->setName ("new component");

    saveButton.reset (new TextButton ("saveButton"));
    addAndMakeVisible (saveButton.get());
    saveButton->setButtonText (TRANS("save"));
    saveButton->addListener (this);

    loadButton.reset (new TextButton ("loadButton"));
    addAndMakeVisible (loadButton.get());
    loadButton->setButtonText (TRANS("load"));
    loadButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	addCarreShapeButton->addListener(this);
	addTriangleShapeButton2->addListener(this);
	addHexaShapeButton->addListener(this);

	Path squarePath, trianglePath, hexaPath;
	squarePath.addPolygon(Point<float>((float)addCarreShapeButton->getWidth() / 2.0f, (float)addCarreShapeButton->getHeight() / 2.0f), 4, (float)addCarreShapeButton->getHeight() / 2.0f);
	//squarePath.addRectangle(float(addCarreShapeButton->getWidth() - addCarreShapeButton->getHeight()) / 2.0f, 0, addCarreShapeButton->getHeight(), addCarreShapeButton->getHeight());
	//trianglePath.addTriangle(float(addCarreShapeButton->getWidth() - addCarreShapeButton->getHeight()) / 2.0f, 0.0f, float(addCarreShapeButton->getWidth() - addCarreShapeButton->getHeight()) / 2.0f, getHeight(), float(addCarreShapeButton->getWidth() + addCarreShapeButton->getHeight()) / 2.0f, (float)getHeight() / 2.0f);
	trianglePath.addPolygon(Point<float>((float)addTriangleShapeButton2->getWidth() / 2.0f, (float)addTriangleShapeButton2->getHeight() / 2.0f), 3, (float)addTriangleShapeButton2->getHeight() / 2.0f,(float)M_PI/2.0f);
	hexaPath.addPolygon(Point<float>((float)addHexaShapeButton->getWidth() / 2.0f, (float)addHexaShapeButton->getHeight() / 2.0f), 6, (float)addHexaShapeButton->getHeight() / 2.0f);

	addCarreShapeButton->setShape(squarePath, false, true, false);
	addTriangleShapeButton2->setShape(trianglePath, false, true, false);
	addHexaShapeButton->setShape(hexaPath, false, true, false);

	addCarreShapeButton->addMouseListener(this, true);
	addTriangleShapeButton2->addMouseListener(this, true);
	addHexaShapeButton->addMouseListener(this, true);

	timeSlider->setValue(4);


	imgPauseButton->setState(Button::ButtonState::buttonDown);
	imgStopButton->setState(Button::ButtonState::buttonDown);

	isAddEnabled = true;

	saveFileExists = true;
	std::ifstream ifs("save.xml");
	if (!ifs.good())
	{
		saveFileExists = false;
		loadButton->setAlpha(0.5f);
		ifs.close();
	}
	else
	{
		ifs.close();
	}

	hasExited = false;
	prepareToAdd = 0;
	//textButton->setVisible(false);
    //[/Constructor]
}

EditScene::~EditScene()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupComponent = nullptr;
    timeSlider = nullptr;
    imgPlayButton = nullptr;
    imgStopButton = nullptr;
    imgPauseButton = nullptr;
    imgOptionButton = nullptr;
    imgDeleteButton = nullptr;
    textButton = nullptr;
    label = nullptr;
    addCarreShapeButton = nullptr;
    addTriangleShapeButton2 = nullptr;
    addHexaShapeButton = nullptr;
    saveButton = nullptr;
    loadButton = nullptr;


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

    groupComponent->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    timeSlider->setBounds (((getWidth() / 2) - (proportionOfWidth (0.8000f) / 2)) + 0, ((((((proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f), proportionOfWidth (0.8000f), proportionOfHeight (0.0752f));
    imgPlayButton->setBounds ((getWidth() / 2) - (proportionOfWidth (0.8001f) / 2), proportionOfHeight (0.0752f), proportionOfWidth (0.8001f), proportionOfHeight (0.0752f));
    imgStopButton->setBounds ((((getWidth() / 2) - (proportionOfWidth (0.8001f) / 2)) + 0) + 0, (proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f), roundToInt (proportionOfWidth (0.8001f) * 1.0000f), proportionOfHeight (0.0752f));
    imgPauseButton->setBounds (((getWidth() / 2) - (proportionOfWidth (0.8001f) / 2)) + 0, proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f), roundToInt (proportionOfWidth (0.8001f) * 1.0000f), proportionOfHeight (0.0752f));
    imgOptionButton->setBounds (proportionOfWidth (0.1000f), proportionOfHeight (0.9487f) + roundToInt (proportionOfHeight (0.0513f) * -1.0000f), proportionOfWidth (0.8000f), proportionOfHeight (0.0752f));
    imgDeleteButton->setBounds (((((getWidth() / 2) - (proportionOfWidth (0.8001f) / 2)) + 0) + 0) + 0, ((proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f), roundToInt (proportionOfWidth (0.8001f) * 1.0000f), proportionOfHeight (0.0752f));
    textButton->setBounds (proportionOfWidth (0.1000f), (((((((proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f) - -32, proportionOfWidth (0.8000f), proportionOfHeight (0.0752f));
    label->setBounds (proportionOfWidth (0.1000f), proportionOfHeight (0.9487f), proportionOfWidth (0.8000f), proportionOfHeight (0.0513f));
    addCarreShapeButton->setBounds ((getWidth() / 2) - (proportionOfWidth (0.8000f) / 2), (((proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f), proportionOfWidth (0.8000f), proportionOfHeight (0.0752f));
    addTriangleShapeButton2->setBounds (((getWidth() / 2) - (proportionOfWidth (0.8000f) / 2)) + 0, ((((proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f), proportionOfWidth (0.8000f), proportionOfHeight (0.0752f));
    addHexaShapeButton->setBounds ((((getWidth() / 2) - (proportionOfWidth (0.8000f) / 2)) + 0) + 0, (((((proportionOfHeight (0.0752f) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f)) + proportionOfHeight (0.0752f), proportionOfWidth (0.8000f), proportionOfHeight (0.0752f));
    saveButton->setBounds (proportionOfWidth (0.1217f), proportionOfHeight (0.0203f), proportionOfWidth (0.2983f), proportionOfHeight (0.0513f));
    loadButton->setBounds (proportionOfWidth (0.6018f), proportionOfHeight (0.0203f), proportionOfWidth (0.2983f), proportionOfHeight (0.0513f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EditScene::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == timeSlider.get())
    {
        //[UserSliderCode_timeSlider] -- add your slider handling code here..
		graphicSessionManager->OnTempoChanged((int)timeSlider->getValue());//*1000);
        //[/UserSliderCode_timeSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void EditScene::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
	if (isAddEnabled)
	{
		if (buttonThatWasClicked == addCarreShapeButton.get())
		{
			graphicSessionManager->OnAddSquare();
			prepareToAdd = 0;
			return;
		}
		else if (buttonThatWasClicked == addTriangleShapeButton2.get())
		{
			graphicSessionManager->OnAddTriangle();
			prepareToAdd = 0;
			return;
		}
		else if (buttonThatWasClicked == addHexaShapeButton.get())
		{
			graphicSessionManager->OnAddHexa();
			prepareToAdd = 0;
			return;
		}

	}
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == imgPlayButton.get())
    {
        //[UserButtonCode_imgPlayButton] -- add your button handler code here..
		graphicSessionManager->HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
		imgPlayButton->setState(Button::ButtonState::buttonDown);
		imgPauseButton->setState(Button::ButtonState::buttonNormal);
		imgStopButton->setState(Button::ButtonState::buttonNormal);
        //[/UserButtonCode_imgPlayButton]
    }
    else if (buttonThatWasClicked == imgStopButton.get())
    {
        //[UserButtonCode_imgStopButton] -- add your button handler code here..
		graphicSessionManager->HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Stop)));
		imgPlayButton->setState(Button::ButtonState::buttonNormal);
		imgPauseButton->setState(Button::ButtonState::buttonDown);
		imgStopButton->setState(Button::ButtonState::buttonDown);
        //[/UserButtonCode_imgStopButton]
    }
    else if (buttonThatWasClicked == imgPauseButton.get())
    {
        //[UserButtonCode_imgPauseButton] -- add your button handler code here..
        graphicSessionManager->HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Pause)));
		imgPlayButton->setState(Button::ButtonState::buttonNormal);
		imgPauseButton->setState(Button::ButtonState::buttonDown);
		imgStopButton->setState(Button::ButtonState::buttonNormal);
        //[/UserButtonCode_imgPauseButton]
    }
    else if (buttonThatWasClicked == imgOptionButton.get())
    {
        //[UserButtonCode_imgOptionButton] -- add your button handler code here..
        graphicSessionManager->OnDeviceOptionsClicked();
        //[/UserButtonCode_imgOptionButton]
    }
    else if (buttonThatWasClicked == imgDeleteButton.get())
    {
        //[UserButtonCode_imgDeleteButton] -- add your button handler code here..
        graphicSessionManager->OnDelete();
        //[/UserButtonCode_imgDeleteButton]
    }
    else if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
		//graphicSessionManager->OnTestChangeSound();
		graphicSessionManager->OnSoundClick();
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == saveButton.get())
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
		graphicSessionManager->OnSave("save.xml");
		loadButton->setAlpha(1.0f);
		saveFileExists = true;
        //[/UserButtonCode_saveButton]
    }
    else if (buttonThatWasClicked == loadButton.get())
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
		if (saveFileExists)
		{
			graphicSessionManager->OnLoad("save.xml");

		}
        //[/UserButtonCode_loadButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void EditScene::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
	if(!contains(e.getPosition()))
		DBG("exit now");
    //[/UserCode_mouseExit]
}

void EditScene::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
	
	if (!contains(e.getPosition()))
	{
		if (!hasExited)
		{
			DBG((String)e.x + " " + (String)e.y);
			hasExited = true;
			switch (prepareToAdd)
			{
			case 0 :
				// nothing to do
				break;
			case 1 :
				exitPosition = e.getEventRelativeTo(this).getPosition();
				graphicSessionManager->OnAddAndSelectSquare(e.withNewPosition(Point<int>(0, e.getEventRelativeTo(this).y)));
				
				break;
			case 2 :
				graphicSessionManager->OnAddTriangle();
				break;
			case 3 :
				graphicSessionManager->OnAddHexa();
				break;
			default:
				break;
			}
		}
		else
		{
			DBG("continueToDrag : " + (String)e.x + " " + (String)e.y);
			
			// reste dehors
			if (prepareToAdd == 0)
				DBG("no need to add shape");
			else if (prepareToAdd == 1)
			{
				DBG("need to add square");
				Point<int> canvasHitPoint(e.getEventRelativeTo(this).getPosition());
				canvasHitPoint.addXY(-exitPosition.x, 0);
				graphicSessionManager->TransmitMouseDrag(e.withNewPosition(canvasHitPoint));
			}
			else if (prepareToAdd == 2)
				DBG("need to add triangle");
			else if (prepareToAdd == 3)
				DBG("need to add hexa");
			
		}
	}
	else
	{
		DBG("drag");
		if (hasExited) // est sorti avant de rerentrer dans le menu
		{
			// il faut supprimer la forme qui a été ajouté en "glissant" vers le canvas : 
			// 1) dire qu'on supprime
			graphicSessionManager->OnDelete();
			// 2) transmettre un mouseDown à l'emplacement de la forme
			Point<int> canvasHitPoint(e.getEventRelativeTo(this).getPosition());
			canvasHitPoint.addXY(-exitPosition.x, 0);
			graphicSessionManager->SendDeletingMouseDown(e.withNewPosition(canvasHitPoint));
			hasExited = false;
		}
	}
    //[/UserCode_mouseDrag]
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

void EditScene::setMidiChannel(int /*chan*/)
{
	//comboBoxMidi->setSelectedId(chan);
}

void EditScene::hideAddPolygon()
{
	addCarreShapeButton->setAlpha(0.5);
	addTriangleShapeButton2->setAlpha(0.5);
	addHexaShapeButton->setAlpha(0.5);
	addCarreShapeButton->setColours(Colours::white, Colours::white, Colours::white);
	addTriangleShapeButton2->setColours(Colours::white, Colours::white, Colours::white);
	addHexaShapeButton->setColours(Colours::white, Colours::white, Colours::white);
	isAddEnabled = false;
}

void EditScene::showAddPolygon()
{
	addCarreShapeButton->setAlpha(1.0);
	addTriangleShapeButton2->setAlpha(1.0);
	addHexaShapeButton->setAlpha(1.0);
	addCarreShapeButton->setColours(Colours::white, Colours::blue, Colours::blue);
	addTriangleShapeButton2->setColours(Colours::white, Colours::blue, Colours::blue);
	addHexaShapeButton->setColours(Colours::white, Colours::blue, Colours::blue);
	isAddEnabled = true;
}

void EditScene::setTempoSlider(int newTempo)
{
	timeSlider->setValue(newTempo);
}

void EditScene::buttonStateChanged(Button *concernedButton)
{
	if (isAddEnabled)
	{
		if (concernedButton == addCarreShapeButton.get())
		{
			Button::ButtonState newState = addCarreShapeButton->getState();
			if (newState == Button::ButtonState::buttonDown)
				prepareToAdd = 1;
			return;
		}
		else if (concernedButton == addTriangleShapeButton2.get())
		{
			Button::ButtonState newState = addTriangleShapeButton2->getState();
			if (newState == Button::ButtonState::buttonDown)
				prepareToAdd = 2;
			return;
		}
		else if (concernedButton == addHexaShapeButton.get())
		{
			Button::ButtonState newState = addHexaShapeButton->getState();
			if (newState == Button::ButtonState::buttonDown)
				prepareToAdd = 3;
			return;
		}
	}
}

void EditScene::mouseUp(const MouseEvent& e)
{
	if (prepareToAdd != 0)
		prepareToAdd = 0;
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
  <METHODS>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="b0496b43bf1770c8" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" outlinecol="ffffffff"
                  title=""/>
  <SLIDER name="new slider" id="31e65db12379ed8f" memberName="timeSlider"
          virtualName="" explicitFocusOrder="0" pos="0 -9R 93.891% 7.556%"
          posRelativeX="15ec0fa26eb54f8b" posRelativeY="15ec0fa26eb54f8b"
          textboxtext="ffffffff" textboxbkgd="152f3c" min="50.00000000000000000000"
          max="200.00000000000000000000" int="1.00000000000000000000" style="IncDecButtons"
          textBoxPos="TextBoxAbove" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <IMAGEBUTTON name="imgPlaybutton" id="2b4803c7ccf2d5d1" memberName="imgPlayButton"
               virtualName="" explicitFocusOrder="0" pos="0 7.556% 100% 7.556%"
               buttonText="new button" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="lecture_png"
               opacityNormal="1.00000000000000000000" colourNormal="0" resourceOver="lectureOn_png"
               opacityOver="1.00000000000000000000" colourOver="0" resourceDown="lectureOn_png"
               opacityDown="1.00000000000000000000" colourDown="0"/>
  <IMAGEBUTTON name="imgStopButton" id="1ea8f58bfe76ab9b" memberName="imgStopButton"
               virtualName="" explicitFocusOrder="0" pos="0 0R 100% 7.556%"
               posRelativeX="210adb97ba16e19" posRelativeY="210adb97ba16e19"
               buttonText="new button" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="stop_png"
               opacityNormal="1.00000000000000000000" colourNormal="0" resourceOver="stopOn_png"
               opacityOver="1.00000000000000000000" colourOver="0" resourceDown="stopOn_png"
               opacityDown="1.00000000000000000000" colourDown="0"/>
  <IMAGEBUTTON name="imgPauseButton" id="210adb97ba16e19" memberName="imgPauseButton"
               virtualName="" explicitFocusOrder="0" pos="0 0R 100% 7.556%"
               posRelativeX="2b4803c7ccf2d5d1" posRelativeY="2b4803c7ccf2d5d1"
               buttonText="new button" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="pause_png"
               opacityNormal="1.00000000000000000000" colourNormal="0" resourceOver="pauseOn_png"
               opacityOver="1.00000000000000000000" colourOver="0" resourceDown="pauseOn_png"
               opacityDown="1.00000000000000000000" colourDown="0"/>
  <IMAGEBUTTON name="imgOptionButton" id="bbfd3cbea2a71bfd" memberName="imgOptionButton"
               virtualName="" explicitFocusOrder="0" pos="0 -100% 100% 7.556%"
               posRelativeY="289502be800b82cf" buttonText="new button" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal="option_png"
               opacityNormal="1.00000000000000000000" colourNormal="0" resourceOver="optionOn_png"
               opacityOver="1.00000000000000000000" colourOver="0" resourceDown="optionOn_png"
               opacityDown="1.00000000000000000000" colourDown="0"/>
  <IMAGEBUTTON name="imgDeleteButton" id="9d5289b3af882e28" memberName="imgDeleteButton"
               virtualName="" explicitFocusOrder="0" pos="0 0R 100% 7.556%"
               posRelativeX="1ea8f58bfe76ab9b" posRelativeY="1ea8f58bfe76ab9b"
               buttonText="new button" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="delete_png"
               opacityNormal="1.00000000000000000000" colourNormal="0" resourceOver="deleteOn_png"
               opacityOver="1.00000000000000000000" colourOver="0" resourceDown="deleteOn_png"
               opacityDown="1.00000000000000000000" colourDown="0"/>
  <TEXTBUTTON name="new button" id="c84bea64b985b44" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="3.537% -32R 94.855% 7.556%"
              posRelativeY="31e65db12379ed8f" buttonText="samples" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="289502be800b82cf" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="2.572% 94.889% 94.855% 5.111%" edTextCol="ff000000"
         edBkgCol="0" labelText="amusing" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="1783a06d564fe381" memberName="addCarreShapeButton"
                    virtualName="ShapeButton" explicitFocusOrder="0" pos="0% 0R 100% 7.556%"
                    posRelativeY="9d5289b3af882e28" class="Component" params="&quot;addCarreShape&quot;,Colours::white,Colours::blue,Colours::blue"/>
  <GENERICCOMPONENT name="new component" id="ffe0a52b5cd87f07" memberName="addTriangleShapeButton2"
                    virtualName="ShapeButton" explicitFocusOrder="0" pos="0 0R 100% 7.556%"
                    posRelativeX="1783a06d564fe381" posRelativeY="1783a06d564fe381"
                    class="Component" params="&quot;addCarreShape&quot;,Colours::white,Colours::blue,Colours::blue"/>
  <GENERICCOMPONENT name="new component" id="581ab4124f4712ed" memberName="addHexaShapeButton"
                    virtualName="ShapeButton" explicitFocusOrder="0" pos="0 0R 100% 7.556%"
                    posRelativeX="ffe0a52b5cd87f07" posRelativeY="ffe0a52b5cd87f07"
                    class="Component" params="&quot;addHexaShape&quot;,Colours::white,Colours::blue,Colours::blue"/>
  <TEXTBUTTON name="saveButton" id="48fde7c3628a30fd" memberName="saveButton"
              virtualName="" explicitFocusOrder="0" pos="12.219% 2% 29.904% 5.111%"
              buttonText="save" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="loadButton" id="3e438b12d4d1e208" memberName="loadButton"
              virtualName="" explicitFocusOrder="0" pos="60.129% 2% 29.904% 5.111%"
              buttonText="load" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: stopOn_png, 4033, "../../MiamCommon/Resources/StopOn.png"
static const unsigned char resource_EditScene_stopOn_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,
156,24,0,0,8,176,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,
48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,
97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,48,
56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,
47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,
58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,
46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,
97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,
47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,115,116,82,101,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,
48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,
99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,
40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,54,43,48,49,58,48,48,34,32,120,109,112,58,77,
101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,53,58,53,54,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,
49,45,48,57,84,48,56,58,53,53,58,53,54,43,48,49,58,48,48,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,
34,120,109,112,46,105,105,100,58,98,48,56,98,99,100,53,101,45,101,48,49,48,45,52,52,99,98,45,98,102,54,56,45,98,50,52,100,49,54,99,56,102,53,50,50,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,
116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,54,54,99,57,99,101,97,50,45,98,51,51,57,45,55,51,52,49,45,57,100,52,100,45,53,55,100,51,51,98,99,54,56,
101,53,50,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,
45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,
80,114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,
58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,102,101,
53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,
52,58,48,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,
116,111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,99,54,57,100,52,97,102,50,45,55,56,51,101,45,52,97,55,48,45,56,48,49,49,45,102,97,97,102,56,99,48,97,56,57,97,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,
45,49,49,45,48,57,84,48,56,58,53,53,58,53,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,
32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,
34,99,111,110,118,101,114,116,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,111,98,
101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,100,101,114,105,118,
101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,99,111,110,118,101,114,116,101,100,32,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,
111,98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,
101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,98,48,56,98,99,100,53,101,45,101,48,49,48,45,52,52,99,98,45,98,102,54,56,45,98,50,52,100,49,
54,99,56,102,53,50,50,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,53,58,53,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,
101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,
47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,82,101,102,58,105,
110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,99,54,57,100,52,97,102,50,45,55,56,51,101,45,52,97,55,48,45,56,48,49,49,45,102,97,97,102,56,99,48,97,56,57,97,55,34,32,115,116,82,101,
102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,
32,115,116,82,101,102,58,111,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,
102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,47,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,
97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,62,72,137,150,126,0,0,6,183,73,68,65,84,120,156,237,156,223,111,83,215,29,192,63,231,250,198,17,53,56,201,42,45,45,34,146,99,144,
104,131,32,234,195,152,34,34,90,137,182,113,197,131,39,241,111,244,161,235,195,34,173,145,186,7,94,216,67,187,135,238,207,64,154,85,24,238,84,54,186,145,69,165,173,38,138,160,85,161,137,69,88,33,173,74,
98,7,135,197,190,190,103,15,223,99,112,81,98,95,114,220,11,142,206,71,178,110,112,208,185,247,251,201,185,63,206,247,156,251,85,90,107,28,91,199,123,210,7,208,235,56,129,150,56,129,150,56,129,150,248,
27,125,249,155,143,222,136,251,56,122,134,191,76,157,251,201,191,93,15,180,100,195,30,216,6,5,36,128,126,243,73,34,127,4,213,229,227,138,11,13,132,64,13,88,55,159,134,249,62,18,81,5,42,243,127,119,0,131,
192,47,204,54,133,72,236,101,129,53,160,10,172,0,119,205,246,62,16,16,65,100,20,129,10,145,52,0,236,6,70,129,140,249,121,16,233,137,94,148,157,61,101,40,164,247,173,35,210,190,3,74,192,130,249,185,140,
200,109,27,87,20,129,62,34,111,31,48,174,148,58,152,27,57,126,248,149,231,143,101,71,119,101,211,26,141,166,55,135,51,10,165,20,138,133,213,249,202,133,219,231,231,139,139,103,47,105,173,175,0,151,129,
27,192,50,80,111,215,70,39,129,10,57,109,119,3,227,233,100,250,232,244,248,204,84,58,57,208,95,40,157,158,159,91,154,157,173,6,213,42,17,187,251,83,134,2,252,148,159,74,77,12,31,201,228,51,39,94,156,28,
62,154,61,117,249,228,80,165,86,1,88,3,254,71,135,216,58,9,76,32,167,233,168,82,234,224,244,248,204,84,185,94,214,239,126,241,251,143,131,48,184,134,116,249,149,78,59,121,74,81,128,95,13,170,131,31,255,
247,111,153,11,183,255,62,246,246,161,233,201,233,241,153,169,153,207,167,151,181,214,119,144,107,226,26,18,223,134,116,18,216,143,220,48,50,185,145,227,135,211,201,129,254,63,124,241,206,249,32,12,230,
128,255,0,139,192,61,228,206,213,139,36,128,157,64,41,8,131,242,123,95,158,226,253,137,63,191,154,27,57,126,248,220,205,51,215,145,235,225,143,88,10,28,4,118,191,242,252,177,108,161,116,122,190,30,214,
175,34,242,174,152,198,235,133,92,49,236,66,48,177,147,47,230,60,160,15,57,139,8,194,96,160,80,58,157,125,109,207,27,217,115,55,207,180,222,36,171,155,181,209,233,65,58,137,60,170,12,102,118,141,166,231,
150,102,75,200,105,187,136,200,171,245,170,60,0,115,236,53,36,150,69,160,52,183,52,91,26,221,149,77,243,211,199,180,77,233,36,208,51,13,244,3,152,27,198,10,114,218,214,233,189,235,222,70,104,36,150,123,
192,74,53,168,86,181,132,213,58,80,216,148,78,2,149,249,120,230,81,37,48,159,6,242,12,181,93,8,145,152,2,32,48,177,54,71,88,109,7,9,81,199,194,186,101,187,29,122,221,102,180,198,23,41,78,151,76,176,196,
9,180,196,9,180,196,9,180,196,9,180,196,9,180,228,113,51,210,86,152,161,83,194,236,183,155,153,236,102,102,57,0,26,113,142,142,98,19,152,47,230,250,128,103,144,220,98,154,135,137,216,110,208,76,140,86,
128,114,190,152,91,43,228,138,109,243,120,221,34,22,129,166,231,61,131,228,21,247,1,123,16,137,137,46,237,162,129,200,187,133,36,66,191,203,23,115,171,113,244,196,184,122,96,2,147,213,222,155,222,247,
250,228,115,47,79,6,58,216,161,186,52,170,209,160,124,229,223,255,247,210,191,102,175,151,191,1,201,158,172,17,195,112,51,46,129,62,210,227,246,76,62,247,242,228,175,126,249,235,253,95,47,95,171,122,94,
162,230,161,26,10,181,37,145,26,173,66,116,34,12,27,201,23,134,198,82,0,215,203,223,92,69,242,120,63,208,33,29,223,13,226,18,232,33,215,188,116,160,131,29,95,47,95,171,126,112,245,79,87,120,152,176,220,
106,160,125,192,179,192,232,155,7,222,58,24,232,96,7,221,191,190,182,37,46,129,10,9,40,161,64,123,94,162,134,200,187,132,228,23,55,77,88,118,32,133,204,16,226,121,137,253,230,146,144,32,198,185,234,88,
31,99,154,120,168,6,210,243,74,192,87,200,13,96,43,164,205,118,175,105,51,118,158,136,64,115,205,171,35,61,175,82,200,21,87,182,210,78,190,152,195,180,81,223,234,117,212,22,55,18,177,196,9,180,196,9,180,
196,9,180,196,9,180,196,9,180,196,9,180,196,9,180,196,9,180,196,9,180,196,9,180,196,9,180,196,9,180,228,137,100,99,52,90,33,201,208,20,144,54,89,149,173,144,54,109,244,153,54,99,231,137,8,12,209,9,36,
147,156,49,95,217,38,84,159,53,109,198,78,92,2,155,243,182,13,13,42,12,27,73,228,125,19,128,189,116,33,165,31,134,141,164,150,44,116,115,237,98,44,249,193,184,4,62,152,183,245,149,127,255,133,161,177,
212,155,7,222,58,232,121,137,253,221,156,84,250,236,251,79,111,33,217,237,117,98,90,0,26,151,192,0,51,111,123,241,206,39,23,1,186,61,173,249,217,247,159,222,154,189,243,207,139,200,220,112,133,54,43,235,
187,73,92,2,27,200,171,83,55,190,173,220,224,219,202,141,107,252,188,19,235,101,98,122,245,34,22,129,133,92,49,204,23,115,107,200,59,104,85,100,70,238,103,91,218,1,172,197,181,62,38,182,187,112,33,87,
172,231,139,185,85,100,197,192,15,184,197,69,143,143,9,44,36,134,21,3,113,225,70,34,150,56,129,150,56,129,150,56,129,150,56,129,150,56,129,150,68,21,168,90,182,189,90,161,35,10,173,241,69,138,179,147,
192,230,203,119,161,66,53,75,159,248,60,92,131,183,93,104,125,123,192,55,177,54,51,58,109,199,235,157,36,180,22,165,33,229,167,82,200,139,200,59,145,84,210,118,232,141,205,228,238,78,96,48,229,167,82,
74,194,90,71,98,111,59,170,233,36,240,65,81,154,210,234,66,101,98,248,72,6,73,96,142,32,121,184,164,89,129,223,147,152,99,79,34,177,140,0,153,137,225,35,153,133,213,249,10,242,98,121,21,113,176,41,157,
134,114,15,138,210,92,184,125,126,62,159,57,241,226,39,183,255,113,160,30,214,203,230,247,139,192,189,124,49,215,235,69,39,70,128,151,124,207,31,203,103,78,100,255,186,248,225,87,72,226,99,5,115,246,109,
70,20,129,119,129,82,113,241,236,165,201,225,163,217,223,30,250,221,145,247,191,252,35,245,176,62,192,54,40,123,130,92,146,50,190,231,143,189,125,104,122,178,82,43,175,23,23,207,54,215,110,223,197,82,
96,3,17,180,160,181,190,114,234,242,201,161,233,241,153,169,247,38,62,56,86,40,157,30,157,91,154,45,109,163,194,59,217,74,173,188,126,234,242,201,143,76,245,162,5,36,246,182,103,151,218,168,8,237,35,245,
3,251,128,33,54,40,253,148,217,53,154,6,122,186,244,19,64,105,117,33,114,233,167,71,235,7,70,73,103,5,152,108,50,176,166,181,190,115,238,230,153,235,143,212,85,217,174,197,199,58,78,11,68,17,216,44,17,
183,140,212,146,186,107,118,50,136,43,127,23,57,161,218,124,45,33,64,50,202,63,226,10,48,2,143,159,145,110,173,29,179,213,201,240,109,69,207,62,4,63,45,56,129,150,108,248,24,227,136,142,235,129,150,56,
129,150,56,129,150,252,31,5,8,148,240,107,224,25,117,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::stopOn_png = (const char*) resource_EditScene_stopOn_png;
const int EditScene::stopOn_pngSize = 4033;

// JUCER_RESOURCE: pauseOn_png, 3602, "../../MiamCommon/Resources/PauseOn.png"
static const unsigned char resource_EditScene_pauseOn_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,
156,24,0,0,5,241,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,
48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,
97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,48,
56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,
47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,
58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,
47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,
112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,
110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,49,43,48,49,58,48,48,34,32,120,109,112,58,77,101,116,97,100,
97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,49,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,
48,56,58,53,52,58,48,49,43,48,49,58,48,48,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,100,99,52,49,50,51,54,45,50,53,50,56,45,52,100,53,57,45,57,
102,100,52,45,101,101,100,50,101,100,52,101,97,101,56,51,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,
112,58,100,101,99,52,56,99,50,55,45,101,54,56,55,45,52,48,52,54,45,57,97,101,56,45,98,55,53,99,49,49,50,57,48,49,97,102,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,
116,73,68,61,34,120,109,112,46,100,105,100,58,54,98,50,102,54,102,54,55,45,98,55,56,101,45,52,99,101,101,45,98,55,100,55,45,56,99,56,51,51,51,56,97,52,102,98,56,34,32,100,99,58,102,111,114,109,97,116,
61,34,105,109,97,103,101,47,112,110,103,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,80,114,111,102,105,
108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,108,105,32,115,
116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,54,98,50,102,54,102,54,55,45,
98,55,56,101,45,52,99,101,101,45,98,55,100,55,45,56,99,56,51,51,51,56,97,52,102,98,56,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,49,43,48,49,
58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,
34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,
100,58,57,100,99,52,49,50,51,54,45,50,53,50,56,45,52,100,53,57,45,57,102,100,52,45,101,101,100,50,101,100,52,101,97,101,56,51,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,
48,57,84,48,56,58,53,52,58,48,49,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,
32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,
114,121,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,
101,110,100,61,34,114,34,63,62,57,60,193,191,0,0,7,199,73,68,65,84,120,156,237,156,207,111,20,231,25,199,63,51,59,172,107,22,175,237,130,112,64,88,93,47,110,8,182,192,66,106,221,88,88,36,82,210,122,83,
14,91,137,191,160,135,222,56,144,28,106,169,181,212,30,184,248,146,244,208,156,122,232,95,128,84,171,252,216,84,161,37,74,28,39,134,8,25,132,73,192,216,91,76,48,38,96,123,215,172,193,187,179,243,246,240,
190,131,55,116,127,12,140,25,88,231,253,74,163,93,102,252,206,206,243,225,125,103,222,247,251,190,243,24,66,8,180,158,93,230,139,190,128,122,151,6,232,83,26,160,79,105,128,62,101,149,219,249,155,143,222,
9,250,58,234,70,255,24,56,243,189,127,235,26,232,83,101,107,96,21,25,64,8,104,80,91,24,249,159,96,172,243,117,5,37,1,56,64,30,88,85,91,81,237,247,36,175,0,13,245,183,141,64,11,240,99,245,25,65,66,172,
103,128,121,32,7,44,1,11,234,243,33,96,227,1,164,23,128,6,18,82,51,176,19,232,0,98,234,123,11,178,38,154,94,126,236,37,147,129,172,125,171,72,104,183,129,52,48,163,190,103,144,112,171,198,229,5,160,133,
132,215,9,244,24,134,177,47,209,126,184,247,205,29,111,197,99,77,29,81,0,65,125,14,103,12,12,3,32,189,60,147,61,55,119,118,58,53,123,106,92,8,113,25,152,0,166,128,69,160,80,237,28,181,0,26,200,102,187,
19,232,137,134,163,135,6,123,134,6,162,225,230,134,145,244,137,233,177,249,209,209,156,157,203,225,177,186,191,100,50,0,43,98,69,34,125,109,7,99,201,216,145,189,253,109,135,226,195,19,199,91,179,249,44,
192,10,240,136,26,177,213,2,24,66,54,211,14,195,48,246,13,246,12,13,100,10,25,241,167,175,254,240,177,237,216,147,200,42,191,84,235,71,94,82,25,128,149,179,115,45,31,127,251,175,216,185,185,127,119,189,
183,127,176,127,176,103,104,96,232,194,224,162,16,226,14,242,158,184,130,140,175,172,106,1,108,64,62,48,98,137,246,195,189,209,112,115,195,159,191,250,227,89,219,177,199,128,139,192,44,240,0,249,228,170,
71,133,128,45,64,218,118,236,204,251,151,134,249,160,239,195,183,19,237,135,123,207,220,60,121,29,121,63,188,143,79,128,45,192,206,55,119,188,21,31,73,159,152,46,56,133,43,72,120,151,213,201,11,35,137,
148,179,14,193,4,174,100,42,97,2,155,144,173,8,219,177,155,71,210,39,226,191,220,245,78,252,204,205,147,165,15,201,92,165,115,212,234,72,135,145,93,149,150,88,83,71,116,108,126,52,141,108,182,179,72,120,
249,122,133,7,160,174,61,143,140,101,22,72,143,205,143,166,59,154,226,81,190,223,77,171,168,90,0,77,117,130,6,0,245,192,88,66,54,219,2,245,119,223,43,39,129,140,229,1,176,148,179,115,57,33,195,42,29,40,
84,84,45,128,134,218,76,213,85,177,213,86,68,246,161,54,138,28,100,76,54,96,171,88,221,17,86,213,65,130,215,177,176,40,249,220,8,181,174,146,74,227,243,20,167,54,19,124,74,3,244,41,13,208,167,52,64,159,
210,0,125,74,3,244,169,167,117,164,125,73,13,157,66,234,119,203,57,217,174,67,108,3,69,119,148,243,172,229,130,80,96,0,147,169,196,38,96,51,210,91,140,178,102,196,150,202,53,56,151,129,165,100,42,177,
162,246,111,70,14,173,154,106,148,203,2,153,100,42,177,50,146,72,85,245,241,214,75,129,0,84,53,104,51,210,87,236,4,118,33,33,134,158,248,211,34,18,194,45,224,6,48,167,246,239,0,118,123,44,55,5,220,78,
166,18,203,65,212,196,160,106,96,8,229,106,239,142,118,254,170,255,149,55,250,109,97,55,26,79,244,246,5,24,150,17,122,52,126,247,139,177,171,75,147,167,145,3,125,128,221,123,91,186,126,221,187,253,245,
62,91,20,127,84,190,156,245,240,243,249,79,71,175,103,174,129,116,79,86,8,96,184,25,20,64,11,89,115,118,245,191,242,70,255,207,183,255,98,207,215,139,147,57,211,12,229,77,140,34,128,131,8,57,78,49,220,
217,252,106,35,192,213,165,201,75,192,29,85,126,87,239,246,215,251,14,108,251,89,231,84,230,218,195,114,229,94,107,237,138,0,92,207,92,187,130,244,241,190,163,134,29,191,94,129,5,33,19,121,239,138,218,
194,110,252,122,113,50,247,215,43,127,185,204,154,97,9,202,184,253,237,158,223,117,23,69,177,1,9,124,179,58,22,45,138,98,195,196,253,139,203,127,255,230,111,87,144,150,218,130,58,182,21,232,56,218,125,
108,159,45,236,70,42,223,95,159,139,130,2,104,32,3,10,25,32,76,51,148,71,194,27,71,194,16,192,79,0,199,50,173,206,130,147,7,217,236,221,123,93,8,192,50,173,85,96,26,56,15,252,87,157,55,6,96,154,161,61,
170,105,135,8,112,174,58,208,110,140,43,213,252,238,35,225,93,101,173,27,210,97,96,84,154,95,17,234,216,61,36,252,111,88,131,180,219,109,210,65,235,133,0,52,48,92,19,51,135,124,122,58,234,123,94,29,171,
86,206,157,8,207,32,107,90,14,40,84,43,247,60,165,71,34,62,165,1,250,148,6,232,83,26,160,79,105,128,62,165,1,250,148,6,232,83,26,160,79,105,128,62,165,1,250,148,6,232,83,26,160,79,105,128,62,245,66,220,
24,129,48,144,11,27,35,72,3,84,168,239,97,117,172,90,57,119,205,98,51,210,206,138,0,155,170,149,123,158,122,33,0,29,68,8,233,36,199,212,46,215,80,221,38,16,22,229,173,120,67,29,219,134,124,213,194,53,
77,99,192,86,117,206,192,21,20,64,215,48,45,10,48,28,167,24,70,66,0,57,219,6,202,210,183,29,187,1,249,162,75,145,181,181,215,69,0,117,44,142,132,247,83,117,108,43,208,225,56,197,176,144,64,221,181,139,
129,248,131,65,1,124,60,111,107,25,214,195,215,90,187,34,71,187,143,237,51,205,208,158,114,147,74,23,239,93,152,71,26,173,238,188,112,54,100,132,86,123,182,30,104,58,218,125,172,199,52,67,123,203,77,42,
157,191,251,229,45,85,110,149,128,22,128,6,5,208,70,205,219,126,118,231,147,207,0,42,77,107,94,188,119,225,209,248,221,47,198,144,115,188,139,234,208,45,181,143,74,211,154,231,239,126,121,235,243,249,
79,71,85,185,44,85,86,214,175,167,130,2,88,68,90,240,83,55,178,83,220,200,78,77,226,109,98,221,157,177,187,113,117,105,242,180,154,234,244,50,177,158,33,160,87,47,2,1,56,146,72,57,106,153,198,109,228,
28,198,12,30,150,118,176,214,132,191,245,88,46,139,132,183,18,212,250,152,192,158,194,35,137,84,33,153,74,44,35,161,124,199,211,45,46,114,203,253,112,23,23,193,227,247,50,28,158,114,197,192,179,150,11,
66,122,36,226,83,26,160,79,105,128,62,165,1,250,148,6,232,83,26,160,79,121,5,104,148,124,214,107,134,14,47,42,141,207,83,156,181,0,186,47,223,57,42,65,131,165,54,119,13,222,70,81,233,91,0,150,138,213,
117,116,170,186,58,181,32,148,38,165,33,98,69,34,200,213,242,91,144,134,232,70,168,141,174,185,187,5,104,137,88,145,136,33,195,90,69,198,94,117,84,83,11,224,227,164,52,233,229,153,108,95,219,193,24,210,
192,108,71,250,112,97,181,2,191,46,165,174,61,140,140,165,29,136,245,181,29,140,205,44,79,103,145,99,241,28,107,11,221,203,170,214,80,238,113,82,154,115,115,103,167,147,177,35,123,63,153,251,79,119,193,
41,100,212,241,89,224,65,50,149,168,247,164,19,237,192,1,203,180,186,146,177,35,241,211,179,255,188,138,52,62,150,80,173,175,146,188,0,92,0,210,169,217,83,227,253,109,135,226,239,238,255,253,193,247,47,
13,11,219,177,155,217,0,105,79,144,183,164,152,101,90,93,239,237,31,236,207,230,51,171,169,217,83,238,218,237,5,124,2,44,34,1,205,8,33,46,15,79,28,111,29,236,25,26,248,160,239,195,183,71,210,39,226,99,
243,163,233,13,148,120,39,158,205,103,86,135,39,142,127,164,178,23,205,32,99,175,218,186,140,114,73,104,159,200,31,184,9,104,69,167,126,2,254,63,127,160,23,59,203,70,185,201,192,138,16,226,206,153,155,
39,175,63,145,87,101,163,38,31,171,57,45,224,5,160,187,50,126,17,153,75,106,65,253,72,11,58,253,157,103,67,213,125,45,193,70,58,195,247,209,9,24,129,167,119,164,75,115,199,84,76,135,244,67,82,221,118,
130,95,22,105,128,62,85,182,27,163,229,93,186,6,250,148,6,232,83,26,160,79,253,15,148,139,57,174,194,247,255,77,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::pauseOn_png = (const char*) resource_EditScene_pauseOn_png;
const int EditScene::pauseOn_pngSize = 3602;

// JUCER_RESOURCE: optionOn_png, 6807, "../../MiamCommon/Resources/OptionOn.png"
static const unsigned char resource_EditScene_optionOn_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,
154,156,24,0,0,8,176,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,
77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,
116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,
48,56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,
50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,
115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,
108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,
47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,
112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,115,116,82,101,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,
49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,
46,99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,
32,40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,54,43,48,49,58,48,48,34,32,120,109,112,58,
77,101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,57,58,49,50,58,48,49,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,57,58,49,50,58,48,49,43,48,49,58,48,48,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,
61,34,120,109,112,46,105,105,100,58,55,51,99,97,98,99,101,48,45,52,101,48,52,45,52,102,98,51,45,97,97,54,48,45,54,101,100,102,51,100,54,54,100,51,99,101,34,32,120,109,112,77,77,58,68,111,99,117,109,101,
110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,53,99,56,56,98,51,49,53,45,100,100,48,51,45,49,102,52,54,45,98,49,99,48,45,56,97,101,97,54,97,49,51,
56,51,48,101,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,
45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,
80,114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,
58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,102,101,
53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,
52,58,48,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,
116,111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,102,53,50,55,53,56,51,55,45,54,54,98,50,45,52,49,55,51,45,98,102,51,98,45,99,55,57,57,99,49,98,49,49,48,52,56,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,57,58,49,50,58,48,49,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,
32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,
34,99,111,110,118,101,114,116,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,111,98,
101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,100,101,114,105,118,
101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,99,111,110,118,101,114,116,101,100,32,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,
111,98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,
101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,55,51,99,97,98,99,101,48,45,52,101,48,52,45,52,102,98,51,45,97,97,54,48,45,54,101,100,102,51,
100,54,54,100,51,99,101,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,57,58,49,50,58,48,49,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,
114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,
47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,82,101,102,
58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,102,53,50,55,53,56,51,55,45,54,54,98,50,45,52,49,55,51,45,98,102,51,98,45,99,55,57,57,99,49,98,49,49,48,52,56,34,32,115,116,82,
101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,
55,34,32,115,116,82,101,102,58,111,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,
98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,47,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,
116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,62,66,31,114,194,0,0,17,141,73,68,65,84,120,156,237,156,121,112,148,101,158,199,63,111,223,221,73,58,247,9,73,56,146,144,196,
132,40,34,8,200,173,197,164,188,50,51,186,186,46,171,83,142,51,58,171,238,128,247,184,181,59,59,71,233,42,10,8,238,108,105,141,107,233,142,91,51,59,179,14,181,30,33,32,34,136,40,168,33,233,132,35,132,
51,55,73,186,211,73,186,211,119,191,251,199,243,54,36,33,193,116,186,131,193,202,175,234,173,134,247,125,243,190,207,251,125,191,191,243,249,61,175,36,203,50,83,50,126,81,125,219,3,184,210,101,10,192,
8,101,10,192,8,101,10,192,8,101,10,192,8,101,10,192,8,69,115,169,131,21,85,229,151,107,28,147,90,182,173,169,28,245,216,20,3,35,148,75,50,48,10,34,41,247,208,1,6,64,171,236,11,2,62,192,163,252,250,149,
125,87,156,76,52,128,90,192,12,164,1,169,64,28,160,70,128,54,0,244,2,118,101,115,42,251,175,168,212,104,34,1,148,16,204,75,5,74,128,171,128,44,101,159,7,1,90,39,208,12,156,5,90,1,27,224,70,176,241,138,
0,50,98,0,71,51,176,138,3,210,0,113,133,9,197,165,143,148,172,251,135,52,99,122,2,50,56,253,142,224,89,199,25,135,197,90,211,126,160,243,115,75,171,179,229,16,112,24,56,1,116,0,206,109,107,42,125,145,
142,109,44,18,169,163,156,104,39,226,7,220,199,236,71,90,43,155,222,63,134,140,164,83,235,84,137,250,36,205,213,201,243,18,238,45,184,191,232,159,230,253,107,197,247,103,222,241,119,192,50,96,30,144,13,
196,84,84,149,171,39,120,108,81,145,9,3,112,219,154,74,25,240,2,86,224,228,7,77,255,183,227,181,163,255,94,227,14,184,135,56,139,44,211,52,253,125,5,63,46,93,87,250,196,90,163,198,184,12,161,238,25,128,
161,162,170,92,154,168,241,69,75,38,218,137,248,16,118,237,56,160,217,213,186,67,5,240,64,225,67,101,70,141,113,200,203,91,145,181,58,211,31,244,223,250,234,225,205,189,8,231,210,175,252,189,119,130,199,
24,145,68,12,160,194,18,137,11,108,150,129,224,182,53,149,242,182,53,149,114,69,85,185,7,232,2,142,1,170,93,173,59,180,26,149,70,255,211,162,159,21,169,37,205,16,134,221,56,125,77,86,67,239,177,213,59,
91,182,119,41,127,227,168,168,42,247,41,108,158,148,18,169,10,75,136,80,37,30,72,7,50,129,68,132,250,169,0,182,173,169,12,34,60,235,57,160,1,168,169,106,254,240,211,125,237,123,237,35,93,240,230,156,91,
139,99,181,113,197,192,116,68,8,52,169,109,97,164,0,170,128,88,196,195,150,2,215,0,115,16,113,223,72,32,118,33,194,150,150,175,186,191,108,11,200,129,139,46,152,29,155,107,156,159,186,224,42,196,203,48,
51,241,102,38,34,137,20,64,13,144,0,228,175,154,118,211,221,183,230,86,252,20,88,10,20,35,24,57,28,68,47,34,96,118,244,122,237,3,238,128,251,34,213,84,75,106,102,155,243,178,184,16,120,235,34,28,227,132,
74,164,111,87,131,120,200,233,179,205,121,243,111,154,246,189,66,135,175,63,125,119,219,46,35,66,189,1,58,43,170,202,93,202,191,181,128,17,48,198,105,227,140,122,149,126,68,47,155,102,76,143,65,152,130,
88,190,227,0,170,0,61,16,167,66,101,214,169,117,234,7,139,31,158,23,68,86,237,105,251,88,133,120,248,19,64,15,194,185,36,3,51,129,236,57,9,69,169,26,213,200,183,55,168,13,90,141,74,19,231,15,250,245,76,
114,27,24,41,128,50,34,237,10,122,131,222,0,128,65,109,84,61,88,244,240,213,128,122,79,219,199,49,8,85,62,167,156,155,6,20,205,73,40,154,191,50,107,117,234,104,23,117,249,93,4,228,128,6,241,114,52,21,
85,229,210,100,245,196,145,2,24,64,216,181,129,246,129,182,254,208,78,147,198,164,122,184,248,231,115,83,13,169,137,187,90,119,212,245,120,122,58,0,226,180,230,180,249,169,215,21,222,147,119,239,204,56,
173,121,84,251,219,227,177,5,101,89,54,33,188,123,28,194,150,186,39,35,136,145,2,232,71,84,85,236,199,236,71,219,252,65,127,137,70,37,98,59,157,90,39,173,205,255,81,206,146,140,101,25,45,142,102,23,64,
102,76,150,33,207,156,175,255,166,139,206,75,157,31,51,187,53,127,214,201,190,198,60,68,110,60,0,116,77,70,16,35,245,194,1,132,87,237,106,117,54,159,170,239,177,244,13,63,97,102,220,44,221,210,204,229,
241,75,51,151,199,143,5,60,128,116,99,134,250,137,178,103,74,243,227,231,44,1,174,69,132,70,169,76,194,244,46,82,0,101,192,1,180,249,130,190,134,157,45,85,71,124,65,95,84,24,146,105,202,210,62,81,246,
204,226,57,9,133,55,2,11,17,32,166,48,201,64,140,6,128,231,179,140,207,58,246,126,186,187,109,87,71,228,195,18,146,110,204,208,174,47,125,106,48,136,133,76,50,16,163,81,141,241,35,146,255,211,64,221,155,
13,175,87,29,236,252,162,39,10,215,5,32,195,148,169,93,95,250,212,226,252,248,130,213,76,66,16,35,6,80,49,234,30,68,117,249,168,203,239,218,191,209,242,194,95,223,59,187,173,201,229,119,69,101,158,35,
195,148,169,125,124,238,51,75,10,226,231,76,58,16,163,82,15,220,182,166,50,128,176,133,77,192,215,238,128,251,163,55,142,189,246,246,243,135,126,181,235,227,182,143,58,154,28,103,60,222,128,247,188,109,
244,7,253,4,229,240,176,205,48,101,106,215,207,125,106,56,136,223,186,99,137,102,162,238,71,204,115,248,16,181,188,14,139,173,246,140,197,86,123,48,213,152,54,59,205,144,150,102,212,152,140,18,146,218,
225,119,168,19,116,9,137,63,46,124,48,55,197,144,58,230,76,35,211,148,165,125,188,236,153,37,27,45,47,72,13,246,99,161,221,50,208,253,109,133,56,81,3,80,25,188,175,162,170,188,15,225,88,236,64,59,208,
216,229,234,76,233,114,117,198,161,100,22,40,21,28,135,207,177,96,253,220,39,175,77,210,39,143,121,28,233,198,12,237,186,210,39,23,111,174,219,192,32,16,27,248,150,226,196,168,151,244,149,170,139,7,81,
137,62,11,212,3,7,129,207,128,189,192,39,192,110,96,119,157,173,118,251,150,186,141,7,186,221,93,254,112,238,145,105,202,210,62,54,247,233,73,17,226,76,200,156,136,82,141,14,108,91,83,233,66,20,18,218,
129,51,192,41,224,36,162,58,125,8,248,188,198,90,93,181,181,126,211,129,30,143,45,44,16,67,76,252,182,109,226,132,183,118,40,42,21,154,231,85,113,161,59,161,15,81,169,249,178,214,122,104,231,43,117,47,
31,28,15,19,31,47,27,226,157,47,123,198,114,185,122,99,52,136,234,242,52,160,64,217,50,148,99,93,192,169,26,107,117,245,230,186,151,14,91,221,214,139,203,212,151,144,116,99,198,112,239,124,89,65,156,144,
114,249,160,137,38,181,178,197,114,1,188,108,68,157,176,27,17,59,202,136,34,171,183,222,102,105,121,169,246,185,184,199,202,158,206,77,53,164,93,17,222,57,234,12,84,192,211,33,42,202,89,192,108,68,91,
199,117,197,137,37,183,63,89,246,236,186,231,22,108,120,98,121,214,170,191,7,110,2,86,198,105,205,75,210,140,233,69,128,241,168,253,72,247,38,203,134,230,113,49,241,91,72,251,162,202,192,65,224,37,1,57,
136,201,166,84,101,155,181,44,115,197,202,37,25,75,179,0,98,52,177,215,159,237,63,157,225,244,57,125,15,21,63,146,157,21,51,205,176,171,117,167,245,195,166,247,90,143,244,212,159,125,201,242,188,251,137,
185,207,228,37,27,82,198,60,198,80,218,247,178,229,223,228,198,222,227,161,221,199,152,64,38,70,155,129,161,89,186,28,224,186,178,228,107,238,186,107,246,61,15,167,27,51,110,153,109,206,95,86,152,80,148,
22,58,49,55,110,134,238,87,243,159,203,127,126,225,75,197,215,166,94,103,206,52,101,233,214,230,223,151,153,29,155,19,3,244,29,237,57,124,122,99,221,139,39,172,238,238,176,152,120,185,211,190,104,219,
192,16,128,217,87,39,207,91,241,104,201,250,91,146,13,41,250,155,115,110,15,234,212,58,201,160,54,12,121,128,120,93,194,69,15,100,247,244,232,16,1,183,231,176,173,238,220,139,181,207,25,158,44,251,69,
78,138,33,117,204,47,59,148,246,109,178,188,200,241,222,134,208,238,9,9,182,163,205,192,144,195,72,45,77,46,43,74,54,164,232,1,204,58,179,106,56,120,163,201,218,252,251,166,101,199,230,228,229,199,207,
153,173,83,233,76,13,246,163,93,27,45,47,158,237,118,119,133,197,196,144,99,153,232,96,59,42,0,86,84,149,75,21,85,229,90,4,120,201,64,74,101,211,251,253,14,95,255,168,111,218,27,244,142,120,108,105,230,
10,195,214,37,175,205,218,112,253,230,226,59,102,221,157,3,56,142,244,212,31,217,84,183,161,222,230,177,142,43,216,30,230,88,162,26,226,68,75,133,85,64,12,194,235,230,1,51,99,181,177,41,106,233,226,203,
239,63,183,175,119,95,251,158,150,1,255,128,43,70,27,99,188,33,99,249,244,69,233,75,226,71,186,232,234,105,55,37,237,104,169,244,116,187,187,26,14,219,234,78,111,169,219,232,124,164,100,221,130,20,67,
234,152,199,29,74,251,54,90,94,96,164,16,135,8,27,57,163,5,160,22,193,188,130,252,248,57,203,151,101,174,88,117,77,202,181,25,70,141,241,252,91,14,200,126,249,157,198,183,207,188,123,250,207,159,32,210,
58,7,16,251,89,199,167,51,238,152,117,215,202,123,242,238,157,161,146,46,40,132,140,204,201,190,19,3,222,160,183,19,97,191,92,53,214,234,174,173,245,155,228,117,165,79,44,76,212,39,133,93,128,216,100,
121,81,30,110,19,17,133,143,113,131,24,45,27,168,65,76,65,230,222,150,91,177,252,214,220,138,233,211,99,178,135,60,224,129,206,47,250,20,240,246,35,138,9,187,148,223,253,127,57,245,167,221,95,118,29,232,
29,124,190,221,211,35,191,126,244,119,205,125,222,94,43,34,151,62,2,28,172,181,30,218,241,74,221,203,227,42,64,140,150,246,113,161,139,34,108,137,22,128,42,4,136,250,236,216,156,132,225,7,131,114,144,
3,231,246,183,33,152,215,128,136,205,26,149,223,6,224,204,222,246,79,90,229,65,68,144,36,9,147,198,164,67,176,91,133,152,54,104,4,14,132,10,16,227,177,137,163,165,125,140,19,196,104,1,232,71,168,164,117,
123,243,135,71,154,28,103,135,52,69,186,3,46,185,199,219,227,84,206,233,67,244,64,15,32,166,68,251,1,71,191,175,111,32,16,12,156,71,48,65,151,40,61,54,247,233,220,242,156,91,230,33,30,82,135,168,236,28,
69,48,177,106,179,101,195,184,153,56,146,119,102,28,32,70,11,64,31,2,152,142,237,205,31,28,250,143,195,91,154,237,222,158,243,96,232,84,122,41,78,107,54,33,188,180,25,209,3,109,66,56,158,56,32,54,78,107,
54,169,85,234,33,15,48,35,110,166,126,69,230,170,66,132,125,53,33,94,84,23,10,136,22,91,109,213,171,245,155,199,199,196,81,210,62,194,4,49,98,0,149,112,32,180,112,198,5,184,237,94,187,39,16,188,16,182,
105,84,26,174,79,91,148,9,228,34,10,10,133,131,126,231,0,51,110,200,88,54,77,26,97,236,173,3,173,189,136,246,145,128,114,159,80,159,225,81,224,96,141,181,186,106,179,101,195,1,171,187,59,44,16,135,205,
246,45,80,198,147,68,152,221,96,145,0,24,234,78,141,69,168,88,22,144,174,87,235,147,31,40,124,40,59,201,144,60,4,141,197,25,55,36,126,127,230,157,43,129,197,192,74,96,149,178,45,190,99,214,93,43,22,166,
45,26,18,202,180,15,180,5,182,55,127,208,86,217,244,254,167,136,201,170,94,32,212,238,59,4,68,139,173,182,106,107,253,166,3,86,119,120,76,28,148,246,173,2,174,70,164,160,177,132,209,17,22,110,24,19,42,
81,133,150,111,197,34,168,159,137,40,83,205,214,171,13,153,133,137,197,166,225,108,82,75,26,238,43,184,127,230,108,115,94,226,87,93,7,219,123,60,182,129,68,125,82,204,117,169,11,51,150,100,44,77,24,124,
238,128,127,64,126,161,230,183,103,206,244,159,222,11,236,67,52,169,219,16,42,140,210,123,29,2,17,128,26,107,53,155,235,54,176,174,244,201,133,201,134,177,207,177,132,210,190,223,86,255,210,214,234,108,
233,68,148,217,156,8,198,127,163,132,11,96,168,161,50,9,81,174,74,3,114,146,13,41,87,77,139,153,158,215,238,108,51,117,185,59,77,239,157,249,107,127,197,204,31,198,3,146,65,109,28,114,129,27,50,150,37,
44,73,95,154,48,16,24,144,77,106,147,36,73,23,171,173,63,232,199,229,119,89,17,147,245,39,16,97,140,83,153,62,5,70,6,177,206,86,203,214,250,141,60,90,178,126,225,104,85,28,95,208,135,221,219,67,80,14,
146,106,72,67,37,169,200,52,101,233,127,56,243,111,22,109,169,223,88,135,240,244,157,8,150,127,163,132,163,194,161,60,55,27,177,32,102,37,74,61,239,190,130,251,239,252,245,252,231,215,172,159,251,228,
124,32,241,143,39,223,113,254,243,151,191,24,120,108,255,163,129,189,237,187,47,186,144,36,73,196,104,98,70,4,15,192,172,51,75,203,179,86,26,17,224,116,3,142,145,86,46,141,164,206,53,214,234,170,87,234,
94,26,209,177,200,200,56,253,14,146,245,41,164,24,82,232,247,245,225,13,138,128,97,94,234,252,52,68,47,99,60,97,216,193,112,24,168,86,46,62,43,39,54,119,245,143,230,60,112,235,185,129,14,117,191,175,95,
59,63,117,97,2,64,65,124,161,241,39,69,63,155,214,235,181,203,197,137,37,218,183,26,222,144,222,105,124,155,5,105,139,48,168,13,151,188,184,55,232,37,40,7,100,131,218,40,117,187,187,60,13,246,99,199,16,
192,92,82,157,70,98,162,197,86,203,150,186,141,60,82,178,110,225,224,180,47,40,7,137,215,37,32,41,171,50,2,114,0,79,192,141,78,165,35,86,19,167,225,194,138,210,168,219,192,144,195,72,0,114,126,48,243,
206,27,231,165,204,207,188,232,98,42,13,55,231,220,118,190,133,205,159,239,151,255,208,248,86,80,175,210,159,103,122,183,187,75,214,169,244,152,117,230,243,244,107,113,54,7,54,89,54,156,233,245,218,187,
203,115,110,137,171,181,30,58,98,177,214,236,5,90,16,113,226,37,237,209,104,54,113,107,253,38,6,167,125,42,73,69,191,183,159,56,109,28,0,178,12,90,149,32,91,175,215,238,71,176,217,247,77,247,27,44,99,
85,97,21,34,14,75,2,210,235,109,150,224,224,214,140,102,71,147,127,79,219,199,206,198,222,227,67,212,172,206,86,43,159,117,156,246,188,117,252,141,129,62,111,95,160,198,90,237,248,77,245,191,28,123,190,
230,215,167,90,156,205,231,207,173,238,254,170,253,100,95,227,167,221,238,174,15,254,235,248,155,175,90,172,53,127,4,190,68,120,95,39,99,200,85,71,82,231,225,105,159,132,132,94,173,199,29,112,227,10,184,
136,213,198,158,215,140,35,61,135,173,202,223,246,17,198,234,168,177,50,80,135,96,223,116,96,90,146,33,37,62,100,191,62,106,169,234,255,207,134,215,235,7,252,3,54,131,218,16,119,251,140,31,92,117,119,
222,218,100,9,137,52,99,58,18,146,107,219,153,255,237,220,211,190,187,99,192,239,60,235,9,120,108,128,254,221,211,127,46,187,57,231,182,146,110,119,151,227,171,206,3,251,16,19,239,141,40,54,111,208,230,
29,107,1,116,20,38,202,91,235,55,201,63,47,125,252,250,36,125,178,70,175,214,19,250,90,83,232,25,28,190,254,64,85,203,135,95,35,156,150,21,209,24,48,38,145,46,245,233,167,65,75,65,227,17,65,239,234,103,
175,249,229,195,101,201,215,100,234,213,122,201,234,182,6,31,251,252,145,154,94,175,253,51,160,13,72,82,73,170,5,47,44,220,180,56,63,190,64,27,144,253,212,219,234,60,91,234,95,174,182,186,173,251,128,
58,68,47,161,17,49,173,153,130,120,219,77,136,156,184,149,11,42,27,68,89,50,54,214,135,25,52,110,9,97,207,82,149,113,95,87,156,88,178,242,129,194,135,22,205,50,207,54,13,62,183,213,217,226,254,67,227,
91,7,63,63,183,239,125,68,161,163,1,145,50,14,246,248,163,222,107,172,12,84,35,202,236,177,11,210,174,207,10,237,60,231,106,15,184,252,3,161,149,231,61,0,65,57,232,104,115,182,120,243,227,11,180,106,73,
67,89,242,53,250,12,99,102,191,213,109,61,142,104,243,104,227,66,245,38,22,1,148,29,241,230,29,128,63,210,146,251,48,38,202,128,255,72,79,189,243,151,95,61,219,180,40,125,241,220,89,230,188,180,160,28,
12,158,236,59,209,117,168,251,235,195,54,143,181,6,176,32,90,81,250,9,227,243,3,99,5,48,128,210,76,190,163,165,242,212,181,41,11,114,147,13,201,234,233,49,57,154,68,125,82,252,57,87,71,186,114,94,188,
78,173,75,156,101,206,211,131,40,34,28,236,252,162,171,195,213,209,136,96,89,59,66,69,85,8,192,53,202,3,250,148,109,92,140,27,73,134,129,232,7,250,250,125,125,103,119,180,108,255,10,17,203,130,210,69,
134,88,126,214,170,140,41,172,207,14,140,21,192,80,179,80,211,239,14,111,249,120,121,214,170,239,253,99,201,99,211,205,58,179,244,147,162,135,242,127,127,236,53,201,238,177,247,154,52,38,227,221,121,107,
243,179,99,115,52,0,85,205,31,246,190,217,240,251,61,136,213,232,29,8,135,16,84,2,226,9,95,145,62,104,181,168,21,65,128,115,136,98,70,40,166,114,35,64,236,83,142,135,253,205,134,177,2,232,67,168,105,59,
208,110,84,27,221,161,251,204,79,93,104,40,136,47,186,170,197,217,228,79,55,102,168,147,13,41,231,61,187,70,165,245,34,84,182,9,193,188,203,222,126,22,90,248,93,81,85,238,67,89,81,192,133,56,47,128,96,
103,128,113,86,165,195,81,225,80,167,213,185,252,248,2,121,240,124,135,89,103,150,138,117,37,218,208,255,101,100,36,36,178,99,114,130,8,0,59,81,236,219,120,6,25,13,81,128,244,15,31,67,164,223,76,8,39,
19,9,53,147,55,191,125,252,205,247,101,184,165,169,255,140,218,29,112,25,254,54,239,222,244,68,125,162,218,19,240,200,239,158,254,31,171,205,99,235,45,74,188,74,218,118,250,47,31,33,12,115,15,97,132,35,
87,146,132,11,160,29,56,217,235,181,243,106,253,166,83,136,220,49,99,182,57,127,229,202,105,55,206,168,179,213,116,254,233,228,127,239,4,90,119,182,108,63,135,80,221,83,12,170,164,124,215,100,172,113,
96,72,180,136,140,36,1,17,134,36,34,226,185,108,101,159,19,225,205,90,17,134,219,142,96,237,21,249,81,157,144,68,35,14,12,73,168,129,220,133,48,198,6,4,203,78,32,128,245,32,212,213,206,5,175,22,32,140,
220,242,74,147,241,204,11,135,86,158,123,17,97,128,19,193,54,245,176,99,87,204,215,135,34,145,72,39,214,67,13,229,99,206,29,191,107,50,245,249,187,8,101,10,192,8,229,146,94,120,74,190,89,166,24,24,161,
76,1,24,161,76,1,24,161,76,1,24,161,76,1,24,161,76,1,24,161,252,63,182,173,108,2,176,166,222,3,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::optionOn_png = (const char*) resource_EditScene_optionOn_png;
const int EditScene::optionOn_pngSize = 6807;

// JUCER_RESOURCE: lectureOn_png, 4316, "../../MiamCommon/Resources/LectureOn.png"
static const unsigned char resource_EditScene_lectureOn_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,
154,156,24,0,0,5,241,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,
77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,
116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,
48,56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,
50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,
115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,
110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,
109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,
103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,
47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,
105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,53,57,43,48,49,58,48,48,34,32,120,109,112,58,77,101,116,97,
100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,53,57,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,
57,84,48,56,58,53,49,58,53,57,43,48,49,58,48,48,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,55,50,48,101,51,52,50,55,45,54,50,48,98,45,52,56,55,49,
45,97,101,99,97,45,100,57,57,53,50,97,56,50,57,101,55,55,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,
112,58,97,49,97,50,57,48,101,56,45,52,56,102,99,45,53,49,52,55,45,56,56,55,50,45,55,97,50,102,51,53,56,101,100,53,49,53,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,
116,73,68,61,34,120,109,112,46,100,105,100,58,49,55,52,99,54,98,101,53,45,57,51,100,53,45,52,48,48,97,45,56,99,102,55,45,98,100,49,51,52,56,51,50,53,51,52,50,34,32,100,99,58,102,111,114,109,97,116,61,
34,105,109,97,103,101,47,112,110,103,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,80,114,111,102,105,108,
101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,108,105,32,115,116,
69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,49,55,52,99,54,98,101,53,45,57,51,
100,53,45,52,48,48,97,45,56,99,102,55,45,98,100,49,51,52,56,51,50,53,51,52,50,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,53,57,43,48,49,58,48,
48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,47,
62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,
58,55,50,48,101,51,52,50,55,45,54,50,48,98,45,52,56,55,49,45,97,101,99,97,45,100,57,57,53,50,97,56,50,57,101,55,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,
56,58,53,49,58,53,57,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,
99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,
32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,
61,34,114,34,63,62,142,235,214,63,0,0,10,145,73,68,65,84,120,156,237,156,123,80,84,247,21,199,63,119,89,23,21,92,240,17,173,15,204,2,34,130,137,196,60,20,171,77,108,72,3,27,147,186,173,25,82,211,71,198,
166,218,137,109,154,52,143,154,73,156,73,58,99,58,53,201,228,209,73,50,147,214,180,54,109,167,147,52,78,164,209,224,102,52,49,227,3,163,160,136,85,80,20,22,16,20,4,129,133,21,89,150,189,253,227,220,171,
196,0,187,102,101,31,186,223,153,157,69,116,118,239,239,227,249,157,123,30,191,123,20,85,85,137,234,155,203,16,234,11,136,116,69,1,6,168,40,192,0,21,5,24,160,140,253,253,210,102,183,6,251,58,34,70,27,
115,11,191,242,231,168,5,6,168,126,45,112,16,41,64,12,16,171,189,76,200,127,130,114,133,175,43,88,82,1,47,224,6,186,181,87,175,246,123,191,228,47,64,69,251,183,35,128,68,96,140,246,30,135,64,140,100,128,
110,192,5,180,1,103,181,247,46,192,131,31,32,253,1,168,32,144,18,128,73,64,50,96,209,126,78,68,44,209,224,207,151,133,153,20,196,250,186,17,104,13,128,3,168,214,126,110,71,224,14,186,46,127,0,26,17,120,
211,128,44,69,81,110,204,75,90,52,103,225,196,156,148,228,81,41,102,21,21,149,200,76,103,20,20,69,65,161,186,163,202,185,253,212,182,170,45,117,155,247,170,170,122,8,56,8,28,7,90,129,158,193,62,195,23,
64,5,217,182,147,128,44,179,201,124,251,170,172,213,185,102,83,66,108,129,99,67,85,81,227,174,93,46,143,203,133,159,230,30,102,82,0,99,156,49,46,110,222,132,249,150,197,150,37,25,11,38,220,158,178,246,
224,154,209,78,183,19,224,28,112,30,31,107,243,5,48,6,217,166,201,138,162,220,184,42,107,117,110,123,79,187,250,124,201,179,91,61,94,207,17,196,228,219,124,125,73,152,74,1,140,46,143,43,113,107,253,167,
150,237,167,62,203,124,98,214,170,5,171,178,86,231,174,46,94,213,170,170,234,105,196,39,158,67,214,215,175,124,1,140,69,110,24,150,188,164,69,115,204,166,132,216,23,74,158,219,230,241,122,138,128,3,64,
29,208,137,220,185,34,81,49,64,60,224,240,120,61,237,175,150,173,229,181,121,111,221,149,151,180,104,78,97,237,166,74,196,31,182,16,32,192,68,96,210,194,137,57,41,5,142,13,85,61,222,158,195,8,188,67,218,
135,247,108,204,45,244,94,129,197,4,93,54,187,213,0,12,67,118,17,30,175,39,161,192,177,33,229,123,83,172,41,133,181,155,250,222,36,93,3,125,134,175,64,218,132,132,42,137,150,81,201,230,162,198,93,14,100,
219,214,33,240,220,145,10,15,64,187,118,55,178,150,58,192,81,212,184,203,145,60,42,197,204,87,195,180,1,229,11,160,65,251,128,88,0,237,134,209,134,108,219,30,34,207,239,245,39,21,89,75,39,208,230,242,
184,92,170,44,171,111,162,48,160,124,1,84,180,151,65,11,85,60,218,171,23,137,161,174,22,121,145,53,121,0,143,182,86,61,195,26,52,73,240,55,23,86,251,188,95,13,86,55,144,250,174,207,175,117,70,139,9,1,
42,10,48,64,69,1,6,168,203,45,103,5,44,45,246,50,32,65,172,94,78,234,221,152,91,24,145,190,53,168,0,53,120,195,129,81,72,6,0,82,58,234,176,217,173,231,1,79,164,129,12,182,5,26,145,0,213,2,76,65,226,172,
86,164,124,116,26,112,106,32,189,145,2,50,216,62,208,4,140,3,50,87,206,252,205,227,247,94,191,120,5,144,11,124,7,184,9,1,59,6,136,213,172,53,236,21,108,11,28,134,212,22,45,169,230,180,140,57,227,179,227,
239,152,120,231,77,235,143,173,43,62,124,246,80,49,80,137,212,225,78,2,205,54,187,213,133,228,218,97,107,141,193,6,24,131,248,64,179,87,245,198,188,184,255,247,173,51,18,51,120,230,166,213,11,202,90,14,
78,95,127,108,93,209,153,174,166,41,8,200,19,64,61,208,106,179,91,187,8,211,27,77,176,183,137,162,125,103,140,162,40,170,2,174,143,107,54,150,255,106,231,242,29,110,175,187,247,141,111,191,189,248,39,
105,15,229,155,12,166,187,128,59,128,91,128,84,100,219,143,176,217,173,49,65,190,94,159,10,122,24,163,75,1,53,198,96,60,11,28,113,186,157,205,111,28,122,101,127,90,66,250,13,15,207,88,113,219,29,19,239,
204,255,215,241,247,202,182,55,108,219,7,28,5,142,33,213,146,38,155,221,218,73,24,149,208,66,6,16,20,213,160,24,92,72,121,172,2,240,84,182,31,173,121,230,203,39,143,47,156,148,115,219,143,167,253,108,
86,94,210,61,211,222,173,248,243,190,202,246,163,147,185,184,173,79,2,45,54,187,245,28,97,224,31,67,8,16,12,24,122,144,238,87,45,208,140,248,188,250,237,13,219,234,119,159,222,81,145,159,186,116,238,11,
183,190,152,179,175,105,79,198,250,163,239,238,105,115,183,86,32,214,120,2,9,125,90,181,176,39,100,254,49,28,66,133,94,36,152,110,70,192,148,0,95,184,189,238,173,255,172,252,251,7,143,239,94,89,96,138,
137,53,190,185,224,157,31,228,167,46,189,223,160,24,116,255,120,51,144,130,132,61,195,67,21,246,132,212,2,251,72,175,53,158,71,250,180,29,104,22,121,166,171,233,228,75,165,47,86,223,48,102,214,236,101,
233,203,111,93,56,49,39,255,31,149,127,43,45,106,220,85,140,116,11,43,17,255,216,28,10,255,24,46,0,1,208,182,161,219,102,183,122,16,171,108,3,154,128,147,255,59,91,86,255,100,209,163,199,242,146,22,101,
175,200,88,121,75,110,210,61,105,235,143,174,251,210,209,81,61,25,137,29,245,109,221,162,197,143,65,73,11,195,97,11,127,77,154,5,117,35,109,69,7,80,10,236,0,62,219,82,183,121,195,202,157,191,248,79,93,
103,109,251,31,230,188,146,247,72,230,163,63,138,31,22,127,55,240,93,96,14,48,29,152,0,140,180,217,173,49,54,187,117,72,143,157,132,37,64,16,107,220,152,91,216,139,244,101,155,17,43,43,6,190,232,242,116,
109,125,183,226,157,247,159,222,243,216,230,177,195,199,197,189,181,224,47,75,238,187,222,118,191,162,40,186,127,156,141,28,65,25,242,180,48,172,182,112,127,210,182,97,143,182,173,117,255,120,6,56,89,
239,58,121,114,205,254,231,171,111,185,238,182,91,31,154,254,243,217,119,77,206,181,172,63,182,174,244,64,115,201,183,16,43,172,64,44,184,197,102,183,158,31,138,45,29,246,0,117,105,139,239,182,217,173,
61,136,85,182,33,32,27,74,206,236,107,220,223,92,92,179,104,234,247,231,62,126,227,83,217,229,173,71,102,188,119,236,175,59,27,206,213,155,145,236,199,141,116,222,6,108,144,127,83,133,237,22,30,72,125,
252,99,11,98,93,135,129,3,170,170,150,111,170,41,56,242,200,142,135,107,27,187,78,143,124,121,222,27,247,61,52,253,225,31,34,119,234,120,36,15,191,226,138,56,128,225,166,136,217,194,186,180,27,130,9,48,
35,126,206,2,100,42,138,146,113,239,212,197,51,239,79,121,96,106,121,235,145,182,167,139,30,251,180,225,92,253,78,36,180,25,178,243,59,17,3,80,11,71,244,163,38,227,144,138,118,26,144,174,223,68,188,170,
170,190,126,232,149,162,3,205,37,197,64,57,23,111,34,29,92,171,0,53,112,250,241,226,209,8,184,84,32,109,114,220,148,27,150,165,47,207,78,75,152,62,230,195,170,247,143,110,170,45,216,167,170,106,5,2,174,
26,104,68,224,185,135,42,168,14,91,128,26,56,189,9,101,6,38,34,177,93,250,72,227,200,25,75,167,253,52,59,103,242,221,150,29,167,182,215,190,126,232,229,207,59,123,58,43,184,88,177,105,64,122,45,67,222,
95,9,75,128,125,252,220,40,96,60,48,21,201,48,166,231,37,45,202,126,32,245,193,204,154,78,135,243,217,189,79,109,113,116,84,151,113,73,42,135,28,71,11,74,42,23,86,0,53,171,27,198,87,253,220,52,32,77,47,
38,140,136,25,49,236,157,242,183,74,246,52,238,46,70,138,173,23,138,9,104,167,198,174,197,98,130,238,231,134,35,126,110,50,154,159,187,110,196,248,153,203,210,151,103,207,26,147,117,221,127,107,62,170,
252,176,234,253,189,94,213,91,129,192,171,70,218,161,29,64,119,40,170,212,225,0,48,6,185,65,140,3,198,162,249,57,147,193,52,35,63,245,193,185,214,169,247,166,238,107,218,211,240,235,157,191,252,168,191,
130,42,226,231,66,86,80,13,41,64,47,94,189,205,57,149,139,207,160,164,235,37,253,230,238,230,115,47,20,63,183,173,178,253,104,41,151,148,244,145,116,238,90,46,233,171,138,87,245,198,33,129,112,60,48,92,
111,42,141,141,29,55,82,107,42,237,69,44,238,66,83,137,16,248,185,193,20,50,128,42,40,189,94,207,24,32,211,108,50,123,150,165,175,152,54,119,124,246,196,79,106,63,62,241,193,137,127,127,233,246,186,47,
245,115,78,196,207,133,213,19,1,193,6,120,225,52,150,170,170,138,10,113,247,93,111,203,200,79,93,154,88,214,114,176,233,177,221,43,11,206,116,53,29,225,146,198,58,82,157,14,203,198,122,176,1,246,34,78,
223,105,80,12,189,207,221,252,252,232,51,93,103,186,254,88,186,102,103,127,71,59,144,120,46,228,126,110,48,5,27,160,222,198,116,156,112,86,150,127,222,176,213,179,169,166,160,20,129,86,137,180,55,155,
184,152,126,133,133,159,27,76,193,6,232,70,44,235,200,219,135,255,244,58,253,28,111,35,8,233,215,149,84,176,1,122,144,74,242,9,36,209,7,237,128,37,218,131,125,145,2,78,87,80,1,110,204,45,244,106,39,9,
220,8,200,232,17,223,203,149,230,215,188,12,65,127,34,20,138,150,244,3,84,20,96,128,138,2,12,80,254,2,84,250,188,71,234,132,14,127,212,119,125,126,173,211,23,64,253,225,59,175,130,162,215,236,140,72,9,
234,106,178,94,253,193,31,35,96,212,214,234,197,143,135,43,125,65,232,59,148,134,56,99,92,28,242,156,71,60,82,57,190,26,172,81,175,130,199,3,137,113,198,184,56,69,150,213,141,172,125,208,108,200,23,192,
11,67,105,28,29,213,206,121,19,230,91,144,242,83,18,82,252,52,69,202,243,28,253,169,79,239,101,44,178,38,203,188,9,243,45,213,29,85,78,36,78,117,33,12,6,148,175,56,240,194,80,154,237,167,182,85,45,182,
44,201,248,226,212,231,51,123,188,61,237,218,223,215,1,157,54,187,53,172,74,76,151,33,125,232,68,18,48,219,104,48,102,46,182,44,73,249,164,238,227,114,36,189,108,67,219,125,3,201,31,128,103,1,199,150,
186,205,123,23,76,184,61,229,183,179,126,55,255,181,178,151,232,241,246,36,112,21,140,61,65,123,244,204,104,48,102,62,49,107,213,2,167,187,189,123,75,221,230,189,200,218,206,18,32,192,94,4,80,181,170,
170,135,214,30,92,51,122,85,214,234,220,87,231,189,153,83,224,216,144,92,212,184,203,113,21,13,222,73,113,186,219,187,215,30,92,99,215,166,23,85,35,107,31,116,119,41,253,13,161,189,100,126,224,48,164,
83,246,181,209,79,150,81,201,102,32,162,71,63,1,56,58,170,253,30,253,116,233,252,64,127,114,97,15,82,195,59,14,156,83,85,245,116,97,237,166,202,75,230,170,92,173,195,199,124,230,235,254,0,212,71,196,233,
45,196,179,218,151,36,18,29,127,231,119,53,70,159,173,226,65,218,137,45,68,7,48,2,151,95,206,234,59,59,102,192,113,72,215,146,34,54,8,14,23,69,1,6,168,126,195,152,168,252,87,212,2,3,84,20,96,128,138,2,
12,80,255,7,227,170,142,248,177,222,253,86,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::lectureOn_png = (const char*) resource_EditScene_lectureOn_png;
const int EditScene::lectureOn_pngSize = 4316;

// JUCER_RESOURCE: deleteOn_png, 4460, "../../MiamCommon/Resources/DeleteOn.png"
static const unsigned char resource_EditScene_deleteOn_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,
154,156,24,0,0,8,176,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,
77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,
116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,
48,56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,
50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,
115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,
108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,
47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,
112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,115,116,82,101,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,
49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,
46,99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,
32,40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,54,43,48,49,58,48,48,34,32,120,109,112,58,
77,101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,56,58,48,51,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,56,58,53,56,58,48,51,43,48,49,58,48,48,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,
61,34,120,109,112,46,105,105,100,58,57,97,102,56,48,101,53,102,45,54,57,102,48,45,52,48,102,101,45,56,101,53,48,45,100,48,52,54,54,52,56,49,49,49,51,55,34,32,120,109,112,77,77,58,68,111,99,117,109,101,
110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,101,56,97,52,56,57,54,99,45,99,55,50,98,45,98,51,52,98,45,98,55,56,51,45,101,54,101,99,57,55,52,49,
48,101,48,49,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,
45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,
80,114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,
58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,102,101,
53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,
52,58,48,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,
116,111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,53,55,53,57,52,55,54,52,45,97,56,99,48,45,52,48,52,53,45,98,56,101,98,45,48,54,98,54,50,98,49,51,101,101,97,99,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,56,58,53,56,58,48,51,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,
32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,
34,99,111,110,118,101,114,116,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,111,98,
101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,100,101,114,105,118,
101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,99,111,110,118,101,114,116,101,100,32,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,
111,98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,
101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,97,102,56,48,101,53,102,45,54,57,102,48,45,52,48,102,101,45,56,101,53,48,45,100,48,52,54,
54,52,56,49,49,49,51,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,56,58,48,51,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,
114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,
47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,82,101,102,
58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,53,55,53,57,52,55,54,52,45,97,56,99,48,45,52,48,52,53,45,98,56,101,98,45,48,54,98,54,50,98,49,51,101,101,97,99,34,32,115,116,
82,101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,
57,55,34,32,115,116,82,101,102,58,111,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,
56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,47,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,
101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,62,176,223,216,95,0,0,8,98,73,68,65,84,120,156,237,156,239,79,84,87,26,199,63,119,24,134,182,163,128,187,219,66,101,39,14,
216,88,20,11,33,182,34,63,194,54,90,195,80,94,176,137,47,250,15,248,194,200,38,104,55,43,155,93,146,221,23,190,208,109,172,154,104,179,47,246,95,48,89,178,82,166,235,143,181,132,17,75,53,141,168,91,119,
219,194,232,4,25,212,202,15,29,10,51,195,189,251,226,156,209,145,12,204,133,123,239,252,96,231,155,220,220,97,184,220,115,158,15,231,220,243,156,231,158,243,40,154,166,145,211,234,101,75,119,5,178,93,
57,128,6,149,3,104,80,57,128,6,101,79,244,229,175,191,104,77,117,61,178,70,127,111,233,123,233,231,92,11,52,168,132,45,112,25,41,64,30,80,32,15,7,226,159,160,152,92,175,84,73,3,84,32,12,204,203,99,65,
126,175,75,122,1,42,242,218,87,129,98,224,103,242,236,68,64,204,102,128,97,32,4,76,1,79,228,249,39,32,138,14,144,122,0,42,8,72,69,192,70,160,28,112,203,207,197,136,150,104,211,83,88,134,73,65,180,190,
121,4,180,7,128,31,24,149,159,167,17,112,151,181,75,15,64,59,2,222,91,64,141,162,40,239,120,92,109,59,223,127,115,79,69,249,250,138,66,13,13,141,236,156,206,40,40,138,130,194,232,211,145,153,43,227,151,
70,188,129,222,33,77,211,110,1,55,129,239,129,73,32,178,220,61,146,1,84,16,221,118,35,80,83,232,40,108,238,170,233,110,41,116,20,21,244,248,207,141,12,78,248,124,161,104,40,132,206,230,158,97,82,0,187,
211,238,116,214,151,52,186,219,221,251,182,54,149,52,87,28,191,121,116,195,76,120,6,96,22,152,35,137,109,201,0,230,33,186,105,185,162,40,239,116,213,116,183,76,71,166,181,63,221,248,195,197,168,26,253,
55,162,201,79,37,43,36,67,165,0,246,80,52,84,124,113,236,159,238,43,227,151,183,125,92,221,213,212,85,211,221,210,125,189,107,82,211,180,32,226,153,56,139,176,47,161,146,1,44,64,12,24,110,143,171,109,
103,161,163,168,224,207,55,254,120,41,170,70,7,129,111,128,0,240,12,49,114,101,163,242,128,117,128,63,170,70,167,63,29,62,206,201,250,179,31,120,92,109,59,251,238,159,255,14,241,60,252,17,131,0,139,129,
141,239,191,185,167,162,199,127,110,36,162,70,238,32,224,221,146,55,143,244,120,188,170,9,198,164,92,237,94,143,13,200,71,244,34,162,106,180,168,199,127,174,98,239,47,91,43,250,238,159,143,31,36,67,75,
221,35,153,35,237,64,184,42,197,238,245,229,133,131,19,62,63,162,219,6,16,240,194,217,10,15,64,214,61,140,176,37,0,248,7,39,124,254,242,245,21,133,188,236,166,45,169,100,0,109,242,6,5,0,114,192,152,66,
116,219,8,217,247,220,75,36,13,97,203,51,96,42,20,13,133,52,97,86,252,68,97,73,37,3,168,200,195,38,93,149,168,60,22,16,62,212,90,145,138,176,41,10,68,165,173,177,25,214,178,147,4,189,115,97,45,238,188,
22,90,221,82,138,183,79,151,157,185,96,130,65,229,0,26,212,74,163,49,186,212,238,245,196,162,54,177,99,33,118,244,120,188,150,62,2,82,93,182,85,45,48,15,225,2,148,2,155,0,23,194,33,47,144,190,151,37,146,
247,142,57,255,46,89,118,169,172,75,158,21,101,90,9,176,24,216,12,236,0,222,3,42,129,55,128,87,172,128,40,239,249,138,44,163,82,150,185,67,214,161,152,44,4,248,26,176,241,147,93,167,186,119,151,237,253,
8,104,0,182,98,1,196,69,240,182,2,13,187,203,246,126,244,201,174,83,221,136,64,200,107,100,25,192,5,132,115,186,208,23,232,189,187,191,242,192,135,77,165,205,173,64,29,38,67,76,0,175,174,169,180,185,117,
127,229,129,15,251,2,189,119,227,235,98,180,172,68,178,100,16,65,84,118,6,24,187,60,118,225,90,120,97,126,195,193,170,206,122,13,108,190,96,127,188,99,250,176,221,235,153,91,237,116,48,1,188,157,141,165,
205,31,28,172,234,172,251,236,206,233,193,129,96,255,53,96,76,214,37,43,1,250,129,87,7,130,253,14,13,108,29,85,157,117,0,190,96,127,252,181,171,130,184,20,188,142,170,206,186,179,119,78,127,229,11,246,
95,66,4,61,252,100,27,192,30,143,87,107,247,122,194,192,99,224,63,196,181,60,51,32,234,128,119,17,248,90,150,253,24,17,244,176,196,125,178,170,5,210,227,241,170,237,94,207,28,240,48,246,93,12,154,17,136,
58,225,13,1,223,202,178,87,253,136,208,35,203,0,130,249,16,51,13,30,88,12,16,204,131,152,137,240,32,5,0,193,56,196,76,133,7,41,2,8,171,135,40,63,103,36,60,72,33,64,88,21,196,71,242,252,58,25,8,15,82,12,
16,86,4,209,134,120,43,6,98,53,196,187,153,6,15,210,0,16,116,67,204,71,116,91,128,205,141,165,205,191,202,52,120,144,38,128,144,28,162,166,169,142,171,19,3,155,0,26,74,154,182,116,84,117,214,102,26,60,
72,35,64,72,8,209,230,11,246,231,107,154,234,232,216,126,104,71,88,13,151,2,116,108,63,84,118,230,246,201,27,87,39,6,190,4,174,147,33,240,32,205,0,225,37,136,143,16,207,188,55,174,78,12,108,10,171,225,
210,223,86,255,222,5,112,98,248,88,224,250,163,161,255,2,63,200,107,30,145,1,240,32,247,78,196,176,210,14,48,206,73,126,29,49,218,110,110,40,105,218,114,184,250,72,217,137,225,99,129,19,195,199,2,135,
171,143,148,53,148,52,109,65,68,151,203,229,181,150,68,182,87,170,180,118,225,4,51,140,119,229,104,91,123,230,246,201,27,178,219,114,246,246,169,45,191,217,126,184,86,81,108,97,95,176,63,194,139,151,250,
134,226,137,102,40,109,0,147,77,207,228,128,241,3,192,213,137,129,123,138,98,11,155,25,79,52,75,105,1,168,115,110,123,157,23,142,244,67,217,242,76,137,39,154,169,148,3,92,97,96,32,54,149,155,5,84,51,226,
137,102,43,165,0,87,19,85,145,127,106,106,80,214,76,165,12,160,145,144,148,21,145,109,179,148,18,128,70,227,121,86,189,30,48,67,150,3,52,43,24,154,169,16,45,5,104,118,36,57,19,33,90,6,208,170,48,124,166,
65,180,114,121,155,3,248,5,240,54,240,158,153,193,80,157,16,99,27,9,85,4,196,121,43,222,13,91,213,2,243,128,66,196,158,186,218,166,210,230,61,7,77,14,134,38,131,168,128,58,16,236,15,35,54,14,206,33,54,
205,44,185,223,99,181,178,26,96,217,238,178,189,187,246,87,30,168,255,236,206,233,65,185,220,226,107,76,138,231,45,1,81,83,64,61,88,213,89,239,200,43,152,188,60,118,225,30,47,54,15,102,21,192,124,32,175,
213,213,86,249,183,187,127,253,92,46,244,249,6,177,220,194,180,96,104,2,136,234,64,176,63,236,200,43,152,108,117,181,85,94,30,187,240,188,46,70,203,74,36,43,23,23,205,2,15,126,119,237,208,81,249,243,24,
98,161,207,99,76,14,134,46,130,168,2,63,93,30,187,112,79,194,123,32,235,146,61,139,139,16,149,157,66,68,83,198,17,235,243,102,228,97,201,238,166,69,16,231,16,224,242,17,240,166,176,8,160,149,11,44,67,
64,16,184,135,216,70,245,4,152,183,210,39,147,247,158,151,101,5,100,217,65,89,151,236,105,129,210,93,136,237,106,74,169,82,93,118,218,67,226,217,174,28,64,131,210,11,80,137,59,103,107,134,14,61,138,183,
79,151,157,201,0,198,54,223,169,10,74,44,245,137,29,225,83,173,165,214,107,67,216,100,7,236,210,86,21,29,155,43,147,65,136,79,74,131,211,238,116,34,54,173,172,67,184,8,107,161,53,42,8,91,214,1,197,78,
187,211,169,8,179,230,17,182,47,235,53,36,3,248,60,41,141,255,233,232,76,125,73,163,27,49,191,117,1,63,7,28,153,240,110,118,181,146,117,119,32,108,113,1,238,250,146,70,247,232,211,145,25,132,239,24,66,
48,88,82,201,220,152,231,73,105,174,140,95,26,105,119,239,219,250,229,248,191,170,34,106,100,90,254,62,0,60,107,247,122,178,61,233,132,11,168,181,219,236,219,218,221,251,42,62,15,252,227,91,132,35,62,
133,236,125,75,73,15,192,39,128,223,27,232,29,106,42,105,174,56,92,125,164,241,228,240,95,136,168,145,34,214,64,218,19,196,35,201,109,183,217,183,125,92,221,213,52,19,158,158,247,6,122,135,16,182,61,193,
32,192,216,148,108,84,211,180,91,199,111,30,221,208,85,211,221,242,105,253,153,61,61,254,115,229,131,19,62,255,26,74,188,83,49,19,158,158,63,126,243,232,23,50,123,209,40,58,166,128,74,162,36,180,139,242,
7,230,3,27,72,144,250,201,189,190,188,16,200,234,212,79,0,254,167,163,186,83,63,45,206,31,168,103,42,23,69,196,210,190,7,102,53,77,11,246,221,63,255,221,162,188,42,107,53,249,88,210,233,160,30,128,177,
20,113,147,188,136,236,142,146,75,127,7,232,15,38,196,114,171,68,17,225,161,31,201,37,96,4,86,30,141,137,143,116,44,153,14,233,255,73,89,235,4,103,138,114,0,13,42,161,27,147,147,126,229,90,160,65,229,
0,26,84,14,160,65,253,15,4,191,148,196,206,110,77,25,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::deleteOn_png = (const char*) resource_EditScene_deleteOn_png;
const int EditScene::deleteOn_pngSize = 4460;

// JUCER_RESOURCE: lecture_png, 2530, "../../MiamCommon/Resources/Lecture.png"
static const unsigned char resource_EditScene_lecture_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,
156,24,0,0,5,241,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,
48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,
97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,48,
56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,
47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,
58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,
47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,
47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,
112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,
110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,120,109,112,58,77,101,116,97,100,
97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,
48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,52,101,56,48,98,50,97,45,49,97,57,102,45,52,55,54,55,45,56,
53,99,53,45,50,56,53,51,97,98,55,101,56,50,57,102,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,
99,101,52,97,101,48,100,101,45,98,100,100,101,45,54,101,52,97,45,56,97,98,56,45,101,48,99,99,97,101,57,53,53,50,97,54,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,
116,73,68,61,34,120,109,112,46,100,105,100,58,100,97,100,55,48,51,100,52,45,98,97,52,102,45,52,101,56,55,45,97,99,98,55,45,53,97,48,48,52,100,98,57,57,52,51,57,34,32,100,99,58,102,111,114,109,97,116,61,
34,105,109,97,103,101,47,112,110,103,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,80,114,111,102,105,108,
101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,108,105,32,115,116,
69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,97,100,55,48,51,100,52,45,98,
97,52,102,45,52,101,56,55,45,97,99,98,55,45,53,97,48,48,52,100,98,57,57,52,51,57,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,51,50,43,48,49,58,
48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,
47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,
100,58,57,52,101,56,48,98,50,97,45,49,97,57,102,45,52,55,54,55,45,56,53,99,53,45,50,56,53,51,97,98,55,101,56,50,57,102,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,
48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,
97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,
62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,
100,61,34,114,34,63,62,222,251,161,205,0,0,3,151,73,68,65,84,120,156,237,220,61,136,93,69,20,192,241,223,91,54,133,46,18,139,164,88,109,130,77,162,18,3,34,178,130,162,54,193,88,4,148,109,140,136,213,70,
176,16,181,177,208,78,133,128,160,177,209,132,109,68,48,130,44,22,41,220,96,37,42,186,126,32,68,81,34,24,72,227,7,104,17,21,69,48,230,89,156,60,72,132,176,119,222,184,111,238,188,157,127,185,48,123,207,
249,51,103,238,220,153,121,51,24,14,135,26,227,51,83,58,128,218,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,153,29,163,205,0,139,88,194,109,24,226,252,255,25,212,4,153,17,249,
124,140,101,172,136,124,58,51,72,252,148,187,10,199,112,13,158,195,187,248,35,229,31,244,144,57,236,197,51,248,1,7,240,123,215,198,41,2,7,56,142,95,68,239,59,151,20,102,255,153,21,189,112,27,246,235,216,
19,83,198,192,69,209,243,14,154,62,121,68,78,75,34,199,197,174,141,82,4,46,137,178,253,59,45,174,170,56,39,114,92,234,218,32,165,132,127,195,188,250,199,188,245,152,195,79,98,188,95,151,212,105,204,180,
203,35,114,236,220,171,82,4,214,58,85,25,135,206,185,182,137,116,38,77,96,38,77,96,38,77,96,38,77,96,38,77,96,38,77,96,38,37,5,62,37,225,155,179,175,148,20,184,15,135,241,30,246,20,140,35,139,210,37,188,
31,111,226,4,142,96,123,217,112,210,41,45,240,60,142,98,23,254,196,55,120,28,91,10,198,148,68,105,129,35,126,197,147,184,93,172,14,127,37,74,188,247,244,69,224,136,111,113,47,158,192,75,120,7,59,139,70,
180,14,125,19,56,98,21,187,197,158,203,7,120,17,91,139,70,116,25,250,42,144,88,249,62,140,27,197,34,231,41,60,162,103,49,247,42,152,203,240,179,16,119,15,30,192,23,184,179,104,68,23,81,131,192,17,39,113,
151,216,179,120,13,111,97,71,185,112,130,154,4,142,88,193,245,66,232,231,120,86,148,120,17,106,20,8,127,225,121,241,5,179,67,188,189,31,20,123,215,19,165,86,129,35,190,199,67,226,155,250,49,124,132,91,
39,25,64,237,2,71,172,97,1,175,226,109,49,70,206,79,226,193,211,34,144,216,138,124,93,124,22,254,136,47,241,232,70,63,116,154,4,22,97,154,4,14,240,176,152,112,207,227,38,188,178,209,15,29,231,124,96,31,
89,192,203,248,7,247,227,211,73,61,184,118,129,215,226,16,238,22,43,220,199,36,30,144,204,165,214,18,190,2,79,139,201,244,25,177,98,243,134,9,203,163,206,30,184,136,23,240,25,110,17,2,139,81,147,192,61,
98,156,187,90,188,44,222,47,26,205,5,106,40,225,237,98,217,255,132,216,63,185,89,79,228,209,111,129,91,196,202,244,215,226,204,222,46,33,178,87,199,236,250,90,194,251,196,146,254,105,220,33,22,11,122,
73,223,4,238,20,226,174,19,189,111,181,108,56,235,211,151,18,222,42,246,61,62,20,251,32,187,85,32,143,242,2,103,196,114,253,41,92,137,27,196,62,72,53,191,4,40,93,194,199,241,157,216,239,56,89,56,150,177,
40,41,112,85,188,36,86,10,198,144,77,202,239,68,206,138,73,236,102,224,172,142,185,150,30,3,171,167,9,204,164,9,204,164,9,204,164,9,204,164,9,204,164,9,204,36,69,224,102,146,221,57,215,84,41,197,14,241,
76,144,57,9,103,108,82,4,174,137,243,203,211,206,94,113,13,74,39,82,4,46,139,171,65,170,57,65,63,6,179,34,199,229,174,13,198,189,246,228,160,138,150,156,58,178,225,215,158,12,197,165,52,219,68,57,223,
103,58,198,196,57,145,203,39,34,183,3,18,246,151,83,111,46,226,210,171,159,22,46,252,173,87,27,61,9,140,58,208,154,9,93,253,212,248,15,155,105,110,183,33,52,129,153,52,129,153,52,129,153,52,129,153,52,
129,153,52,129,153,52,129,153,52,129,153,252,11,241,216,160,159,249,219,245,255,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::lecture_png = (const char*) resource_EditScene_lecture_png;
const int EditScene::lecture_pngSize = 2530;

// JUCER_RESOURCE: pause_png, 2946, "../../MiamCommon/Resources/Pause.png"
static const unsigned char resource_EditScene_pause_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,
156,24,0,0,8,176,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,
48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,
97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,48,
56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,
47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,
58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,
46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,
97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,
47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,115,116,82,101,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,
48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,
99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,
40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,54,43,48,49,58,48,48,34,32,120,109,112,58,77,
101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,49,53,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,
49,45,48,57,84,48,56,58,53,52,58,49,53,43,48,49,58,48,48,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,
34,120,109,112,46,105,105,100,58,102,53,99,50,57,57,52,55,45,48,49,98,54,45,52,51,99,49,45,97,56,51,55,45,57,97,49,56,54,55,49,100,48,55,99,51,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,
73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,100,51,99,48,102,48,56,53,45,51,48,54,99,45,97,101,52,50,45,98,52,101,54,45,49,98,98,51,55,55,99,54,57,56,
50,48,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,
98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,80,114,
111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,108,
105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,102,101,53,102,
56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,
48,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,
111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,
112,46,105,105,100,58,102,98,97,102,50,98,55,99,45,100,102,102,99,45,52,51,51,48,45,56,51,55,98,45,50,56,56,50,99,48,102,97,48,99,55,52,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,56,58,53,52,58,49,53,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,
32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,
34,99,111,110,118,101,114,116,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,111,98,
101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,100,101,114,105,118,
101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,99,111,110,118,101,114,116,101,100,32,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,
111,98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,
101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,102,53,99,50,57,57,52,55,45,48,49,98,54,45,52,51,99,49,45,97,56,51,55,45,57,97,49,56,54,55,49,
100,48,55,99,51,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,49,53,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,
65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,
62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,82,101,102,58,105,110,
115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,102,98,97,102,50,98,55,99,45,100,102,102,99,45,52,51,51,48,45,56,51,55,98,45,50,56,56,50,99,48,102,97,48,99,55,52,34,32,115,116,82,101,102,
58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,
115,116,82,101,102,58,111,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,
45,100,51,102,99,53,55,49,102,48,48,57,55,34,47,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,
32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,62,178,6,88,204,0,0,2,120,73,68,65,84,120,156,237,220,49,110,19,65,24,197,241,247,33,83,69,150,92,208,80,80,113,137,116,233,34,154,68,
138,32,18,74,67,149,158,35,112,132,244,92,128,34,40,18,110,162,116,169,72,138,28,1,41,116,20,20,17,18,29,226,81,120,12,146,181,142,191,217,231,245,98,231,253,36,55,222,221,100,230,159,93,107,109,71,19,
36,97,237,61,234,123,0,235,206,1,69,14,40,114,64,145,3,138,28,80,228,128,34,7,20,57,160,104,144,221,49,34,0,0,36,3,192,43,0,199,0,182,203,230,223,75,31,217,106,76,79,160,43,0,239,1,156,70,4,1,32,253,14,
141,100,234,81,246,29,146,28,147,188,33,121,64,114,107,249,115,90,45,146,91,101,46,55,101,110,195,242,124,234,17,21,239,133,3,192,39,0,223,1,28,71,196,175,142,230,212,11,146,3,76,206,194,39,0,246,1,228,
194,100,75,147,60,44,127,165,199,29,206,163,87,36,7,101,142,135,233,43,179,34,224,5,201,131,190,39,217,181,114,57,95,116,113,9,255,0,240,52,34,126,118,56,254,222,149,215,245,111,0,134,153,253,171,110,
99,54,61,30,240,119,142,233,179,170,38,224,186,222,170,180,145,158,171,111,164,69,14,40,114,64,145,3,138,28,80,228,128,34,7,20,57,160,200,1,69,14,40,74,127,34,189,44,36,159,1,120,185,96,183,113,68,124,
153,57,238,57,128,189,5,199,157,69,196,87,101,124,181,86,30,16,147,120,111,0,92,206,217,62,253,154,224,100,230,249,61,0,175,49,249,248,189,201,206,156,227,58,213,71,64,0,184,140,136,183,77,27,72,190,187,
231,184,243,136,104,220,78,242,68,30,85,11,126,13,20,57,160,200,1,69,14,40,114,64,145,3,138,28,80,228,128,34,7,20,57,160,200,1,69,14,40,114,64,145,3,138,28,80,228,128,34,7,20,57,160,200,1,69,125,125,39,
178,115,207,119,24,219,0,62,204,217,246,130,228,104,222,207,4,112,171,13,171,94,31,1,63,46,216,126,11,96,220,240,124,211,115,179,199,157,181,24,143,164,230,159,204,239,34,98,212,225,88,254,27,36,239,0,
140,50,251,250,53,80,228,128,34,7,20,57,160,200,1,69,14,40,114,64,145,3,138,28,80,228,128,162,154,128,15,41,118,122,174,85,81,54,97,145,137,69,202,28,35,187,127,77,192,43,0,187,213,35,90,63,187,0,62,167,
247,246,162,19,255,180,89,116,194,203,158,20,109,151,61,73,95,194,101,69,159,163,242,11,174,55,109,225,29,0,215,152,204,237,104,186,122,81,70,250,12,244,210,79,205,106,46,97,107,240,144,238,237,58,225,
128,34,7,20,57,160,200,1,69,14,40,114,64,145,3,138,28,80,244,7,86,19,209,167,101,88,94,195,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::pause_png = (const char*) resource_EditScene_pause_png;
const int EditScene::pause_pngSize = 2946;

// JUCER_RESOURCE: stop_png, 2912, "../../MiamCommon/Resources/Stop.png"
static const unsigned char resource_EditScene_stop_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,
156,24,0,0,8,176,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,
48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,
97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,48,
56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,
47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,
58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,
46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,
97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,
47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,115,116,82,101,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,
48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,
99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,
40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,54,43,48,49,58,48,48,34,32,120,109,112,58,77,
101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,53,58,52,49,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,
49,45,48,57,84,48,56,58,53,53,58,52,49,43,48,49,58,48,48,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,
34,120,109,112,46,105,105,100,58,57,102,54,102,57,98,52,55,45,52,52,100,56,45,52,51,100,51,45,97,101,54,54,45,53,55,57,53,50,102,48,101,52,52,51,100,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,
116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,101,56,51,48,56,100,48,56,45,101,102,51,53,45,51,56,52,57,45,56,53,56,50,45,97,101,51,53,98,57,101,48,
97,101,97,55,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,
45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,
80,114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,
58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,102,101,
53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,
52,58,48,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,
116,111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,52,51,50,57,50,97,98,98,45,55,52,48,48,45,52,101,100,99,45,98,50,99,50,45,98,55,97,56,98,99,98,98,50,97,54,51,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,56,58,53,53,58,52,49,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,
32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,
34,99,111,110,118,101,114,116,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,111,98,
101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,100,101,114,105,118,
101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,99,111,110,118,101,114,116,101,100,32,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,
111,98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,
101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,102,54,102,57,98,52,55,45,52,52,100,56,45,52,51,100,51,45,97,101,54,54,45,53,55,57,53,50,
102,48,101,52,52,51,100,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,53,58,52,49,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,
114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,
47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,82,101,102,
58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,52,51,50,57,50,97,98,98,45,55,52,48,48,45,52,101,100,99,45,98,50,99,50,45,98,55,97,56,98,99,98,98,50,97,54,51,34,32,115,116,82,
101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,
55,34,32,115,116,82,101,102,58,111,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,
98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,47,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,
116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,62,193,32,33,78,0,0,2,86,73,68,65,84,120,156,237,220,191,78,20,81,24,134,241,231,35,107,69,54,161,176,177,240,62,180,218,142,
210,132,40,13,141,21,214,94,10,189,183,224,159,68,59,58,42,215,130,123,192,206,194,130,152,88,105,252,44,246,160,13,200,57,243,206,217,113,151,247,151,76,72,96,56,204,121,152,153,204,178,228,68,102,98,
195,237,76,125,0,155,206,1,69,14,40,114,64,145,3,138,28,80,228,128,34,7,20,57,160,104,86,187,99,68,0,144,153,1,60,3,142,129,199,64,2,191,122,28,220,26,236,0,1,124,4,94,1,175,35,34,1,170,95,161,101,102,
213,86,246,157,103,230,135,204,60,207,204,131,204,220,237,50,173,53,202,204,221,50,151,243,50,183,121,249,124,213,22,13,175,133,3,120,15,124,5,142,35,226,103,167,57,77,34,51,103,172,206,194,251,192,19,
86,87,86,213,55,214,110,135,229,183,116,175,227,60,38,149,153,179,50,199,195,234,43,179,33,224,105,102,30,76,61,201,222,202,229,124,218,227,18,254,6,60,136,136,239,29,143,127,114,229,190,254,5,152,215,
236,223,244,24,179,237,241,224,207,28,171,207,170,150,128,155,250,168,50,68,245,92,253,32,45,114,64,145,3,138,28,80,228,128,34,7,20,57,160,200,1,69,14,40,114,64,81,245,95,164,199,146,153,15,129,167,157,
134,127,23,17,159,59,141,125,173,181,7,100,21,239,57,112,54,242,184,139,242,241,100,228,113,255,105,138,128,0,103,17,241,114,204,1,51,243,100,204,241,106,249,30,40,114,64,145,3,138,28,80,228,128,34,7,
20,57,160,200,1,69,14,40,114,64,145,3,138,28,80,228,128,34,7,20,57,160,200,1,69,14,40,114,64,145,3,138,166,122,79,100,209,225,61,140,5,112,49,242,152,183,154,34,224,155,78,227,94,0,111,59,141,125,163,
150,127,50,191,140,136,189,142,199,242,223,200,204,75,96,175,102,95,223,3,69,14,40,114,64,145,3,138,28,80,228,128,34,7,20,57,160,200,1,69,14,40,106,9,120,151,98,87,207,181,41,202,54,44,50,113,155,50,199,
168,221,191,37,224,18,216,111,62,162,205,179,207,106,25,148,58,94,116,226,175,33,139,78,12,93,246,228,69,68,252,232,54,147,9,12,93,246,164,250,18,46,43,250,28,149,31,176,220,182,133,119,128,79,172,230,
118,116,181,122,81,141,234,51,240,134,165,159,30,149,47,111,234,122,10,87,39,208,146,129,75,63,181,92,194,118,141,187,244,108,215,133,3,138,28,80,228,128,34,7,20,57,160,200,1,69,14,40,114,64,209,111,70,
62,192,61,163,138,69,63,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::stop_png = (const char*) resource_EditScene_stop_png;
const int EditScene::stop_pngSize = 2912;

// JUCER_RESOURCE: delete_png, 3013, "../../MiamCommon/Resources/Delete.png"
static const unsigned char resource_EditScene_delete_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,
156,24,0,0,8,176,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,
48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,
97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,48,
56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,
47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,
58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,
46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,
97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,
47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,115,116,82,101,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,
48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,
99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,
40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,54,43,48,49,58,48,48,34,32,120,109,112,58,77,
101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,55,58,52,57,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,
49,45,48,57,84,48,56,58,53,55,58,52,57,43,48,49,58,48,48,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,
34,120,109,112,46,105,105,100,58,57,101,53,52,51,52,56,51,45,97,55,56,56,45,52,56,55,98,45,98,102,55,49,45,98,56,51,50,49,50,57,53,50,54,51,53,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,
73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,54,49,100,50,48,98,54,100,45,97,97,56,54,45,49,54,52,48,45,57,57,97,54,45,50,53,99,49,98,56,101,99,52,100,
100,99,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,
56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,80,
114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,
108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,102,101,53,
102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,
58,48,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,
111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,
112,46,105,105,100,58,49,53,54,101,98,51,99,55,45,52,52,97,97,45,52,53,56,101,45,97,48,54,100,45,57,101,56,48,55,53,52,56,102,57,98,98,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,56,58,53,55,58,52,57,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,
32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,
34,99,111,110,118,101,114,116,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,111,98,
101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,100,101,114,105,118,
101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,99,111,110,118,101,114,116,101,100,32,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,
111,98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,
101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,101,53,52,51,52,56,51,45,97,55,56,56,45,52,56,55,98,45,98,102,55,49,45,98,56,51,50,49,50,
57,53,50,54,51,53,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,55,58,52,57,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,
65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,
62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,82,101,102,58,105,110,
115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,49,53,54,101,98,51,99,55,45,52,52,97,97,45,52,53,56,101,45,97,48,54,100,45,57,101,56,48,55,53,52,56,102,57,98,98,34,32,115,116,82,101,102,
58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,
115,116,82,101,102,58,111,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,
45,100,51,102,99,53,55,49,102,48,48,57,55,34,47,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,
32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,62,210,67,134,124,0,0,2,187,73,68,65,84,120,156,237,156,49,110,20,65,16,69,127,89,75,100,173,228,128,132,155,64,186,129,67,36,75,56,113,
110,46,197,13,44,34,200,124,1,150,192,247,32,112,176,32,17,129,248,4,91,182,144,176,229,234,169,233,158,238,245,127,210,36,246,120,235,215,219,238,241,76,175,221,70,18,98,58,71,75,7,24,29,9,76,34,129,
73,36,48,137,4,38,145,192,36,18,152,68,2,147,72,96,146,85,244,68,51,3,0,144,52,0,239,0,92,2,120,3,128,0,254,212,8,215,128,35,0,6,224,11,128,15,0,62,154,25,1,32,252,132,70,50,116,248,185,107,146,159,73,
222,144,60,35,121,92,165,173,134,144,60,246,94,110,188,183,181,127,61,116,88,193,179,176,1,248,4,224,22,192,165,153,253,174,212,211,34,144,92,97,63,10,95,2,120,139,253,204,10,253,96,244,56,247,119,233,
69,197,62,22,133,228,202,123,60,15,207,204,2,129,215,36,207,150,110,178,54,62,157,175,107,76,225,31,0,94,153,217,207,138,249,23,199,175,235,223,0,172,35,231,23,221,198,28,186,60,224,190,199,240,168,42,
17,56,234,173,202,20,194,189,234,70,58,137,4,38,145,192,36,18,152,68,2,147,72,96,146,234,2,73,94,145,220,212,174,243,64,221,13,201,171,22,133,162,199,110,226,235,111,72,222,182,148,152,173,73,114,87,227,
89,120,183,84,67,173,107,117,39,112,174,198,90,213,232,82,224,156,13,214,126,237,110,5,122,184,217,37,206,253,154,93,11,244,128,115,142,150,26,111,72,223,2,61,228,28,23,251,42,151,132,33,4,122,208,201,
2,42,95,79,199,16,232,97,139,69,212,148,231,175,63,142,64,15,28,22,82,91,158,215,24,75,160,135,126,82,76,11,121,94,103,60,129,30,252,81,65,173,228,121,173,49,5,122,248,255,68,181,148,231,245,198,21,232,
13,220,11,107,45,207,235,143,45,208,155,216,144,252,238,71,211,229,176,18,129,90,80,205,210,227,8,212,20,206,133,215,47,145,68,112,221,198,36,66,235,70,58,17,88,143,114,137,176,90,76,72,4,213,114,86,34,
164,22,84,19,1,181,164,159,8,167,15,149,18,193,244,177,102,15,13,214,174,209,157,192,22,242,230,172,213,149,192,150,242,230,170,217,155,192,225,254,188,173,68,96,201,63,218,236,204,236,100,74,160,209,
240,193,114,18,57,87,11,170,73,36,48,137,4,38,145,192,36,18,152,68,2,147,148,8,124,78,178,195,189,22,73,225,1,108,50,241,20,222,163,69,207,47,17,184,5,112,90,156,104,60,78,177,223,6,37,70,193,163,156,
54,157,72,62,202,253,187,237,201,123,51,251,85,173,147,5,224,196,109,79,194,83,216,119,244,185,240,2,91,30,216,198,59,0,190,98,223,219,197,221,238,69,17,194,35,240,145,173,159,94,251,183,71,221,79,225,
110,0,109,49,113,235,167,146,41,44,30,224,57,221,219,85,65,2,147,72,96,18,9,76,34,129,73,36,48,137,4,38,145,192,36,18,152,228,47,3,106,10,113,242,139,211,145,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::delete_png = (const char*) resource_EditScene_delete_png;
const int EditScene::delete_pngSize = 3013;

// JUCER_RESOURCE: option_png, 4008, "../../MiamCommon/Resources/Option.png"
static const unsigned char resource_EditScene_option_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,
156,24,0,0,8,176,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,
48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,
97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,45,48,49,58,48,
56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,
47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,
58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,
46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,
97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,
47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,115,116,82,101,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,
48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,
99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,
40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,52,58,48,54,43,48,49,58,48,48,34,32,120,109,112,58,77,
101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,57,58,49,49,58,52,54,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,49,
49,45,48,57,84,48,57,58,49,49,58,52,54,43,48,49,58,48,48,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,
34,120,109,112,46,105,105,100,58,50,55,100,50,54,54,100,51,45,57,54,102,51,45,52,102,49,53,45,56,100,101,53,45,52,53,57,49,54,57,99,99,101,101,48,51,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,
116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,53,49,100,102,51,56,52,52,45,55,54,51,102,45,97,100,52,99,45,57,101,48,101,45,98,50,51,50,48,51,55,56,
52,55,98,54,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,
45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,
80,114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,
58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,102,101,
53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,
52,58,48,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,
116,111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,101,53,51,57,97,98,53,102,45,97,48,57,54,45,52,49,49,101,45,56,99,101,101,45,100,102,98,48,55,49,56,49,99,53,57,49,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,
55,45,49,49,45,48,57,84,48,57,58,49,49,58,52,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,
112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,
61,34,99,111,110,118,101,114,116,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,97,100,111,
98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,100,101,114,105,
118,101,100,34,32,115,116,69,118,116,58,112,97,114,97,109,101,116,101,114,115,61,34,99,111,110,118,101,114,116,101,100,32,102,114,111,109,32,97,112,112,108,105,99,97,116,105,111,110,47,118,110,100,46,
97,100,111,98,101,46,112,104,111,116,111,115,104,111,112,32,116,111,32,105,109,97,103,101,47,112,110,103,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,
97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,50,55,100,50,54,54,100,51,45,57,54,102,51,45,52,102,49,53,45,56,100,101,53,45,52,53,57,
49,54,57,99,99,101,101,48,51,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,57,58,49,49,58,52,54,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,
97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,
34,47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,82,101,
102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,101,53,51,57,97,98,53,102,45,97,48,57,54,45,52,49,49,101,45,56,99,101,101,45,100,102,98,48,55,49,56,49,99,53,57,49,34,32,
115,116,82,101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,
48,48,57,55,34,32,115,116,82,101,102,58,111,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,102,101,53,102,56,101,97,45,56,98,50,48,45,52,100,55,
101,45,56,98,97,102,45,100,51,102,99,53,55,49,102,48,48,57,55,34,47,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,
112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,62,173,119,111,3,0,0,6,158,73,68,65,84,120,156,237,156,107,168,21,85,24,134,31,205,212,242,152,154,153,101,217,197,
52,143,149,37,234,161,232,94,80,73,65,229,201,10,42,232,74,87,42,36,186,253,169,160,34,34,43,42,197,126,104,86,102,209,61,45,34,136,136,8,234,135,25,165,166,167,155,215,178,212,138,188,229,165,163,111,
63,190,217,237,57,227,236,115,246,204,90,51,123,132,121,96,56,123,175,219,94,243,238,111,173,111,173,111,205,62,221,36,81,146,158,238,141,238,192,158,78,41,160,35,165,128,142,148,2,58,82,10,232,72,41,
160,35,165,128,142,148,2,58,82,10,232,72,41,160,35,165,128,142,148,2,58,146,167,128,135,1,95,2,91,128,205,192,42,224,29,224,6,96,72,142,253,240,74,183,156,163,49,147,128,151,128,125,35,233,223,1,83,128,
23,243,236,140,15,242,22,16,224,66,224,21,160,111,76,222,115,192,61,192,182,92,123,228,64,35,230,192,121,192,85,192,134,152,188,219,129,71,242,237,142,27,141,176,192,10,151,1,179,129,158,49,121,19,129,
247,114,237,77,74,26,233,133,223,0,102,213,200,187,31,232,147,99,95,82,211,232,101,204,60,160,61,38,125,44,112,122,206,125,73,133,111,1,79,5,46,74,80,126,61,176,41,38,189,7,112,162,151,30,101,140,111,
1,79,195,60,236,5,117,150,31,64,237,161,122,148,151,30,101,140,111,1,251,0,77,192,171,192,249,117,148,63,133,120,39,2,208,15,179,196,66,227,91,192,173,193,223,253,128,57,116,110,137,163,129,91,58,201,
223,12,236,244,212,175,204,240,45,224,143,161,215,253,49,79,123,27,54,84,43,244,5,46,6,222,5,6,117,210,214,111,64,225,15,173,125,175,3,135,1,75,128,94,145,244,249,192,162,224,117,51,112,114,29,109,253,
132,237,90,150,122,235,93,6,248,22,176,7,102,117,19,61,181,215,6,180,82,96,17,125,15,225,118,96,26,254,246,178,205,192,92,224,24,79,237,121,39,139,133,244,39,192,84,143,237,141,192,194,94,133,20,49,171,
189,112,111,96,38,112,133,199,54,191,199,156,79,155,199,54,157,201,106,43,183,13,184,14,120,136,248,168,75,26,70,98,1,134,81,158,218,243,66,30,209,152,49,192,181,216,54,175,153,106,48,117,7,176,87,112,
37,161,80,142,37,207,112,214,1,88,232,190,9,19,109,19,48,24,120,30,56,34,97,91,63,98,195,121,137,191,238,165,163,145,241,192,10,227,48,39,113,88,194,122,109,192,37,52,88,196,70,135,179,0,22,96,103,37,
203,19,214,107,198,230,196,134,122,231,34,88,96,133,241,152,37,14,77,88,175,161,115,98,17,44,176,194,87,216,144,76,99,137,115,105,144,119,46,146,128,96,123,230,203,176,51,227,36,84,22,219,185,139,88,52,
1,193,44,177,149,244,150,152,235,156,216,8,1,71,2,211,129,15,128,115,67,233,77,216,178,6,204,177,92,78,122,75,204,79,68,73,121,95,143,169,202,2,73,35,37,29,44,233,77,73,75,36,221,33,169,41,40,59,78,210,
114,37,167,77,82,115,30,247,147,135,96,163,37,221,44,105,136,164,227,2,209,194,252,46,105,101,36,109,76,168,126,139,164,21,73,212,11,104,147,52,42,235,251,203,90,188,19,36,253,28,220,208,90,73,155,234,
188,249,161,145,118,198,42,157,136,75,149,177,136,89,207,129,103,99,81,106,128,3,177,121,174,30,30,1,134,99,231,38,61,129,175,129,75,129,21,9,63,63,123,199,146,229,183,35,105,176,164,63,58,177,144,127,
234,176,162,123,67,237,141,211,238,195,189,30,150,74,58,198,225,62,26,102,129,3,128,189,99,210,95,6,206,3,206,2,38,96,103,201,181,184,153,234,225,83,241,182,125,89,124,43,146,134,73,122,80,210,226,136,
37,108,151,57,148,184,58,183,74,106,143,148,223,37,233,53,73,125,35,101,199,75,90,85,191,1,254,143,247,57,49,43,1,159,173,113,3,47,117,81,111,78,164,252,175,146,14,170,81,182,69,210,178,250,116,235,192,
15,242,40,98,86,67,120,108,76,218,78,224,205,46,234,205,166,227,89,112,119,226,31,196,132,130,108,251,178,18,240,73,224,155,72,218,38,96,109,23,245,254,196,34,213,21,14,194,30,19,105,173,81,190,241,219,
62,95,166,28,115,141,147,244,91,104,232,108,151,52,161,139,58,19,100,243,94,148,143,186,168,215,162,116,222,185,77,142,222,57,75,47,188,30,248,55,244,190,39,22,174,234,140,43,129,110,49,233,139,98,210,
194,204,199,44,113,69,189,157,11,24,137,13,231,230,132,245,170,120,176,180,184,171,167,164,183,98,172,105,135,164,155,106,212,185,69,187,123,225,54,217,222,121,88,157,159,155,251,182,47,171,136,116,63,
224,103,96,96,141,252,23,176,57,232,119,108,158,155,8,92,19,41,179,1,56,137,228,231,192,99,49,171,58,60,97,189,37,192,57,192,154,68,181,28,172,44,122,245,151,5,1,6,7,239,239,149,180,81,181,247,191,59,
37,253,21,252,141,99,157,164,67,82,246,165,69,157,71,113,182,201,230,204,101,234,104,245,79,37,253,44,159,2,78,15,58,241,113,40,109,188,164,17,218,125,125,87,47,55,58,244,167,214,182,111,151,44,2,212,
46,155,82,214,74,218,26,228,173,78,250,57,46,67,120,8,240,4,176,26,216,136,253,64,166,31,182,12,153,6,172,3,206,4,238,11,242,23,210,245,147,247,219,48,199,211,23,155,2,38,210,181,3,233,140,22,224,117,
224,200,80,90,59,118,46,93,113,86,107,176,199,241,6,2,219,177,199,82,234,199,225,27,158,82,167,21,189,47,11,107,133,135,234,74,217,16,13,179,72,22,47,28,44,105,178,44,142,232,99,100,68,183,125,187,36,
173,15,250,179,83,210,47,146,54,7,121,203,147,182,239,210,177,73,234,56,127,44,148,52,67,210,23,17,97,38,75,234,46,233,78,217,208,121,79,210,209,193,141,45,12,149,123,192,147,96,113,87,116,219,183,69,
54,63,111,8,94,87,152,158,167,128,119,169,186,76,153,42,169,79,144,222,91,210,221,161,188,103,36,237,21,228,13,148,45,113,42,109,156,47,233,51,73,51,37,29,155,161,128,21,75,12,207,137,21,11,172,176,78,
41,172,62,109,103,102,169,106,246,171,36,237,31,201,239,174,170,37,238,144,244,161,164,65,25,11,84,207,53,90,187,143,16,73,250,86,210,25,105,218,76,235,68,194,149,62,197,98,122,59,34,101,102,0,215,135,
222,31,143,155,67,240,69,19,182,222,107,193,28,202,124,224,115,224,239,84,173,165,252,38,31,85,117,56,172,147,116,104,36,191,151,170,243,219,70,217,220,210,191,0,22,232,253,114,169,220,42,233,223,64,164,
183,101,7,65,251,200,134,106,56,30,248,112,163,111,50,203,203,229,151,64,189,168,14,229,86,236,103,94,139,177,159,104,133,31,85,75,250,0,229,30,133,139,128,199,211,241,188,99,16,118,198,81,65,216,98,117,
184,195,103,20,30,23,1,31,197,68,250,9,91,193,63,14,28,130,253,83,137,135,176,72,113,11,240,180,91,23,139,141,207,104,204,195,192,100,224,109,224,106,95,141,22,157,34,61,96,185,71,82,196,199,219,246,40,
74,1,29,41,5,116,164,20,208,145,82,64,71,74,1,29,41,5,116,164,20,208,145,82,64,71,74,1,29,41,5,116,228,63,99,175,46,33,163,223,75,119,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* EditScene::option_png = (const char*) resource_EditScene_option_png;
const int EditScene::option_pngSize = 4008;


//[EndFile] You can add extra defines here...
//[/EndFile]
