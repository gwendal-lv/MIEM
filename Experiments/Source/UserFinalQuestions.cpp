/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "UserFinalQuestions.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
UserFinalQuestions::UserFinalQuestions ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    similarInterfaceToggleButton.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (similarInterfaceToggleButton.get());
    similarInterfaceToggleButton->setButtonText (TRANS("If have already tested a similar interface"));
    similarInterfaceToggleButton->addListener (this);

    similarInterfaceToggleButton->setBounds (8, 136, 672, 24);

    label.reset (new Label ("new label",
                            TRANS("Please read the assertions below, and check the true ones.")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (16, 16, 648, 24);

    label2.reset (new Label ("new label",
                             TRANS("If you have already tested a similar interface, please tell us its name in the form below:")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (8, 168, 672, 24);

    similarInterfaceTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (similarInterfaceTextEditor.get());
    similarInterfaceTextEditor->setMultiLine (false);
    similarInterfaceTextEditor->setReturnKeyStartsNewLine (false);
    similarInterfaceTextEditor->setReadOnly (false);
    similarInterfaceTextEditor->setScrollbarsShown (true);
    similarInterfaceTextEditor->setCaretVisible (true);
    similarInterfaceTextEditor->setPopupMenuEnabled (true);
    similarInterfaceTextEditor->setText (String());

    similarInterfaceTextEditor->setBounds (16, 200, 656, 24);

    similarExperimentToggleButton.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (similarExperimentToggleButton.get());
    similarExperimentToggleButton->setButtonText (TRANS("I already participated in a similar scientific experiment"));
    similarExperimentToggleButton->addListener (this);

    similarExperimentToggleButton->setBounds (8, 304, 672, 24);

    label4.reset (new Label ("new label",
                             TRANS("If you already participated in a similar experiment, please tell us its name, location and/or context in the form below: ")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (8, 336, 696, 24);

    similarInterfaceTextEditor2.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (similarInterfaceTextEditor2.get());
    similarInterfaceTextEditor2->setMultiLine (false);
    similarInterfaceTextEditor2->setReturnKeyStartsNewLine (false);
    similarInterfaceTextEditor2->setReadOnly (false);
    similarInterfaceTextEditor2->setScrollbarsShown (true);
    similarInterfaceTextEditor2->setCaretVisible (true);
    similarInterfaceTextEditor2->setPopupMenuEnabled (true);
    similarInterfaceTextEditor2->setText (String());

    similarInterfaceTextEditor2->setBounds (16, 368, 656, 24);

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("OK, save data and finish the experiment"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff2ac858));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

UserFinalQuestions::~UserFinalQuestions()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    similarInterfaceToggleButton = nullptr;
    label = nullptr;
    label2 = nullptr;
    similarInterfaceTextEditor = nullptr;
    similarExperimentToggleButton = nullptr;
    label4 = nullptr;
    similarInterfaceTextEditor2 = nullptr;
    finishedButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UserFinalQuestions::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff111111));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UserFinalQuestions::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 88, 500, 80);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void UserFinalQuestions::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == similarInterfaceToggleButton.get())
    {
        //[UserButtonCode_similarInterfaceToggleButton] -- add your button handler code here..
        //[/UserButtonCode_similarInterfaceToggleButton]
    }
    else if (buttonThatWasClicked == similarExperimentToggleButton.get())
    {
        //[UserButtonCode_similarExperimentToggleButton] -- add your button handler code here..
        //[/UserButtonCode_similarExperimentToggleButton]
    }
    else if (buttonThatWasClicked == finishedButton.get())
    {
        //[UserButtonCode_finishedButton] -- add your button handler code here..
        userQuestionsManager->OnFinalQuestionsAnswered(this);
        //[/UserButtonCode_finishedButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

std::shared_ptr<bptree::ptree> UserFinalQuestions::GetQuestionsBPTree()
{
    auto questionsChildrenTree = std::make_shared<bptree::ptree>();
    //questionsChildrenTree->put("data_usage.<xmlattr>.allow",
      //                         userQuestions->allowDataToggleButton->getToggleState());
    
    return questionsChildrenTree;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UserFinalQuestions" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff111111"/>
  <TOGGLEBUTTON name="new toggle button" id="62554cf2d48d535" memberName="similarInterfaceToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 136 672 24" buttonText="If have already tested a similar interface"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="6cd75e74f70bffa5" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="16 16 648 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Please read the assertions below, and check the true ones."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <LABEL name="new label" id="a8708d571b59b51" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 168 672 24" edTextCol="ff000000"
         edBkgCol="0" labelText="If you have already tested a similar interface, please tell us its name in the form below:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="91cc4dd0fcb0d776" memberName="similarInterfaceTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 200 656 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="new toggle button" id="9994df3dee6fae04" memberName="similarExperimentToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 304 672 24" buttonText="I already participated in a similar scientific experiment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="94f1b219535bfde4" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="8 336 696 24" edTextCol="ff000000"
         edBkgCol="0" labelText="If you already participated in a similar experiment, please tell us its name, location and/or context in the form below: "
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="f104febd5cf021fd" memberName="similarInterfaceTextEditor2"
              virtualName="" explicitFocusOrder="0" pos="16 368 656 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 88R 500 80" bgColOff="ff2ac858"
              bgColOn="ff252525" buttonText="OK, save data and finish the experiment"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

