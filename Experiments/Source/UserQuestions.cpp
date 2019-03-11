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

#include "UserQuestions.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
UserQuestions::UserQuestions ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label.reset (new Label ("new label",
                            TRANS("Before starting the experiment, please read and fill the form below.")));
    addAndMakeVisible (label.get());
    label->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::outlineColourId, Colours::white);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("Begin the experiment"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff097c2a));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    allowDataToggleButton.reset (new ToggleButton ("Question toggle button"));
    addAndMakeVisible (allowDataToggleButton.get());
    allowDataToggleButton->setButtonText (TRANS("I allow the anonymous usage of data collected during this experiment"));
    allowDataToggleButton->addListener (this);

    label2.reset (new Label ("new label",
                             TRANS("Collected data might be anonymously used and published for scientific research purposes.")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.00f, Font::italic));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    visionDisorderToggleButton.reset (new ToggleButton ("Question toggle button"));
    addAndMakeVisible (visionDisorderToggleButton.get());
    visionDisorderToggleButton->setButtonText (TRANS("I suffer from a vision impairment, such as uncorrected short-sightedness, color blindness, ..."));
    visionDisorderToggleButton->addListener (this);

    visionQuestionLabel.reset (new Label ("Vision Question label",
                                          TRANS("Please tell us the nature of this impairment in the form below:")));
    addAndMakeVisible (visionQuestionLabel.get());
    visionQuestionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    visionQuestionLabel->setJustificationType (Justification::centredLeft);
    visionQuestionLabel->setEditable (false, false, false);
    visionQuestionLabel->setColour (TextEditor::textColourId, Colours::black);
    visionQuestionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    visionDisorderTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (visionDisorderTextEditor.get());
    visionDisorderTextEditor->setMultiLine (false);
    visionDisorderTextEditor->setReturnKeyStartsNewLine (false);
    visionDisorderTextEditor->setReadOnly (false);
    visionDisorderTextEditor->setScrollbarsShown (true);
    visionDisorderTextEditor->setCaretVisible (true);
    visionDisorderTextEditor->setPopupMenuEnabled (true);
    visionDisorderTextEditor->setText (String());

    visionDisorderTextEditor->setBounds (16, 400, 752, 24);

    hearingImpairmentToggleButton.reset (new ToggleButton ("Audition Question toggle button"));
    addAndMakeVisible (hearingImpairmentToggleButton.get());
    hearingImpairmentToggleButton->setButtonText (TRANS("I suffer from an hearing impairment"));
    hearingImpairmentToggleButton->addListener (this);

    hearingQuestionLabel.reset (new Label ("Hearing Question label",
                                           TRANS("Please tell us the nature of this impairment in the form below:")));
    addAndMakeVisible (hearingQuestionLabel.get());
    hearingQuestionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    hearingQuestionLabel->setJustificationType (Justification::centredLeft);
    hearingQuestionLabel->setEditable (false, false, false);
    hearingQuestionLabel->setColour (TextEditor::textColourId, Colours::black);
    hearingQuestionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    hearingTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (hearingTextEditor.get());
    hearingTextEditor->setMultiLine (false);
    hearingTextEditor->setReturnKeyStartsNewLine (false);
    hearingTextEditor->setReadOnly (false);
    hearingTextEditor->setScrollbarsShown (true);
    hearingTextEditor->setCaretVisible (true);
    hearingTextEditor->setPopupMenuEnabled (true);
    hearingTextEditor->setText (String());

    hearingTextEditor->setBounds (16, 544, 752, 24);

    ageLabel.reset (new Label ("Age label",
                               TRANS("How old are you?")));
    addAndMakeVisible (ageLabel.get());
    ageLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    ageLabel->setJustificationType (Justification::centredLeft);
    ageLabel->setEditable (false, false, false);
    ageLabel->setColour (TextEditor::textColourId, Colours::black);
    ageLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ageLabel->setBounds (8, 248, 144, 24);

    ageSlider.reset (new Slider ("age slider"));
    addAndMakeVisible (ageSlider.get());
    ageSlider->setRange (18, 100, 1);
    ageSlider->setSliderStyle (Slider::LinearHorizontal);
    ageSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    ageSlider->setColour (Slider::backgroundColourId, Colour (0x00263238));
    ageSlider->setColour (Slider::thumbColourId, Colour (0x0042a2c8));
    ageSlider->setColour (Slider::trackColourId, Colour (0x00181f22));
    ageSlider->addListener (this);

    ageSlider->setBounds (160, 248, 150, 24);


    //[UserPreSize]

    // button deactivated while "allow data collection" is not checked
    finishedButton->setEnabled(false);

    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    buttonClicked(nullptr); // to force updates

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
    visionQuestionLabel = nullptr;
    visionDisorderTextEditor = nullptr;
    hearingImpairmentToggleButton = nullptr;
    hearingQuestionLabel = nullptr;
    hearingTextEditor = nullptr;
    ageLabel = nullptr;
    ageSlider = nullptr;


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

    label->setBounds ((getWidth() / 2) - ((getWidth() - 40) / 2), 8, getWidth() - 40, 40);
    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 88, 500, 80);
    allowDataToggleButton->setBounds (8, 128, getWidth() - 35, 24);
    label2->setBounds (8, 152, getWidth() - 35, 24);
    visionDisorderToggleButton->setBounds (8, 344, getWidth() - 35, 24);
    visionQuestionLabel->setBounds (8, 368, getWidth() - 27, 24);
    hearingImpairmentToggleButton->setBounds (8, 483, getWidth() - 35, 24);
    hearingQuestionLabel->setBounds (8, 512, getWidth() - 27, 24);
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
    else if (buttonThatWasClicked == hearingImpairmentToggleButton.get())
    {
        //[UserButtonCode_hearingImpairmentToggleButton] -- add your button handler code here..
        //[/UserButtonCode_hearingImpairmentToggleButton]
    }

    //[UserbuttonClicked_Post]

    // Affichage des champs de texte nécessaires selon état des toggle buttons
    visionQuestionLabel->setVisible(visionDisorderToggleButton->getToggleState());
    visionDisorderTextEditor->setVisible(visionDisorderToggleButton->getToggleState());
    hearingQuestionLabel->setVisible(hearingImpairmentToggleButton->getToggleState());
    hearingTextEditor->setVisible(hearingImpairmentToggleButton->getToggleState());


    //[/UserbuttonClicked_Post]
}

