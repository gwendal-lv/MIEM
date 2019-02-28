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
    similarInterfaceToggleButton->setButtonText (TRANS("I have already tested a similar touch app"));
    similarInterfaceToggleButton->addListener (this);

    similarInterfaceToggleButton->setBounds (8, 304, 672, 24);

    label.reset (new Label ("new label",
                            TRANS("It is almost finished! Please read and answer the last few questions below.")));
    addAndMakeVisible (label.get());
    label->setFont (Font (19.00f, Font::plain).withTypefaceStyle ("Bold"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (16, 8, 664, 40);

    similarInterfaceQuestionLabel.reset (new Label ("Similar Interface Question label",
                                                    TRANS("Please tell us its name in the form below:")));
    addAndMakeVisible (similarInterfaceQuestionLabel.get());
    similarInterfaceQuestionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    similarInterfaceQuestionLabel->setJustificationType (Justification::centredLeft);
    similarInterfaceQuestionLabel->setEditable (false, false, false);
    similarInterfaceQuestionLabel->setColour (TextEditor::textColourId, Colours::black);
    similarInterfaceQuestionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    similarInterfaceQuestionLabel->setBounds (8, 328, 672, 24);

    similarInterfaceTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (similarInterfaceTextEditor.get());
    similarInterfaceTextEditor->setMultiLine (false);
    similarInterfaceTextEditor->setReturnKeyStartsNewLine (false);
    similarInterfaceTextEditor->setReadOnly (false);
    similarInterfaceTextEditor->setScrollbarsShown (true);
    similarInterfaceTextEditor->setCaretVisible (true);
    similarInterfaceTextEditor->setPopupMenuEnabled (true);
    similarInterfaceTextEditor->setText (String());

    similarInterfaceTextEditor->setBounds (16, 360, 824, 24);

    similarExperimentToggleButton.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (similarExperimentToggleButton.get());
    similarExperimentToggleButton->setButtonText (TRANS("I have already participated in a similar scientific experiment"));
    similarExperimentToggleButton->addListener (this);

    similarExperimentToggleButton->setBounds (8, 440, 672, 24);

    similarExpeQuestionLabel.reset (new Label ("Similar EXperiment Question label",
                                               TRANS("Please tell us its name, location and/or context in the form below: ")));
    addAndMakeVisible (similarExpeQuestionLabel.get());
    similarExpeQuestionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    similarExpeQuestionLabel->setJustificationType (Justification::centredLeft);
    similarExpeQuestionLabel->setEditable (false, false, false);
    similarExpeQuestionLabel->setColour (TextEditor::textColourId, Colours::black);
    similarExpeQuestionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    similarExpeQuestionLabel->setBounds (8, 464, 696, 24);

    similarExpeTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (similarExpeTextEditor.get());
    similarExpeTextEditor->setMultiLine (false);
    similarExpeTextEditor->setReturnKeyStartsNewLine (false);
    similarExpeTextEditor->setReadOnly (false);
    similarExpeTextEditor->setScrollbarsShown (true);
    similarExpeTextEditor->setCaretVisible (true);
    similarExpeTextEditor->setPopupMenuEnabled (true);
    similarExpeTextEditor->setText (String());

    similarExpeTextEditor->setBounds (16, 496, 824, 24);

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("OK, save data and finish the experiment"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff2ac858));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    selfExpertiseLevelLabel.reset (new Label ("expertise level label",
                                              TRANS("Concerning sound synthesisers and filters, what is your level of expertise ?")));
    addAndMakeVisible (selfExpertiseLevelLabel.get());
    selfExpertiseLevelLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    selfExpertiseLevelLabel->setJustificationType (Justification::centredLeft);
    selfExpertiseLevelLabel->setEditable (false, false, false);
    selfExpertiseLevelLabel->setColour (TextEditor::textColourId, Colours::black);
    selfExpertiseLevelLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    selfExpertiseLevelLabel->setBounds (8, 80, 672, 24);

    expertiseSlider.reset (new Slider ("new slider"));
    addAndMakeVisible (expertiseSlider.get());
    expertiseSlider->setRange (1, 5, 1);
    expertiseSlider->setSliderStyle (Slider::LinearHorizontal);
    expertiseSlider->setTextBoxStyle (Slider::TextBoxAbove, true, 80, 20);
    expertiseSlider->setColour (Slider::backgroundColourId, Colour (0xff353c40));
    expertiseSlider->setColour (Slider::thumbColourId, Colour (0xff08cef3));
    expertiseSlider->setColour (Slider::trackColourId, Colour (0xff353c40));
    expertiseSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x0042a2c8));

    expertiseSlider->setBounds (56, 112, 736, 48);

    expertiseLevelLabel.reset (new Label ("expertise level  label",
                                          TRANS("Before this experiment, I did not know anything about sound synthesisers and filters")));
    addAndMakeVisible (expertiseLevelLabel.get());
    expertiseLevelLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel->setJustificationType (Justification::centredTop);
    expertiseLevelLabel->setEditable (false, false, false);
    expertiseLevelLabel->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel->setBounds (8, 160, 152, 120);

    expertiseLevelLabel2.reset (new Label ("expertise level  label",
                                           TRANS("I have already played with synthesisers and filters, but I don\'t know how they work")));
    addAndMakeVisible (expertiseLevelLabel2.get());
    expertiseLevelLabel2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel2->setJustificationType (Justification::centredTop);
    expertiseLevelLabel2->setEditable (false, false, false);
    expertiseLevelLabel2->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel2->setBounds (176, 160, 152, 128);

    expertiseLevelLabel3.reset (new Label ("expertise level  label",
                                           TRANS("As an amateur, I have already played with synthesisers and filters, and I know a bit how to use them")));
    addAndMakeVisible (expertiseLevelLabel3.get());
    expertiseLevelLabel3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel3->setJustificationType (Justification::centredTop);
    expertiseLevelLabel3->setEditable (false, false, false);
    expertiseLevelLabel3->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel3->setBounds (344, 160, 152, 128);

    expertiseLevelLabel4.reset (new Label ("expertise level  label",
                                           TRANS("I play quite often with synthesisers and filters, and I can configure them to produce the sounds that I want")));
    addAndMakeVisible (expertiseLevelLabel4.get());
    expertiseLevelLabel4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel4->setJustificationType (Justification::centredTop);
    expertiseLevelLabel4->setEditable (false, false, false);
    expertiseLevelLabel4->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel4->setBounds (520, 160, 152, 120);

    expertiseLevelLabel5.reset (new Label ("expertise level  label",
                                           TRANS("As a professional, I understand and I can manipulate classical sound synthesisers and filtering techniques")));
    addAndMakeVisible (expertiseLevelLabel5.get());
    expertiseLevelLabel5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel5->setJustificationType (Justification::centredTop);
    expertiseLevelLabel5->setEditable (false, false, false);
    expertiseLevelLabel5->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel5->setBounds (696, 160, 152, 136);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    // VALEUR PAR DEFAUT DU SLIDER
    expertiseSlider->setValue(2.0);

    // dé-enregistrement, pour ré-enregistrement du listener de click ?
    removeMouseListener(this);
    addMouseListener(this, true); // events from all (deeply-)nested child components

    // actualisations forcées
    buttonClicked(nullptr);

    //[/Constructor]
}

