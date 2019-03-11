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
#include "OSCRecorder.h"
//[/Headers]

#include "OSCRecorderIntroComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCRecorderIntroComponent::OSCRecorderIntroComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("Continue"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff097c2a));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    introTextEditor.reset (new TextEditor ("intro text editor"));
    addAndMakeVisible (introTextEditor.get());
    introTextEditor->setMultiLine (true);
    introTextEditor->setReturnKeyStartsNewLine (true);
    introTextEditor->setReadOnly (true);
    introTextEditor->setScrollbarsShown (false);
    introTextEditor->setCaretVisible (false);
    introTextEditor->setPopupMenuEnabled (false);
    introTextEditor->setColour (TextEditor::backgroundColourId, Colour (0x00495358));
    introTextEditor->setColour (TextEditor::highlightColourId, Colour (0x00e0ec65));
    introTextEditor->setColour (TextEditor::outlineColourId, Colour (0xff565656));
    introTextEditor->setText (String());


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1000, 700);


    //[Constructor] You can add your own custom stuff here..

    // Custom Fonts (bigger... for low/medium quality screens)
    Font textEditorFont = introTextEditor->getFont(); // copy
    textEditorFont.setHeight(22.0);
    introTextEditor->setFont(textEditorFont);

    //[/Constructor]
}

OSCRecorderIntroComponent::~OSCRecorderIntroComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    finishedButton = nullptr;
    introTextEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCRecorderIntroComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff111111));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCRecorderIntroComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 96, 500, 80);
    introTextEditor->setBounds ((getWidth() / 2) - ((getWidth() - 80) / 2), 40, getWidth() - 80, getHeight() - 200);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCRecorderIntroComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == finishedButton.get())
    {
        //[UserButtonCode_finishedButton] -- add your button handler code here..
        if (userQuestionsManager)
            userQuestionsManager->OnIntroFinished(this);
        else
            assert(false); // the listener should be set at this point...
        //[/UserButtonCode_finishedButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OSCRecorderIntroComponent::SetUserQuestionsManager(UserQuestionsManager* _manager)
{
    userQuestionsManager = _manager;
}

void OSCRecorderIntroComponent::SetMainText(const char* mainText)
{
    introTextEditor->setText(CharPointer_UTF8(mainText));
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCRecorderIntroComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="1000" initialHeight="700">
  <BACKGROUND backgroundColour="ff111111"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 96R 500 80" bgColOff="ff097c2a"
              bgColOn="ff252525" buttonText="Continue" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTEDITOR name="intro text editor" id="6f5b21f19397a4f5" memberName="introTextEditor"
              virtualName="" explicitFocusOrder="0" pos="0.5Cc 40 80M 200M"
              bkgcol="495358" hilitecol="e0ec65" outlinecol="ff565656" initialText=""
              multiline="1" retKeyStartsLine="1" readonly="1" scrollbars="0"
              caret="0" popupmenu="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

