/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "GraphicSessionManager.h"

#include <cmath>
#include <algorithm>

#include "IDrawableArea.h" // areas default types defines

#include "HelpTexts.h"
//[/Headers]

#include "SceneEditionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
SceneEditionComponent::SceneEditionComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    transparentLookAndFeel.reset( new MiamLookAndFeel(true) );
    //[/Constructor_pre]

    infoGroupComponent.reset (new GroupComponent ("Info group component",
                                                  TRANS("Info")));
    addAndMakeVisible (infoGroupComponent.get());
    infoGroupComponent->setTextLabelPosition (Justification::centredLeft);
    infoGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    infoGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    infoTextEditor.reset (new TextEditor ("Info text editor"));
    addAndMakeVisible (infoTextEditor.get());
    infoTextEditor->setMultiLine (true);
    infoTextEditor->setReturnKeyStartsNewLine (true);
    infoTextEditor->setReadOnly (true);
    infoTextEditor->setScrollbarsShown (true);
    infoTextEditor->setCaretVisible (false);
    infoTextEditor->setPopupMenuEnabled (false);
    infoTextEditor->setColour (TextEditor::textColourId, Colours::black);
    infoTextEditor->setColour (TextEditor::backgroundColourId, Colour (0x00ffffff));
    infoTextEditor->setText (TRANS("Multi\n"
    "L\n"
    "I\n"
    "N\n"
    "E\n"
    "and scrollable information textbox for help contents"));

    areaGroupComponent.reset (new GroupComponent ("Area edition group component",
                                                  TRANS("Area edition")));
    addAndMakeVisible (areaGroupComponent.get());
    areaGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    areaGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    controlGroupComponent.reset (new GroupComponent ("Control group component",
                                                     TRANS("Control data")));
    addAndMakeVisible (controlGroupComponent.get());
    controlGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    controlGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addPointTextButton.reset (new TextButton ("Add Point text button"));
    addAndMakeVisible (addPointTextButton.get());
    addPointTextButton->setButtonText (TRANS("Add Point"));
    addPointTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addPointTextButton->addListener (this);
    addPointTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    addPointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addPointTextButton->setColour (TextButton::textColourOffId, Colours::black);

    deletePointTextButton.reset (new TextButton ("Delete Point text button"));
    addAndMakeVisible (deletePointTextButton.get());
    deletePointTextButton->setButtonText (TRANS("Delete Point"));
    deletePointTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deletePointTextButton->addListener (this);
    deletePointTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    deletePointTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deletePointTextButton->setColour (TextButton::textColourOffId, Colours::black);

    copyTextButton.reset (new TextButton ("Copy Text Button"));
    addAndMakeVisible (copyTextButton.get());
    copyTextButton->setButtonText (TRANS("Copy"));
    copyTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    copyTextButton->addListener (this);
    copyTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    copyTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    copyTextButton->setColour (TextButton::textColourOffId, Colours::black);

    pasteTextButton.reset (new TextButton ("Delete Point"));
    addAndMakeVisible (pasteTextButton.get());
    pasteTextButton->setButtonText (TRANS("Paste"));
    pasteTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    pasteTextButton->addListener (this);
    pasteTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    pasteTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    pasteTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAreaTextButton.reset (new TextButton ("Add Area text button"));
    addAndMakeVisible (addAreaTextButton.get());
    addAreaTextButton->setButtonText (TRANS("Add Area"));
    addAreaTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    addAreaTextButton->addListener (this);
    addAreaTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    addAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addAreaTextButton->setColour (TextButton::textColourOffId, Colours::black);

    deleteAreaTextButton.reset (new TextButton ("Delete Area text button"));
    addAndMakeVisible (deleteAreaTextButton.get());
    deleteAreaTextButton->setButtonText (TRANS("Delete Area"));
    deleteAreaTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    deleteAreaTextButton->addListener (this);
    deleteAreaTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    deleteAreaTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteAreaTextButton->setColour (TextButton::textColourOffId, Colours::black);

    sendBackwardTextButton.reset (new TextButton ("Send backward text button"));
    addAndMakeVisible (sendBackwardTextButton.get());
    sendBackwardTextButton->setButtonText (TRANS("Send backward"));
    sendBackwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    sendBackwardTextButton->addListener (this);
    sendBackwardTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    sendBackwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sendBackwardTextButton->setColour (TextButton::textColourOffId, Colours::black);

    bringForwardTextButton.reset (new TextButton ("Bring forward text button"));
    addAndMakeVisible (bringForwardTextButton.get());
    bringForwardTextButton->setButtonText (TRANS("Bring forward"));
    bringForwardTextButton->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    bringForwardTextButton->addListener (this);
    bringForwardTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    bringForwardTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    bringForwardTextButton->setColour (TextButton::textColourOffId, Colours::black);

    bringToFrontTextButton.reset (new TextButton ("Bring to front text button"));
    addAndMakeVisible (bringToFrontTextButton.get());
    bringToFrontTextButton->setButtonText (TRANS("to front"));
    bringToFrontTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    bringToFrontTextButton->addListener (this);
    bringToFrontTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    bringToFrontTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    bringToFrontTextButton->setColour (TextButton::textColourOffId, Colours::black);

    sendToBackTextButton.reset (new TextButton ("Send to back text button"));
    addAndMakeVisible (sendToBackTextButton.get());
    sendToBackTextButton->setButtonText (TRANS("to back"));
    sendToBackTextButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    sendToBackTextButton->addListener (this);
    sendToBackTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    sendToBackTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sendToBackTextButton->setColour (TextButton::textColourOffId, Colours::black);

    canvasGroupComponent.reset (new GroupComponent ("Canvas edition group component",
                                                    TRANS("Scene edition")));
    addAndMakeVisible (canvasGroupComponent.get());
    canvasGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    canvasGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    canvasGroupComponent->setBounds (8, 8, 192, 128);

    spatStatesComboBox.reset (new ComboBox ("Spat States combo box"));
    addAndMakeVisible (spatStatesComboBox.get());
    spatStatesComboBox->setEditableText (false);
    spatStatesComboBox->setJustificationType (Justification::centredLeft);
    spatStatesComboBox->setTextWhenNothingSelected (String());
    spatStatesComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    spatStatesComboBox->addItem (TRANS("-1 undefined"), 1);
    spatStatesComboBox->addListener (this);

    controlStateLabel.reset (new Label ("Control State label",
                                        TRANS("Link to control state:")));
    addAndMakeVisible (controlStateLabel.get());
    controlStateLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    controlStateLabel->setJustificationType (Justification::centredLeft);
    controlStateLabel->setEditable (false, false, false);
    controlStateLabel->setColour (Label::textColourId, Colours::black);
    controlStateLabel->setColour (TextEditor::textColourId, Colours::black);
    controlStateLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addSceneTextButton.reset (new TextButton ("Add Scene text button"));
    addAndMakeVisible (addSceneTextButton.get());
    addSceneTextButton->setButtonText (TRANS("Add Scene"));
    addSceneTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addSceneTextButton->addListener (this);
    addSceneTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    addSceneTextButton->setColour (TextButton::buttonOnColourId, Colours::black);
    addSceneTextButton->setColour (TextButton::textColourOffId, Colours::black);

    deleteSceneTextButton.reset (new TextButton ("Delete Scene text button"));
    addAndMakeVisible (deleteSceneTextButton.get());
    deleteSceneTextButton->setButtonText (TRANS("Delete Scene"));
    deleteSceneTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deleteSceneTextButton->addListener (this);
    deleteSceneTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    deleteSceneTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteSceneTextButton->setColour (TextButton::textColourOffId, Colours::black);

    sceneLeftTextButton.reset (new TextButton ("Scene Left text button"));
    addAndMakeVisible (sceneLeftTextButton.get());
    sceneLeftTextButton->setButtonText (TRANS("Move left"));
    sceneLeftTextButton->setConnectedEdges (Button::ConnectedOnRight);
    sceneLeftTextButton->addListener (this);
    sceneLeftTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    sceneLeftTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sceneLeftTextButton->setColour (TextButton::textColourOffId, Colours::black);

    sceneRightTextButton.reset (new TextButton ("Scene Right text button"));
    addAndMakeVisible (sceneRightTextButton.get());
    sceneRightTextButton->setButtonText (TRANS("Move right"));
    sceneRightTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    sceneRightTextButton->addListener (this);
    sceneRightTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    sceneRightTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    sceneRightTextButton->setColour (TextButton::textColourOffId, Colours::black);

    canvasInfoLabel.reset (new Label ("Canvas Info label",
                                      TRANS("...selected canvas info...")));
    addAndMakeVisible (canvasInfoLabel.get());
    canvasInfoLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Italic"));
    canvasInfoLabel->setJustificationType (Justification::centred);
    canvasInfoLabel->setEditable (false, false, false);
    canvasInfoLabel->setColour (Label::textColourId, Colours::black);
    canvasInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    canvasInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    initialStateGroupComponent.reset (new GroupComponent ("Initial state group component",
                                                          TRANS("Scene initial state")));
    addAndMakeVisible (initialStateGroupComponent.get());
    initialStateGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    initialStateGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addExciterTextButton.reset (new TextButton ("Add Area text button"));
    addAndMakeVisible (addExciterTextButton.get());
    addExciterTextButton->setButtonText (TRANS("Add Exciter"));
    addExciterTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addExciterTextButton->addListener (this);
    addExciterTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    addExciterTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addExciterTextButton->setColour (TextButton::textColourOffId, Colours::black);

    deleteExciterTextButton.reset (new TextButton ("Delete Exciter text button"));
    addAndMakeVisible (deleteExciterTextButton.get());
    deleteExciterTextButton->setButtonText (TRANS("Delete"));
    deleteExciterTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deleteExciterTextButton->addListener (this);
    deleteExciterTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    deleteExciterTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteExciterTextButton->setColour (TextButton::textColourOffId, Colours::black);

    sceneNameLabel.reset (new Label ("Scene Name Label",
                                     TRANS("Name:")));
    addAndMakeVisible (sceneNameLabel.get());
    sceneNameLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    sceneNameLabel->setJustificationType (Justification::centredLeft);
    sceneNameLabel->setEditable (false, false, false);
    sceneNameLabel->setColour (Label::textColourId, Colours::black);
    sceneNameLabel->setColour (TextEditor::textColourId, Colours::black);
    sceneNameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sceneNameTextEditor.reset (new TextEditor ("Scene Name text editor"));
    addAndMakeVisible (sceneNameTextEditor.get());
    sceneNameTextEditor->setMultiLine (false);
    sceneNameTextEditor->setReturnKeyStartsNewLine (false);
    sceneNameTextEditor->setReadOnly (false);
    sceneNameTextEditor->setScrollbarsShown (true);
    sceneNameTextEditor->setCaretVisible (true);
    sceneNameTextEditor->setPopupMenuEnabled (true);
    sceneNameTextEditor->setText (String());

    sceneNameTextEditor->setBounds (64, 72, 128, 24);

    excitersEditionButton.reset (new ToggleButton ("Exciters Edition button"));
    addAndMakeVisible (excitersEditionButton.get());
    excitersEditionButton->setButtonText (TRANS("Edit exciters and play!"));
    excitersEditionButton->addListener (this);
    excitersEditionButton->setColour (ToggleButton::textColourId, Colours::black);

    showInfoTextButton.reset (new TextButton ("Show Info text button"));
    addAndMakeVisible (showInfoTextButton.get());
    showInfoTextButton->setButtonText (TRANS("[+]    Show context information      "));
    showInfoTextButton->addListener (this);
    showInfoTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    showInfoTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    showInfoTextButton->setColour (TextButton::textColourOffId, Colours::black);

    excitersConstraintButton.reset (new ToggleButton ("Exciters Constraint button"));
    addAndMakeVisible (excitersConstraintButton.get());
    excitersConstraintButton->setTooltip (TRANS("Free exciters"));
    excitersConstraintButton->setButtonText (TRANS("No-constraint edition"));
    excitersConstraintButton->addListener (this);
    excitersConstraintButton->setColour (ToggleButton::textColourId, Colours::black);


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
    initialStateGroupReducedH = 56; // 2-lines toggle button only
    infoGroupReducedH = 44;

    // Special treatements for the info group
    SetInfoGroupReduced(false);
    infoTextEditor->setLookAndFeel(transparentLookAndFeel.get());

    // Tooltips for shortcuts
    // all work with Cmd (Ctrl) at the moment
	TextUtils::AddShortcutToTooltip(*excitersEditionButton.get(),
		TextUtils::GetCommandKeyDescription(editExcitersCmdKey));
    TextUtils::AddShortcutToTooltip(*excitersConstraintButton.get(),
        TextUtils::GetCommandKeyDescription(freeExcitersMoveCmdKey));
	TextUtils::AddShortcutToTooltip(*copyTextButton.get(),
		TextUtils::GetCommandKeyDescription(copyCmdKey));
	TextUtils::AddShortcutToTooltip(*pasteTextButton.get(),
		TextUtils::GetCommandKeyDescription(pasteCmdKey));
	TextUtils::AddShortcutToTooltip(*addAreaTextButton.get(),
		TextUtils::GetCommandKeyDescription(newAreaCmdKey));
	TextUtils::AddShortcutToTooltip(*deleteAreaTextButton.get(),
		TextUtils::GetCommandKeyDescription(deleteAreaCmdKey));
	TextUtils::AddShortcutToTooltip(*addExciterTextButton.get(),
		TextUtils::GetCommandKeyDescription(newExciterCmdKey));
	TextUtils::AddShortcutToTooltip(*deleteExciterTextButton.get(),
		TextUtils::GetCommandKeyDescription(deleteExciterCmdKey));
	TextUtils::AddShortcutToTooltip(*bringForwardTextButton.get(),
		TextUtils::GetCommandKeyDescription(areaForwardCmdKey));
	TextUtils::AddShortcutToTooltip(*sendBackwardTextButton.get(),
		TextUtils::GetCommandKeyDescription(areaBackwardCmdKey));
	TextUtils::AddShortcutToTooltip(*addPointTextButton.get(),
		TextUtils::GetCommandKeyDescription(addPointCmdKey));
    //  or Cmd+Shift
	TextUtils::AddShortcutToTooltip(*deletePointTextButton.get(),
		TextUtils::GetCommandShiftKeyDescription(deletePointCmdShiftKey));
	TextUtils::AddShortcutToTooltip(*bringToFrontTextButton.get(),
		TextUtils::GetCommandShiftKeyDescription(areaToFrontCmdShiftKey));
	TextUtils::AddShortcutToTooltip(*sendToBackTextButton.get(),
		TextUtils::GetCommandShiftKeyDescription(areaToBackCmdShiftKey));
	TextUtils::AddShortcutToTooltip(*addSceneTextButton.get(),
		TextUtils::GetCommandShiftKeyDescription(newSceneCmdShiftKey));
	TextUtils::AddShortcutToTooltip(*deleteSceneTextButton.get(),
		TextUtils::GetCommandShiftKeyDescription(deleteSceneCmdShiftKey));
	TextUtils::AddShortcutToTooltip(*sceneLeftTextButton.get(),
		TextUtils::GetCommandShiftKeyDescription(sceneLeftCmdShiftKey));
	TextUtils::AddShortcutToTooltip(*sceneRightTextButton.get(),
		TextUtils::GetCommandShiftKeyDescription(sceneRightCmdShiftKey));

    //[/Constructor]
}

