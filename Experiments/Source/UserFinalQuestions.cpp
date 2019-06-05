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

    similarInterfaceToggleButton->setBounds (79, 512, 672, 24);

    label.reset (new Label ("new label",
                            TRANS("It is almost finished! Please read and answer these last few questions.")));
    addAndMakeVisible (label.get());
    label->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::outlineColourId, Colours::white);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    similarInterfaceQuestionLabel.reset (new Label ("Similar Interface Question label",
                                                    TRANS("If you can, please tell us the name of the app:")));
    addAndMakeVisible (similarInterfaceQuestionLabel.get());
    similarInterfaceQuestionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    similarInterfaceQuestionLabel->setJustificationType (Justification::centredRight);
    similarInterfaceQuestionLabel->setEditable (false, false, false);
    similarInterfaceQuestionLabel->setColour (TextEditor::textColourId, Colours::black);
    similarInterfaceQuestionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    similarInterfaceQuestionLabel->setBounds (79, 536, 392, 24);

    similarInterfaceTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (similarInterfaceTextEditor.get());
    similarInterfaceTextEditor->setMultiLine (false);
    similarInterfaceTextEditor->setReturnKeyStartsNewLine (false);
    similarInterfaceTextEditor->setReadOnly (false);
    similarInterfaceTextEditor->setScrollbarsShown (true);
    similarInterfaceTextEditor->setCaretVisible (true);
    similarInterfaceTextEditor->setPopupMenuEnabled (true);
    similarInterfaceTextEditor->setText (String());

    similarInterfaceTextEditor->setBounds (471, 536, 376, 24);

    similarExperimentToggleButton.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (similarExperimentToggleButton.get());
    similarExperimentToggleButton->setButtonText (TRANS("I have already participated in a similar scientific experiment"));
    similarExperimentToggleButton->addListener (this);

    similarExperimentToggleButton->setBounds (80, 576, 672, 24);

    similarExpeQuestionLabel.reset (new Label ("Similar EXperiment Question label",
                                               TRANS("Please tell us its name, location and/or context in the form below: ")));
    addAndMakeVisible (similarExpeQuestionLabel.get());
    similarExpeQuestionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    similarExpeQuestionLabel->setJustificationType (Justification::centredLeft);
    similarExpeQuestionLabel->setEditable (false, false, false);
    similarExpeQuestionLabel->setColour (TextEditor::textColourId, Colours::black);
    similarExpeQuestionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    similarExpeQuestionLabel->setBounds (80, 600, 696, 24);

    similarExpeTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (similarExpeTextEditor.get());
    similarExpeTextEditor->setMultiLine (false);
    similarExpeTextEditor->setReturnKeyStartsNewLine (false);
    similarExpeTextEditor->setReadOnly (false);
    similarExpeTextEditor->setScrollbarsShown (true);
    similarExpeTextEditor->setCaretVisible (true);
    similarExpeTextEditor->setPopupMenuEnabled (true);
    similarExpeTextEditor->setText (String());

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("OK, save data and finish the experiment"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff097c2a));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    selfExpertiseLevelLabel.reset (new Label ("expertise level label",
                                              TRANS("Concerning sound synthesisers and filters, what is your level of expertise ? (Click on the right answer or move the blue cursor)")));
    addAndMakeVisible (selfExpertiseLevelLabel.get());
    selfExpertiseLevelLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    selfExpertiseLevelLabel->setJustificationType (Justification::centredLeft);
    selfExpertiseLevelLabel->setEditable (false, false, false);
    selfExpertiseLevelLabel->setColour (TextEditor::textColourId, Colours::black);
    selfExpertiseLevelLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    selfExpertiseLevelLabel->setBounds (60, 56, 1012, 24);

    expertiseSlider.reset (new Slider ("new slider"));
    addAndMakeVisible (expertiseSlider.get());
    expertiseSlider->setRange (1, 5, 1);
    expertiseSlider->setSliderStyle (Slider::LinearHorizontal);
    expertiseSlider->setTextBoxStyle (Slider::TextBoxAbove, true, 80, 20);
    expertiseSlider->setColour (Slider::backgroundColourId, Colour (0xff353c40));
    expertiseSlider->setColour (Slider::thumbColourId, Colour (0xff08cef3));
    expertiseSlider->setColour (Slider::trackColourId, Colour (0xff353c40));
    expertiseSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x0042a2c8));

    expertiseSlider->setBounds (56, 88, 1200, 48);

    expertiseLevelLabel.reset (new Label ("expertise level  label",
                                          TRANS("Before this experiment, I did not know anything about sound synthesisers and filters")));
    addAndMakeVisible (expertiseLevelLabel.get());
    expertiseLevelLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel->setJustificationType (Justification::centredTop);
    expertiseLevelLabel->setEditable (false, false, false);
    expertiseLevelLabel->setColour (Label::textColourId, Colour (0xffb0b0b0));
    expertiseLevelLabel->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel->setBounds (8, 136, 208, 80);

    expertiseLevelLabel2.reset (new Label ("expertise level  label",
                                           TRANS("I have already played with synthesisers and filters, but I don\'t know how they work")));
    addAndMakeVisible (expertiseLevelLabel2.get());
    expertiseLevelLabel2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel2->setJustificationType (Justification::centredTop);
    expertiseLevelLabel2->setEditable (false, false, false);
    expertiseLevelLabel2->setColour (Label::textColourId, Colour (0xffb0b0b0));
    expertiseLevelLabel2->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel2->setBounds (248, 136, 224, 80);

    expertiseLevelLabel3.reset (new Label ("expertise level  label",
                                           TRANS("As an amateur, I have already played with synths and filters, and I know a bit how to use them. I can identify some sound characteristics.")));
    addAndMakeVisible (expertiseLevelLabel3.get());
    expertiseLevelLabel3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel3->setJustificationType (Justification::centredTop);
    expertiseLevelLabel3->setEditable (false, false, false);
    expertiseLevelLabel3->setColour (Label::textColourId, Colour (0xffb0b0b0));
    expertiseLevelLabel3->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel3->setBounds (504, 136, 296, 80);

    expertiseLevelLabel4.reset (new Label ("expertise level  label",
                                           TRANS("I play quite often with synths and filters, and I can configure them to produce sounds I want. I can easily identify the main sound characteristics.")));
    addAndMakeVisible (expertiseLevelLabel4.get());
    expertiseLevelLabel4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel4->setJustificationType (Justification::centredTop);
    expertiseLevelLabel4->setEditable (false, false, false);
    expertiseLevelLabel4->setColour (Label::textColourId, Colour (0xffb0b0b0));
    expertiseLevelLabel4->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel4->setBounds (824, 136, 280, 80);

    expertiseLevelLabel5.reset (new Label ("expertise level  label",
                                           TRANS("As a professional, I understand and I can manipulate classical sound synthesisers and filtering techniques")));
    addAndMakeVisible (expertiseLevelLabel5.get());
    expertiseLevelLabel5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    expertiseLevelLabel5->setJustificationType (Justification::centredTop);
    expertiseLevelLabel5->setEditable (false, false, false);
    expertiseLevelLabel5->setColour (Label::textColourId, Colour (0xffb0b0b0));
    expertiseLevelLabel5->setColour (TextEditor::textColourId, Colours::black);
    expertiseLevelLabel5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expertiseLevelLabel5->setBounds (1144, 136, 200, 80);

    remarkLabel.reset (new Label ("remark label",
                                  TRANS("If you have any remark, comment or suggestion, please feel free to write it below:")));
    addAndMakeVisible (remarkLabel.get());
    remarkLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    remarkLabel->setJustificationType (Justification::centredLeft);
    remarkLabel->setEditable (false, false, false);
    remarkLabel->setColour (TextEditor::textColourId, Colours::black);
    remarkLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    remarkLabel->setBounds (79, 668, 680, 24);

    remarkTextEditor.reset (new TextEditor ("remark text editor"));
    addAndMakeVisible (remarkTextEditor.get());
    remarkTextEditor->setMultiLine (false);
    remarkTextEditor->setReturnKeyStartsNewLine (false);
    remarkTextEditor->setReadOnly (false);
    remarkTextEditor->setScrollbarsShown (true);
    remarkTextEditor->setCaretVisible (true);
    remarkTextEditor->setPopupMenuEnabled (true);
    remarkTextEditor->setText (String());

    fastLabel.reset (new Label ("fast label",
                                TRANS("Which method seemed to be the fastest?")));
    addAndMakeVisible (fastLabel.get());
    fastLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    fastLabel->setJustificationType (Justification::centred);
    fastLabel->setEditable (false, false, false);
    fastLabel->setColour (TextEditor::textColourId, Colours::black);
    fastLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    fastLabel->setBounds (294, 292, 568, 24);

    imageButton.reset (new ImageButton ("new button"));
    addAndMakeVisible (imageButton.get());

    imageButton->setImages (false, true, false,
                            ImageCache::getFromMemory (BinaryData::_4_Faders_png, BinaryData::_4_Faders_pngSize), 1.000f, Colour (0x00c80606),
                            ImageCache::getFromMemory (BinaryData::_4_Faders_png, BinaryData::_4_Faders_pngSize), 1.000f, Colour (0x00000000),
                            ImageCache::getFromMemory (BinaryData::_4_Faders_png, BinaryData::_4_Faders_pngSize), 1.000f, Colour (0x00000000));
    imageButton->setBounds (150, 312, 256, 160);

    preferLabel.reset (new Label ("prefer label",
                                  TRANS("In general, which method did you prefer?")));
    addAndMakeVisible (preferLabel.get());
    preferLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    preferLabel->setJustificationType (Justification::centred);
    preferLabel->setEditable (false, false, false);
    preferLabel->setColour (TextEditor::textColourId, Colours::black);
    preferLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    preferLabel->setBounds (294, 448, 568, 24);

    fastSlider.reset (new Slider ("fast slider"));
    addAndMakeVisible (fastSlider.get());
    fastSlider->setRange (1, 3, 1);
    fastSlider->setSliderStyle (Slider::LinearHorizontal);
    fastSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fastSlider->setColour (Slider::backgroundColourId, Colour (0xff353c40));
    fastSlider->setColour (Slider::thumbColourId, Colour (0xff08cef3));
    fastSlider->setColour (Slider::trackColourId, Colour (0xff353c40));
    fastSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x0042a2c8));

    fastSlider->setBounds (488, 312, 176, 24);

    intuitiveSlider.reset (new Slider ("intuitive slider"));
    addAndMakeVisible (intuitiveSlider.get());
    intuitiveSlider->setRange (1, 3, 1);
    intuitiveSlider->setSliderStyle (Slider::LinearHorizontal);
    intuitiveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    intuitiveSlider->setColour (Slider::backgroundColourId, Colour (0xff353c40));
    intuitiveSlider->setColour (Slider::thumbColourId, Colour (0xff08cef3));
    intuitiveSlider->setColour (Slider::trackColourId, Colour (0xff353c40));
    intuitiveSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x0042a2c8));

    intuitiveSlider->setBounds (488, 412, 176, 24);

    preferSlider.reset (new Slider ("prefer slider"));
    addAndMakeVisible (preferSlider.get());
    preferSlider->setRange (1, 3, 1);
    preferSlider->setSliderStyle (Slider::LinearHorizontal);
    preferSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    preferSlider->setColour (Slider::backgroundColourId, Colour (0xff353c40));
    preferSlider->setColour (Slider::thumbColourId, Colour (0xff08cef3));
    preferSlider->setColour (Slider::trackColourId, Colour (0xff353c40));
    preferSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x0042a2c8));

    preferSlider->setBounds (488, 464, 176, 32);

    imageButton2.reset (new ImageButton ("new button"));
    addAndMakeVisible (imageButton2.get());

    imageButton2->setImages (false, true, false,
                             ImageCache::getFromMemory (BinaryData::_4_Interpolation_areas_png, BinaryData::_4_Interpolation_areas_pngSize), 1.000f, Colour (0x00000000),
                             ImageCache::getFromMemory (BinaryData::_4_Interpolation_areas_png, BinaryData::_4_Interpolation_areas_pngSize), 1.000f, Colour (0x00000000),
                             ImageCache::getFromMemory (BinaryData::_4_Interpolation_areas_png, BinaryData::_4_Interpolation_areas_pngSize), 1.000f, Colour (0x00000000));
    imageButton2->setBounds (750, 304, 232, 176);

    precisionLabel2.reset (new Label ("precision label",
                                      TRANS("For each of the following questions, please move the blue cursor to indicate the method you would chose. If you do not have an opinion, leave the blue cursor at center position.")));
    addAndMakeVisible (precisionLabel2.get());
    precisionLabel2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    precisionLabel2->setJustificationType (Justification::centred);
    precisionLabel2->setEditable (false, false, false);
    precisionLabel2->setColour (Label::textColourId, Colours::white);
    precisionLabel2->setColour (TextEditor::textColourId, Colours::black);
    precisionLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    precisionLabel2->setBounds (224, 224, 744, 56);

    precisionLabel.reset (new Label ("precision label",
                                     TRANS("Which method seemed to be the most precise?")));
    addAndMakeVisible (precisionLabel.get());
    precisionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    precisionLabel->setJustificationType (Justification::centred);
    precisionLabel->setEditable (false, false, false);
    precisionLabel->setColour (TextEditor::textColourId, Colours::black);
    precisionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    precisionLabel->setBounds (294, 340, 568, 24);

    preciseSlider.reset (new Slider ("precise slider"));
    addAndMakeVisible (preciseSlider.get());
    preciseSlider->setRange (1, 3, 1);
    preciseSlider->setSliderStyle (Slider::LinearHorizontal);
    preciseSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    preciseSlider->setColour (Slider::backgroundColourId, Colour (0xff353c40));
    preciseSlider->setColour (Slider::thumbColourId, Colour (0xff08cef3));
    preciseSlider->setColour (Slider::trackColourId, Colour (0xff353c40));
    preciseSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x0042a2c8));

    preciseSlider->setBounds (488, 360, 176, 24);

    intuitiveLabel.reset (new Label ("intuitive label",
                                     TRANS("Which method seemed to be the most intuitive?")));
    addAndMakeVisible (intuitiveLabel.get());
    intuitiveLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    intuitiveLabel->setJustificationType (Justification::centred);
    intuitiveLabel->setEditable (false, false, false);
    intuitiveLabel->setColour (TextEditor::textColourId, Colours::black);
    intuitiveLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    intuitiveLabel->setBounds (294, 392, 568, 24);

    separationBar2.reset (new GroupComponent ("new group",
                                              String()));
    addAndMakeVisible (separationBar2.get());

    separationBar3.reset (new GroupComponent ("new group",
                                              String()));
    addAndMakeVisible (separationBar3.get());

    separationBar4.reset (new GroupComponent ("new group",
                                              String()));
    addAndMakeVisible (separationBar4.get());

    separationBar5.reset (new GroupComponent ("new group",
                                              String()));
    addAndMakeVisible (separationBar5.get());

    labelQ1.reset (new Label ("new label",
                              TRANS("1")));
    addAndMakeVisible (labelQ1.get());
    labelQ1->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold"));
    labelQ1->setJustificationType (Justification::centred);
    labelQ1->setEditable (false, false, false);
    labelQ1->setColour (Label::outlineColourId, Colours::white);
    labelQ1->setColour (TextEditor::textColourId, Colours::black);
    labelQ1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelQ1->setBounds (8, 56, 48, 40);

    selfExpertiseLevelLabel2.reset (new Label ("expertise level label",
                                               TRANS("My level = ")));
    addAndMakeVisible (selfExpertiseLevelLabel2.get());
    selfExpertiseLevelLabel2->setFont (Font (15.00f, Font::italic));
    selfExpertiseLevelLabel2->setJustificationType (Justification::centredRight);
    selfExpertiseLevelLabel2->setEditable (false, false, false);
    selfExpertiseLevelLabel2->setColour (TextEditor::textColourId, Colours::black);
    selfExpertiseLevelLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    selfExpertiseLevelLabel2->setBounds (496, 86, 116, 24);

    labelQ2.reset (new Label ("new label",
                              TRANS("2")));
    addAndMakeVisible (labelQ2.get());
    labelQ2->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold"));
    labelQ2->setJustificationType (Justification::centred);
    labelQ2->setEditable (false, false, false);
    labelQ2->setColour (Label::outlineColourId, Colours::white);
    labelQ2->setColour (TextEditor::textColourId, Colours::black);
    labelQ2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelQ2->setBounds (8, 336, 48, 40);

    labelQ3.reset (new Label ("new label",
                              TRANS("3")));
    addAndMakeVisible (labelQ3.get());
    labelQ3->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold"));
    labelQ3->setJustificationType (Justification::centred);
    labelQ3->setEditable (false, false, false);
    labelQ3->setColour (Label::outlineColourId, Colours::white);
    labelQ3->setColour (TextEditor::textColourId, Colours::black);
    labelQ3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelQ3->setBounds (8, 512, 48, 40);

    labelQ4.reset (new Label ("new label",
                              TRANS("4")));
    addAndMakeVisible (labelQ4.get());
    labelQ4->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold"));
    labelQ4->setJustificationType (Justification::centred);
    labelQ4->setEditable (false, false, false);
    labelQ4->setColour (Label::outlineColourId, Colours::white);
    labelQ4->setColour (TextEditor::textColourId, Colours::black);
    labelQ4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelQ4->setBounds (8, 592, 48, 40);

    labelQ5.reset (new Label ("new label",
                              TRANS("5")));
    addAndMakeVisible (labelQ5.get());
    labelQ5->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold"));
    labelQ5->setJustificationType (Justification::centred);
    labelQ5->setEditable (false, false, false);
    labelQ5->setColour (Label::outlineColourId, Colours::white);
    labelQ5->setColour (TextEditor::textColourId, Colours::black);
    labelQ5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelQ5->setBounds (8, 680, 48, 40);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1450, 830);


    //[Constructor] You can add your own custom stuff here..

    // VALEURS PAR DEFAUT DES SLIDER
    expertiseSlider->setValue(2.0); // moyenne = 3
    preciseSlider->setValue(2.0); // moyenne = 2
    intuitiveSlider->setValue(2.0); // moyenne = 2
    fastSlider->setValue(2.0); // moyenne = 2
    preferSlider->setValue(2.0); // moyenne = 2

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
    remarkLabel = nullptr;
    remarkTextEditor = nullptr;
    fastLabel = nullptr;
    imageButton = nullptr;
    preferLabel = nullptr;
    fastSlider = nullptr;
    intuitiveSlider = nullptr;
    preferSlider = nullptr;
    imageButton2 = nullptr;
    precisionLabel2 = nullptr;
    precisionLabel = nullptr;
    preciseSlider = nullptr;
    intuitiveLabel = nullptr;
    separationBar2 = nullptr;
    separationBar3 = nullptr;
    separationBar4 = nullptr;
    separationBar5 = nullptr;
    labelQ1 = nullptr;
    selfExpertiseLevelLabel2 = nullptr;
    labelQ2 = nullptr;
    labelQ3 = nullptr;
    labelQ4 = nullptr;
    labelQ5 = nullptr;


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

    label->setBounds ((getWidth() / 2) - ((getWidth() - 80) / 2), 8, getWidth() - 80, 40);
    similarExpeTextEditor->setBounds (80, 624, getWidth() - 100, 24);
    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 100, 500, 80);
    remarkTextEditor->setBounds (79, 696, getWidth() - 100, 24);
    separationBar2->setBounds (0, 488, proportionOfWidth (1.0000f), 13);
    separationBar3->setBounds (0, 560, proportionOfWidth (1.0000f), 13);
    separationBar4->setBounds (0, 651, proportionOfWidth (1.0000f), 13);
    separationBar5->setBounds (0, 213, proportionOfWidth (1.0000f), 13);
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
        questionsAnswered = true;
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

    // infos dans l'ordre d'affichage (de haut en bas)

    questionsChildrenTree->put("<xmlattr>.answered", questionsAnswered);
    questionsChildrenTree->put("expertise_level",
                               (int) expertiseSlider->getValue());

    bptree::ptree methodsOpinionTree;
    methodsOpinionTree.put("<xmlattr>.fastest", sliderValueToMethodName(fastSlider->getValue()));
    methodsOpinionTree.put("<xmlattr>.most_precise", sliderValueToMethodName(preciseSlider->getValue()));
    methodsOpinionTree.put("<xmlattr>.most_intuitive", sliderValueToMethodName(intuitiveSlider->getValue()));
    methodsOpinionTree.put("<xmlattr>.preferred", sliderValueToMethodName(preferSlider->getValue()));
    questionsChildrenTree->add_child("methods_opinion", methodsOpinionTree);

    bptree::ptree similarApp;
    similarApp.put("<xmlattr>.checked", (bool)similarInterfaceToggleButton->getToggleState());
    if (similarInterfaceToggleButton->getToggleState())
        similarApp.put_value(similarInterfaceTextEditor->getText().toRawUTF8());
    questionsChildrenTree->add_child("similar_interface", similarApp);

    bptree::ptree similarExpe;
    similarExpe.put("<xmlattr>.checked", (bool)similarExperimentToggleButton->getToggleState());
    if (similarExperimentToggleButton->getToggleState())
        similarExpe.put_value(similarExpeTextEditor->getText().toRawUTF8());
    questionsChildrenTree->add_child("similar_expe", similarExpe);

    bptree::ptree remarkTree;
    remarkTree.put("<xmlattr>.is_empty", (bool)remarkTextEditor->isEmpty());
    if (! remarkTextEditor->isEmpty())
        remarkTree.put_value(remarkTextEditor->getText().toRawUTF8());
    questionsChildrenTree->add_child("remark", remarkTree);

    return questionsChildrenTree;
}