UserFinalQuestions::~UserFinalQuestions()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    similarInterfaceToggleButton = nullptr;
    label = nullptr;
    similarInterfaceQuestionLabel = nullptr;
    similarInterfaceTextEditor = nullptr;
    similarExperimentToggleButton = nullptr;
    similarExpeQuestionLabel = nullptr;
    similarExpeTextEditor = nullptr;
    finishedButton = nullptr;
    selfExpertiseLevelLabel = nullptr;
    expertiseSlider = nullptr;
    expertiseLevelLabel = nullptr;
    expertiseLevelLabel2 = nullptr;
    expertiseLevelLabel3 = nullptr;
    expertiseLevelLabel4 = nullptr;
    expertiseLevelLabel5 = nullptr;


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

    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 100, 500, 80);
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

    // Affichage ou non de certains éléments (suite aux réponses
    // aux toggle buttons)
    similarInterfaceQuestionLabel->setVisible(similarInterfaceToggleButton->getToggleState());
    similarInterfaceTextEditor->setVisible(similarInterfaceToggleButton->getToggleState());
    similarExpeQuestionLabel->setVisible(similarExperimentToggleButton->getToggleState());
    similarExpeTextEditor->setVisible(similarExperimentToggleButton->getToggleState());

    //[/UserbuttonClicked_Post]
}

