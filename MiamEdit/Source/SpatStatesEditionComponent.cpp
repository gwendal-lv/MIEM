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
#include <regex> // c++11

#include "SpatStateFadersDisplayComponent.h"
#include "AudioDefines.h"

#include "MatrixComponent.h"

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

    addAndMakeVisible (stateEditorGroupComponent = new GroupComponent ("State Editor group component",
                                                                       TRANS("Routing matrix for selected state")));
    stateEditorGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    stateEditorGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (labelledMatrixComponent = new Miam::LabelledMatrixComponent (this, Miam_MaxNumInputs, Miam_MaxNumOutputs));
    labelledMatrixComponent->setName ("Labelled Matrix component");

    addAndMakeVisible (spatStateGroupComponent = new GroupComponent ("Spat state group component",
                                                                     TRANS("Spatialization states list")));
    spatStateGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    spatStateGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (addSpatStateTextButton = new TextButton ("Add state text button"));
    addSpatStateTextButton->setButtonText (TRANS("Add State"));
    addSpatStateTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addSpatStateTextButton->addListener (this);
    addSpatStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addSpatStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    addSpatStateTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (deleteSpatStateTextButton = new TextButton ("Delete spat state text button"));
    deleteSpatStateTextButton->setButtonText (TRANS("Delete"));
    deleteSpatStateTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deleteSpatStateTextButton->addListener (this);
    deleteSpatStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteSpatStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    deleteSpatStateTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (stateUpTextButton = new TextButton ("State up text button"));
    stateUpTextButton->setButtonText (TRANS("Move Up"));
    stateUpTextButton->setConnectedEdges (Button::ConnectedOnRight);
    stateUpTextButton->addListener (this);
    stateUpTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    stateUpTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    stateUpTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (stateDownTextButton = new TextButton ("State down text button"));
    stateDownTextButton->setButtonText (TRANS("Down"));
    stateDownTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    stateDownTextButton->addListener (this);
    stateDownTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    stateDownTextButton->setColour (TextButton::buttonOnColourId, Colours::white);
    stateDownTextButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (linksInfoLabel = new Label ("Links info label",
                                                   TRANS("Linked to ? area")));
    linksInfoLabel->setFont (Font (15.00f, Font::italic));
    linksInfoLabel->setJustificationType (Justification::centred);
    linksInfoLabel->setEditable (false, false, false);
    linksInfoLabel->setColour (Label::textColourId, Colours::black);
    linksInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    linksInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (spatStatesComboBox = new ComboBox ("Spat states combo box"));
    spatStatesComboBox->setEditableText (true);
    spatStatesComboBox->setJustificationType (Justification::centredLeft);
    spatStatesComboBox->setTextWhenNothingSelected (String());
    spatStatesComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    spatStatesComboBox->addItem (TRANS("-1 undefined"), 1);
    spatStatesComboBox->addListener (this);


    //[UserPreSize]

    //[/UserPreSize]

    setSize (1024, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SpatStatesEditionComponent::~SpatStatesEditionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    stateEditorGroupComponent = nullptr;
    labelledMatrixComponent = nullptr;
    spatStateGroupComponent = nullptr;
    addSpatStateTextButton = nullptr;
    deleteSpatStateTextButton = nullptr;
    stateUpTextButton = nullptr;
    stateDownTextButton = nullptr;
    linksInfoLabel = nullptr;
    spatStatesComboBox = nullptr;


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

    g.fillAll (Colour (0xffd9d9d9));

    //[UserPaint] Add your own custom painting code here..

    if (editionManager->GetSpatType() == Miam::SpatType::None)
    {
    }
    else if (editionManager->GetSpatType() == Miam::SpatType::RoutingMatrix)
    {
    }
    else if (editionManager->GetSpatType() == Miam::SpatType::Volumes1d)
    {
        throw std::runtime_error("Édition de spatialisation 1D par volumes de HPs non-implémentée pour l'instant...");
    }
    else
        throw std::runtime_error("Édition de spatialisation par méthode non définie");

    //[/UserPaint]
}

void SpatStatesEditionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    stateEditorGroupComponent->setBounds (0, 60, getWidth() - 0, getHeight() - 60);
    labelledMatrixComponent->setBounds (0 + 8, 60 + 16, (getWidth() - 0) - 16, (getHeight() - 60) - 24);
    spatStateGroupComponent->setBounds (0, 4, getWidth() - 0, 52);
    addSpatStateTextButton->setBounds (0 + 8, 4 + 20, 80, 24);
    deleteSpatStateTextButton->setBounds (0 + 88, 4 + 20, 80, 24);
    stateUpTextButton->setBounds ((((0 + 88) + 80 - -8) + (getWidth() - 476) - -8) + proportionOfWidth (0.1299f) - -8, 4 + 20, 72, 24);
    stateDownTextButton->setBounds (((((0 + 88) + 80 - -8) + (getWidth() - 476) - -8) + proportionOfWidth (0.1299f) - -8) + 72, 4 + 20, 72, 24);
    linksInfoLabel->setBounds (((0 + 88) + 80 - -8) + (getWidth() - 476) - -8, 4 + 20, proportionOfWidth (0.1299f), 24);
    spatStatesComboBox->setBounds ((0 + 88) + 80 - -8, 24, getWidth() - 476, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SpatStatesEditionComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == addSpatStateTextButton)
    {
        //[UserButtonCode_addSpatStateTextButton] -- add your button handler code here..
        //[/UserButtonCode_addSpatStateTextButton]
    }
    else if (buttonThatWasClicked == deleteSpatStateTextButton)
    {
        //[UserButtonCode_deleteSpatStateTextButton] -- add your button handler code here..
        //[/UserButtonCode_deleteSpatStateTextButton]
    }
    else if (buttonThatWasClicked == stateUpTextButton)
    {
        //[UserButtonCode_stateUpTextButton] -- add your button handler code here..
        //[/UserButtonCode_stateUpTextButton]
    }
    else if (buttonThatWasClicked == stateDownTextButton)
    {
        //[UserButtonCode_stateDownTextButton] -- add your button handler code here..
        //[/UserButtonCode_stateDownTextButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SpatStatesEditionComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == spatStatesComboBox)
    {
        //[UserComboBoxCode_spatStatesComboBox] -- add your combo box handling code here..
        // Change might happen when editing the text has just finished ("new" item)
        if (spatStatesComboBox->getSelectedItemIndex() == -1)
        {
            // If a state was selected before...
            if (previousStateIndex >= 0)
            {
                // Using a regex, we remove the number (if existing)
                // from the state's name
                std::string nameToProcess = spatStatesComboBox->getText().toStdString();
                std::regex deleteIndexAndDotRegex("[0-9]{1,}\\. "); // and the space
                std::string processedString
                = std::regex_replace(nameToProcess,
                                     deleteIndexAndDotRegex,
                                     ""); // replacement with an empty string...
                // Actual renaming
                editionManager->OnRenameState(processedString, previousStateIndex);
            }
            else
                spatStatesComboBox->setSelectedItemIndex(-1); // nothing with notif
        }
        // Or when another existing item has just been selected
        else
        {
            editionManager->OnSpatStateSelectedById(GetDisplayedSpatMatrix(), spatStatesComboBox->getSelectedItemIndex());
        }

        //[/UserComboBoxCode_spatStatesComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpatStatesEditionComponent::CompleteInitialization(SpatStatesEditionManager* _editionManager)
{
    editionManager = _editionManager;
}

void SpatStatesEditionComponent::UpdateStatesList(std::vector< std::shared_ptr<SpatState<double>> > &newSpatStates)
{
    // Empties the combo box at first
    spatStatesComboBox->clear();

    // Addition of items==names one by one
    for (size_t i=0 ; i<newSpatStates.size() ; i++)
        spatStatesComboBox->addItem(newSpatStates[i]->GetName(), i+1); // Id == Index+1
    // Normally : no item selected at this point
    //editionManager->OnSpatStateSelectedById(spatStatesComboBox->getSelectedItemIndex());
}
void SpatStatesEditionComponent::SelectAndUpdateState(int stateIndex, std::string infoText, std::shared_ptr<SpatMatrix> newSpatMatrix)
{
    // We keep here this copy of the model internal matrix
    spatMatrix = newSpatMatrix;

    // Buttons state (activated or not)
    bool isAnyStateSelected = spatStatesComboBox->getSelectedItemIndex() != -1;
    stateUpTextButton->setEnabled(isAnyStateSelected);
    stateDownTextButton->setEnabled(isAnyStateSelected);

    // Text elements
    linksInfoLabel->setText(infoText, NotificationType::dontSendNotification);

    // Internal updates
    spatStatesComboBox->setSelectedItemIndex(stateIndex, NotificationType::dontSendNotification);
    previousStateIndex = spatStatesComboBox->getSelectedItemIndex();
    updateMatrix();
}
void SpatStatesEditionComponent::updateMatrix()
{
    // matrix data sent from the most recent that this class got from model
    labelledMatrixComponent->GetMatrixComponent()->SetSpatMatrix(spatMatrix);

    // Graphical attributes
    // if a valid state is selected
    if (spatStatesComboBox->getSelectedItemIndex() >= 0)
        labelledMatrixComponent->GetMatrixComponent()->SetActiveSliders(inputsCount, outputsCount);
    // else : disabled matrix
    else
        labelledMatrixComponent->GetMatrixComponent()->SetActiveSliders(0,0);
}
void SpatStatesEditionComponent::SetInsOutsCount(int _inputsCount, int _outputsCount)
{
    // Backup
    inputsCount = _inputsCount;
    outputsCount = _outputsCount;

    // Applying of changes
    updateMatrix();
}
std::shared_ptr<SpatMatrix> SpatStatesEditionComponent::GetDisplayedSpatMatrix()
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
                 componentName="" parentClasses="public Component, public ISlidersMatrixListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="1024"
                 initialHeight="600">
  <BACKGROUND backgroundColour="ffd9d9d9"/>
  <GROUPCOMPONENT name="State Editor group component" id="1b9d22beb5fc6bfd" memberName="stateEditorGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 60 0M 60M" outlinecol="ff454545"
                  textcol="ff000000" title="Routing matrix for selected state"/>
  <GENERICCOMPONENT name="Labelled Matrix component" id="d34fbdc233d627af" memberName="labelledMatrixComponent"
                    virtualName="" explicitFocusOrder="0" pos="8 16 16M 24M" posRelativeX="1b9d22beb5fc6bfd"
                    posRelativeY="1b9d22beb5fc6bfd" posRelativeW="1b9d22beb5fc6bfd"
                    posRelativeH="1b9d22beb5fc6bfd" class="Miam::LabelledMatrixComponent"
                    params="this, Miam_MaxNumInputs, Miam_MaxNumOutputs"/>
  <GROUPCOMPONENT name="Spat state group component" id="4250d5155a80be70" memberName="spatStateGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 4 0M 52" outlinecol="ff454545"
                  textcol="ff000000" title="Spatialization states list"/>
  <TEXTBUTTON name="Add state text button" id="47bebc9d3a03780d" memberName="addSpatStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="8 20 80 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Add State" connectedEdges="2"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete spat state text button" id="5f4e8653b868a323" memberName="deleteSpatStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="88 20 80 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Delete" connectedEdges="1" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="State up text button" id="43b96ebd16bb5586" memberName="stateUpTextButton"
              virtualName="" explicitFocusOrder="0" pos="-8R 20 72 24" posRelativeX="3577c0e2ccd44371"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Move Up" connectedEdges="2" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="State down text button" id="e6cf4b99a12776ee" memberName="stateDownTextButton"
              virtualName="" explicitFocusOrder="0" pos="0R 20 72 24" posRelativeX="43b96ebd16bb5586"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              textCol="ff000000" buttonText="Down" connectedEdges="1" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="Links info label" id="3577c0e2ccd44371" memberName="linksInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="-8R 20 12.988% 24"
         posRelativeX="89ad7c0a3be5a39c" posRelativeY="4250d5155a80be70"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="Linked to ? area"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="1"
         justification="36" typefaceStyle="Italic"/>
  <COMBOBOX name="Spat states combo box" id="89ad7c0a3be5a39c" memberName="spatStatesComboBox"
            virtualName="" explicitFocusOrder="0" pos="-8R 24 476M 24" posRelativeX="5f4e8653b868a323"
            posRelativeY="90b16e3024c520fd" editable="1" layout="33" items="-1 undefined"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