void UserQuestions::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == ageSlider.get())
    {
        //[UserSliderCode_ageSlider] -- add your slider handling code here..
        //[/UserSliderCode_ageSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

std::shared_ptr<bptree::ptree> UserQuestions::GetQuestionsBPTree()
{
    auto questionsChildrenTree = std::make_shared<bptree::ptree>();
    questionsChildrenTree->put("data_usage.<xmlattr>.allow",
                               (bool)allowDataToggleButton->getToggleState());
    questionsChildrenTree->put("age", (int)ageSlider->getValue());
    bptree::ptree visionTree;
    visionTree.put("<xmlattr>.checked", (bool)visionDisorderToggleButton->getToggleState());
    if (visionDisorderToggleButton->getToggleState())
        visionTree.put_value(visionDisorderTextEditor->getText().toRawUTF8());
    questionsChildrenTree->add_child("vision_impairment", visionTree);
    bptree::ptree hearingTree;
    hearingTree.put("<xmlattr>.checked", (bool)hearingImpairmentToggleButton->getToggleState());
    if (hearingImpairmentToggleButton->getToggleState())
        hearingTree.put_value(hearingTextEditor->getText().toRawUTF8());
    questionsChildrenTree->add_child("hearing_impairment", hearingTree);
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
         explicitFocusOrder="0" pos="0.5Cc 8 40M 40" outlineCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Before starting the experiment, please read and fill the form below."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="2e1" kerning="0" bold="1" italic="0"
         justification="36" typefaceStyle="Bold"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 88R 500 80" bgColOff="ff097c2a"
              bgColOn="ff252525" buttonText="Begin the experiment" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="Question toggle button" id="ba23ef5bc6fe327c" memberName="allowDataToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 128 35M 24" buttonText="I allow the anonymous usage of data collected during this experiment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="91bd908b7f2dbb2f" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 152 35M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Collected data might be anonymously used and published for scientific research purposes."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <TOGGLEBUTTON name="Question toggle button" id="acc7621165a9b88a" memberName="visionDisorderToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 344 35M 24" buttonText="I suffer from a vision impairment, such as uncorrected short-sightedness, color blindness, ..."
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="Vision Question label" id="e9827abd77de5abc" memberName="visionQuestionLabel"
         virtualName="" explicitFocusOrder="0" pos="8 368 27M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Please tell us the nature of this impairment in the form below:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="a8feea107d85baef" memberName="visionDisorderTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 400 752 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="Audition Question toggle button" id="62cad4dc7d1ac60c"
                memberName="hearingImpairmentToggleButton" virtualName="" explicitFocusOrder="0"
                pos="8 483 35M 24" buttonText="I suffer from an hearing impairment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="Hearing Question label" id="ca8fb6087ad7469d" memberName="hearingQuestionLabel"
         virtualName="" explicitFocusOrder="0" pos="8 512 27M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Please tell us the nature of this impairment in the form below:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="97593eef0b567a69" memberName="hearingTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 544 752 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="Age label" id="cad86b63b6b409a7" memberName="ageLabel"
         virtualName="" explicitFocusOrder="0" pos="8 248 144 24" edTextCol="ff000000"
         edBkgCol="0" labelText="How old are you?" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <SLIDER name="age slider" id="8a696570d1136395" memberName="ageSlider"
          virtualName="" explicitFocusOrder="0" pos="160 248 150 24" bkgcol="263238"
          thumbcol="42a2c8" trackcol="181f22" min="1.8e1" max="1e2" int="1"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

