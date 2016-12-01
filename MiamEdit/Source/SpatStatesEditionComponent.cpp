/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SpatStateFadersDisplayComponent.h"
//[/Headers]

#include "SpatStatesEditionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SpatStatesEditionComponent::SpatStatesEditionComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (spatStateGroupComponent = new GroupComponent ("Spat state group component",
                                                                     TRANS("Spatialisation state")));

    addAndMakeVisible (addSpatStateTextButton = new TextButton ("Add state text button"));
    addSpatStateTextButton->setButtonText (TRANS("Add State"));
    addSpatStateTextButton->setConnectedEdges (Button::ConnectedOnRight);
    addSpatStateTextButton->addListener (this);
    addSpatStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    addSpatStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (deleteSpatStateTextButton = new TextButton ("Delete spat state text button"));
    deleteSpatStateTextButton->setButtonText (TRANS("Delete"));
    deleteSpatStateTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    deleteSpatStateTextButton->addListener (this);
    deleteSpatStateTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    deleteSpatStateTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (stateUpTextButton = new TextButton ("State up text button"));
    stateUpTextButton->setButtonText (TRANS("Move Up"));
    stateUpTextButton->setConnectedEdges (Button::ConnectedOnRight);
    stateUpTextButton->addListener (this);
    stateUpTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    stateUpTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (stateDownTextButton = new TextButton ("State down text button"));
    stateDownTextButton->setButtonText (TRANS("Down"));
    stateDownTextButton->setConnectedEdges (Button::ConnectedOnLeft);
    stateDownTextButton->addListener (this);
    stateDownTextButton->setColour (TextButton::buttonColourId, Colour (0x33000000));
    stateDownTextButton->setColour (TextButton::buttonOnColourId, Colours::white);

    addAndMakeVisible (linksInfoLabel = new Label ("Links info label",
                                                   TRANS("Linked to 0 area")));
    linksInfoLabel->setFont (Font (15.00f, Font::italic));
    linksInfoLabel->setJustificationType (Justification::centred);
    linksInfoLabel->setEditable (false, false, false);
    linksInfoLabel->setColour (TextEditor::textColourId, Colours::black);
    linksInfoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (renameLabel = new Label ("Rename label",
                                                TRANS("Rename:")));
    renameLabel->setFont (Font (15.00f, Font::plain));
    renameLabel->setJustificationType (Justification::centredLeft);
    renameLabel->setEditable (false, false, false);
    renameLabel->setColour (TextEditor::textColourId, Colours::black);
    renameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (stateNameTextEditor = new TextEditor ("State name text editor"));
    stateNameTextEditor->setMultiLine (false);
    stateNameTextEditor->setReturnKeyStartsNewLine (false);
    stateNameTextEditor->setReadOnly (false);
    stateNameTextEditor->setScrollbarsShown (true);
    stateNameTextEditor->setCaretVisible (true);
    stateNameTextEditor->setPopupMenuEnabled (true);
    stateNameTextEditor->setText (String());

    addAndMakeVisible (spatStatesComboBox = new ComboBox ("Spat states combo box"));
    spatStatesComboBox->setEditableText (false);
    spatStatesComboBox->setJustificationType (Justification::centredLeft);
    spatStatesComboBox->setTextWhenNothingSelected (String());
    spatStatesComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    spatStatesComboBox->addItem (TRANS("-1 undefined"), 1);
    spatStatesComboBox->addListener (this);


    //[UserPreSize]
    
    // FADERS DISPLAY ONLY
    fadersDisplayComponent = new SpatStateFadersDisplayComponent();
    addChildComponent(fadersDisplayComponent);
    // MATRIX VIEW MISSING HERE
    
    //[/UserPreSize]

    setSize (1024, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SpatStatesEditionComponent::~SpatStatesEditionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    spatStateGroupComponent = nullptr;
    addSpatStateTextButton = nullptr;
    deleteSpatStateTextButton = nullptr;
    stateUpTextButton = nullptr;
    stateDownTextButton = nullptr;
    linksInfoLabel = nullptr;
    renameLabel = nullptr;
    stateNameTextEditor = nullptr;
    spatStatesComboBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    fadersDisplayComponent = nullptr;
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
        fadersDisplayComponent->setVisible(false);
        fadersDisplayComponent->setEnabled(false);
    }
    else if (editionManager->GetSpatType() == Miam::SpatType::Volumes1d)
    {
        fadersDisplayComponent->setVisible(true);
        fadersDisplayComponent->setEnabled(true);
    }
    else
        throw std::runtime_error("Édition de spatialisation par matrice non-implémentée pour l'instant...");
    
    //[/UserPaint]
}

void SpatStatesEditionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    spatStateGroupComponent->setBounds (8, 8, getWidth() - 16, 64);
    addSpatStateTextButton->setBounds (8 + 8, 8 + 24, 80, 24);
    deleteSpatStateTextButton->setBounds (8 + 88, 8 + 24, 80, 24);
    stateUpTextButton->setBounds ((((8 + 88) + 80 - -8) + proportionOfWidth (0.2500f) - -8) + proportionOfWidth (0.1299f) - -8, 8 + 24, 72, 24);
    stateDownTextButton->setBounds (((((8 + 88) + 80 - -8) + proportionOfWidth (0.2500f) - -8) + proportionOfWidth (0.1299f) - -8) + 72, 8 + 24, 72, 24);
    linksInfoLabel->setBounds (((8 + 88) + 80 - -8) + proportionOfWidth (0.2500f) - -8, 8 + 24, proportionOfWidth (0.1299f), 24);
    renameLabel->setBounds ((((((8 + 88) + 80 - -8) + proportionOfWidth (0.2500f) - -8) + proportionOfWidth (0.1299f) - -8) + 72) + 72 - -11, 8 + 24, 64, 24);
    stateNameTextEditor->setBounds (((((((8 + 88) + 80 - -8) + proportionOfWidth (0.2500f) - -8) + proportionOfWidth (0.1299f) - -8) + 72) + 72 - -11) + 64, 32, 88, 24);
    spatStatesComboBox->setBounds ((8 + 88) + 80 - -8, 32, proportionOfWidth (0.2500f), 24);
    //[UserResized] Add your own custom resize handling here..

    // The last text editor (for name editing) simply fills the remaining space
    // to the right of the group parent (spatStateGroupComponent)
    Rectangle<int> currentBounds = stateNameTextEditor->getBounds();
    currentBounds.setRight(spatStateGroupComponent->getRight() - 8);
    stateNameTextEditor->setBounds(currentBounds);
    
    // The child(ren) display(s)
    currentBounds = this->getLocalBounds();
    currentBounds.removeFromTop(spatStateGroupComponent->getBottom());
    currentBounds.reduce(8, 8); // 8 px margin all around
    fadersDisplayComponent->setBounds(currentBounds);

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
        //[/UserComboBoxCode_spatStatesComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SpatStatesEditionComponent::CompleteInitialization(SpatStatesEditionManager* _editionManager)
{
    editionManager = _editionManager;
    fadersDisplayComponent->CompleteInitialization(editionManager);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpatStatesEditionComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="1024" initialHeight="600">
  <BACKGROUND backgroundColour="ffd9d9d9"/>
  <GROUPCOMPONENT name="Spat state group component" id="4250d5155a80be70" memberName="spatStateGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 8 16M 64" title="Spatialisation state"/>
  <TEXTBUTTON name="Add state text button" id="47bebc9d3a03780d" memberName="addSpatStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="8 24 80 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              buttonText="Add State" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Delete spat state text button" id="5f4e8653b868a323" memberName="deleteSpatStateTextButton"
              virtualName="" explicitFocusOrder="0" pos="88 24 80 24" posRelativeX="4250d5155a80be70"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              buttonText="Delete" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="State up text button" id="43b96ebd16bb5586" memberName="stateUpTextButton"
              virtualName="" explicitFocusOrder="0" pos="-8R 24 72 24" posRelativeX="3577c0e2ccd44371"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              buttonText="Move Up" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="State down text button" id="e6cf4b99a12776ee" memberName="stateDownTextButton"
              virtualName="" explicitFocusOrder="0" pos="0R 24 72 24" posRelativeX="43b96ebd16bb5586"
              posRelativeY="4250d5155a80be70" bgColOff="33000000" bgColOn="ffffffff"
              buttonText="Down" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Links info label" id="3577c0e2ccd44371" memberName="linksInfoLabel"
         virtualName="" explicitFocusOrder="0" pos="-8R 24 12.988% 24"
         posRelativeX="89ad7c0a3be5a39c" posRelativeY="4250d5155a80be70"
         edTextCol="ff000000" edBkgCol="0" labelText="Linked to 0 area"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="1" justification="36"/>
  <LABEL name="Rename label" id="fbdd06d6ea5f9471" memberName="renameLabel"
         virtualName="" explicitFocusOrder="0" pos="-11R 24 64 24" posRelativeX="e6cf4b99a12776ee"
         posRelativeY="4250d5155a80be70" edTextCol="ff000000" edBkgCol="0"
         labelText="Rename:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="State name text editor" id="fd7eace3e677fc36" memberName="stateNameTextEditor"
              virtualName="" explicitFocusOrder="0" pos="0R 32 88 24" posRelativeX="fbdd06d6ea5f9471"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <COMBOBOX name="Spat states combo box" id="89ad7c0a3be5a39c" memberName="spatStatesComboBox"
            virtualName="" explicitFocusOrder="0" pos="-8R 32 25% 24" posRelativeX="5f4e8653b868a323"
            posRelativeY="90b16e3024c520fd" editable="0" layout="33" items="-1 undefined"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
