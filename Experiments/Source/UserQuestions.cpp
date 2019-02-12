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

#include "UserQuestions.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
UserQuestions::UserQuestions ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label.reset (new Label ("new label",
                            TRANS("Please read the assertions below, and check the true ones.")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("Begin the experiment"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff2ac858));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    allowDataToggleButton.reset (new ToggleButton ("Question toggle button"));
    addAndMakeVisible (allowDataToggleButton.get());
    allowDataToggleButton->setButtonText (TRANS("I allow the anonymous usage of data collected during this experiment"));
    allowDataToggleButton->addListener (this);

    label2.reset (new Label ("new label",
                             TRANS("Collected data might be anonymously used and published for scientific research purposes.")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    visionDisorderToggleButton.reset (new ToggleButton ("Question toggle button"));
    addAndMakeVisible (visionDisorderToggleButton.get());
    visionDisorderToggleButton->setButtonText (TRANS("I suffer from a vision disorder"));
    visionDisorderToggleButton->addListener (this);

    label3.reset (new Label ("new label",
                             TRANS("If you suffer from a vision disorder, please tell us its nature in the form below:")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    visionDisorderTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (visionDisorderTextEditor.get());
    visionDisorderTextEditor->setMultiLine (false);
    visionDisorderTextEditor->setReturnKeyStartsNewLine (false);
    visionDisorderTextEditor->setReadOnly (false);
    visionDisorderTextEditor->setScrollbarsShown (true);
    visionDisorderTextEditor->setCaretVisible (true);
    visionDisorderTextEditor->setPopupMenuEnabled (true);
    visionDisorderTextEditor->setText (String());

    visionDisorderTextEditor->setBounds (16, 256, 664, 24);

    physicalDisorderToggleButton.reset (new ToggleButton ("Question toggle button"));
    addAndMakeVisible (physicalDisorderToggleButton.get());
    physicalDisorderToggleButton->setButtonText (TRANS("I suffer from an arm, hand or finger physical disorder"));
    physicalDisorderToggleButton->addListener (this);

    label4.reset (new Label ("new label",
                             TRANS("If you suffer from such a disorder, please tell us its nature in the form below:")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    physicalDisorderTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (physicalDisorderTextEditor.get());
    physicalDisorderTextEditor->setMultiLine (false);
    physicalDisorderTextEditor->setReturnKeyStartsNewLine (false);
    physicalDisorderTextEditor->setReadOnly (false);
    physicalDisorderTextEditor->setScrollbarsShown (true);
    physicalDisorderTextEditor->setCaretVisible (true);
    physicalDisorderTextEditor->setPopupMenuEnabled (true);
    physicalDisorderTextEditor->setText (String());

    physicalDisorderTextEditor->setBounds (16, 411, 664, 24);


    //[UserPreSize]

    // button deactivated while "allow data collection" is not checked
    finishedButton->setEnabled(false);

    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

UserQuestions::~UserQuestions()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label = nullptr;
    finishedButton = nullptr;
    allowDataToggleButton = nullptr;
    label2 = nullptr;
    visionDisorderToggleButton = nullptr;
    label3 = nullptr;
    visionDisorderTextEditor = nullptr;
    physicalDisorderToggleButton = nullptr;
    label4 = nullptr;
    physicalDisorderTextEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UserQuestions::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff111111));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UserQuestions::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label->setBounds (16, 8, getWidth() - 40, 24);
    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 88, 500, 80);
    allowDataToggleButton->setBounds (8, 80, getWidth() - 35, 24);
    label2->setBounds (8, 104, getWidth() - 35, 24);
    visionDisorderToggleButton->setBounds (8, 192, getWidth() - 35, 24);
    label3->setBounds (8, 224, getWidth() - 27, 24);
    physicalDisorderToggleButton->setBounds (16, 347, getWidth() - 35, 24);
    label4->setBounds (8, 379, getWidth() - 27, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void UserQuestions::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == finishedButton.get())
    {
        //[UserButtonCode_finishedButton] -- add your button handler code here..
        userQuestionsManager->OnFirstQuestionsAnswered(this);
        //[/UserButtonCode_finishedButton]
    }
    else if (buttonThatWasClicked == allowDataToggleButton.get())
    {
        //[UserButtonCode_allowDataToggleButton] -- add your button handler code here..
        finishedButton->setEnabled(allowDataToggleButton->getToggleState());
        //[/UserButtonCode_allowDataToggleButton]
    }
    else if (buttonThatWasClicked == visionDisorderToggleButton.get())
    {
        //[UserButtonCode_visionDisorderToggleButton] -- add your button handler code here..
        //[/UserButtonCode_visionDisorderToggleButton]
    }
    else if (buttonThatWasClicked == physicalDisorderToggleButton.get())
    {
        //[UserButtonCode_physicalDisorderToggleButton] -- add your button handler code here..
        //[/UserButtonCode_physicalDisorderToggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

std::shared_ptr<bptree::ptree> UserQuestions::GetQuestionsBPTree()
{
    auto questionsChildrenTree = std::make_shared<bptree::ptree>();
    questionsChildrenTree->put("data_usage.<xmlattr>.allow",
                               allowDataToggleButton->getToggleState());

    return questionsChildrenTree;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UserQuestions" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff111111"/>
  <LABEL name="new label" id="dd908f7016e646c5" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="16 8 40M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Please read the assertions below, and check the true ones."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 88R 500 80" bgColOff="ff2ac858"
              bgColOn="ff252525" buttonText="Begin the experiment" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="Question toggle button" id="ba23ef5bc6fe327c" memberName="allowDataToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 80 35M 24" buttonText="I allow the anonymous usage of data collected during this experiment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="91bd908b7f2dbb2f" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 104 35M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Collected data might be anonymously used and published for scientific research purposes."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TOGGLEBUTTON name="Question toggle button" id="acc7621165a9b88a" memberName="visionDisorderToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 192 35M 24" buttonText="I suffer from a vision disorder"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="e9827abd77de5abc" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 224 27M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="If you suffer from a vision disorder, please tell us its nature in the form below:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="a8feea107d85baef" memberName="visionDisorderTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 256 664 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="Question toggle button" id="62cad4dc7d1ac60c" memberName="physicalDisorderToggleButton"
                virtualName="" explicitFocusOrder="0" pos="16 347 35M 24" buttonText="I suffer from an arm, hand or finger physical disorder"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="ca8fb6087ad7469d" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="8 379 27M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="If you suffer from such a disorder, please tell us its nature in the form below:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="97593eef0b567a69" memberName="physicalDisorderTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 411 664 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

