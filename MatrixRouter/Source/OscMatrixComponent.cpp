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
#include "Presenter.h"
//[/Headers]

#include "OscMatrixComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
OscMatrixComponent::OscMatrixComponent (Presenter* _presenter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    presenter = _presenter;
    //[/Constructor_pre]

    addAndMakeVisible (oscGroupComponent = new GroupComponent ("OSC Group Component",
                                                               TRANS("OSC configuration")));
    oscGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff454545));
    oscGroupComponent->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (udpPortTextEditor = new TextEditor ("UDP Port Text Editor"));
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setText (TRANS("9001"));

    addAndMakeVisible (udpPortLabel = new Label ("UPD Port Label",
                                                 TRANS("UDP port")));
    udpPortLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredLeft);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (Label::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (udpStatusLabel = new Label ("UPD Status Label",
                                                   TRANS("Status : ...")));
    udpStatusLabel->setFont (Font (15.00f, Font::italic));
    udpStatusLabel->setJustificationType (Justification::centredLeft);
    udpStatusLabel->setEditable (false, false, false);
    udpStatusLabel->setColour (Label::textColourId, Colours::black);
    udpStatusLabel->setColour (TextEditor::textColourId, Colours::black);
    udpStatusLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slidersMatrix = new Miam::LabelledMatrixComponent (this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels));
    slidersMatrix->setName ("Labelled Matrix of Sliders");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 300);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

OscMatrixComponent::~OscMatrixComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    oscGroupComponent = nullptr;
    udpPortTextEditor = nullptr;
    udpPortLabel = nullptr;
    udpStatusLabel = nullptr;
    slidersMatrix = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OscMatrixComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffd9d9d9));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OscMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    oscGroupComponent->setBounds (8, 0, getWidth() - 16, 56);
    udpPortTextEditor->setBounds (88, 24, 64, 24);
    udpPortLabel->setBounds (16, 24, 72, 24);
    udpStatusLabel->setBounds (88 + 64 - -8, 24, (getWidth() - 16) - 160, 24);
    slidersMatrix->setBounds (8, 64, getWidth() - 16, getHeight() - 72);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OscMatrixComponent::OnSliderValueChanged(int row, int col, double value)
{
    presenter->OnSliderValueChanged(row, col, value);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OscMatrixComponent" componentName=""
                 parentClasses="public Component, public ISlidersMatrixListener"
                 constructorParams="Presenter* _presenter" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="400" initialHeight="300">
  <BACKGROUND backgroundColour="ffd9d9d9"/>
  <GROUPCOMPONENT name="OSC Group Component" id="dfbb24a51fa3d6c0" memberName="oscGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 0 16M 56" outlinecol="ff454545"
                  textcol="ff000000" title="OSC configuration"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="88 24 64 24" initialText="9001"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="16 24 72 24" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="UDP port" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="UPD Status Label" id="3fd4c40c4d48dfec" memberName="udpStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="-8R 24 160M 24" posRelativeX="e4ef4437203ce19e"
         posRelativeW="dfbb24a51fa3d6c0" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Status : ..." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="1" justification="33"
         typefaceStyle="Italic"/>
  <GENERICCOMPONENT name="Labelled Matrix of Sliders" id="216c3aa5980dcb58" memberName="slidersMatrix"
                    virtualName="" explicitFocusOrder="0" pos="8 64 16M 72M" class="Miam::LabelledMatrixComponent"
                    params="this, JucePlugin_MaxNumInputChannels, JucePlugin_MaxNumOutputChannels"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
