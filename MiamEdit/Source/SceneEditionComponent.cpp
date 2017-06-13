/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "GraphicSessionManager.h"

#include <cmath>

#include "IDrawableArea.h" // areas default types defines
//[/Headers]

#include "SceneEditionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
SceneEditionComponent::SceneEditionComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (areaGroupComponent = new GroupComponent ("Area edition group component",
                                                                TRANS("Area edition")));
    areaGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    areaGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (spatGroupComponent = new GroupComponent ("Spatialization group component",
                                                                TRANS("Spatialization effect")));
    spatGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    spatGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (addPointTextButton = new TextButton ("Add Point text button"));
    addPointTextButton->setButtonText (TRANS("Add Point"));
    addPointTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addPointTextButton->addListener (this);
    addPointTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addPointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addPointTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (deletePointTextButton = new TextButton ("Delete Point text button"));
    deletePointTextButton->setButtonText (TRANS("Delete Point"));
    deletePointTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deletePointTextButton->addListener (this);
    deletePointTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deletePointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deletePointTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (copyTextButton = new TextButton ("Copy Text Button"));
    copyTextButton->setButtonText (TRANS("Copy"));
    copyTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    copyTextButton->addListener (this);
    copyTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    copyTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    copyTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (pasteTextButton = new TextButton ("Delete Point"));
    pasteTextButton->setButtonText (TRANS("Paste"));
    pasteTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    pasteTextButton->addListener (this);
    pasteTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    pasteTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    pasteTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (addAreaTextButton = new TextButton ("Add Area text button"));
    addAreaTextButton->setButtonText (TRANS("Add Area"));
    addAreaTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    addAreaTextButton->addListener (this);
    addAreaTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addAreaTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (deleteAreaTextButton = new TextButton ("Delete Area text button"));
    deleteAreaTextButton->setButtonText (TRANS("Delete Area"));
    deleteAreaTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    deleteAreaTextButton->addListener (this);
    deleteAreaTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteAreaTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (labelR = new Label ("new label",
                                           TRANS("R")));
    labelR->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelR->setJustificationType (Justification::centredLeft);
    labelR->setEditable (false, false, false);
    labelR->setColour (Label::textColourId, Colours::black);
    labelR->setColour (TextEditor::textColourId, Colours::black);
    labelR->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderR = new Slider ("Slider R"));
    sliderR->setRange (0, 255, 1);
    sliderR->setSliderStyle (Slider::LinearHorizontal);
    sliderR->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderR->setColour (Slider::thumbColourId, Colour (0xa0ff0000));
    sliderR->setColour (Slider::textBoxTextColourId, Colours::black);
    sliderR->addListener (this);

    addAndMakeVisible (labelG = new Label ("new label",
                                           TRANS("G")));
    labelG->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelG->setJustificationType (Justification::centredLeft);
    labelG->setEditable (false, false, false);
    labelG->setColour (Label::textColourId, Colours::black);
    labelG->setColour (TextEditor::textColourId, Colours::black);
    labelG->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderG = new Slider ("SliderG"));
    sliderG->setRange (0, 255, 1);
    sliderG->setSliderStyle (Slider::LinearHorizontal);
    sliderG->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderG->setColour (Slider::thumbColourId, Colour (0xa000ff00));
    sliderG->setColour (Slider::textBoxTextColourId, Colours::black);
    sliderG->addListener (this);

    addAndMakeVisible (labelB = new Label ("new label",
                                           TRANS("B")));
    labelB->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelB->setJustificationType (Justification::centredLeft);
    labelB->setEditable (false, false, false);
    labelB->setColour (Label::textColourId, Colours::black);
    labelB->setColour (TextEditor::textColourId, Colours::black);
    labelB->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderB = new Slider ("Slider B"));
    sliderB->setRange (0, 255, 1);
    sliderB->setSliderStyle (Slider::LinearHorizontal);
    sliderB->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderB->setColour (Slider::thumbColourId, Colour (0xa00000ff));
    sliderB->setColour (Slider::textBoxTextColourId, Colours::black);
    sliderB->addListener (this);

    addAndMakeVisible (sendBackwardTextButton = new TextButton ("Send backward text button"));
    sendBackwardTextButton->setButtonText (TRANS("Send backward"));
    sendBackwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    sendBackwardTextButton->addListener (this);
    sendBackwardTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sendBackwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sendBackwardTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (bringForwardTextButton = new TextButton ("Bring forward text button"));
    bringForwardTextButton->setButtonText (TRANS("Bring forward"));
    bringForwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    bringForwardTextButton->addListener (this);
    bringForwardTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    bringForwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    bringForwardTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (bringToFrontTextButton = new TextButton ("Bring to front text button"));
    bringToFrontTextButton->setButtonText (TRANS("to front"));
    bringToFrontTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    bringToFrontTextButton->addListener (this);
    bringToFrontTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    bringToFrontTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    bringToFrontTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (sendToBackTextButton = new TextButton ("Send to back text button"));
    sendToBackTextButton->setButtonText (TRANS("to back"));
    sendToBackTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    sendToBackTextButton->addListener (this);
    sendToBackTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sendToBackTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sendToBackTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (canvasGroupComponent = new GroupComponent ("Canvas edition group component",
                                                                  TRANS("Scene edition")));
    canvasGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    canvasGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (spatStatesComboBox = new ComboBox ("Spat States combo box"));
    spatStatesComboBox->setEditableText (false);
    spatStatesComboBox->setJustificationType (Justification::centredLeft);
    spatStatesComboBox->setTextWhenNothingSelected (String());
    spatStatesComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    spatStatesComboBox->addItem (TRANS("-1 undefined"), 1);
    spatStatesComboBox->addListener (this);

    addAndMakeVisible (spatLabel = new Label ("Spat label",
                                              TRANS("Link to spat state:")));
    spatLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    spatLabel->setJustificationType (Justification::centredLeft);
    spatLabel->setEditable (false, false, false);
    spatLabel->setColour (Label::textColourId, Colours::black);
    spatLabel->setColour (TextEditor::textColourId, Colours::black);
    spatLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (addSceneTextButton = new TextButton ("Add Scene text button"));
    addSceneTextButton->setButtonText (TRANS("Add Scene"));
    addSceneTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addSceneTextButton->addListener (this);
    addSceneTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addSceneTextButton->setColour (TextButton::buttonOnColourId, Colours::black);
    addSceneTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (deleteSceneTextButton = new TextButton ("Delete Scene text button"));
    deleteSceneTextButton->setButtonText (TRANS("Delete Scene"));
    deleteSceneTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deleteSceneTextButton->addListener (this);
    deleteSceneTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteSceneTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteSceneTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (sceneLeftTextButton = new TextButton ("Scene Left text button"));
    sceneLeftTextButton->setButtonText (TRANS("Move left"));
    sceneLeftTextButton->setConnectedEdges (Button::ConnectedOnRight);
    sceneLeftTextButton->addListener (this);
    sceneLeftTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sceneLeftTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sceneLeftTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (sceneRightTextButton = new TextButton ("Scene Right text button"));
    sceneRightTextButton->setButtonText (TRANS("Move right"));
    sceneRightTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    sceneRightTextButton->addListener (this);
    sceneRightTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sceneRightTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sceneRightTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (canvasInfoLabel = new Label ("Canvas Info label",
                                                    TRANS("...selected canvas info...")));
    canvasInfoLabel->setFont (Font (15.00f, Font::italic));
    canvasInfoLabel->setJustificationType (Justification::centred);
    canvasInfoLabel->setEditable (false, false, false);
    canvasInfoLabel->setColour (Label::textColourId, Colours::black);
    canvasInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    canvasInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (initialStateGroupComponent = new GroupComponent ("Initial state group component",
                                                                        TRANS("Scene initial state")));
    initialStateGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    initialStateGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (addExciterTextButton = new TextButton ("Add Area text button"));
    addExciterTextButton->setButtonText (TRANS("Add Exciter"));
    addExciterTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addExciterTextButton->addListener (this);
    addExciterTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addExciterTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addExciterTextButton->setColour (TextButton::textColourOffId, Colour (0x66000000));

    addAndMakeVisible (deleteExciterTextButton = new TextButton ("Delete Exciter text button"));
    deleteExciterTextButton->setButtonText (TRANS("Delete"));
    deleteExciterTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deleteExciterTextButton->addListener (this);
    deleteExciterTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteExciterTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteExciterTextButton->setColour (TextButton::textColourOffId, Colour (0x66000000));

    addAndMakeVisible (editInitialStateTextButton = new TextButton ("Edit Initial State text button"));
    editInitialStateTextButton->setButtonText (TRANS("Edit initial scene state"));
    editInitialStateTextButton->addListener (this);
    editInitialStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    editInitialStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    editInitialStateTextButton->setColour (TextButton::textColourOffId, Colour (0x66000000));

    addAndMakeVisible (endEditInitialStateTextButton = new TextButton ("End Edit Initial State text button"));
    endEditInitialStateTextButton->setButtonText (TRANS("Go back to areas edition"));
    endEditInitialStateTextButton->addListener (this);
    endEditInitialStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    endEditInitialStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    endEditInitialStateTextButton->setColour (TextButton::textColourOffId, Colour (0x66000000));

    addAndMakeVisible (sceneNameLabel = new Label ("Scene Name Label",
                                                   TRANS("Name:")));
    sceneNameLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    sceneNameLabel->setJustificationType (Justification::centredLeft);
    sceneNameLabel->setEditable (false, false, false);
    sceneNameLabel->setColour (Label::textColourId, Colours::black);
    sceneNameLabel->setColour (TextEditor::textColourId, Colours::black);
    sceneNameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sceneNameTextEditor = new TextEditor ("Scene Name text editor"));
    sceneNameTextEditor->setMultiLine (false);
    sceneNameTextEditor->setReturnKeyStartsNewLine (false);
    sceneNameTextEditor->setReadOnly (false);
    sceneNameTextEditor->setScrollbarsShown (true);
    sceneNameTextEditor->setCaretVisible (true);
    sceneNameTextEditor->setPopupMenuEnabled (true);
    sceneNameTextEditor->setText (String());


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1024, 1024);


    //[Constructor] You can add your own custom stuff here..

    // Very small hack for this component to because a text editor listener
    sceneNameTextEditor->addListener(this);

    // Then, HERE ARE PRECISED the heights of groups when they are reduced
    canvasGroupReducedH = 0; // nothing inside
    areaGroupReducedH = 80; // first bloc of buttons only
    spatGroupReducedH = 0; // nothing inside

    //[/Constructor]
}

