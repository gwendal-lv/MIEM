/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SceneEditionManager.h"

#include <cmath>
//[/Headers]

#include "SceneEditionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
SceneEditionComponent::SceneEditionComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..

    // Manual canvas add
    for (int i=0 ; i<SceneCanvasComponent::SceneCanvasesCount ; i++)
    {
        sceneCanvasComponents.push_back(new SceneCanvasComponent((SceneCanvasComponent::Id)i));
        addAndMakeVisible(sceneCanvasComponents.back());
    }

    //[/Constructor_pre]

    addAndMakeVisible (areaGroupComponent = new GroupComponent ("Area edition group component",
                                                                TRANS("Area edition")));

    addAndMakeVisible (spatGroupComponent = new GroupComponent ("Spatialization group component",
                                                                TRANS("Spatialization effect")));

    addAndMakeVisible (addPointTextButton = new TextButton ("Add Point text button"));
    addPointTextButton->setButtonText (TRANS("Add Point"));
    addPointTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addPointTextButton->addListener (this);
    addPointTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addPointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (deletePointTextButton = new TextButton ("Delete Point text button"));
    deletePointTextButton->setButtonText (TRANS("Delete Point"));
    deletePointTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deletePointTextButton->addListener (this);
    deletePointTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deletePointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (copyTextButton = new TextButton ("Copy Text Button"));
    copyTextButton->setButtonText (TRANS("Copy"));
    copyTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    copyTextButton->addListener (this);
    copyTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    copyTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (pasteTextButton = new TextButton ("Delete Point"));
    pasteTextButton->setButtonText (TRANS("Paste"));
    pasteTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    pasteTextButton->addListener (this);
    pasteTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    pasteTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (addAreaTextButton = new TextButton ("Add Area text button"));
    addAreaTextButton->setButtonText (TRANS("Add Area"));
    addAreaTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    addAreaTextButton->addListener (this);
    addAreaTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (deleteAreaTextButton = new TextButton ("Delete Area text button"));
    deleteAreaTextButton->setButtonText (TRANS("Delete Area"));
    deleteAreaTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    deleteAreaTextButton->addListener (this);
    deleteAreaTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (labelR = new Label ("new label",
                                           TRANS("R")));
    labelR->setFont (Font (15.00f, Font::plain));
    labelR->setJustificationType (Justification::centredLeft);
    labelR->setEditable (false, false, false);
    labelR->setColour (TextEditor::textColourId, Colours::black);
    labelR->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderR = new Slider ("Slider R"));
    sliderR->setRange (0, 255, 1);
    sliderR->setSliderStyle (Slider::LinearHorizontal);
    sliderR->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderR->setColour (Slider::thumbColourId, Colour (0xffbbbbbb));
    sliderR->addListener (this);

    addAndMakeVisible (labelG = new Label ("new label",
                                           TRANS("G")));
    labelG->setFont (Font (15.00f, Font::plain));
    labelG->setJustificationType (Justification::centredLeft);
    labelG->setEditable (false, false, false);
    labelG->setColour (TextEditor::textColourId, Colours::black);
    labelG->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderG = new Slider ("SliderG"));
    sliderG->setRange (0, 255, 1);
    sliderG->setSliderStyle (Slider::LinearHorizontal);
    sliderG->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderG->setColour (Slider::thumbColourId, Colour (0xffbbbbbb));
    sliderG->addListener (this);

    addAndMakeVisible (labelB = new Label ("new label",
                                           TRANS("B")));
    labelB->setFont (Font (15.00f, Font::plain));
    labelB->setJustificationType (Justification::centredLeft);
    labelB->setEditable (false, false, false);
    labelB->setColour (TextEditor::textColourId, Colours::black);
    labelB->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sliderB = new Slider ("Slider B"));
    sliderB->setRange (0, 255, 1);
    sliderB->setSliderStyle (Slider::LinearHorizontal);
    sliderB->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderB->setColour (Slider::thumbColourId, Colour (0xffbbbbbb));
    sliderB->addListener (this);

    addAndMakeVisible (sendBackwardTextButton = new TextButton ("Send backward text button"));
    sendBackwardTextButton->setButtonText (TRANS("Send backward"));
    sendBackwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    sendBackwardTextButton->addListener (this);
    sendBackwardTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sendBackwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (bringForwardTextButton = new TextButton ("Bring forward text button"));
    bringForwardTextButton->setButtonText (TRANS("Bring forward"));
    bringForwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    bringForwardTextButton->addListener (this);
    bringForwardTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    bringForwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (bringToFrontTextButton = new TextButton ("Bring to front text button"));
    bringToFrontTextButton->setButtonText (TRANS("to front"));
    bringToFrontTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    bringToFrontTextButton->addListener (this);
    bringToFrontTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    bringToFrontTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (sendToBackTextButton = new TextButton ("Send to back text button"));
    sendToBackTextButton->setButtonText (TRANS("to back"));
    sendToBackTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    sendToBackTextButton->addListener (this);
    sendToBackTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sendToBackTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (canvasGroupComponent = new GroupComponent ("Canvas edition group component",
                                                                  TRANS("Scenes edition")));

    addAndMakeVisible (speakersGroupComboBox = new ComboBox ("Speakers group combo box"));
    speakersGroupComboBox->setEditableText (false);
    speakersGroupComboBox->setJustificationType (Justification::centredLeft);
    speakersGroupComboBox->setTextWhenNothingSelected (String());
    speakersGroupComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    speakersGroupComboBox->addItem (TRANS("-1 undefined"), 1);
    speakersGroupComboBox->addListener (this);

    addAndMakeVisible (spatLabel = new Label ("Spat label",
                                              TRANS("Link to speakers group :")));
    spatLabel->setFont (Font (15.00f, Font::plain));
    spatLabel->setJustificationType (Justification::centredLeft);
    spatLabel->setEditable (false, false, false);
    spatLabel->setColour (TextEditor::textColourId, Colours::black);
    spatLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (addSceneTextButton = new TextButton ("Add Scene text button"));
    addSceneTextButton->setButtonText (TRANS("Add Scene"));
    addSceneTextButton->setConnectedEdges (Button::ConnectedOnBottom);
    addSceneTextButton->addListener (this);
    addSceneTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addSceneTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (deleteSceneTextButton = new TextButton ("Delete Scene text button"));
    deleteSceneTextButton->setButtonText (TRANS("Delete Scene"));
    deleteSceneTextButton->setConnectedEdges (Button::ConnectedOnTop);
    deleteSceneTextButton->addListener (this);
    deleteSceneTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteSceneTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (sceneUpTextButton = new TextButton ("Scene Up text button"));
    sceneUpTextButton->setButtonText (CharPointer_UTF8 ("\xe2\x86\x91 Up"));
    sceneUpTextButton->setConnectedEdges (Button::ConnectedOnBottom);
    sceneUpTextButton->addListener (this);
    sceneUpTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sceneUpTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (sceneDownTextButton = new TextButton ("Scene Down text button"));
    sceneDownTextButton->setButtonText (CharPointer_UTF8 ("\xe2\x86\x93 Down"));
    sceneDownTextButton->setConnectedEdges (Button::ConnectedOnTop);
    sceneDownTextButton->addListener (this);
    sceneDownTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    sceneDownTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (canvasInfoLabel = new Label ("Canvas Info label",
                                                    TRANS("...selected canvas info...")));
    canvasInfoLabel->setFont (Font (15.00f, Font::italic));
    canvasInfoLabel->setJustificationType (Justification::centredLeft);
    canvasInfoLabel->setEditable (false, false, false);
    canvasInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    canvasInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sceneNameTextEditor = new TextEditor ("new text editor"));
    sceneNameTextEditor->setMultiLine (false);
    sceneNameTextEditor->setReturnKeyStartsNewLine (false);
    sceneNameTextEditor->setReadOnly (false);
    sceneNameTextEditor->setScrollbarsShown (true);
    sceneNameTextEditor->setCaretVisible (true);
    sceneNameTextEditor->setPopupMenuEnabled (true);
    sceneNameTextEditor->setText (TRANS("...name of scene..."));

    addAndMakeVisible (initialStateGroupComponent = new GroupComponent ("Initial state group component",
                                                                        TRANS("Scene initial state")));

    addAndMakeVisible (addExciterTextButton = new TextButton ("Add Area text button"));
    addExciterTextButton->setButtonText (TRANS("Add Exciter"));
    addExciterTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addExciterTextButton->addListener (this);
    addExciterTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addExciterTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (deleteAreaTextButton2 = new TextButton ("Delete Area text button"));
    deleteAreaTextButton2->setButtonText (TRANS("Delete"));
    deleteAreaTextButton2->setConnectedEdges (Button::ConnectedOnLeft);
    deleteAreaTextButton2->addListener (this);
    deleteAreaTextButton2->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteAreaTextButton2->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (editInitialStateTextButton = new TextButton ("Edit Initial State text button"));
    editInitialStateTextButton->setButtonText (TRANS("Edit initial scene state"));
    editInitialStateTextButton->addListener (this);
    editInitialStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    editInitialStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (endEditInitialStateTextButton = new TextButton ("End Edit Initial State text button"));
    endEditInitialStateTextButton->setButtonText (TRANS("Go back to areas edition"));
    endEditInitialStateTextButton->addListener (this);
    endEditInitialStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    endEditInitialStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1024, 1024);


    //[Constructor] You can add your own custom stuff here..

    /* Manual initial resize --- Useless !!!
     * Because the window was far too high, to able to all menus (knowing that all menu
     * elements will never be displayed all at the same time, that's okay)
     */

    // Then, HERE ARE PRECISED the heights of groups when they are reduced
    canvasGroupReducedH = 0; // nothing inside
    areaGroupReducedH = 80; // first bloc of buttons only
    spatGroupReducedH = 0; // nothing inside

    //[/Constructor]
}