std::string UserFinalQuestions::sliderValueToMethodName(double sliderValue)
{
    int intChoice = (int)sliderValue;
    assert( (1 <= intChoice) && (intChoice <= 3) ); // slider à 3 positions seulement !!
    switch (intChoice)
    {
        case 1:
            return "fader";
        case 2:
            return "none";
        case 3:
            return "interpolation";
        default:
            return "undefined";
    }
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
                 fixedSize="1" initialWidth="1450" initialHeight="830">
  <METHODS>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff111111"/>
  <TOGGLEBUTTON name="new toggle button" id="62554cf2d48d535" memberName="similarInterfaceToggleButton"
                virtualName="" explicitFocusOrder="0" pos="79 512 672 24" buttonText="I have already tested a similar touch app"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="6cd75e74f70bffa5" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0Cc 8 80M 40" outlineCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="It is almost finished! Please read and answer these last few questions."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="2e1" kerning="0" bold="1" italic="0"
         justification="36" typefaceStyle="Bold"/>
  <LABEL name="Similar Interface Question label" id="a8708d571b59b51"
         memberName="similarInterfaceQuestionLabel" virtualName="" explicitFocusOrder="0"
         pos="79 536 392 24" edTextCol="ff000000" edBkgCol="0" labelText="If you can, please tell us the name of the app:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="34"/>
  <TEXTEDITOR name="new text editor" id="91cc4dd0fcb0d776" memberName="similarInterfaceTextEditor"
              virtualName="" explicitFocusOrder="0" pos="471 536 376 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="new toggle button" id="9994df3dee6fae04" memberName="similarExperimentToggleButton"
                virtualName="" explicitFocusOrder="0" pos="80 576 672 24" buttonText="I have already participated in a similar scientific experiment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="Similar EXperiment Question label" id="94f1b219535bfde4"
         memberName="similarExpeQuestionLabel" virtualName="" explicitFocusOrder="0"
         pos="80 600 696 24" edTextCol="ff000000" edBkgCol="0" labelText="Please tell us its name, location and/or context in the form below: "
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="f104febd5cf021fd" memberName="similarExpeTextEditor"
              virtualName="" explicitFocusOrder="0" pos="80 624 100M 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 100R 500 80" bgColOff="ff097c2a"
              bgColOn="ff252525" buttonText="OK, save data and finish the experiment"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="expertise level label" id="b435fe1e58647b28" memberName="selfExpertiseLevelLabel"
         virtualName="" explicitFocusOrder="0" pos="60 56 1012 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Concerning sound synthesisers and filters, what is your level of expertise ? (Click on the right answer or move the blue cursor)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <SLIDER name="new slider" id="bfaef998cabecf5d" memberName="expertiseSlider"
          virtualName="" explicitFocusOrder="0" pos="56 88 1200 48" bkgcol="ff353c40"
          thumbcol="ff08cef3" trackcol="ff353c40" textboxhighlight="42a2c8"
          min="1" max="5" int="1" style="LinearHorizontal" textBoxPos="TextBoxAbove"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <LABEL name="expertise level  label" id="2f66faf604745e12" memberName="expertiseLevelLabel"
         virtualName="Label" explicitFocusOrder="0" pos="8 136 208 80"
         textCol="ffb0b0b0" edTextCol="ff000000" edBkgCol="0" labelText="Before this experiment, I did not know anything about sound synthesisers and filters"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="8fb437ba6c83e57d" memberName="expertiseLevelLabel2"
         virtualName="" explicitFocusOrder="0" pos="248 136 224 80" textCol="ffb0b0b0"
         edTextCol="ff000000" edBkgCol="0" labelText="I have already played with synthesisers and filters, but I don't know how they work"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="ea90a2c7054e88e" memberName="expertiseLevelLabel3"
         virtualName="" explicitFocusOrder="0" pos="504 136 296 80" textCol="ffb0b0b0"
         edTextCol="ff000000" edBkgCol="0" labelText="As an amateur, I have already played with synths and filters, and I know a bit how to use them. I can identify some sound characteristics."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="dd0cf4ee1b6450df" memberName="expertiseLevelLabel4"
         virtualName="" explicitFocusOrder="0" pos="824 136 280 80" textCol="ffb0b0b0"
         edTextCol="ff000000" edBkgCol="0" labelText="I play quite often with synths and filters, and I can configure them to produce sounds I want. I can easily identify the main sound characteristics."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="expertise level  label" id="fa98c1f744ec1239" memberName="expertiseLevelLabel5"
         virtualName="" explicitFocusOrder="0" pos="1144 136 200 80" textCol="ffb0b0b0"
         edTextCol="ff000000" edBkgCol="0" labelText="As a professional, I understand and I can manipulate classical sound synthesisers and filtering techniques"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="12"/>
  <LABEL name="remark label" id="5612d9d376b01f96" memberName="remarkLabel"
         virtualName="" explicitFocusOrder="0" pos="79 668 680 24" edTextCol="ff000000"
         edBkgCol="0" labelText="If you have any remark, comment or suggestion, please feel free to write it below:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <TEXTEDITOR name="remark text editor" id="2b57c09bc226c2d2" memberName="remarkTextEditor"
              virtualName="" explicitFocusOrder="0" pos="79 696 100M 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="fast label" id="4bc9d2e8edc70f1d" memberName="fastLabel"
         virtualName="" explicitFocusOrder="0" pos="294 292 568 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Which method seemed to be the fastest?"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <IMAGEBUTTON name="new button" id="c34d6540fcb75f68" memberName="imageButton"
               virtualName="" explicitFocusOrder="0" pos="150 312 256 160" buttonText="new button"
               connectedEdges="0" needsCallback="0" radioGroupId="0" keepProportions="0"
               resourceNormal="BinaryData::_4_Faders_png" opacityNormal="1"
               colourNormal="c80606" resourceOver="BinaryData::_4_Faders_png"
               opacityOver="1" colourOver="0" resourceDown="BinaryData::_4_Faders_png"
               opacityDown="1" colourDown="0"/>
  <LABEL name="prefer label" id="fadad2e5f94a2ff2" memberName="preferLabel"
         virtualName="" explicitFocusOrder="0" pos="294 448 568 24" edTextCol="ff000000"
         edBkgCol="0" labelText="In general, which method did you prefer?"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="fast slider" id="9c198d9742f2d6de" memberName="fastSlider"
          virtualName="" explicitFocusOrder="0" pos="488 312 176 24" bkgcol="ff353c40"
          thumbcol="ff08cef3" trackcol="ff353c40" textboxhighlight="42a2c8"
          min="1" max="3" int="1" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="intuitive slider" id="37905ac2c9d91ce3" memberName="intuitiveSlider"
          virtualName="" explicitFocusOrder="0" pos="488 412 176 24" bkgcol="ff353c40"
          thumbcol="ff08cef3" trackcol="ff353c40" textboxhighlight="42a2c8"
          min="1" max="3" int="1" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="prefer slider" id="d70c806ce2b41274" memberName="preferSlider"
          virtualName="" explicitFocusOrder="0" pos="488 464 176 32" bkgcol="ff353c40"
          thumbcol="ff08cef3" trackcol="ff353c40" textboxhighlight="42a2c8"
          min="1" max="3" int="1" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <IMAGEBUTTON name="new button" id="77a64db2cbc4aa79" memberName="imageButton2"
               virtualName="" explicitFocusOrder="0" pos="750 304 232 176" buttonText="new button"
               connectedEdges="0" needsCallback="0" radioGroupId="0" keepProportions="0"
               resourceNormal="BinaryData::_4_Interpolation_areas_png" opacityNormal="1"
               colourNormal="0" resourceOver="BinaryData::_4_Interpolation_areas_png"
               opacityOver="1" colourOver="0" resourceDown="BinaryData::_4_Interpolation_areas_png"
               opacityDown="1" colourDown="0"/>
  <LABEL name="precision label" id="4d1ebe657734ff2" memberName="precisionLabel2"
         virtualName="" explicitFocusOrder="0" pos="224 224 744 56" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="For each of the following questions, please move the blue cursor to indicate the method you would chose. If you do not have an opinion, leave the blue cursor at center position."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="1"
         italic="0" justification="36" typefaceStyle="Bold"/>
  <LABEL name="precision label" id="4b32204f928efd51" memberName="precisionLabel"
         virtualName="" explicitFocusOrder="0" pos="294 340 568 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Which method seemed to be the most precise?"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="precise slider" id="f33978f42f481274" memberName="preciseSlider"
          virtualName="" explicitFocusOrder="0" pos="488 360 176 24" bkgcol="ff353c40"
          thumbcol="ff08cef3" trackcol="ff353c40" textboxhighlight="42a2c8"
          min="1" max="3" int="1" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <LABEL name="intuitive label" id="6c58855651dc83ea" memberName="intuitiveLabel"
         virtualName="" explicitFocusOrder="0" pos="294 392 568 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Which method seemed to be the most intuitive?"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <GROUPCOMPONENT name="new group" id="87b52c6c24bf665c" memberName="separationBar2"
                  virtualName="" explicitFocusOrder="0" pos="0 488 100% 13" title=""/>
  <GROUPCOMPONENT name="new group" id="ce14edd37f9bd52f" memberName="separationBar3"
                  virtualName="" explicitFocusOrder="0" pos="0 560 100% 13" title=""/>
  <GROUPCOMPONENT name="new group" id="563567b5cba66acb" memberName="separationBar4"
                  virtualName="" explicitFocusOrder="0" pos="0 651 100% 13" title=""/>
  <GROUPCOMPONENT name="new group" id="9bdf8efe9a1fcd21" memberName="separationBar5"
                  virtualName="" explicitFocusOrder="0" pos="0 213 100% 13" title=""/>
  <LABEL name="new label" id="1ac79a99bf2ecec2" memberName="labelQ1" virtualName=""
         explicitFocusOrder="0" pos="8 56 48 40" outlineCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2e1" kerning="0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <LABEL name="expertise level label" id="e1f0a84536dc9b30" memberName="selfExpertiseLevelLabel2"
         virtualName="" explicitFocusOrder="0" pos="496 86 116 24" edTextCol="ff000000"
         edBkgCol="0" labelText="My level = " editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="1" justification="34"
         typefaceStyle="Italic"/>
  <LABEL name="new label" id="eed87bdbdef9c6d7" memberName="labelQ2" virtualName=""
         explicitFocusOrder="0" pos="8 336 48 40" outlineCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2e1" kerning="0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="b9e69ee9ba396e60" memberName="labelQ3" virtualName=""
         explicitFocusOrder="0" pos="8 512 48 40" outlineCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2e1" kerning="0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="c99ca99be276b61d" memberName="labelQ4" virtualName=""
         explicitFocusOrder="0" pos="8 592 48 40" outlineCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2e1" kerning="0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="c0c05fea5aa78c67" memberName="labelQ5" virtualName=""
         explicitFocusOrder="0" pos="8 680 48 40" outlineCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="5" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2e1" kerning="0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