SceneEditionComponent::~SceneEditionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    //[/Destructor_pre]

    infoGroupComponent = nullptr;
    infoTextEditor = nullptr;
    areaGroupComponent = nullptr;
    controlGroupComponent = nullptr;
    addPointTextButton = nullptr;
    deletePointTextButton = nullptr;
    copyTextButton = nullptr;
    pasteTextButton = nullptr;
    addAreaTextButton = nullptr;
    deleteAreaTextButton = nullptr;
    sendBackwardTextButton = nullptr;
    bringForwardTextButton = nullptr;
    bringToFrontTextButton = nullptr;
    sendToBackTextButton = nullptr;
    canvasGroupComponent = nullptr;
    spatStatesComboBox = nullptr;
    controlStateLabel = nullptr;
    addSceneTextButton = nullptr;
    deleteSceneTextButton = nullptr;
    sceneLeftTextButton = nullptr;
    sceneRightTextButton = nullptr;
    canvasInfoLabel = nullptr;
    initialStateGroupComponent = nullptr;
    addExciterTextButton = nullptr;
    deleteExciterTextButton = nullptr;
    sceneNameLabel = nullptr;
    sceneNameTextEditor = nullptr;
    excitersEditionButton = nullptr;
    showInfoTextButton = nullptr;
    excitersConstraintButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    transparentLookAndFeel = nullptr;
    //[/Destructor]
}

