/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "SoundFileViewer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void SoundFileViewer::completeInitialization(SoundFilesManager *m_manager)
{
	manager = m_manager;
}

  void SoundFileViewer::release()
  {
	  manager = nullptr;
  }

void SoundFileViewer::setSoundPath(String m_path)
{
	sampleLabel->setText(m_path, NotificationType::dontSendNotification);
}

void SoundFileViewer::setColourSample(Colour newColor)
{
	colorLabel->setColour(Label::backgroundColourId, newColor);
}

Colour SoundFileViewer::getSampleColour()
{
	return colorLabel->findColour(Label::backgroundColourId);
}

String SoundFileViewer::getSoundPath()
{
	return sampleLabel->getText();
}

//[/MiscUserDefs]

//==============================================================================
SoundFileViewer::SoundFileViewer ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (sampleLabel = new Label ("new label",
                                                TRANS("sample")));
    sampleLabel->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Regular"));
    sampleLabel->setJustificationType (Justification::centredLeft);
    sampleLabel->setEditable (false, false, false);
    sampleLabel->setColour (TextEditor::textColourId, Colours::black);
    sampleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (colorLabel = new Label ("new label",
                                               String()));
    colorLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    colorLabel->setJustificationType (Justification::centredLeft);
    colorLabel->setEditable (false, false, false);
    colorLabel->setColour (Label::backgroundColourId, Colours::blue);
    colorLabel->setColour (TextEditor::textColourId, Colours::black);
    colorLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (deleteButton = new TextButton ("new button"));
    deleteButton->setButtonText (TRANS("D"));
    deleteButton->addListener (this);

    addAndMakeVisible (loadButton = new TextButton ("new button"));
    loadButton->setButtonText (TRANS("L"));
    loadButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 24);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SoundFileViewer::~SoundFileViewer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    sampleLabel = nullptr;
    colorLabel = nullptr;
    deleteButton = nullptr;
    loadButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SoundFileViewer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SoundFileViewer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    sampleLabel->setBounds (0, 0, proportionOfWidth (0.7002f), 24);
    colorLabel->setBounds (proportionOfWidth (0.7498f), 0 + 0, 30, int(24 * 1.0000f));
    deleteButton->setBounds (proportionOfWidth (0.7498f) + 30 - -2, 0, int(30 * 1.0000f), 24);
    loadButton->setBounds ((proportionOfWidth (0.7498f) + 30 - -2) + (int (30 * 1.0000f)) - -2, 0, int (30 * 1.0000f), 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SoundFileViewer::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == deleteButton)
    {
        //[UserButtonCode_deleteButton] -- add your button handler code here..
		setSoundPath(manager->getDefaultPath());
        //[/UserButtonCode_deleteButton]
    }
    else if (buttonThatWasClicked == loadButton)
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
		manager->loadSoundFile(this);
        //[/UserButtonCode_loadButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SoundFileViewer" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="24">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="58e3d52550dbb06c" memberName="sampleLabel"
         virtualName="" explicitFocusOrder="0" pos="0 0 70.023% 24" edTextCol="ff000000"
         edBkgCol="0" labelText="sample" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="20"
         kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="58c61de356dc76f2" memberName="colorLabel"
         virtualName="" explicitFocusOrder="0" pos="74.981% 0 30 100%"
         posRelativeY="58e3d52550dbb06c" posRelativeH="58e3d52550dbb06c"
         bkgCol="ff0000ff" edTextCol="ff000000" edBkgCol="0" labelText=""
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <TEXTBUTTON name="new button" id="c0d80ae5e1d7b066" memberName="deleteButton"
              virtualName="" explicitFocusOrder="0" pos="-2R 0 100% 24" posRelativeX="58c61de356dc76f2"
              posRelativeW="58c61de356dc76f2" buttonText="D" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e037833b40ce0505" memberName="loadButton"
              virtualName="" explicitFocusOrder="0" pos="-2R 0 100% 24" posRelativeX="c0d80ae5e1d7b066"
              posRelativeW="58c61de356dc76f2" buttonText="L" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