SceneEditionComponent::~SceneEditionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    //[/Destructor_pre]

    areaGroupComponent = nullptr;
    spatGroupComponent = nullptr;
    addPointTextButton = nullptr;
    deletePointTextButton = nullptr;
    copyTextButton = nullptr;
    pasteTextButton = nullptr;
    addAreaTextButton = nullptr;
    deleteAreaTextButton = nullptr;
    labelR = nullptr;
    sliderR = nullptr;
    labelG = nullptr;
    sliderG = nullptr;
    labelB = nullptr;
    sliderB = nullptr;
    sendBackwardTextButton = nullptr;
    bringForwardTextButton = nullptr;
    bringToFrontTextButton = nullptr;
    sendToBackTextButton = nullptr;
    canvasGroupComponent = nullptr;
    spatStatesComboBox = nullptr;
    spatLabel = nullptr;
    addSceneTextButton = nullptr;
    deleteSceneTextButton = nullptr;
    sceneLeftTextButton = nullptr;
    sceneRightTextButton = nullptr;
    canvasInfoLabel = nullptr;
    initialStateGroupComponent = nullptr;
    addExciterTextButton = nullptr;
    deleteExciterTextButton = nullptr;
    editInitialStateTextButton = nullptr;
    endEditInitialStateTextButton = nullptr;
    sceneNameLabel = nullptr;
    sceneNameTextEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SceneEditionComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffd9d9d9));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SceneEditionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    if (multiCanvasComponent)
    {
        Rectangle<int> multiCanvasBounds = this->getLocalBounds();
        multiCanvasBounds.removeFromLeft(areaGroupComponent->getWidth() + 8 + 8);

        multiCanvasComponent->setBounds(multiCanvasBounds);
    }

    //[/UserPreResize]

    areaGroupComponent->setBounds (8, 8 + 168 - -8, 192, 240);
    spatGroupComponent->setBounds (8, (8 + 168 - -8) + 240 - -8, 192, 80);
    addPointTextButton->setBounds (16, (8 + 168 - -8) + 208, 88, 24);
    deletePointTextButton->setBounds (104, (8 + 168 - -8) + 208, 88, 24);
    copyTextButton->setBounds (16, (8 + 168 - -8) + 48, 88, 24);
    pasteTextButton->setBounds (104, (8 + 168 - -8) + 48, 88, 24);
    addAreaTextButton->setBounds (16, (8 + 168 - -8) + 24, 88, 24);
    deleteAreaTextButton->setBounds (104, (8 + 168 - -8) + 24, 88, 24);
    labelR->setBounds (14, (8 + 168 - -8) + 132, 24, 24);
    sliderR->setBounds (32, (8 + 168 - -8) + 136, 158, 16);
    labelG->setBounds (14, (8 + 168 - -8) + 156, 24, 24);
    sliderG->setBounds (32, (8 + 168 - -8) + 160, 158, 16);
    labelB->setBounds (15, (8 + 168 - -8) + 180, 24, 24);
    sliderB->setBounds (32, (8 + 168 - -8) + 184, 158, 16);
    sendBackwardTextButton->setBounds (16, (8 + 168 - -8) + 104, 104, 24);
    bringForwardTextButton->setBounds (16, (8 + 168 - -8) + 80, 104, 24);
    bringToFrontTextButton->setBounds (120, (8 + 168 - -8) + 80, 72, 24);
    sendToBackTextButton->setBounds (120, (8 + 168 - -8) + 104, 72, 24);
    canvasGroupComponent->setBounds (8, 8, 192, 168);
    spatStatesComboBox->setBounds (16, ((8 + 168 - -8) + 240 - -8) + 40, 176, 24);
    spatLabel->setBounds (16, ((8 + 168 - -8) + 240 - -8) + 16, 176, 24);
    addSceneTextButton->setBounds (16, 8 + 24, 88, 24);
    deleteSceneTextButton->setBounds (104, 8 + 24, 88, 24);
    sceneLeftTextButton->setBounds (16, 8 + 72, 88, 24);
    sceneRightTextButton->setBounds (104, 8 + 72, 88, 24);
    canvasInfoLabel->setBounds (16, 8 + 48, 176, 24);
    initialStateGroupComponent->setBounds (8, ((8 + 168 - -8) + 240 - -8) + 80 - -8, 192, 88);
    addExciterTextButton->setBounds (16, (((8 + 168 - -8) + 240 - -8) + 80 - -8) + 24, 88, 24);
    deleteExciterTextButton->setBounds (104, (((8 + 168 - -8) + 240 - -8) + 80 - -8) + 24, 88, 24);
    editInitialStateTextButton->setBounds (16, 8 + 136, 176, 24);
    endEditInitialStateTextButton->setBounds (16, (((8 + 168 - -8) + 240 - -8) + 80 - -8) + 56, 176, 24);
    sceneNameLabel->setBounds (15, 8 + 103, 56, 24);
    sceneNameTextEditor->setBounds (64, 112, 128, 24);
    //[UserResized] Add your own custom resize handling here..

    // Backup of Projucer's sizes
    int canvasGroupInitH = canvasGroupComponent->getHeight();
    int areaGroupInitH = areaGroupComponent->getHeight();
    int spatGroupInitH = spatGroupComponent->getHeight();

    // Variable menus' size
    if (isAreaGroupReduced)
        areaGroupComponent->setSize(areaGroupComponent->getWidth(), areaGroupReducedH);
    // !! Other menus are invisible... And were set invisible when the info came !!

    /* In the end, we may have to translate some groups towards the top of the component,
     * because some groups' height may have change (depending on the running mode).
     */
    if (isCanvasGroupHidden)
    {
        int dY = 0 - canvasGroupInitH - 8; // < 0
        areaGroupTranslateY(dY);
    }
    if (isAreaGroupReduced)
    {
        int dY = areaGroupReducedH - areaGroupInitH - 8; // < 0
        spatGroupTranslateY(dY);
    }
    if (isSpatGroupHidden)
    {
        int dY = 0 - spatGroupInitH /*- 8*/; // < 0
        initialStateGroupTranslateY(dY);
    }

    //[/UserResized]
}

void SceneEditionComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == addPointTextButton)
    {
        //[UserButtonCode_addPointTextButton] -- add your button handler code here..
        graphicSessionManager->OnAddPoint();
        //[/UserButtonCode_addPointTextButton]
    }
    else if (buttonThatWasClicked == deletePointTextButton)
    {
        //[UserButtonCode_deletePointTextButton] -- add your button handler code here..
        graphicSessionManager->OnDeletePoint();
        //[/UserButtonCode_deletePointTextButton]
    }
    else if (buttonThatWasClicked == copyTextButton)
    {
        //[UserButtonCode_copyTextButton] -- add your button handler code here..
        graphicSessionManager->OnCopyArea();
        //[/UserButtonCode_copyTextButton]
    }
    else if (buttonThatWasClicked == pasteTextButton)
    {
        //[UserButtonCode_pasteTextButton] -- add your button handler code here..
        graphicSessionManager->OnPasteArea();
        //[/UserButtonCode_pasteTextButton]
    }
    else if (buttonThatWasClicked == addAreaTextButton)
    {
        //[UserButtonCode_addAreaTextButton] -- add your button handler code here..
        // Pop-up menu to get the kind of area to add
        PopupMenu menu;
        menu.addItem (AreaDefaultType::Ellipse, "Ellipse", false); // disabled
        menu.addSeparator();
        menu.addItem(AreaDefaultType::Polygon, "Triangle");
        menu.addItem(AreaDefaultType::Polygon+1, "Square");
        menu.addItem(AreaDefaultType::Polygon+2, "Pentagon");
        menu.addItem(AreaDefaultType::Polygon+3, "Hexagon");
        menu.addItem(AreaDefaultType::Polygon+4, "Heptagon");
        menu.addItem(AreaDefaultType::Polygon+5, "Octogon");
        const int userChoice = menu.show();
        if (userChoice != 0) // (0 : case "user didn't chose something")
        {
            // Actual addition then
            graphicSessionManager->OnAddArea(userChoice);
        }
        //[/UserButtonCode_addAreaTextButton]
    }
    else if (buttonThatWasClicked == deleteAreaTextButton)
    {
        //[UserButtonCode_deleteAreaTextButton] -- add your button handler code here..
        graphicSessionManager->OnDeleteArea();
        //[/UserButtonCode_deleteAreaTextButton]
    }
    else if (buttonThatWasClicked == sendBackwardTextButton)
    {
        //[UserButtonCode_sendBackwardTextButton] -- add your button handler code here..
        graphicSessionManager->OnSendBackward();
        //[/UserButtonCode_sendBackwardTextButton]
    }
    else if (buttonThatWasClicked == bringForwardTextButton)
    {
        //[UserButtonCode_bringForwardTextButton] -- add your button handler code here..
        graphicSessionManager->OnBringForward();
        //[/UserButtonCode_bringForwardTextButton]
    }
    else if (buttonThatWasClicked == bringToFrontTextButton)
    {
        //[UserButtonCode_bringToFrontTextButton] -- add your button handler code here..
        graphicSessionManager->OnBringToFront();
        //[/UserButtonCode_bringToFrontTextButton]
    }
    else if (buttonThatWasClicked == sendToBackTextButton)
    {
        //[UserButtonCode_sendToBackTextButton] -- add your button handler code here..
        graphicSessionManager->OnSendToBack();
        //[/UserButtonCode_sendToBackTextButton]
    }
    else if (buttonThatWasClicked == addSceneTextButton)
    {
        //[UserButtonCode_addSceneTextButton] -- add your button handler code here..
        graphicSessionManager->OnAddScene();
        //[/UserButtonCode_addSceneTextButton]
    }
    else if (buttonThatWasClicked == deleteSceneTextButton)
    {
        //[UserButtonCode_deleteSceneTextButton] -- add your button handler code here..
        graphicSessionManager->OnDeleteScene();
        //[/UserButtonCode_deleteSceneTextButton]
    }
    else if (buttonThatWasClicked == sceneLeftTextButton)
    {
        //[UserButtonCode_sceneLeftTextButton] -- add your button handler code here..
        graphicSessionManager->OnSceneLeft();
        //[/UserButtonCode_sceneLeftTextButton]
    }
    else if (buttonThatWasClicked == sceneRightTextButton)
    {
        //[UserButtonCode_sceneRightTextButton] -- add your button handler code here..
        graphicSessionManager->OnSceneRight();
        //[/UserButtonCode_sceneRightTextButton]
    }
    else if (buttonThatWasClicked == addExciterTextButton)
    {
        //[UserButtonCode_addExciterTextButton] -- add your button handler code here..
        //[/UserButtonCode_addExciterTextButton]
    }
    else if (buttonThatWasClicked == deleteExciterTextButton)
    {
        //[UserButtonCode_deleteExciterTextButton] -- add your button handler code here..
        //[/UserButtonCode_deleteExciterTextButton]
    }
    else if (buttonThatWasClicked == editInitialStateTextButton)
    {
        //[UserButtonCode_editInitialStateTextButton] -- add your button handler code here..
        //[/UserButtonCode_editInitialStateTextButton]
    }
    else if (buttonThatWasClicked == endEditInitialStateTextButton)
    {
        //[UserButtonCode_endEditInitialStateTextButton] -- add your button handler code here..
        //[/UserButtonCode_endEditInitialStateTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SceneEditionComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderR)
    {
        //[UserSliderCode_sliderR] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderR]
    }
    else if (sliderThatWasMoved == sliderG)
    {
        //[UserSliderCode_sliderG] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderG]
    }
    else if (sliderThatWasMoved == sliderB)
    {
        //[UserSliderCode_sliderB] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderB]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SceneEditionComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == spatStatesComboBox)
    {
        //[UserComboBoxCode_spatStatesComboBox] -- add your combo box handling code here..
        graphicSessionManager->OnSpatStateChanged(spatStatesComboBox->getSelectedItemIndex());
        //[/UserComboBoxCode_spatStatesComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


void SceneEditionComponent::textEditorTextChanged(TextEditor& editorThatHasChanged)
{
    if (&editorThatHasChanged == sceneNameTextEditor.get())
        graphicSessionManager->OnSceneNameChange(sceneNameTextEditor->getText().toStdString());
}




// CONSTRUCTION HELPERS
void SceneEditionComponent::CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponent* _multiCanvasComponent)
{
    graphicSessionManager = _graphicSessionManager;

    multiCanvasComponent = _multiCanvasComponent;
    addAndMakeVisible(multiCanvasComponent);

    multiCanvasComponent->CompleteInitialization();
}







// ========== (SETTERS AND GETTERS) ORDERS FROM PRESENTER ==========

// - - - - - Common to all UI elements - - - - -

void SceneEditionComponent::SetEnabledAllControls(bool areEnabled, bool controlsBackUp)
{
    // Canvas (nothing yet : controls are visible or not, but always enabled)

    // Main area controls
    addAreaTextButton->setEnabled(areEnabled);
    deleteAreaTextButton->setEnabled(areEnabled);
    copyTextButton->setEnabled(areEnabled);

    // Special buttons with a backup of their state (may not be enabled !)
    if (controlsBackUp)
        pasteTextButton->setEnabled(pasteTextButtonEnabledBackUp);
    else
        pasteTextButton->setEnabled(areEnabled);

    setEnabledSelectedAreaControls(areEnabled); // Selected Area

    // Spat effect
}
void SceneEditionComponent::CloseTemporaryDisplayedObjects()
{
    spatStatesComboBox->hidePopup();
}

// - - - - - Canvases & canvas group - - - - -

void SceneEditionComponent::SetCanvasGroupHidden(bool _isHidden)
{
    canvasGroupComponent->setVisible(!_isHidden);
    canvasInfoLabel->setVisible(!_isHidden);
    addSceneTextButton->setVisible(!_isHidden);
    deleteSceneTextButton->setVisible(!_isHidden);
    sceneLeftTextButton->setVisible(!_isHidden);
    sceneRightTextButton->setVisible(!_isHidden);
    sceneNameLabel->setVisible(!_isHidden);
    sceneNameTextEditor->setVisible(!_isHidden);
    editInitialStateTextButton->setVisible(!_isHidden);

    isCanvasGroupHidden = _isHidden;
}
void SceneEditionComponent::SetCanvasGroupReduced(bool _isReduced)
{
    SetCanvasGroupHidden(_isReduced);
    // isCanvasGroupReduced = true;
}

void SceneEditionComponent::SetDeleteSceneButtonEnabled(bool isEnabled)
{
    deleteSceneTextButton->setEnabled(isEnabled);
}

// - - - - - Area group - - - - -

// We know that the "selection of an area" state is equal to the _idReduced....
void SceneEditionComponent::SetAreaGroupReduced(bool _isReduced)
{
    isAreaGroupReduced = _isReduced;

    SetVisibleAreaEditionControls(!_isReduced);

    // Buttons (de)activated on area (de)selection
    deleteAreaTextButton->setEnabled(!_isReduced);
    copyTextButton->setEnabled(!_isReduced);
}
void SceneEditionComponent::SetEnabledAreaEditionControls(bool areEnabled)
{
    SetEnabledAreaMainControls(areEnabled);
    setEnabledSelectedAreaControls(areEnabled);
}
void SceneEditionComponent::SetEnabledAreaMainControls(bool areEnabled)
{
    addAreaTextButton->setEnabled(areEnabled);
    deleteAreaTextButton->setEnabled(areEnabled);
    copyTextButton->setEnabled(areEnabled);
    pasteTextButton->setEnabled(areEnabled);
}
void SceneEditionComponent::SetVisibleAreaEditionControls(bool areVisible)
{
    // For a selected area
    bringForwardTextButton->setVisible(areVisible);
    bringToFrontTextButton->setVisible(areVisible);
    sendBackwardTextButton->setVisible(areVisible);
    sendToBackTextButton->setVisible(areVisible);
    labelR->setVisible(areVisible);
    labelG->setVisible(areVisible);
    labelB->setVisible(areVisible);
    sliderR->setVisible(areVisible);
    sliderG->setVisible(areVisible);
    sliderB->setVisible(areVisible);

    // Polygon
    addPointTextButton->setVisible(areVisible);
    deletePointTextButton->setVisible(areVisible);
}
void SceneEditionComponent::DisableAllButtonsBut(juce::String onlyActiveButtonName)
{
    // Backup of some controls' state
    pasteTextButtonEnabledBackUp = pasteTextButton->isEnabled();
    SetEnabledAllControls(false, true);

    if (onlyActiveButtonName == "Add Point text button")
        addPointTextButton->setEnabled(true);
    else if (onlyActiveButtonName == "Delete Point text button")
        deletePointTextButton->setEnabled(true);
}
void SceneEditionComponent::SetPasteEnabled(bool _isEnabled)
{
    pasteTextButton->setEnabled(_isEnabled);
}
void SceneEditionComponent::setEnabledSelectedAreaControls(bool areEnabled)
{
    // For a selected area
    bringForwardTextButton->setEnabled(areEnabled);
    bringToFrontTextButton->setEnabled(areEnabled);
    sendBackwardTextButton->setEnabled(areEnabled);
    sendToBackTextButton->setEnabled(areEnabled);
    labelR->setEnabled(areEnabled);
    labelG->setEnabled(areEnabled);
    labelB->setEnabled(areEnabled);
    sliderR->setEnabled(areEnabled);
    sliderG->setEnabled(areEnabled);
    sliderB->setEnabled(areEnabled);

    // Polygon
    addPointTextButton->setEnabled(areEnabled);
    deletePointTextButton->setEnabled(areEnabled);
}
void SceneEditionComponent::setVisibleSpatControls(bool areVisible)
{
    spatGroupComponent->setVisible(areVisible);
    if (spatStatesComboBox->isPopupActive())
        spatStatesComboBox->hidePopup();
    spatStatesComboBox->setVisible(areVisible);
    spatLabel->setVisible(areVisible);
    spatStatesComboBox->setVisible(areVisible);
}


// - - - - - Spat group - - - - -
void SceneEditionComponent::SetSpatGroupHidden(bool _isHidden)
{
    setVisibleSpatControls(!_isHidden);
    isSpatGroupHidden = _isHidden;
}
void SceneEditionComponent::SetSpatGroupReduced(bool _isReduced)
{
    SetSpatGroupHidden(_isReduced);
    // isSpatGroupReduced = _isReduced;
}
void SceneEditionComponent::UpdateStatesList(std::vector< std::shared_ptr<SpatState<double>> > &newSpatStates)
{
    // Empties the combo box at first
    spatStatesComboBox->clear();

    // Addition of items==names one by one
    for (size_t i=0 ; i<newSpatStates.size() ; i++)
        spatStatesComboBox->addItem(newSpatStates[i]->GetName(), i+1); // Id==Idx+1
}
void SceneEditionComponent::SelectSpatState(int index, NotificationType notificationType)
{
    spatStatesComboBox->setSelectedItemIndex(index, notificationType);
}


// - - - - - Initial Scene State group - - - - -
void SceneEditionComponent::SetInitialStateGroupHidden(bool _isHidden)
{
    initialStateGroupComponent->setVisible(!_isHidden);
    addExciterTextButton->setVisible(!_isHidden);
    deleteExciterTextButton->setVisible(!_isHidden);
    endEditInitialStateTextButton->setVisible(!_isHidden);
}
void SceneEditionComponent::SetInitialStateGroupReduced(bool _isReduced)
{
    SetInitialStateGroupHidden(_isReduced);
}





// ----- Other setters and getters -----

// - - - - - Colours - - - - -
void SceneEditionComponent::SetAreaColourValue(juce::Colour colour)
{
    sliderR->setValue(colour.getRed());
    sliderG->setValue(colour.getGreen());
    sliderB->setValue(colour.getBlue());
}

// - - - - - Text Values - - - - -

void SceneEditionComponent::SetSceneName(std::string _name)
{
    // Send text without update messages sent to listeners
    sceneNameTextEditor->setText(_name, false);
}

void SceneEditionComponent::SetCanvasInfo(SceneCanvasComponent::Id _id)
{
    canvasInfoLabel->setText("on canvas " + static_cast<String>(_id+1),
                             NotificationType::dontSendNotification);
}







// ----- Helpers -----

void SceneEditionComponent::colourSliderMoved()
{
    Colour newColour = Colour((uint8)sliderR->getValue(),
		(uint8)sliderG->getValue(),
		(uint8)sliderB->getValue());
    graphicSessionManager->OnNewColour(newColour);
}



// ----- Geometric transformation of controls -----
void SceneEditionComponent::areaGroupTranslateY(int dY)
{
    componentTranslateY(areaGroupComponent.get(), dY);
    componentTranslateY(addPointTextButton.get(), dY);
    componentTranslateY(deletePointTextButton.get(), dY);
    componentTranslateY(copyTextButton.get(), dY);
    componentTranslateY(pasteTextButton.get(), dY);
    componentTranslateY(addAreaTextButton.get(), dY);
    componentTranslateY(deleteAreaTextButton.get(), dY);
    componentTranslateY(labelR.get(), dY);
    componentTranslateY(labelG.get(), dY);
    componentTranslateY(labelB.get(), dY);
    componentTranslateY(sliderR.get(), dY);
    componentTranslateY(sliderG.get(), dY);
    componentTranslateY(sliderB.get(), dY);
    componentTranslateY(sendBackwardTextButton.get(), dY);
    componentTranslateY(sendToBackTextButton.get(), dY);
    componentTranslateY(bringForwardTextButton.get(), dY);
    componentTranslateY(bringToFrontTextButton.get(), dY);

    spatGroupTranslateY(dY);
}
void SceneEditionComponent::spatGroupTranslateY(int dY)
{
    componentTranslateY(spatGroupComponent.get(), dY);
    componentTranslateY(spatLabel.get(), dY);
    componentTranslateY(spatStatesComboBox.get(), dY);

    initialStateGroupTranslateY(dY);
}
void SceneEditionComponent::initialStateGroupTranslateY(int dY)
{
    componentTranslateY(initialStateGroupComponent, dY);
    componentTranslateY(addExciterTextButton, dY);
    componentTranslateY(deleteExciterTextButton, dY);
    componentTranslateY(endEditInitialStateTextButton, dY);
}
void SceneEditionComponent::componentTranslateY(Component* component, int dY)
{
    component->setTopLeftPosition(component->getX(), component->getY() + dY);
}



//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SceneEditionComponent" componentName=""
                 parentClasses="public Component, public TextEditorListener" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="1024" initialHeight="1024">
  <BACKGROUND backgroundColour="ffd9d9d9"/>
  <GROUPCOMPONENT name="Area edition group component" id="87d416270d41f58c" memberName="areaGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 240" posRelativeY="4250d5155a80be70"
                  outlinecol="ff454545" textcol="ff000000" title="Area edition"/>
  <GROUPCOMPONENT name="Spatialization group component" id="90b16e3024c520fd" memberName="spatGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 80" posRelativeY="87d416270d41f58c"
                  outlinecol="ff454545" textcol="ff000000" title="Spatialization effect"/>
  <TEXTBUTTON name="Add Point text button" id="71769222a7765795" memberName="addPointTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 208 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Add Point"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point text button" id="2bdfdc3999886d02" memberName="deletePointTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 208 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Delete Point"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Copy Text Button" id="33b309fbb149d4ee" memberName="copyTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 48 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Copy"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point" id="653c7d46ffba1120" memberName="pasteTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 48 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Paste"
              connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Add Area text button" id="d377c545746b9e8d" memberName="addAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 24 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Add Area"
              connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Area text button" id="3c0e2321516c0809" memberName="deleteAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 24 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Delete Area"
              connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="893f3de478724dd" memberName="labelR" virtualName=""
         explicitFocusOrder="0" pos="14 132 24 24" posRelativeY="87d416270d41f58c"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="R"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <SLIDER name="Slider R" id="c86e71b3772d49e9" memberName="sliderR" virtualName=""
          explicitFocusOrder="0" pos="32 136 158 16" posRelativeY="87d416270d41f58c"
          thumbcol="a0ff0000" textboxtext="ff000000" min="0" max="255"
          int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="d31faee34d487318" memberName="labelG" virtualName=""
         explicitFocusOrder="0" pos="14 156 24 24" posRelativeY="87d416270d41f58c"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="G"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <SLIDER name="SliderG" id="8455ecc13f47ec08" memberName="sliderG" virtualName=""
          explicitFocusOrder="0" pos="32 160 158 16" posRelativeY="87d416270d41f58c"
          thumbcol="a000ff00" textboxtext="ff000000" min="0" max="255"
          int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="ae5abc145d9184cf" memberName="labelB" virtualName=""
         explicitFocusOrder="0" pos="15 180 24 24" posRelativeY="87d416270d41f58c"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="B"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <SLIDER name="Slider B" id="15aa4b94476e3563" memberName="sliderB" virtualName=""
          explicitFocusOrder="0" pos="32 184 158 16" posRelativeY="87d416270d41f58c"
          thumbcol="a00000ff" textboxtext="ff000000" min="0" max="255"
          int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTBUTTON name="Send backward text button" id="a356ce3575b4d98b" memberName="sendBackwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 104 104 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Send backward"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring forward text button" id="d626373b1db0a49" memberName="bringForwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 80 104 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Bring forward"
              connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring to front text button" id="43d8b27a93068fcf" memberName="bringToFrontTextButton"
              virtualName="" explicitFocusOrder="0" pos="120 80 72 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="to front"
              connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Send to back text button" id="1fe791d6bf4489ca" memberName="sendToBackTextButton"
              virtualName="" explicitFocusOrder="0" pos="120 104 72 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="to back"
              connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="Canvas edition group component" id="4250d5155a80be70" memberName="canvasGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 8 192 168" outlinecol="ff454545"
                  textcol="ff000000" title="Scene edition"/>
  <COMBOBOX name="Spat States combo box" id="89ad7c0a3be5a39c" memberName="spatStatesComboBox"
            virtualName="" explicitFocusOrder="0" pos="16 40 176 24" posRelativeY="90b16e3024c520fd"
            editable="0" layout="33" items="-1 undefined" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="Spat label" id="b1f047be2f31dc5" memberName="spatLabel"
         virtualName="" explicitFocusOrder="0" pos="16 16 176 24" posRelativeY="90b16e3024c520fd"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Link to spat state:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <TEXTBUTTON name="Add Scene text button" id="47bebc9d3a03780d" memberName="addSceneTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 24 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ff000000" textCol="ff000000" buttonText="Add Scene"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Scene text button" id="5f4e8653b868a323" memberName="deleteSceneTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 24 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Delete Scene"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Scene Left text button" id="43b96ebd16bb5586" memberName="sceneLeftTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 72 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Move left"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Scene Right text button" id="e6cf4b99a12776ee" memberName="sceneRightTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 72 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" textCol="ff000000" buttonText="Move right"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Canvas Info label" id="3577c0e2ccd44371" memberName="canvasInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="16 48 176 24" posRelativeY="4250d5155a80be70"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="...selected canvas info..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="1"
         justification="36" typefaceStyle="Italic"/>
  <GROUPCOMPONENT name="Initial state group component" id="cc3bdf8d18c3f428" memberName="initialStateGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 88" posRelativeY="90b16e3024c520fd"
                  outlinecol="ff454545" textcol="ff000000" title="Scene initial state"/>
  <TEXTBUTTON name="Add Area text button" id="b6820308eb03f341" memberName="addExciterTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 24 88 24" posRelativeY="cc3bdf8d18c3f428"
              bgColOff="33000000" bgColOn="ffffffff" textCol="66000000" buttonText="Add Exciter"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Exciter text button" id="11692d0648a2e8a4" memberName="deleteExciterTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 24 88 24" posRelativeY="cc3bdf8d18c3f428"
              bgColOff="33000000" bgColOn="ffffffff" textCol="66000000" buttonText="Delete"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Edit Initial State text button" id="d8f32d6e3ea6e87a" memberName="editInitialStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 136 176 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" textCol="66000000" buttonText="Edit initial scene state"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="End Edit Initial State text button" id="3fe924c475527418"
              memberName="endEditInitialStateTextButton" virtualName="" explicitFocusOrder="0"
              pos="16 56 176 24" posRelativeY="cc3bdf8d18c3f428" bgColOff="33000000"
              bgColOn="ffffffff" textCol="66000000" buttonText="Go back to areas edition"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Scene Name Label" id="fbdd06d6ea5f9471" memberName="sceneNameLabel"
         virtualName="" explicitFocusOrder="0" pos="15 103 56 24" posRelativeY="4250d5155a80be70"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Name:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <TEXTEDITOR name="Scene Name text editor" id="fd7eace3e677fc36" memberName="sceneNameTextEditor"
              virtualName="" explicitFocusOrder="0" pos="64 112 128 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
