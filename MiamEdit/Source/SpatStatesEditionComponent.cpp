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
#include <regex> // c++11

#include "AudioDefines.h"

#include "MatrixComponent.h"

#include "AppPurpose.h"

//[/Headers]

#include "SpatStatesEditionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
SpatStatesEditionComponent::SpatStatesEditionComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    stateParametersGroupComponent.reset (new GroupComponent ("State Parameters group component",
                                                             TRANS("State parameters")));
    addAndMakeVisible (stateParametersGroupComponent.get());
    stateParametersGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    stateParametersGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    stateEditorGroupComponent.reset (new GroupComponent ("State Editor group component",
                                                         TRANS("State control data")));
    addAndMakeVisible (stateEditorGroupComponent.get());
    stateEditorGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    stateEditorGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    labelledMatrixComponent.reset (new Miam::LabelledMatrixComponent (this, Miam_MaxNumInputs, Miam_MaxNumOutputs));
    addAndMakeVisible (labelledMatrixComponent.get());
    labelledMatrixComponent->setName ("Labelled Matrix component");

    statesListGroupComponent.reset (new GroupComponent ("States list group component",
                                                        TRANS("List of states")));
    addAndMakeVisible (statesListGroupComponent.get());
    statesListGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    statesListGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    statesListGroupComponent->setBounds (0, 4, 331, 84);

    addStateTextButton.reset (new TextButton ("Add state text button"));
    addAndMakeVisible (addStateTextButton.get());
    addStateTextButton->setButtonText (TRANS("Add State"));
    addStateTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addStateTextButton->addListener (this);
    addStateTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    addStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addStateTextButton->setColour (TextButton::textColourOffId, Colours::black);

    deleteStateTextButton.reset (new TextButton ("Delete State text button"));
    addAndMakeVisible (deleteStateTextButton.get());
    deleteStateTextButton->setButtonText (TRANS("Delete"));
    deleteStateTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deleteStateTextButton->addListener (this);
    deleteStateTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    deleteStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteStateTextButton->setColour (TextButton::textColourOffId, Colours::black);

    stateUpTextButton.reset (new TextButton ("State up text button"));
    addAndMakeVisible (stateUpTextButton.get());
    stateUpTextButton->setButtonText (TRANS("Move Up"));
    stateUpTextButton->setConnectedEdges (Button::ConnectedOnRight);
    stateUpTextButton->addListener (this);
    stateUpTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    stateUpTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    stateUpTextButton->setColour (TextButton::textColourOffId, Colours::black);

    stateDownTextButton.reset (new TextButton ("State down text button"));
    addAndMakeVisible (stateDownTextButton.get());
    stateDownTextButton->setButtonText (TRANS("Down"));
    stateDownTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    stateDownTextButton->addListener (this);
    stateDownTextButton->setColour (TextButton::buttonColourId, Colour (0xfff0f0f0));
    stateDownTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    stateDownTextButton->setColour (TextButton::textColourOffId, Colours::black);

    linksInfoLabel.reset (new Label ("Links info label",
                                     TRANS("Linked to ? area")));
    addAndMakeVisible (linksInfoLabel.get());
    linksInfoLabel->setFont (Font (15.00f, Font::italic));
    linksInfoLabel->setJustificationType (Justification::centred);
    linksInfoLabel->setEditable (false, false, false);
    linksInfoLabel->setColour (Label::textColourId, Colours::black);
    linksInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    linksInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    statesComboBox.reset (new ComboBox ("States combo box"));
    addAndMakeVisible (statesComboBox.get());
    statesComboBox->setEditableText (true);
    statesComboBox->setJustificationType (Justification::centredLeft);
    statesComboBox->setTextWhenNothingSelected (String());
    statesComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    statesComboBox->addItem (TRANS("-1 undefined"), 1);
    statesComboBox->addListener (this);

    labelR.reset (new Label ("new label",
                             TRANS("R")));
    addAndMakeVisible (labelR.get());
    labelR->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelR->setJustificationType (Justification::centredLeft);
    labelR->setEditable (false, false, false);
    labelR->setColour (Label::textColourId, Colours::black);
    labelR->setColour (TextEditor::textColourId, Colours::black);
    labelR->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sliderR.reset (new Slider ("Slider R"));
    addAndMakeVisible (sliderR.get());
    sliderR->setRange (0, 255, 1);
    sliderR->setSliderStyle (Slider::LinearHorizontal);
    sliderR->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderR->setColour (Slider::thumbColourId, Colour (0xa0ff0000));
    sliderR->setColour (Slider::textBoxTextColourId, Colours::black);
    sliderR->addListener (this);

    labelG.reset (new Label ("new label",
                             TRANS("G")));
    addAndMakeVisible (labelG.get());
    labelG->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelG->setJustificationType (Justification::centredLeft);
    labelG->setEditable (false, false, false);
    labelG->setColour (Label::textColourId, Colours::black);
    labelG->setColour (TextEditor::textColourId, Colours::black);
    labelG->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sliderG.reset (new Slider ("SliderG"));
    addAndMakeVisible (sliderG.get());
    sliderG->setRange (0, 255, 1);
    sliderG->setSliderStyle (Slider::LinearHorizontal);
    sliderG->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderG->setColour (Slider::thumbColourId, Colour (0xa000ff00));
    sliderG->setColour (Slider::textBoxTextColourId, Colours::black);
    sliderG->addListener (this);

    labelB.reset (new Label ("new label",
                             TRANS("B")));
    addAndMakeVisible (labelB.get());
    labelB->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelB->setJustificationType (Justification::centredLeft);
    labelB->setEditable (false, false, false);
    labelB->setColour (Label::textColourId, Colours::black);
    labelB->setColour (TextEditor::textColourId, Colours::black);
    labelB->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sliderB.reset (new Slider ("Slider B"));
    addAndMakeVisible (sliderB.get());
    sliderB->setRange (0, 255, 1);
    sliderB->setSliderStyle (Slider::LinearHorizontal);
    sliderB->setTextBoxStyle (Slider::TextBoxLeft, false, 40, 20);
    sliderB->setColour (Slider::thumbColourId, Colour (0xa00000ff));
    sliderB->setColour (Slider::textBoxTextColourId, Colours::black);
    sliderB->addListener (this);

    colourVisualisationLabel.reset (new Label ("Colour Visualisation Label",
                                               TRANS("\n")));
    addAndMakeVisible (colourVisualisationLabel.get());
    colourVisualisationLabel->setTooltip (TRANS("The chosen color will be applied to all geometrical shapes linked to this state."));
    colourVisualisationLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    colourVisualisationLabel->setJustificationType (Justification::centredLeft);
    colourVisualisationLabel->setEditable (false, false, false);
    colourVisualisationLabel->setColour (Label::backgroundColourId, Colours::black);
    colourVisualisationLabel->setColour (Label::outlineColourId, Colours::white);
    colourVisualisationLabel->setColour (TextEditor::textColourId, Colours::black);
    colourVisualisationLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sendStateTextButton.reset (new TextButton ("Send State text button"));
    addAndMakeVisible (sendStateTextButton.get());
    sendStateTextButton->setTooltip (TRANS("Sends all current values of this state via OSC messages, to the OSC remote device configured in the \"Configuration\" tab."));
    sendStateTextButton->setButtonText (TRANS("Send state"));
    sendStateTextButton->setConnectedEdges (Button::ConnectedOnRight);
    sendStateTextButton->addListener (this);
    sendStateTextButton->setColour (TextButton::buttonColourId, Colours::black);
    sendStateTextButton->setColour (TextButton::buttonOnColourId, Colours::darkgrey);
    sendStateTextButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    sendZerosTextButton.reset (new TextButton ("Send zeros text button"));
    addAndMakeVisible (sendZerosTextButton.get());
    sendZerosTextButton->setTooltip (TRANS("Sends null values for all parameters of this state, via OSC messages, to the OSC remote device configured in the \"Configuration\" tab."));
    sendZerosTextButton->setButtonText (TRANS("Send zeros"));
    sendZerosTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    sendZerosTextButton->addListener (this);
    sendZerosTextButton->setColour (TextButton::buttonColourId, Colours::black);
    sendZerosTextButton->setColour (TextButton::buttonOnColourId, Colours::darkgrey);
    sendZerosTextButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    matrixInfoLabel1.reset (new Label ("Matrix Info label 1",
                                       TRANS("Total matrix volume")));
    addAndMakeVisible (matrixInfoLabel1.get());
    matrixInfoLabel1->setTooltip (TRANS("The two volumes displayed represent the  total volume of the sum of all outputs of the matrix, considering that inputs are all 0 dB signals."));
    matrixInfoLabel1->setFont (Font (15.00f, Font::italic));
    matrixInfoLabel1->setJustificationType (Justification::centredLeft);
    matrixInfoLabel1->setEditable (false, false, false);
    matrixInfoLabel1->setColour (Label::textColourId, Colours::black);
    matrixInfoLabel1->setColour (TextEditor::textColourId, Colours::black);
    matrixInfoLabel1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    matrixInfoLabel2.reset (new Label ("Matrix Info label 2",
                                       TRANS("Correlated inputs: -9.99 dB FS")));
    addAndMakeVisible (matrixInfoLabel2.get());
    matrixInfoLabel2->setTooltip (TRANS("Input signals are considered correlated when they are very resembling and in phase, such as the L/R tracks of a non-spatialised stereo mix."));
    matrixInfoLabel2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    matrixInfoLabel2->setJustificationType (Justification::centredLeft);
    matrixInfoLabel2->setEditable (false, false, false);
    matrixInfoLabel2->setColour (Label::textColourId, Colours::black);
    matrixInfoLabel2->setColour (TextEditor::textColourId, Colours::black);
    matrixInfoLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    matrixInfoLabel3.reset (new Label ("Matrix Info label 3",
                                       TRANS("Decorrelated inputs: -9.99 dB FS")));
    addAndMakeVisible (matrixInfoLabel3.get());
    matrixInfoLabel3->setTooltip (TRANS("Input signals are considered decorrelated when they are out of phase, or not resembling, such as two mono recordings of two different music instruments."));
    matrixInfoLabel3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    matrixInfoLabel3->setJustificationType (Justification::centredLeft);
    matrixInfoLabel3->setEditable (false, false, false);
    matrixInfoLabel3->setColour (Label::textColourId, Colours::black);
    matrixInfoLabel3->setColour (TextEditor::textColourId, Colours::black);
    matrixInfoLabel3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    oscAddressLabel.reset (new Label ("OSC Address label",
                                      TRANS("OSC address")));
    addAndMakeVisible (oscAddressLabel.get());
    oscAddressLabel->setTooltip (TRANS("Enter a valid OSC address, or leave the field empty to use /miem/1, /miem/2, etc. as automatic OSC addresses. "));
    oscAddressLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    oscAddressLabel->setJustificationType (Justification::centredLeft);
    oscAddressLabel->setEditable (false, false, false);
    oscAddressLabel->setColour (Label::textColourId, Colours::black);
    oscAddressLabel->setColour (TextEditor::textColourId, Colours::black);
    oscAddressLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    oscAddressLabel->setBounds (144, 112, 150, 24);

    minLabel.reset (new Label ("Min label",
                               TRANS("Min. value")));
    addAndMakeVisible (minLabel.get());
    minLabel->setTooltip (TRANS("The minimal value that a parameter can reach. Click to edit."));
    minLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    minLabel->setJustificationType (Justification::centredLeft);
    minLabel->setEditable (false, false, false);
    minLabel->setColour (Label::textColourId, Colours::black);
    minLabel->setColour (TextEditor::textColourId, Colours::black);
    minLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    minLabel->setBounds (288, 112, 80, 24);

    maxLabel.reset (new Label ("Max label",
                               TRANS("Max. value")));
    addAndMakeVisible (maxLabel.get());
    maxLabel->setTooltip (TRANS("The maximal value that a parameter can reach. Click to edit."));
    maxLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    maxLabel->setJustificationType (Justification::centredLeft);
    maxLabel->setEditable (false, false, false);
    maxLabel->setColour (Label::textColourId, Colours::black);
    maxLabel->setColour (TextEditor::textColourId, Colours::black);
    maxLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    maxLabel->setBounds (368, 112, 80, 24);

    interpolationCurveLabel.reset (new Label ("Interpolation Curve label",
                                              TRANS("Interpolation curve")));
    addAndMakeVisible (interpolationCurveLabel.get());
    interpolationCurveLabel->setTooltip (TRANS("The type of curve which will be used to compute the interpolation of values for a parameter."));
    interpolationCurveLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    interpolationCurveLabel->setJustificationType (Justification::centredLeft);
    interpolationCurveLabel->setEditable (false, false, false);
    interpolationCurveLabel->setColour (Label::textColourId, Colours::black);
    interpolationCurveLabel->setColour (TextEditor::textColourId, Colours::black);
    interpolationCurveLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    interpolationCurveLabel->setBounds (480, 112, 150, 24);

    valueLabel.reset (new Label ("value label",
                                 TRANS("Parameter value")));
    addAndMakeVisible (valueLabel.get());
    valueLabel->setTooltip (TRANS("The defined value of a parameter, for this particular state."));
    valueLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    valueLabel->setJustificationType (Justification::centredLeft);
    valueLabel->setEditable (false, false, false);
    valueLabel->setColour (Label::textColourId, Colours::black);
    valueLabel->setColour (TextEditor::textColourId, Colours::black);
    valueLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    valueLabel->setBounds (704, 112, 168, 24);


    //[UserPreSize]
    labelledMatrixComponent->SetButtonsListener(this);
    //[/UserPreSize]

    setSize (1024, 600);


    //[Constructor] You can add your own custom stuff here..

    // On sauvegarde le texte générique, mais on précise ici le texte pour spat
    genericStatesListText = statesListGroupComponent->getText();
    genericStateEditorText = stateEditorGroupComponent->getText();
    spatStatesListText = TRANS("Spatialization states list");
    spatStateEditorText = TRANS("Routing matrix for selected state");

    // Tooltips des raccourcis clavier
    // Cmd
    TextUtils::AddShortcutToTooltip(*addStateTextButton.get(),
                                    TextUtils::GetCommandKeyDescription(newStateCmdKey));
    TextUtils::AddShortcutToTooltip(*deleteStateTextButton.get(),
                                    TextUtils::GetCommandKeyDescription(deleteStateCmdKey));
    TextUtils::AddShortcutToTooltip(*sendStateTextButton.get(),
                                    TextUtils::GetCommandKeyDescription(sendStateCmdKey));
    // Combo box : double-shortcut, texte spécial
    String comboBoxDoubleShortcut = TextUtils::GetCommandKeyDescription(previousStateCmdKey);
    comboBoxDoubleShortcut += ", " + TextUtils::GetCommandKeyDescription(nextStateCmdKey);
    TextUtils::AddShortcutToTooltip(*statesComboBox.get(),
                                    comboBoxDoubleShortcut);
    // Cmd + Shift
    TextUtils::AddShortcutToTooltip(*sendZerosTextButton.get(),
                                    TextUtils::GetCommandShiftKeyDescription(sendZerosCmdShiftKey));
    TextUtils::AddShortcutToTooltip(*stateUpTextButton.get(),
                                    TextUtils::GetCommandShiftKeyDescription(stateUpCmdShiftKey));
    TextUtils::AddShortcutToTooltip(*stateDownTextButton.get(),
                                    TextUtils::GetCommandShiftKeyDescription(stateDownCmdShiftKey));

    //[/Constructor]
}