void UserFinalQuestions::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    if (e.originalComponent == expertiseLevelLabel.get())
        expertiseSlider->setValue(1.0);
    if (e.originalComponent == expertiseLevelLabel2.get())
        expertiseSlider->setValue(2.0);
    if (e.originalComponent == expertiseLevelLabel3.get())
        expertiseSlider->setValue(3.0);
    if (e.originalComponent == expertiseLevelLabel4.get())
        expertiseSlider->setValue(4.0);
    if (e.originalComponent == expertiseLevelLabel5.get())
        expertiseSlider->setValue(5.0);
    //[/UserCode_mouseUp]
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
  <METHODS>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff111111"/>
  <TOGGLEBUTTON name="new toggle button" id="62554cf2d48d535" memberName="similarInterfaceToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 304 672 24" buttonText="I have already tested a similar touch app"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="6cd75e74f70bffa5" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="16 8 664 40" edTextCol="ff000000"
         edBkgCol="0" labelText="It is almost finished! Please read and answer the last few questions below."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.9e1" kerning="0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <LABEL name="Similar Interface Question label" id="a8708d571b59b51"
         memberName="similarInterfaceQuestionLabel" virtualName="" explicitFocusOrder="0"
         pos="8 328 672 24" edTextCol="ff000000" edBkgCol="0" labelText="Please tell us its name in the form below:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="91cc4dd0fcb0d776" memberName="similarInterfaceTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 360 824 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="new toggle button" id="9994df3dee6fae04" memberName="similarExperimentToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 440 672 24" buttonText="I have already participated in a similar scientific experiment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="Similar EXperiment Question label" id="94f1b219535bfde4"
         memberName="similarExpeQuestionLabel" virtualName="" explicitFocusOrder="0"
         pos="8 464 696 24" edTextCol="ff000000" edBkgCol="0" labelText="Please tell us its name, location and/or context in the form below: "
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="f104febd5cf021fd" memberName="similarExpeTextEditor"
              virtualName="" explicitFocusOrder="0" pos="16 496 824 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 100R 500 80" bgColOff="ff2ac858"
              bgColOn="ff252525" buttonText="OK, save data and finish the experiment"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="expertise level label" id="b435fe1e58647b28" memberName="selfExpertiseLevelLabel"
         virtualName="" explicitFocusOrder="0" pos="8 80 672 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Concerning sound synthesisers and filters, what is your level of expertise ?"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="new slider" id="bfaef998cabecf5d" memberName="expertiseSlider"
          virtualName="" explicitFocusOrder="0" pos="56 112 736 48" bkgcol="ff353c40"
          thumbcol="ff08cef3" trackcol="ff353c40" textboxhighlight="42a2c8"
          min="1" max="5" int="1" style="LinearHorizontal" textBoxPos="TextBoxAbove"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <LABEL name="expertise level  label" id="2f66faf604745e12" memberName="expertiseLevelLabel"
         virtualName="Label" explicitFocusOrder="0" pos="8 160 152 120"
         edTextCol="ff000000" edBkgCol="0" labelText="Before this experiment, I did not know anything about sound synthesisers and filters"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="8fb437ba6c83e57d" memberName="expertiseLevelLabel2"
         virtualName="" explicitFocusOrder="0" pos="176 160 152 128" edTextCol="ff000000"
         edBkgCol="0" labelText="I have already played with synthesisers and filters, but I don't know how they work"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="ea90a2c7054e88e" memberName="expertiseLevelLabel3"
         virtualName="" explicitFocusOrder="0" pos="344 160 152 128" edTextCol="ff000000"
         edBkgCol="0" labelText="As an amateur, I have already played with synthesisers and filters, and I know a bit how to use them"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="dd0cf4ee1b6450df" memberName="expertiseLevelLabel4"
         virtualName="" explicitFocusOrder="0" pos="520 160 152 120" edTextCol="ff000000"
         edBkgCol="0" labelText="I play quite often with synthesisers and filters, and I can configure them to produce the sounds that I want"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="fa98c1f744ec1239" memberName="expertiseLevelLabel5"
         virtualName="" explicitFocusOrder="0" pos="696 160 152 136" edTextCol="ff000000"
         edBkgCol="0" labelText="As a professional, I understand and I can manipulate classical sound synthesisers and filtering techniques"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

