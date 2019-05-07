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
//[/Headers]

#include "MyWidgetBox.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MyWidgetBox::MyWidgetBox ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    widgetLabel.reset (new Label ("name",
                                  TRANS("label text")));
    addAndMakeVisible (widgetLabel.get());
    widgetLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    widgetLabel->setJustificationType (Justification::centredLeft);
    widgetLabel->setEditable (false, false, false);
    widgetLabel->setColour (Label::backgroundColourId, Colour (0x3e2500ff));
    widgetLabel->setColour (TextEditor::textColourId, Colours::black);
    widgetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    widgetValue.reset (new TextEditor ("value"));
    addAndMakeVisible (widgetValue.get());
    widgetValue->setMultiLine (false);
    widgetValue->setReturnKeyStartsNewLine (false);
    widgetValue->setReadOnly (true);
    widgetValue->setScrollbarsShown (true);
    widgetValue->setCaretVisible (false);
    widgetValue->setPopupMenuEnabled (true);
    widgetValue->setText (TRANS("0.0"));

    widgetId.reset (new Label ("Id",
                               TRANS("0\n")));
    addAndMakeVisible (widgetId.get());
    widgetId->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    widgetId->setJustificationType (Justification::centredLeft);
    widgetId->setEditable (false, false, false);
    widgetId->setColour (Label::backgroundColourId, Colour (0x5fff0000));
    widgetId->setColour (TextEditor::textColourId, Colours::black);
    widgetId->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (100, 75);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MyWidgetBox::~MyWidgetBox()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    widgetLabel = nullptr;
    widgetValue = nullptr;
    widgetId = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MyWidgetBox::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MyWidgetBox::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    widgetLabel->setBounds (getWidth() - proportionOfWidth (0.8000f), 0, proportionOfWidth (0.8000f), proportionOfHeight (0.5067f));
    widgetValue->setBounds (0, getHeight() - proportionOfHeight (0.5067f), proportionOfWidth (1.0000f), proportionOfHeight (0.5067f));
    widgetId->setBounds (0, 0, proportionOfWidth (0.2000f), proportionOfHeight (0.5067f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MyWidgetBox" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="100" initialHeight="75">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="name" id="8ced25d5f6e57537" memberName="widgetLabel" virtualName=""
         explicitFocusOrder="0" pos="0Rr 0 80% 50.667%" bkgCol="3e2500ff"
         edTextCol="ff000000" edBkgCol="0" labelText="label text" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="value" id="646d236614fdf6f2" memberName="widgetValue" virtualName=""
              explicitFocusOrder="0" pos="0 0Rr 100% 50.667%" initialText="0.0"
              multiline="0" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <LABEL name="Id" id="1632ee7c6f93e94b" memberName="widgetId" virtualName=""
         explicitFocusOrder="0" pos="0 0 20% 50.667%" bkgCol="5fff0000"
         edTextCol="ff000000" edBkgCol="0" labelText="0&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