SpatStatesEditionComponent::~SpatStatesEditionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    stateParametersGroupComponent = nullptr;
    stateEditorGroupComponent = nullptr;
    labelledMatrixComponent = nullptr;
    statesListGroupComponent = nullptr;
    addStateTextButton = nullptr;
    deleteStateTextButton = nullptr;
    stateUpTextButton = nullptr;
    stateDownTextButton = nullptr;
    linksInfoLabel = nullptr;
    statesComboBox = nullptr;
    labelR = nullptr;
    sliderR = nullptr;
    labelG = nullptr;
    sliderG = nullptr;
    labelB = nullptr;
    sliderB = nullptr;
    colourVisualisationLabel = nullptr;
    sendStateTextButton = nullptr;
    sendZerosTextButton = nullptr;
    matrixInfoLabel1 = nullptr;
    matrixInfoLabel2 = nullptr;
    matrixInfoLabel3 = nullptr;
    oscAddressLabel = nullptr;
    minLabel = nullptr;
    maxLabel = nullptr;
    interpolationCurveLabel = nullptr;
    valueLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SpatStatesEditionComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

    // NOT OPTIMIZED
    // Actualisation of state's list

    // TO DO LIST OF STATES' UPDATE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!???

    //[/UserPrePaint]

    g.fillAll (Colour (0xffbfbfbf));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SpatStatesEditionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    stateParametersGroupComponent->setBounds (getWidth() - (getWidth() - 339), 4, getWidth() - 339, 84);
    stateEditorGroupComponent->setBounds (0, 96, getWidth() - 0, getHeight() - 96);
    labelledMatrixComponent->setBounds (0 + 8, 96 + 40, (getWidth() - 0) - 16, (getHeight() - 96) - 48);
    addStateTextButton->setBounds (0 + 8, 4 + 20, 80, 24);
    deleteStateTextButton->setBounds (0 + 88, 4 + 20, 80, 24);
    stateUpTextButton->setBounds (0 + 331 - 154, 4 + 20, 72, 24);
    stateDownTextButton->setBounds (0 + 331 - 82, 4 + 20, 74, 24);
    linksInfoLabel->setBounds ((getWidth() - (getWidth() - 339)) + 13, 4 + 20, (getWidth() - 339) - 520, 24);
    statesComboBox->setBounds (0 + 8, 56, 331 - 16, 24);
    labelR->setBounds (getWidth() - 258, 20, 24, 24);
    sliderR->setBounds (getWidth() - 240, 24, 158, 16);
    labelG->setBounds (getWidth() - 258, 40, 24, 24);
    sliderG->setBounds (getWidth() - 240, 44, 158, 16);
    labelB->setBounds (getWidth() - 257, 60, 24, 24);
    sliderB->setBounds (getWidth() - 240, 64, 158, 16);
    colourVisualisationLabel->setBounds ((getWidth() - (getWidth() - 339)) + (getWidth() - 339) - 80, 24, 72, 56);
    sendStateTextButton->setBounds ((getWidth() - (getWidth() - 339)) + 8, 4 + 52, 80, 24);
    sendZerosTextButton->setBounds ((getWidth() - (getWidth() - 339)) + 88, 4 + 52, 88, 24);
    matrixInfoLabel1->setBounds ((getWidth() - (getWidth() - 339)) + (getWidth() - 339) - 502, 4 + 12, 245, 24);
    matrixInfoLabel2->setBounds ((getWidth() - (getWidth() - 339)) + (getWidth() - 339) - 502, 4 + 32, 245, 24);
    matrixInfoLabel3->setBounds ((getWidth() - (getWidth() - 339)) + (getWidth() - 339) - 502, 4 + 52, 245, 24);
    //[UserResized] Add your own custom resize handling here..

    if (editionManager)
    {
        // Columns labels visible for GenCon only
        oscAddressLabel->setVisible(editionManager->GetSessionPurpose() == AppPurpose::GenericController);
        minLabel->setVisible(editionManager->GetSessionPurpose() == AppPurpose::GenericController);
        maxLabel->setVisible(editionManager->GetSessionPurpose() == AppPurpose::GenericController);
        interpolationCurveLabel->setVisible(editionManager->GetSessionPurpose() == AppPurpose::GenericController);
        valueLabel->setVisible(editionManager->GetSessionPurpose() == AppPurpose::GenericController);

        // Now, the labelled matrix has properly replaced its elements.
        if (editionManager->GetSessionPurpose() == AppPurpose::GenericController)
        {
            const int leftMargin = 24;
            oscAddressLabel->setTopLeftPosition(labelledMatrixComponent->GetOscAddressPositionX()
                                                + leftMargin,
                                                oscAddressLabel->getY());
            minLabel->setTopLeftPosition(labelledMatrixComponent->GetMinimaPositionX()
                                         + leftMargin,
                                         minLabel->getY());
            maxLabel->setTopLeftPosition(labelledMatrixComponent->GetMaximaPositionX()
                                         + leftMargin,
                                         maxLabel->getY());
            interpolationCurveLabel->setTopLeftPosition(labelledMatrixComponent->GetInterpolationCurvesPositionX()
                                                        + leftMargin,
                                                        interpolationCurveLabel->getY());
            valueLabel->setTopLeftPosition(labelledMatrixComponent->GetParametersValuesPositionX()
                                           + leftMargin,
                                           valueLabel->getY());
        }
    }

    //[/UserResized]
}

void SpatStatesEditionComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == addStateTextButton.get())
    {
        //[UserButtonCode_addStateTextButton] -- add your button handler code here..
        editionManager->OnAddState();
        //[/UserButtonCode_addStateTextButton]
    }
    else if (buttonThatWasClicked == deleteStateTextButton.get())
    {
        //[UserButtonCode_deleteStateTextButton] -- add your button handler code here..
        editionManager->OnDeleteSelectedState();
        //[/UserButtonCode_deleteStateTextButton]
    }
    else if (buttonThatWasClicked == stateUpTextButton.get())
    {
        //[UserButtonCode_stateUpTextButton] -- add your button handler code here..
        editionManager->OnMoveSelectedStateUp();
        //[/UserButtonCode_stateUpTextButton]
    }
    else if (buttonThatWasClicked == stateDownTextButton.get())
    {
        //[UserButtonCode_stateDownTextButton] -- add your button handler code here..
        editionManager->OnMoveSelectedStateDown();
        //[/UserButtonCode_stateDownTextButton]
    }
    else if (buttonThatWasClicked == sendStateTextButton.get())
    {
        //[UserButtonCode_sendStateTextButton] -- add your button handler code here..
        editionManager->OnSendState();
        //[/UserButtonCode_sendStateTextButton]
    }
    else if (buttonThatWasClicked == sendZerosTextButton.get())
    {
        //[UserButtonCode_sendZerosTextButton] -- add your button handler code here..
        editionManager->OnSendZeros();
        //[/UserButtonCode_sendZerosTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SpatStatesEditionComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == statesComboBox.get())
    {
        //[UserComboBoxCode_statesComboBox] -- add your combo box handling code here..

        // Change might happen when editing the text has just finished ("new" item)
        if (statesComboBox->getSelectedItemIndex() == -1)
        {
            // If a state was selected before...
            if (previousStateIndex >= 0)
            {
                // Using a regex, we remove the number (if existing)
                // from the state's name
                std::string nameToProcess = statesComboBox->getText().toStdString();
                std::regex deleteIndexAndDotRegex("[0-9]{1,}\\. "); // and the space
                std::string processedString
                = std::regex_replace(nameToProcess,
                                     deleteIndexAndDotRegex,
                                     ""); // replacement with an empty string...
                // Actual renaming
                editionManager->OnRenameState(processedString, previousStateIndex);
            }
            else
                statesComboBox->setSelectedItemIndex(-1); // nothing with notif
        }

        // Or when another existing item has just been selected
        else
            editionManager->OnSpatStateSelectedById(GetDisplayedSpatMatrix(), statesComboBox->getSelectedItemIndex());


        //[/UserComboBoxCode_statesComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SpatStatesEditionComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderR.get())
    {
        //[UserSliderCode_sliderR] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderR]
    }
    else if (sliderThatWasMoved == sliderG.get())
    {
        //[UserSliderCode_sliderG] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderG]
    }
    else if (sliderThatWasMoved == sliderB.get())
    {
        //[UserSliderCode_sliderB] -- add your slider handling code here..
        colourSliderMoved();
        //[/UserSliderCode_sliderB]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SpatStatesEditionComponent::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    if (editionManager)
    {
        switch (editionManager->GetSessionPurpose()) {

            case Miam::AppPurpose::None:
                break;

            case Miam::AppPurpose::Spatialisation:
                // textes
                statesListGroupComponent->setText(spatStatesListText);
                stateEditorGroupComponent->setText(spatStateEditorText);
                // format matrice de sliders colorés
                break;

            case Miam::AppPurpose::GenericController:
                // textes
                statesListGroupComponent->setText(genericStatesListText);
                stateEditorGroupComponent->setText(genericStateEditorText);
                // format sliders linéaires simples
                break;

            default:
                throw std::runtime_error("Édition de truc ?? Non défini...");
                break;
        }

        // S'adaptera si nécessaire
        labelledMatrixComponent->SetDisplayPurpose(editionManager->GetSessionPurpose());

        // For labels update
        resized();

        // forced keyboard focus grab, for shortcuts
        if (isVisible())
            Timer::callAfterDelay(100, [this] { this->grabKeyboardFocus(); } );
    }
    //[/UserCode_visibilityChanged]
}

bool SpatStatesEditionComponent::keyPressed (const KeyPress& key)
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
            // Send Zeros
            if ( (key.getKeyCode() == sendZerosCmdShiftKey)
                && sendZerosTextButton->isEnabled() && sendZerosTextButton->isVisible())
                sendZerosTextButton->triggerClick();
            // state Up/Down
            else if ( (key.getKeyCode() == stateUpCmdShiftKey)
                && stateUpTextButton->isEnabled() && stateUpTextButton->isVisible())
                stateUpTextButton->triggerClick();
            else if ( (key.getKeyCode() == stateDownCmdShiftKey)
                     && stateDownTextButton->isEnabled() && stateDownTextButton->isVisible())
                stateDownTextButton->triggerClick();
            // remise à false si vraiment aucune touche ne convenait
            else
                keyWasUsed = false;
        }
        // Send State
        else if ( (key.getKeyCode() == sendStateCmdKey)
                 && sendStateTextButton->isEnabled() && sendStateTextButton->isVisible())
            sendStateTextButton->triggerClick();
        // Add/delete State
        else if ( (key.getKeyCode() == newStateCmdKey)
                 && addStateTextButton->isEnabled() && addStateTextButton->isVisible())
            addStateTextButton->triggerClick();
        else if ( (key.getKeyCode() == deleteStateCmdKey)
                 && deleteStateTextButton->isEnabled() && deleteStateTextButton->isVisible())
            deleteStateTextButton->triggerClick();
        else if ( (key.getKeyCode() == sendStateCmdKey)
                 && sendStateTextButton->isEnabled() && sendStateTextButton->isVisible())
            sendStateTextButton->triggerClick();
        else if ( (key.getKeyCode() == nextStateCmdKey) )
            keyWasUsed = trySelectNextState(true); // downward
        else if ( (key.getKeyCode() == previousStateCmdKey) )
            keyWasUsed = trySelectNextState(false); // upward
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



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpatStatesEditionComponent::CompleteInitialization(SpatStatesEditionManager* _editionManager)
{
    editionManager = _editionManager;
}


void SpatStatesEditionComponent::Reinit()
{
	// if we dont do this, the old selected state remains (even if the list is
	// emptied and re-filled)
	statesComboBox->setText("", NotificationType::dontSendNotification);
	// Full reinit of all interp curves
	labelledMatrixComponent->SetInterpolationCurves(nullptr);
}

void SpatStatesEditionComponent::UpdateStatesList(std::vector< std::shared_ptr<ControlState<double>> > &newSpatStates)
{
    // Empties the combo box at first
    statesComboBox->clear();

    // Addition of items==names one by one
    for (size_t i=0 ; i<newSpatStates.size() ; i++)
        statesComboBox->addItem(newSpatStates[i]->GetName(), (int)i+1); // Id == Index+1
    // Normally : no item selected at this point
    //editionManager->OnSpatStateSelectedById(spatStatesComboBox->getSelectedItemIndex());
    stateUpTextButton->setEnabled(false);
    stateDownTextButton->setEnabled(false);
}


// - - - - - Colours - - - - -
void SpatStatesEditionComponent::SetAreaColourValue(juce::Colour colour)
{
    sliderR->setValue(colour.getRed());
    sliderG->setValue(colour.getGreen());
    sliderB->setValue(colour.getBlue());
}
void SpatStatesEditionComponent::colourSliderMoved()
{
    Colour newColour = Colour((uint8)sliderR->getValue(),
                              (uint8)sliderG->getValue(),
                              (uint8)sliderB->getValue());

    // il faudra transmettre l'évènement à l'état puis aux formes liées
    editionManager->OnColourChanged(newColour);

    // actualisation directe de la prévisualisation de la couleur
    colourVisualisationLabel->setColour(Label::backgroundColourId, newColour);
}

void SpatStatesEditionComponent::OnSliderValueChanged(int row, int col, double value)
{
    editionManager->OnMatrixValueChanged(row, col, value);
}
void SpatStatesEditionComponent::OnMatrixButtonClicked(int row, int col, std::string matrixText, double matrixValue)
{
    editionManager->OnMatrixButtonClicked(row, col, matrixText, matrixValue);
}
void SpatStatesEditionComponent::OnMatrixZeroed()
{
    editionManager->UpdateView();
}


void SpatStatesEditionComponent::SelectAndUpdateState(int stateIndex, std::string infoText, std::shared_ptr<ControlMatrix<double>> newSpatMatrix, const Colour& stateColour,
    std::shared_ptr<BasicInterpCurves> newInterpCurves)
{
    // We keep here this copy of the model internal matrix
    spatMatrix = newSpatMatrix;

    // Internal updates
    statesComboBox->setSelectedItemIndex(stateIndex, NotificationType::dontSendNotification);
    previousStateIndex = statesComboBox->getSelectedItemIndex();

    // Text and colour elements
    UpdateLinksLabel(infoText);
    setColour(stateColour);

    // ACTUAL DATA UPDATES
    // interp curves before matrix data
    labelledMatrixComponent->SetInterpolationCurves(newInterpCurves);
    updateMatrix();
    
    
    // Buttons enabled state (should be PRESENTER code normally....)
    bool isAnyStateSelected = statesComboBox->getSelectedItemIndex() != -1;
    deleteStateTextButton->setEnabled(isAnyStateSelected);
    stateUpTextButton->setEnabled(isAnyStateSelected);
    stateDownTextButton->setEnabled(isAnyStateSelected);
    if (isAnyStateSelected)
    {
        // up/down may be deactivated depending on state's count
        if (statesComboBox->getSelectedItemIndex() == 0)
            stateUpTextButton->setEnabled(false);
        else if (statesComboBox->getSelectedItemIndex()
                 == statesComboBox->getNumItems()-1)
            stateDownTextButton->setEnabled(false);
    }
    setParametersGroupEnabled(isAnyStateSelected);
}

void SpatStatesEditionComponent::UpdateLinksLabel(const std::string& infoText)
{
    linksInfoLabel->setText(infoText, NotificationType::dontSendNotification);
}
void SpatStatesEditionComponent::UpdateMatrixData(const std::string& volume1, const std::string& volume2)
{
    // DIsplay reserved to SPAT at the moment
    matrixInfoLabel2->setText(std::string("Correlated inputs: ") + volume1 + std::string(" dB FS"), NotificationType::dontSendNotification);
    matrixInfoLabel3->setText(std::string("Decorrelated inputs: ") + volume2 + std::string(" dB FS"), NotificationType::dontSendNotification);
}
void SpatStatesEditionComponent::SetVisibleMatrixData(bool shouldBeVisible)
{
    matrixInfoLabel1->setVisible(shouldBeVisible);
    matrixInfoLabel2->setVisible(shouldBeVisible);
    matrixInfoLabel3->setVisible(shouldBeVisible);
}

void SpatStatesEditionComponent::updateMatrix()
{
    // matrix data sent from the most recent that this class got from model
    if (spatMatrix) // may not exist if no state is selected
        labelledMatrixComponent->GetMatrixComponent()->SetSpatMatrix(spatMatrix);
    else // forced null matrix update
        labelledMatrixComponent->GetMatrixComponent()->SetSpatMatrix(std::make_shared<ControlMatrix<double>>());

    // Graphical attributes
    // if a valid state is selected
    if (statesComboBox->getSelectedItemIndex() >= 0)
        labelledMatrixComponent->SetActiveSliders(inputsCount, outputsCount);
    // else : disabled matrix
    else
        labelledMatrixComponent->SetActiveSliders(0,0);
}

void SpatStatesEditionComponent::setParametersGroupEnabled(bool shouldBeEnabled)
{
    labelR->setEnabled(shouldBeEnabled);
    sliderR->setEnabled(shouldBeEnabled);
    labelG->setEnabled(shouldBeEnabled);
    sliderG->setEnabled(shouldBeEnabled);
    labelB->setEnabled(shouldBeEnabled);
    sliderB->setEnabled(shouldBeEnabled);
    colourVisualisationLabel->setEnabled(shouldBeEnabled);
    // Colour set to black if disabled
    if (! shouldBeEnabled)
    {
        // Special "disabled" black == actual grey
        colourVisualisationLabel->setColour(Label::backgroundColourId, Colours::grey);
    }
}
void SpatStatesEditionComponent::setColour(const Colour& stateColour)
{
    sliderR->setValue((double)stateColour.getRed(), NotificationType::dontSendNotification);
    sliderG->setValue((double)stateColour.getGreen(), NotificationType::dontSendNotification);
    sliderB->setValue((double)stateColour.getBlue(), NotificationType::dontSendNotification);
    colourVisualisationLabel->setColour(Label::backgroundColourId, stateColour);
}
bool SpatStatesEditionComponent::trySelectNextState(bool shouldGoDownward)
{
    int targetItemIndex = statesComboBox->getSelectedItemIndex();
    targetItemIndex += shouldGoDownward ? (1) : (-1);
    if ( (targetItemIndex < 0) || (statesComboBox->getNumItems() <= targetItemIndex) )
        return false;
    else
    {
        statesComboBox->setSelectedItemIndex(targetItemIndex);
        return true;
    }
}

void SpatStatesEditionComponent::SetInsOutsCount(int _inputsCount, int _outputsCount)
{
    // Backup
    inputsCount = _inputsCount;
    outputsCount = _outputsCount;

    // Applying of changes
    updateMatrix();
}
void SpatStatesEditionComponent::SetInOutNames(InOutChannelsName &channelsName)
{
    labelledMatrixComponent->SetChannelsNames(channelsName);
}
void SpatStatesEditionComponent::SetInOutNamesDisplayed(bool areInputNamesVisible, bool areOutputNamesVisible)
{
    labelledMatrixComponent->SetInputNamesVisible(areInputNamesVisible);
    labelledMatrixComponent->SetOutputNamesVisible(areOutputNamesVisible);
}
std::shared_ptr<ControlMatrix<double>> SpatStatesEditionComponent::GetDisplayedSpatMatrix()
{
    return labelledMatrixComponent->GetMatrixComponent()->GetSpatMatrix();
}
void SpatStatesEditionComponent::AllowKeyboardEdition(bool allow)
{
    labelledMatrixComponent->GetMatrixComponent()->SetSlidersTextBoxesAreEditable(allow);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpatStatesEditionComponent"
                 componentName="" parentClasses="public Component, public ISlidersMatrixListener, public IMatrixButtonListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="1024"
                 initialHeight="600">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffbfbfbf"/>
  <GROUPCOMPONENT name="State Parameters group component" id="9d63d9acaf1299f6"
                  memberName="stateParametersGroupComponent" virtualName="" explicitFocusOrder="0"
                  pos="0Rr 4 339M 84" posRelativeH="4250d5155a80be70" outlinecol="ff454545"
                  textcol="ff000000" title="State parameters"/>
  <GROUPCOMPONENT name="State Editor group component" id="1b9d22beb5fc6bfd" memberName="stateEditorGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 96 0M 96M" outlinecol="ff454545"
                  textcol="ff000000" title="State control data"/>
  <GENERICCOMPONENT name="Labelled Matrix component" id="d34fbdc233d627af" memberName="labelledMatrixComponent"
                    virtualName="" explicitFocusOrder="0" pos="8 40 16M 48M" posRelativeX="1b9d22beb5fc6bfd"
                    posRelativeY="1b9d22beb5fc6bfd" posRelativeW="1b9d22beb5fc6bfd"
                    posRelativeH="1b9d22beb5fc6bfd" class="Miam::LabelledMatrixComponent"
                    params="this, Miam_MaxNumInputs, Miam_MaxNumOutputs"/>
  <GROUPCOMPONENT name="States list group component" id="4250d5155a80be70" memberName="statesListGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 4 331 84" outlinecol="ff454545"
                  textcol="ff000000" title="List of states"/>
  <TEXTBUTTON name="Add state text button" id="47bebc9d3a03780d" memberName="addStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="8 20 80 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="fff0f0f0" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Add State" connectedEdges="2"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete State text button" id="5f4e8653b868a323" memberName="deleteStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="88 20 80 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="fff0f0f0" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Delete" connectedEdges="1" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="State up text button" id="43b96ebd16bb5586" memberName="stateUpTextButton"
              virtualName="" explicitFocusOrder="0" pos="154R 20 72 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="fff0f0f0" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Move Up" connectedEdges="2" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="State down text button" id="e6cf4b99a12776ee" memberName="stateDownTextButton"
              virtualName="" explicitFocusOrder="0" pos="82R 20 74 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="fff0f0f0" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Down" connectedEdges="1" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="Links info label" id="3577c0e2ccd44371" memberName="linksInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="13 20 520M 24" posRelativeX="9d63d9acaf1299f6"
         posRelativeY="4250d5155a80be70" posRelativeW="9d63d9acaf1299f6"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Linked to ? area"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="1" justification="36" typefaceStyle="Italic"/>
  <COMBOBOX name="States combo box" id="89ad7c0a3be5a39c" memberName="statesComboBox"
            virtualName="" explicitFocusOrder="0" pos="8 56 16M 24" posRelativeX="4250d5155a80be70"
            posRelativeY="90b16e3024c520fd" posRelativeW="4250d5155a80be70"
            editable="1" layout="33" items="-1 undefined" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="893f3de478724dd" memberName="labelR" virtualName=""
         explicitFocusOrder="0" pos="258R 20 24 24" posRelativeY="87d416270d41f58c"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="R"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="Slider R" id="c86e71b3772d49e9" memberName="sliderR" virtualName=""
          explicitFocusOrder="0" pos="240R 24 158 16" posRelativeY="87d416270d41f58c"
          thumbcol="a0ff0000" textboxtext="ff000000" min="0.0" max="255.0"
          int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="d31faee34d487318" memberName="labelG" virtualName=""
         explicitFocusOrder="0" pos="258R 40 24 24" posRelativeY="87d416270d41f58c"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="G"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="SliderG" id="8455ecc13f47ec08" memberName="sliderG" virtualName=""
          explicitFocusOrder="0" pos="240R 44 158 16" posRelativeY="87d416270d41f58c"
          thumbcol="a000ff00" textboxtext="ff000000" min="0.0" max="255.0"
          int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="ae5abc145d9184cf" memberName="labelB" virtualName=""
         explicitFocusOrder="0" pos="257R 60 24 24" posRelativeY="87d416270d41f58c"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="B"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="Slider B" id="15aa4b94476e3563" memberName="sliderB" virtualName=""
          explicitFocusOrder="0" pos="240R 64 158 16" posRelativeY="87d416270d41f58c"
          thumbcol="a00000ff" textboxtext="ff000000" min="0.0" max="255.0"
          int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Colour Visualisation Label" id="7c0151d2e33a36b5" memberName="colourVisualisationLabel"
         virtualName="" explicitFocusOrder="0" pos="80R 24 72 56" posRelativeX="9d63d9acaf1299f6"
         tooltip="The chosen color will be applied to all geometrical shapes linked to this state."
         bkgCol="ff000000" outlineCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="Send State text button" id="cb3b69496db256cf" memberName="sendStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="8 52 80 24" posRelativeX="9d63d9acaf1299f6"
              posRelativeY="4250d5155a80be70" tooltip="Sends all current values of this state via OSC messages, to the OSC remote device configured in the &quot;Configuration&quot; tab."
              bgColOff="ff000000" bgColOn="ff555555" textCol="fff0f0f0" buttonText="Send state"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Send zeros text button" id="886c4f50da968984" memberName="sendZerosTextButton"
              virtualName="" explicitFocusOrder="0" pos="88 52 88 24" posRelativeX="9d63d9acaf1299f6"
              posRelativeY="4250d5155a80be70" tooltip="Sends null values for all parameters of this state, via OSC messages, to the OSC remote device configured in the &quot;Configuration&quot; tab."
              bgColOff="ff000000" bgColOn="ff555555" textCol="fff0f0f0" buttonText="Send zeros"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Matrix Info label 1" id="b0451e4b253fe39" memberName="matrixInfoLabel1"
         virtualName="" explicitFocusOrder="0" pos="502R 12 245 24" posRelativeX="9d63d9acaf1299f6"
         posRelativeY="4250d5155a80be70" posRelativeW="9d63d9acaf1299f6"
         tooltip="The two volumes displayed represent the  total volume of the sum of all outputs of the matrix, considering that inputs are all 0 dB signals."
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Total matrix volume"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <LABEL name="Matrix Info label 2" id="5cfce6441fd18d14" memberName="matrixInfoLabel2"
         virtualName="" explicitFocusOrder="0" pos="502R 32 245 24" posRelativeX="9d63d9acaf1299f6"
         posRelativeY="4250d5155a80be70" posRelativeW="9d63d9acaf1299f6"
         tooltip="Input signals are considered correlated when they are very resembling and in phase, such as the L/R tracks of a non-spatialised stereo mix."
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Correlated inputs: -9.99 dB FS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="Matrix Info label 3" id="2e6b0acdfb779683" memberName="matrixInfoLabel3"
         virtualName="" explicitFocusOrder="0" pos="502R 52 245 24" posRelativeX="9d63d9acaf1299f6"
         posRelativeY="4250d5155a80be70" posRelativeW="9d63d9acaf1299f6"
         tooltip="Input signals are considered decorrelated when they are out of phase, or not resembling, such as two mono recordings of two different music instruments."
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Decorrelated inputs: -9.99 dB FS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="OSC Address label" id="ec34ed0b42663136" memberName="oscAddressLabel"
         virtualName="" explicitFocusOrder="0" pos="144 112 150 24" tooltip="Enter a valid OSC address, or leave the field empty to use /miem/1, /miem/2, etc. as automatic OSC addresses. "
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="OSC address"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="Min label" id="8ff821b0c7b4bbee" memberName="minLabel"
         virtualName="" explicitFocusOrder="0" pos="288 112 80 24" tooltip="The minimal value that a parameter can reach. Click to edit."
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Min. value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="Max label" id="51fc4d52b6f5297e" memberName="maxLabel"
         virtualName="" explicitFocusOrder="0" pos="368 112 80 24" tooltip="The maximal value that a parameter can reach. Click to edit."
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Max. value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="Interpolation Curve label" id="5f84bf03765206ae" memberName="interpolationCurveLabel"
         virtualName="" explicitFocusOrder="0" pos="480 112 150 24" tooltip="The type of curve which will be used to compute the interpolation of values for a parameter."
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Interpolation curve"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="value label" id="12d040c7555f2549" memberName="valueLabel"
         virtualName="" explicitFocusOrder="0" pos="704 112 168 24" tooltip="The defined value of a parameter, for this particular state."
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Parameter value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