SceneEditionComponent::~SceneEditionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    for (size_t i=0 ; i < sceneCanvasComponents.size() ; i++)
        delete sceneCanvasComponents[i];
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
    speakersGroupComboBox = nullptr;
    spatLabel = nullptr;
    addSceneTextButton = nullptr;
    deleteSceneTextButton = nullptr;
    sceneUpTextButton = nullptr;
    sceneDownTextButton = nullptr;
    canvasInfoLabel = nullptr;
    sceneNameTextEditor = nullptr;
    initialStateGroupComponent = nullptr;
    addExciterTextButton = nullptr;
    deleteAreaTextButton2 = nullptr;
    editInitialStateTextButton = nullptr;
    endEditInitialStateTextButton = nullptr;


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

    // Canvas bounds  !!!!!!!!!!!!!!!!!!!!!!!!!!!!! À MODIFIER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    float pourcentageLargeurMainScene = 0.7f;

    sceneCanvasComponents[SceneCanvasComponent::MainScene]->setBounds(
        208, // next to the left menu
        8, // always the same Y coordinate
        juce::roundFloatToInt((getWidth() - areaGroupComponent->getWidth() - 24)*pourcentageLargeurMainScene) - 8,
        getHeight()-16); // always the same Y coordinate

    sceneCanvasComponents[SceneCanvasComponent::FixedScene]->setBounds(
         sceneCanvasComponents[SceneCanvasComponent::MainScene]->getX() // on the right of
         + sceneCanvasComponents[SceneCanvasComponent::MainScene]->getWidth()+8, //MainScene
         8, // always the same Y coordinate
         juce::roundFloatToInt((getWidth()-areaGroupComponent->getWidth()-24)*(1.0f-pourcentageLargeurMainScene))-8,
         getHeight()-16); // always the same Y coordinate

    //[/UserPreResize]

    areaGroupComponent->setBounds (8, 8 + 168 - -8, 192, 248);
    spatGroupComponent->setBounds (8, (8 + 168 - -8) + 248 - -8, 192, 80);
    addPointTextButton->setBounds (16, (8 + 168 - -8) + 216, 88, 24);
    deletePointTextButton->setBounds (104, (8 + 168 - -8) + 216, 88, 24);
    copyTextButton->setBounds (104, (8 + 168 - -8) + 24, 88, 24);
    pasteTextButton->setBounds (104, (8 + 168 - -8) + 48, 88, 24);
    addAreaTextButton->setBounds (16, (8 + 168 - -8) + 24, 88, 24);
    deleteAreaTextButton->setBounds (16, (8 + 168 - -8) + 48, 88, 24);
    labelR->setBounds (16, (8 + 168 - -8) + 136, 24, 24);
    sliderR->setBounds (40, (8 + 168 - -8) + 136, 150, 24);
    labelG->setBounds (16, (8 + 168 - -8) + 160, 24, 24);
    sliderG->setBounds (40, (8 + 168 - -8) + 160, 150, 24);
    labelB->setBounds (16, (8 + 168 - -8) + 184, 24, 24);
    sliderB->setBounds (40, (8 + 168 - -8) + 184, 150, 24);
    sendBackwardTextButton->setBounds (16, (8 + 168 - -8) + 104, 104, 24);
    bringForwardTextButton->setBounds (16, (8 + 168 - -8) + 80, 104, 24);
    bringToFrontTextButton->setBounds (120, (8 + 168 - -8) + 80, 72, 24);
    sendToBackTextButton->setBounds (120, (8 + 168 - -8) + 104, 72, 24);
    canvasGroupComponent->setBounds (8, 8, 192, 168);
    speakersGroupComboBox->setBounds (16, ((8 + 168 - -8) + 248 - -8) + 40, 176, 24);
    spatLabel->setBounds (16, ((8 + 168 - -8) + 248 - -8) + 16, 176, 24);
    addSceneTextButton->setBounds (16, 8 + 48, 104, 24);
    deleteSceneTextButton->setBounds (16, 8 + 72, 104, 24);
    sceneUpTextButton->setBounds (128, 8 + 48, 64, 24);
    sceneDownTextButton->setBounds (128, 8 + 72, 64, 24);
    canvasInfoLabel->setBounds (16, 8 + 16, 176, 24);
    sceneNameTextEditor->setBounds (16, 112, 176, 24);
    initialStateGroupComponent->setBounds (8, ((8 + 168 - -8) + 248 - -8) + 80 - -8, 192, 88);
    addExciterTextButton->setBounds (16, (8 + 168 - -8) + 368, 88, 24);
    deleteAreaTextButton2->setBounds (104, (((8 + 168 - -8) + 248 - -8) + 80 - -8) + 24, 88, 24);
    editInitialStateTextButton->setBounds (16, 8 + 136, 176, 24);
    endEditInitialStateTextButton->setBounds (16, 8 + 576, 176, 24);
    //[UserResized] Add your own custom resize handling here..

    // Backup of Projucer's sizes
    int canvasGroupInitH = canvasGroupComponent->getHeight();
    int areaGroupInitH = areaGroupComponent->getHeight();

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
        spatGroupTranslateY(dY);
    }
    /*else if (isCanvasGroupReduced)
    {
        int dY = canvasGroupReducedH - canvasGroupInitH - 8; // < 0
        areaGroupTranslateY(dY);
        spatGroupTranslateY(dY);
    }
    if (isAreaGroupHidden)
    {
        int dY = 0 - areaGroupInitH - 8; // < 0
        spatGroupTranslateY(dY);
    }
    else*/ if (isAreaGroupReduced)
    {
        int dY = areaGroupReducedH - areaGroupInitH - 8; // < 0
        spatGroupTranslateY(dY);
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
        sceneEditionManager->OnAddPoint();
        //[/UserButtonCode_addPointTextButton]
    }
    else if (buttonThatWasClicked == deletePointTextButton)
    {
        //[UserButtonCode_deletePointTextButton] -- add your button handler code here..
        sceneEditionManager->OnDeletePoint();
        //[/UserButtonCode_deletePointTextButton]
    }
    else if (buttonThatWasClicked == copyTextButton)
    {
        //[UserButtonCode_copyTextButton] -- add your button handler code here..
        sceneEditionManager->OnCopyArea();
        //[/UserButtonCode_copyTextButton]
    }
    else if (buttonThatWasClicked == pasteTextButton)
    {
        //[UserButtonCode_pasteTextButton] -- add your button handler code here..
        sceneEditionManager->OnPasteArea();
        //[/UserButtonCode_pasteTextButton]
    }
    else if (buttonThatWasClicked == addAreaTextButton)
    {
        //[UserButtonCode_addAreaTextButton] -- add your button handler code here..
        sceneEditionManager->OnAddArea();
        //[/UserButtonCode_addAreaTextButton]
    }
    else if (buttonThatWasClicked == deleteAreaTextButton)
    {
        //[UserButtonCode_deleteAreaTextButton] -- add your button handler code here..
        sceneEditionManager->OnDeleteArea();
        //[/UserButtonCode_deleteAreaTextButton]
    }
    else if (buttonThatWasClicked == sendBackwardTextButton)
    {
        //[UserButtonCode_sendBackwardTextButton] -- add your button handler code here..
        sceneEditionManager->OnSendBackward();
        //[/UserButtonCode_sendBackwardTextButton]
    }
    else if (buttonThatWasClicked == bringForwardTextButton)
    {
        //[UserButtonCode_bringForwardTextButton] -- add your button handler code here..
        sceneEditionManager->OnBringForward();
        //[/UserButtonCode_bringForwardTextButton]
    }
    else if (buttonThatWasClicked == bringToFrontTextButton)
    {
        //[UserButtonCode_bringToFrontTextButton] -- add your button handler code here..
        sceneEditionManager->OnBringToFront();
        //[/UserButtonCode_bringToFrontTextButton]
    }
    else if (buttonThatWasClicked == sendToBackTextButton)
    {
        //[UserButtonCode_sendToBackTextButton] -- add your button handler code here..
        sceneEditionManager->OnSendToBack();
        //[/UserButtonCode_sendToBackTextButton]
    }
    else if (buttonThatWasClicked == addSceneTextButton)
    {
        //[UserButtonCode_addSceneTextButton] -- add your button handler code here..
        //[/UserButtonCode_addSceneTextButton]
    }
    else if (buttonThatWasClicked == deleteSceneTextButton)
    {
        //[UserButtonCode_deleteSceneTextButton] -- add your button handler code here..
        //[/UserButtonCode_deleteSceneTextButton]
    }
    else if (buttonThatWasClicked == sceneUpTextButton)
    {
        //[UserButtonCode_sceneUpTextButton] -- add your button handler code here..
        //[/UserButtonCode_sceneUpTextButton]
    }
    else if (buttonThatWasClicked == sceneDownTextButton)
    {
        //[UserButtonCode_sceneDownTextButton] -- add your button handler code here..
        //[/UserButtonCode_sceneDownTextButton]
    }
    else if (buttonThatWasClicked == addExciterTextButton)
    {
        //[UserButtonCode_addExciterTextButton] -- add your button handler code here..
        //[/UserButtonCode_addExciterTextButton]
    }
    else if (buttonThatWasClicked == deleteAreaTextButton2)
    {
        //[UserButtonCode_deleteAreaTextButton2] -- add your button handler code here..
        //[/UserButtonCode_deleteAreaTextButton2]
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

    if (comboBoxThatHasChanged == speakersGroupComboBox)
    {
        //[UserComboBoxCode_speakersGroupComboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_speakersGroupComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// CONSTRUCTION HELPERS
void SceneEditionComponent::CompleteInitialization(SceneEditionManager* _sceneEditionManager)
{
    sceneEditionManager = _sceneEditionManager;
    for (size_t i=0 ; i<sceneCanvasComponents.size() ; i++)
        sceneCanvasComponents[i]->CompleteInitialization(sceneEditionManager);
}







// ========== (SETTERS AND GETTERS) ORDERS FROM PRESENTER ==========

// - - - - - Canvases & canvas group - - - - -
void SceneEditionComponent::SetCanvasGroupHidden(bool _isHidden)
{
    canvasGroupComponent->setVisible(!_isHidden);
    canvasInfoLabel->setVisible(!_isHidden);
    addSceneTextButton->setVisible(!_isHidden);
    deleteSceneTextButton->setVisible(!_isHidden);
    sceneUpTextButton->setVisible(!_isHidden);
    sceneDownTextButton->setVisible(!_isHidden);
    sceneNameTextEditor->setVisible(!_isHidden);
    editInitialStateTextButton->setVisible(!_isHidden);

    isCanvasGroupHidden = _isHidden;
}
void SceneEditionComponent::SetCanvasGroupReduced(bool _isReduced)
{
    SetCanvasGroupHidden(_isReduced);
    // isCanvasGroupReduced = true;
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
void SceneEditionComponent::SetEnabledAllControls(bool areEnabled, bool controlsBackUp)
{
    // Canvas

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
    spatLabel->setVisible(areVisible);
    speakersGroupComboBox->setVisible(areVisible);
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




// ----- Parameters setting -----


void SceneEditionComponent::SetAreaColourValue(juce::Colour colour)
{
    sliderR->setValue(colour.getRed());
    sliderG->setValue(colour.getGreen());
    sliderB->setValue(colour.getBlue());
}







// ----- Helpers -----

void SceneEditionComponent::colourSliderMoved()
{
    Colour newColour = Colour((uint8)sliderR->getValue(),
		(uint8)sliderG->getValue(),
		(uint8)sliderB->getValue());
    sceneEditionManager->OnNewColour(newColour);
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
}
void SceneEditionComponent::spatGroupTranslateY(int dY)
{
    componentTranslateY(spatGroupComponent.get(), dY);
    componentTranslateY(spatLabel.get(), dY);
    componentTranslateY(speakersGroupComboBox.get(), dY);
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
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1024" initialHeight="1024">
  <BACKGROUND backgroundColour="ffd9d9d9"/>
  <GROUPCOMPONENT name="Area edition group component" id="87d416270d41f58c" memberName="areaGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -609R 192 248" posRelativeY="4250d5155a80be70"
                  title="Area edition"/>
  <GROUPCOMPONENT name="Spatialization group component" id="90b16e3024c520fd" memberName="spatGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 80" posRelativeY="87d416270d41f58c"
                  title="Spatialization effect"/>
  <TEXTBUTTON name="Add Point text button" id="71769222a7765795" memberName="addPointTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 216 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Add Point"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point text button" id="2bdfdc3999886d02" memberName="deletePointTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 216 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Delete Point"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Copy Text Button" id="33b309fbb149d4ee" memberName="copyTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 24 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Copy" connectedEdges="9"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point" id="653c7d46ffba1120" memberName="pasteTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 48 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Paste" connectedEdges="5"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Add Area text button" id="d377c545746b9e8d" memberName="addAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 24 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Add Area"
              connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Area text button" id="3c0e2321516c0809" memberName="deleteAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 48 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Delete Area"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="893f3de478724dd" memberName="labelR" virtualName=""
         explicitFocusOrder="0" pos="16 136 24 24" posRelativeY="87d416270d41f58c"
         edTextCol="ff000000" edBkgCol="0" labelText="R" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="Slider R" id="c86e71b3772d49e9" memberName="sliderR" virtualName=""
          explicitFocusOrder="0" pos="40 136 150 24" posRelativeY="87d416270d41f58c"
          thumbcol="ffbbbbbb" min="0" max="255" int="1" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="d31faee34d487318" memberName="labelG" virtualName=""
         explicitFocusOrder="0" pos="16 160 24 24" posRelativeY="87d416270d41f58c"
         edTextCol="ff000000" edBkgCol="0" labelText="G" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="SliderG" id="8455ecc13f47ec08" memberName="sliderG" virtualName=""
          explicitFocusOrder="0" pos="40 160 150 24" posRelativeY="87d416270d41f58c"
          thumbcol="ffbbbbbb" min="0" max="255" int="1" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="ae5abc145d9184cf" memberName="labelB" virtualName=""
         explicitFocusOrder="0" pos="16 184 24 24" posRelativeY="87d416270d41f58c"
         edTextCol="ff000000" edBkgCol="0" labelText="B" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="Slider B" id="15aa4b94476e3563" memberName="sliderB" virtualName=""
          explicitFocusOrder="0" pos="40 184 150 24" posRelativeY="87d416270d41f58c"
          thumbcol="ffbbbbbb" min="0" max="255" int="1" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TEXTBUTTON name="Send backward text button" id="a356ce3575b4d98b" memberName="sendBackwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 104 104 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Send backward"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring forward text button" id="d626373b1db0a49" memberName="bringForwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 80 104 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Bring forward"
              connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring to front text button" id="43d8b27a93068fcf" memberName="bringToFrontTextButton"
              virtualName="" explicitFocusOrder="0" pos="120 80 72 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="to front"
              connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Send to back text button" id="1fe791d6bf4489ca" memberName="sendToBackTextButton"
              virtualName="" explicitFocusOrder="0" pos="120 104 72 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="to back" connectedEdges="5"
              needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="Canvas edition group component" id="4250d5155a80be70" memberName="canvasGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 8 192 168" title="Scenes edition"/>
  <COMBOBOX name="Speakers group combo box" id="89ad7c0a3be5a39c" memberName="speakersGroupComboBox"
            virtualName="" explicitFocusOrder="0" pos="16 40 176 24" posRelativeY="90b16e3024c520fd"
            editable="0" layout="33" items="-1 undefined" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="Spat label" id="b1f047be2f31dc5" memberName="spatLabel"
         virtualName="" explicitFocusOrder="0" pos="16 16 176 24" posRelativeY="90b16e3024c520fd"
         edTextCol="ff000000" edBkgCol="0" labelText="Link to speakers group :"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Add Scene text button" id="47bebc9d3a03780d" memberName="addSceneTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 48 104 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Add Scene"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Scene text button" id="5f4e8653b868a323" memberName="deleteSceneTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 72 104 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Delete Scene"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Scene Up text button" id="43b96ebd16bb5586" memberName="sceneUpTextButton"
              virtualName="" explicitFocusOrder="0" pos="128 48 64 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="&#8593; Up"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Scene Down text button" id="e6cf4b99a12776ee" memberName="sceneDownTextButton"
              virtualName="" explicitFocusOrder="0" pos="128 72 64 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="&#8595; Down"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Canvas Info label" id="3577c0e2ccd44371" memberName="canvasInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="16 16 176 24" posRelativeY="4250d5155a80be70"
         edTextCol="ff000000" edBkgCol="0" labelText="...selected canvas info..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="1" justification="33"/>
  <TEXTEDITOR name="new text editor" id="fd7eace3e677fc36" memberName="sceneNameTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 112 176 24" initialText="...name of scene..."
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <GROUPCOMPONENT name="Initial state group component" id="cc3bdf8d18c3f428" memberName="initialStateGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 88" posRelativeY="90b16e3024c520fd"
                  title="Scene initial state"/>
  <TEXTBUTTON name="Add Area text button" id="b6820308eb03f341" memberName="addExciterTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 368 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Add Exciter"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Area text button" id="11692d0648a2e8a4" memberName="deleteAreaTextButton2"
              virtualName="" explicitFocusOrder="0" pos="104 24 88 24" posRelativeY="cc3bdf8d18c3f428"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Delete" connectedEdges="1"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Edit Initial State text button" id="d8f32d6e3ea6e87a" memberName="editInitialStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 136 176 24" posRelativeY="4250d5155a80be70"
              bgColOff="33000000" bgColOn="ffffffff" buttonText="Edit initial scene state"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="End Edit Initial State text button" id="3fe924c475527418"
              memberName="endEditInitialStateTextButton" virtualName="" explicitFocusOrder="0"
              pos="16 576 176 24" posRelativeY="4250d5155a80be70" bgColOff="33000000"
              bgColOn="ffffffff" buttonText="Go back to areas edition" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