//==============================================================================
void SceneEditionComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbfbfbf));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SceneEditionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    // Remise des contenus des groupes à leur position initiale !
    // Car des optimisation dans Juce 5.2.1 font que le code de placement n'est exécuté
    // qu'une seule fois (dans le constructeur...)
    areaGroupTranslateY(-areaGroupCurrentDy);
    spatGroupTranslateY(-spatGroupCurrentDy);
    initialStateGroupTranslateY(-initialStateGroupCurrentDy);

    // Placement des canevas
    if (multiCanvasComponent)
    {
        Rectangle<int> multiCanvasBounds = this->getLocalBounds();
        multiCanvasBounds.removeFromLeft(areaGroupComponent->getWidth() + 8 + 8);

        multiCanvasComponent->setBounds(multiCanvasBounds);
    }

    //[/UserPreResize]

    infoGroupComponent->setBounds (8, (((8 + 128 - -8) + 168 - -8) + 80 - -8) + 112 - -8, 192, 160);
    infoTextEditor->setBounds (16, ((((8 + 128 - -8) + 168 - -8) + 80 - -8) + 112 - -8) + 16, 176, 104);
    areaGroupComponent->setBounds (8, 8 + 128 - -8, 192, 168);
    controlGroupComponent->setBounds (8, (8 + 128 - -8) + 168 - -8, 192, 80);
    addPointTextButton->setBounds (16, (8 + 128 - -8) + 136, 88, 24);
    deletePointTextButton->setBounds (104, (8 + 128 - -8) + 136, 88, 24);
    copyTextButton->setBounds (16, (8 + 128 - -8) + 48, 88, 24);
    pasteTextButton->setBounds (104, (8 + 128 - -8) + 48, 88, 24);
    addAreaTextButton->setBounds (16, (8 + 128 - -8) + 24, 88, 24);
    deleteAreaTextButton->setBounds (104, (8 + 128 - -8) + 24, 88, 24);
    sendBackwardTextButton->setBounds (16, (8 + 128 - -8) + 104, 112, 24);
    bringForwardTextButton->setBounds (16, (8 + 128 - -8) + 80, 112, 24);
    bringToFrontTextButton->setBounds (128, (8 + 128 - -8) + 80, 64, 24);
    sendToBackTextButton->setBounds (128, (8 + 128 - -8) + 104, 64, 24);
    spatStatesComboBox->setBounds (16, ((8 + 128 - -8) + 168 - -8) + 40, 176, 24);
    controlStateLabel->setBounds (16, ((8 + 128 - -8) + 168 - -8) + 16, 176, 24);
    addSceneTextButton->setBounds (16, 8 + 24, 88, 24);
    deleteSceneTextButton->setBounds (104, 8 + 24, 88, 24);
    sceneLeftTextButton->setBounds (16, 8 + 96, 88, 24);
    sceneRightTextButton->setBounds (104, 8 + 96, 88, 24);
    canvasInfoLabel->setBounds (16, 8 + 48, 176, 8);
    initialStateGroupComponent->setBounds (8, ((8 + 128 - -8) + 168 - -8) + 80 - -8, 192, 112);
    addExciterTextButton->setBounds (16, (((8 + 128 - -8) + 168 - -8) + 80 - -8) + 80, 88, 24);
    deleteExciterTextButton->setBounds (104, (((8 + 128 - -8) + 168 - -8) + 80 - -8) + 80, 88, 24);
    sceneNameLabel->setBounds (15, 8 + 63, 56, 24);
    excitersEditionButton->setBounds (16, (((8 + 128 - -8) + 168 - -8) + 80 - -8) + 20, 176, 24);
    showInfoTextButton->setBounds (16, ((((8 + 128 - -8) + 168 - -8) + 80 - -8) + 112 - -8) + 160 - 8 - 20, 176, 20);
    excitersConstraintButton->setBounds (16, (((8 + 128 - -8) + 168 - -8) + 80 - -8) + 48, 176, 24);
    //[UserResized] Add your own custom resize handling here..

    // Backup of Projucer's sizes
    const int canvasGroupInitH = canvasGroupComponent->getHeight();
    const int areaGroupInitH = areaGroupComponent->getHeight();
    const int spatGroupInitH = controlGroupComponent->getHeight();
    //int initialStateGroupInitH = initialStateGroupComponent->getHeight();

    // Variable menus' size
    if (isAreaGroupReduced)
        areaGroupComponent->setSize(areaGroupComponent->getWidth(), areaGroupReducedH);
    // -
    if (isInitialStateGroupReduced)
        initialStateGroupComponent->setSize(initialStateGroupComponent->getWidth(), initialStateGroupReducedH);
    // -
    // !! Other menus are invisible... And were set invisible when the info came !!

    /* In the end, we may have to translate some groups towards the top of the component,
     * because some groups' height may have change (depending on the running mode).
     */
    if (isCanvasGroupHidden)
    {
        int dY = 0 - canvasGroupInitH - 8; // < 0
        areaGroupTranslateY(dY);
    }
    // -
    if (isAreaGroupHidden)
    {
        int dY = 0 - areaGroupInitH - 8 -8; // < 0
        spatGroupTranslateY(dY);
    }
    else if (isAreaGroupReduced)
    {
        int dY = areaGroupReducedH - areaGroupInitH - 8; // < 0
        spatGroupTranslateY(dY);
    }
    // -
    if (isSpatGroupHidden)
    {
        int dY = 0 - spatGroupInitH /*- 8*/; // < 0
        initialStateGroupTranslateY(dY);
    }

    // Remaining space will be used for the help contents display
    int remainingTopPosition = initialStateGroupComponent->getBottom() + 24; // used height + top margin
    infoGroupComponent->setTopLeftPosition(8, remainingTopPosition);
    infoTextEditor->setTopLeftPosition(16, remainingTopPosition + 16);
    int remainingHeight = getHeight();
    remainingHeight -= remainingTopPosition;
    remainingHeight -= 8; // bottom margin
    remainingHeight = std::max(0, remainingHeight);
    if (isInfoGroupReduced)
    {
        infoGroupComponent->setSize(infoGroupComponent->getWidth(),
                                    std::min(infoGroupReducedH, remainingHeight));
    }
    else
    {
        infoGroupComponent->setSize(infoGroupComponent->getWidth(),
                                    remainingHeight);
    }
    showInfoTextButton->setTopLeftPosition(16, infoGroupComponent->getBottom() - 8 - showInfoTextButton->getHeight());
    infoTextEditor->setSize(infoTextEditor->getWidth(),
                            std::max(showInfoTextButton->getY() - infoTextEditor->getY() - 8, 0));

    //[/UserResized]
}

void SceneEditionComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == addPointTextButton.get())
    {
        //[UserButtonCode_addPointTextButton] -- add your button handler code here..
        graphicSessionManager->OnAddPoint();
        //[/UserButtonCode_addPointTextButton]
    }
    else if (buttonThatWasClicked == deletePointTextButton.get())
    {
        //[UserButtonCode_deletePointTextButton] -- add your button handler code here..
        graphicSessionManager->OnDeletePoint();
        //[/UserButtonCode_deletePointTextButton]
    }
    else if (buttonThatWasClicked == copyTextButton.get())
    {
        //[UserButtonCode_copyTextButton] -- add your button handler code here..
        graphicSessionManager->OnCopyArea();
        //[/UserButtonCode_copyTextButton]
    }
    else if (buttonThatWasClicked == pasteTextButton.get())
    {
        //[UserButtonCode_pasteTextButton] -- add your button handler code here..
        graphicSessionManager->OnPasteArea();
        //[/UserButtonCode_pasteTextButton]
    }
    else if (buttonThatWasClicked == addAreaTextButton.get())
    {
        //[UserButtonCode_addAreaTextButton] -- add your button handler code here..
        // Pop-up menu to get the kind of area to add
        PopupMenu menu;
        menu.addItem (AreaDefaultType::Ellipse, "Ellipse", false); // disabled
        menu.addSeparator();
        menu.addItem(AreaDefaultType::Polygon+0, "Triangle");
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
    else if (buttonThatWasClicked == deleteAreaTextButton.get())
    {
        //[UserButtonCode_deleteAreaTextButton] -- add your button handler code here..
        graphicSessionManager->OnDeleteArea();
        //[/UserButtonCode_deleteAreaTextButton]
    }
    else if (buttonThatWasClicked == sendBackwardTextButton.get())
    {
        //[UserButtonCode_sendBackwardTextButton] -- add your button handler code here..
        graphicSessionManager->OnSendBackward();
        //[/UserButtonCode_sendBackwardTextButton]
    }
    else if (buttonThatWasClicked == bringForwardTextButton.get())
    {
        //[UserButtonCode_bringForwardTextButton] -- add your button handler code here..
        graphicSessionManager->OnBringForward();
        //[/UserButtonCode_bringForwardTextButton]
    }
    else if (buttonThatWasClicked == bringToFrontTextButton.get())
    {
        //[UserButtonCode_bringToFrontTextButton] -- add your button handler code here..
        graphicSessionManager->OnBringToFront();
        //[/UserButtonCode_bringToFrontTextButton]
    }
    else if (buttonThatWasClicked == sendToBackTextButton.get())
    {
        //[UserButtonCode_sendToBackTextButton] -- add your button handler code here..
        graphicSessionManager->OnSendToBack();
        //[/UserButtonCode_sendToBackTextButton]
    }
    else if (buttonThatWasClicked == addSceneTextButton.get())
    {
        //[UserButtonCode_addSceneTextButton] -- add your button handler code here..
        graphicSessionManager->OnAddScene();
        //[/UserButtonCode_addSceneTextButton]
    }
    else if (buttonThatWasClicked == deleteSceneTextButton.get())
    {
        //[UserButtonCode_deleteSceneTextButton] -- add your button handler code here..
        graphicSessionManager->OnDeleteScene();
        //[/UserButtonCode_deleteSceneTextButton]
    }
    else if (buttonThatWasClicked == sceneLeftTextButton.get())
    {
        //[UserButtonCode_sceneLeftTextButton] -- add your button handler code here..
        graphicSessionManager->OnSceneLeft();
        //[/UserButtonCode_sceneLeftTextButton]
    }
    else if (buttonThatWasClicked == sceneRightTextButton.get())
    {
        //[UserButtonCode_sceneRightTextButton] -- add your button handler code here..
        graphicSessionManager->OnSceneRight();
        //[/UserButtonCode_sceneRightTextButton]
    }
    else if (buttonThatWasClicked == addExciterTextButton.get())
    {
        //[UserButtonCode_addExciterTextButton] -- add your button handler code here..
        graphicSessionManager->OnAddExciter();
        //[/UserButtonCode_addExciterTextButton]
    }
    else if (buttonThatWasClicked == deleteExciterTextButton.get())
    {
        //[UserButtonCode_deleteExciterTextButton] -- add your button handler code here..
        graphicSessionManager->OnDeleteExciter();
        //[/UserButtonCode_deleteExciterTextButton]
    }
    else if (buttonThatWasClicked == excitersEditionButton.get())
    {
        //[UserButtonCode_excitersEditionButton] -- add your button handler code here..
        if( excitersEditionButton->getToggleState() )
            graphicSessionManager->OnEnterExcitersEdition();
        else
            graphicSessionManager->OnQuitExcitersEdition();
        //[/UserButtonCode_excitersEditionButton]
    }
    else if (buttonThatWasClicked == showInfoTextButton.get())
    {
        //[UserButtonCode_showInfoTextButton] -- add your button handler code here..
        this->SetInfoGroupReduced(!isInfoGroupReduced); // self-management because simple logic...
        // to force updates
        resized();
        //[/UserButtonCode_showInfoTextButton]
    }
    else if (buttonThatWasClicked == excitersConstraintButton.get())
    {
        //[UserButtonCode_excitersConstraintButton] -- add your button handler code here..
        graphicSessionManager->BypassGlobalExcitersConstraint(excitersConstraintButton->getToggleState());
        //[/UserButtonCode_excitersConstraintButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SceneEditionComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == spatStatesComboBox.get())
    {
        //[UserComboBoxCode_spatStatesComboBox] -- add your combo box handling code here..
        graphicSessionManager->OnSpatStateChanged(spatStatesComboBox->getSelectedItemIndex());
        //[/UserComboBoxCode_spatStatesComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SceneEditionComponent::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    switch (graphicSessionManager->GetSessionPurpose())
    {
        case AppPurpose::Spatialisation :
            controlStateLabel->setText(TRANS("Link to spatialization state:"), NotificationType::sendNotification);
            break;
        case AppPurpose::GenericController :
            controlStateLabel->setText(TRANS("Link to control state:"), NotificationType::sendNotification);
            break;

        default :
            break;
    }

    // forced keyboard focus grab, for shortcuts
    if (isVisible())
        Timer::callAfterDelay(100, [this] { this->grabKeyboardFocus(); } );

    //[/UserCode_visibilityChanged]
}

bool SceneEditionComponent::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...

    bool keyWasUsed = false;


    // ====================== Keyboard SHORTCUTS =====================
    if (key.getModifiers().isCommandDown())
    {
        keyWasUsed = true;
        // Test avec le modifier-key SHIFT pour commencer (pour les scènes)
        if (key.getModifiers().isShiftDown())
        {
            if ( (key.getKeyCode() == newSceneCmdShiftKey)
                && (! isCanvasGroupHidden)
                && addSceneTextButton->isEnabled() && addSceneTextButton->isVisible())
                addSceneTextButton->triggerClick();
            else if ( (key.getKeyCode() == deleteSceneCmdShiftKey)
                && (! isCanvasGroupHidden)
                && deleteSceneTextButton->isEnabled() && deleteSceneTextButton->isVisible())
                deleteSceneTextButton->triggerClick();
            else if ( (key.getKeyCode() == sceneLeftCmdShiftKey)
                     && (! isCanvasGroupHidden)
                     && sceneLeftTextButton->isEnabled() && sceneLeftTextButton->isVisible())
                sceneLeftTextButton->triggerClick();
            else if ( (key.getKeyCode() == sceneRightCmdShiftKey)
                     && (! isAreaGroupHidden)
                     && sceneRightTextButton->isEnabled() && sceneRightTextButton->isVisible())
                sceneRightTextButton->triggerClick();
            else if ( (key.getKeyCode() == areaToFrontCmdShiftKey)
                     && (! isAreaGroupHidden)
                     && bringToFrontTextButton->isEnabled() && bringToFrontTextButton->isVisible())
                bringToFrontTextButton->triggerClick();
            else if ( (key.getKeyCode() == deletePointCmdShiftKey)
                     && (! isAreaGroupHidden)
                     && deletePointTextButton->isEnabled() && deletePointTextButton->isVisible())
                deletePointTextButton->triggerClick();
            else
                keyWasUsed = false;
        }
        // Le raccourci Cmd E (ou ctrl E sous linux/windows) déclenche Edit Exciter,
        // si ce bouton était bien affiché. Idem pour Cmd F : Free exciter move
        else if ( (key.getKeyCode() == editExcitersCmdKey)
            && ( !isInitialStateGroupHidden )
            && excitersEditionButton->isEnabled() && excitersEditionButton->isVisible())
            excitersEditionButton->triggerClick();
        else if ( (key.getKeyCode() == freeExcitersMoveCmdKey)
                 && ( !isInitialStateGroupHidden )
                 && excitersConstraintButton->isEnabled() && excitersConstraintButton->isVisible())
            excitersConstraintButton->triggerClick();
        // Cmd C : Copier
        else if ( (key.getKeyCode() == copyCmdKey)
                 && (! isAreaGroupHidden)
                 && copyTextButton->isEnabled() && copyTextButton->isVisible())
            copyTextButton->triggerClick();
        // Cmd V : Coller
        else if ( (key.getKeyCode() == pasteCmdKey)
                 && (! isAreaGroupHidden)
                 && pasteTextButton->isEnabled() && pasteTextButton->isVisible())
            pasteTextButton->triggerClick();
		// New/Delete area is tested first.....
        else if ( (key.getKeyCode() == newAreaCmdKey)
                 && (! isAreaGroupHidden)
                 && addAreaTextButton->isEnabled() && addAreaTextButton->isVisible())
            addAreaTextButton->triggerClick();
        else if ( (key.getKeyCode() == deleteAreaCmdKey)
                 && (! isAreaGroupHidden)
                 && deleteAreaTextButton->isEnabled() && deleteAreaTextButton->isVisible())
            deleteAreaTextButton->triggerClick();
		// ...and new/delete exciters come after (in case the 2 buttons are displayed at the same time (in a future version...))
		else if ((key.getKeyCode() == newExciterCmdKey)
			&& (!isInitialStateGroupHidden)
			&& addExciterTextButton->isEnabled() && addExciterTextButton->isVisible())
			addExciterTextButton->triggerClick();
		else if ((key.getKeyCode() == deleteExciterCmdKey)
			&& (!isInitialStateGroupHidden)
			&& deleteExciterTextButton->isEnabled() && deleteExciterTextButton->isVisible())
			deleteExciterTextButton->triggerClick();
        else if ( (key.getKeyCode() == areaBackwardCmdKey)
                 && (! isAreaGroupHidden)
                 && sendBackwardTextButton->isEnabled() && sendBackwardTextButton->isVisible())
            sendBackwardTextButton->triggerClick();
        else if ( (key.getKeyCode() == areaForwardCmdKey)
                 && (! isAreaGroupHidden)
                 && bringForwardTextButton->isEnabled() && bringForwardTextButton->isVisible())
            bringForwardTextButton->triggerClick();
        else if ( (key.getKeyCode() == addPointCmdKey)
                 && (! isAreaGroupHidden)
                 && addPointTextButton->isEnabled() && addPointTextButton->isVisible())
            addPointTextButton->triggerClick();
        // Si vraiment rien n'a convenu... on remet à false
        else
            keyWasUsed = false;
    }

    // Forced callback to parent, if unused
    if (! keyWasUsed)
        return getParentComponent()->keyPressed(key);
    // or return true to prevent the event to be passed-on.
    else
        return keyWasUsed;

    //[/UserCode_keyPressed]
}

void SceneEditionComponent::modifierKeysChanged (const ModifierKeys& modifiers)
{
    //[UserCode_modifierKeysChanged] -- Add your code here...
	boost::ignore_unused(modifiers);
    //[/UserCode_modifierKeysChanged]
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
void SceneEditionComponent::PrepareVisible()
{
    multiCanvasComponent->PrepareVisible();
}
void SceneEditionComponent::PrepareUnvisible()
{
    multiCanvasComponent->PrepareUnvisible();
}
void SceneEditionComponent::ReleaseOpenGLResources()
{
    multiCanvasComponent->ReleaseOpengGLResources();

    // On pourrait ici aussi mettre un rendu graphique du fait que les ressources soient effacées...
}


// - - - - - Canvases & canvas group - - - - -

void SceneEditionComponent::SetCanvasGroupHidden(bool _isHidden)
{
    canvasGroupComponent->setVisible(!_isHidden);
    canvasInfoLabel->setVisible(false); // Only ONE CANVAS currently
    addSceneTextButton->setVisible(!_isHidden);
    deleteSceneTextButton->setVisible(!_isHidden);
    sceneLeftTextButton->setVisible(!_isHidden);
    sceneRightTextButton->setVisible(!_isHidden);
    sceneNameLabel->setVisible(!_isHidden);
    sceneNameTextEditor->setVisible(!_isHidden);

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

void SceneEditionComponent::SetAreaGroupHidden(bool _isHidden)
{
    isAreaGroupHidden = _isHidden;

    setVisibleAllAreaControls(!_isHidden);
}
void SceneEditionComponent::SetAreaGroupReduced(bool _isReduced)
{
    isAreaGroupReduced = _isReduced;

    SetVisibleAreaEditionControls(!_isReduced);

    // We know that the "selection of an area" state is equal to the _isReduced....
    // -> Buttons (de)activated on area (de)selection
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

    // Polygon
    addPointTextButton->setEnabled(areEnabled);
    deletePointTextButton->setEnabled(areEnabled);
}
void SceneEditionComponent::setVisibleSpatControls(bool areVisible)
{
    controlGroupComponent->setVisible(areVisible);
    if (spatStatesComboBox->isPopupActive())
        spatStatesComboBox->hidePopup();
    spatStatesComboBox->setVisible(areVisible);
    controlStateLabel->setVisible(areVisible);
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
void SceneEditionComponent::UpdateStatesList(std::vector< std::shared_ptr<ControlState<double>> > &newSpatStates)
{
    // Empties the combo box at first
    spatStatesComboBox->clear();

    // Addition of items==names one by one
    for (size_t i=0 ; i<newSpatStates.size() ; i++)
        spatStatesComboBox->addItem(newSpatStates[i]->GetName(), (int)i+1); // Id==Idx+1
}
void SceneEditionComponent::SelectSpatState(int index, NotificationType notificationType)
{
    spatStatesComboBox->setSelectedItemIndex(index, notificationType);
}


// - - - - - Initial Scene State group - - - - -
void SceneEditionComponent::SetInitialStateGroupHidden(bool _isHidden)
{
    isInitialStateGroupHidden = _isHidden;

    initialStateGroupComponent->setVisible(!_isHidden);
    addExciterTextButton->setVisible(!_isHidden);
    deleteExciterTextButton->setVisible(!_isHidden);
    excitersEditionButton->setVisible(!_isHidden);
    excitersConstraintButton->setVisible(!_isHidden);
}
void SceneEditionComponent::SetInitialStateGroupReduced(bool _isReduced)
{
    isInitialStateGroupReduced = _isReduced;

    addExciterTextButton->setVisible(!_isReduced);
    deleteExciterTextButton->setVisible(!_isReduced);
    excitersConstraintButton->setVisible(!_isReduced);

    // si on est réduit, on décoche le mode correspondant...
    if (_isReduced)
        excitersEditionButton->setToggleState(false, NotificationType::dontSendNotification);
}

void SceneEditionComponent::SetDeleteExciterButtonEnabled(bool _isEnabled)
{
    deleteExciterTextButton->setEnabled(_isEnabled);
}
void SceneEditionComponent::SetExciterConstraintButtonEnabled(bool _isEnabled)
{
    excitersConstraintButton->setEnabled(_isEnabled);
    // Default behavior : if disabled, stays checked
    if (! _isEnabled)
        excitersConstraintButton->setToggleState(true, NotificationType::dontSendNotification);
}
// - - - - - Info group - - - - -
void SceneEditionComponent::SetInfoGroupReduced(bool _isReduced)
{
    if (_isReduced)
    {
        showInfoTextButton->setButtonText(HelpTexts::GetShowHelpButtonText());
    }
    else
    {
        showInfoTextButton->setButtonText(HelpTexts::GetHideHelpButtonText());
    }

    infoTextEditor->setVisible(! _isReduced);

    isInfoGroupReduced = _isReduced;
}
void SceneEditionComponent::SetInfoHelpText(const juce::String& infoHelpText)
{
    infoTextEditor->setText(infoHelpText);
}





// ----- Other setters and getters -----

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

void SceneEditionComponent::setVisibleAllAreaControls(bool areVisible)
{
    // ré-emploi de fonctions déjà écrites
    SetVisibleAreaEditionControls(areVisible);

    // autres boutons à cacher : le quatuor add/delete/copy/paste
    addAreaTextButton->setVisible(areVisible);
    deleteAreaTextButton->setVisible(areVisible);
    copyTextButton->setVisible(areVisible);
    pasteTextButton->setVisible(areVisible);
    areaGroupComponent->setVisible(areVisible);
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
    componentTranslateY(sendBackwardTextButton.get(), dY);
    componentTranslateY(sendToBackTextButton.get(), dY);
    componentTranslateY(bringForwardTextButton.get(), dY);
    componentTranslateY(bringToFrontTextButton.get(), dY);

    areaGroupCurrentDy += dY; // accumulation du dY

    spatGroupTranslateY(dY);
}
void SceneEditionComponent::spatGroupTranslateY(int dY)
{
    componentTranslateY(controlGroupComponent.get(), dY);
    componentTranslateY(controlStateLabel.get(), dY);
    componentTranslateY(spatStatesComboBox.get(), dY);

    spatGroupCurrentDy += dY; // accumulation du dY

    initialStateGroupTranslateY(dY);
}
void SceneEditionComponent::initialStateGroupTranslateY(int dY)
{
    componentTranslateY(initialStateGroupComponent.get(), dY);
    componentTranslateY(addExciterTextButton.get(), dY);
    componentTranslateY(deleteExciterTextButton.get(), dY);
    componentTranslateY(excitersEditionButton.get(), dY);
    componentTranslateY(excitersConstraintButton.get(), dY);

    initialStateGroupCurrentDy += dY; // accumulation du dY
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
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="1024"
                 initialHeight="1024">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffbfbfbf"/>
  <GROUPCOMPONENT name="Info group component" id="97c294b92cbc0a85" memberName="infoGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 160" posRelativeY="cc3bdf8d18c3f428"
                  outlinecol="ff454545" textcol="ff000000" title="Info" textpos="33"/>
  <TEXTEDITOR name="Info text editor" id="a4539a25a9aebf1" memberName="infoTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 16 176 104" posRelativeY="97c294b92cbc0a85"
              textcol="ff000000" bkgcol="ffffff" initialText="Multi&#10;L&#10;I&#10;N&#10;E&#10;and scrollable information textbox for help contents"
              multiline="1" retKeyStartsLine="1" readonly="1" scrollbars="1"
              caret="0" popupmenu="0"/>
  <GROUPCOMPONENT name="Area edition group component" id="87d416270d41f58c" memberName="areaGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 168" posRelativeY="4250d5155a80be70"
                  outlinecol="ff454545" textcol="ff000000" title="Area edition"/>
  <GROUPCOMPONENT name="Control group component" id="90b16e3024c520fd" memberName="controlGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 80" posRelativeY="87d416270d41f58c"
                  outlinecol="ff454545" textcol="ff000000" title="Control data"/>
  <TEXTBUTTON name="Add Point text button" id="71769222a7765795" memberName="addPointTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 136 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Add Point"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point text button" id="2bdfdc3999886d02" memberName="deletePointTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 136 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Delete Point"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Copy Text Button" id="33b309fbb149d4ee" memberName="copyTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 48 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Copy"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Point" id="653c7d46ffba1120" memberName="pasteTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 48 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Paste"
              connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Add Area text button" id="d377c545746b9e8d" memberName="addAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 24 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Add Area"
              connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Area text button" id="3c0e2321516c0809" memberName="deleteAreaTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 24 88 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Delete Area"
              connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Send backward text button" id="a356ce3575b4d98b" memberName="sendBackwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 104 112 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Send backward"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring forward text button" id="d626373b1db0a49" memberName="bringForwardTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 80 112 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Bring forward"
              connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Bring to front text button" id="43d8b27a93068fcf" memberName="bringToFrontTextButton"
              virtualName="" explicitFocusOrder="0" pos="128 80 64 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="to front"
              connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Send to back text button" id="1fe791d6bf4489ca" memberName="sendToBackTextButton"
              virtualName="" explicitFocusOrder="0" pos="128 104 64 24" posRelativeY="87d416270d41f58c"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="to back"
              connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="Canvas edition group component" id="4250d5155a80be70" memberName="canvasGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 8 192 128" outlinecol="ff454545"
                  textcol="ff000000" title="Scene edition"/>
  <COMBOBOX name="Spat States combo box" id="89ad7c0a3be5a39c" memberName="spatStatesComboBox"
            virtualName="" explicitFocusOrder="0" pos="16 40 176 24" posRelativeY="90b16e3024c520fd"
            editable="0" layout="33" items="-1 undefined" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="Control State label" id="b1f047be2f31dc5" memberName="controlStateLabel"
         virtualName="" explicitFocusOrder="0" pos="16 16 176 24" posRelativeY="90b16e3024c520fd"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Link to control state:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="Add Scene text button" id="47bebc9d3a03780d" memberName="addSceneTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 24 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="fff0f0f0" bgColOn="ff000000" textCol="ff000000" buttonText="Add Scene"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Scene text button" id="5f4e8653b868a323" memberName="deleteSceneTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 24 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Delete Scene"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Scene Left text button" id="43b96ebd16bb5586" memberName="sceneLeftTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 96 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Move left"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Scene Right text button" id="e6cf4b99a12776ee" memberName="sceneRightTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 96 88 24" posRelativeY="4250d5155a80be70"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Move right"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Canvas Info label" id="3577c0e2ccd44371" memberName="canvasInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="16 48 176 8" posRelativeY="4250d5155a80be70"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="...selected canvas info..."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="1" justification="36" typefaceStyle="Italic"/>
  <GROUPCOMPONENT name="Initial state group component" id="cc3bdf8d18c3f428" memberName="initialStateGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 -8R 192 112" posRelativeY="90b16e3024c520fd"
                  outlinecol="ff454545" textcol="ff000000" title="Scene initial state"/>
  <TEXTBUTTON name="Add Area text button" id="b6820308eb03f341" memberName="addExciterTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 80 88 24" posRelativeY="cc3bdf8d18c3f428"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Add Exciter"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete Exciter text button" id="11692d0648a2e8a4" memberName="deleteExciterTextButton"
              virtualName="" explicitFocusOrder="0" pos="104 80 88 24" posRelativeY="cc3bdf8d18c3f428"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="Delete"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Scene Name Label" id="fbdd06d6ea5f9471" memberName="sceneNameLabel"
         virtualName="" explicitFocusOrder="0" pos="15 63 56 24" posRelativeY="4250d5155a80be70"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Name:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="Scene Name text editor" id="fd7eace3e677fc36" memberName="sceneNameTextEditor"
              virtualName="" explicitFocusOrder="0" pos="64 72 128 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="Exciters Edition button" id="854f1e6b59cc6866" memberName="excitersEditionButton"
                virtualName="" explicitFocusOrder="0" pos="16 20 176 24" posRelativeY="cc3bdf8d18c3f428"
                txtcol="ff000000" buttonText="Edit exciters and play!" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="Show Info text button" id="91eae70b2ba9c50a" memberName="showInfoTextButton"
              virtualName="" explicitFocusOrder="0" pos="16 8Rr 176 20" posRelativeY="97c294b92cbc0a85"
              bgColOff="fff0f0f0" bgColOn="ffffffff" textCol="ff000000" buttonText="[+]    Show context information      "
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="Exciters Constraint button" id="2223a0663bb90743" memberName="excitersConstraintButton"
                virtualName="" explicitFocusOrder="0" pos="16 48 176 24" posRelativeY="cc3bdf8d18c3f428"
                tooltip="Free exciters" txtcol="ff000000" buttonText="No-constraint edition"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

