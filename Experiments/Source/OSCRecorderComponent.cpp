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

#include "OSCRecorderComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCRecorderComponent::OSCRecorderComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    startButton.reset (new TextButton ("start button"));
    addAndMakeVisible (startButton.get());
    startButton->setButtonText (TRANS("BEGIN PRESET RESEARCH"));
    startButton->addListener (this);
    startButton->setColour (TextButton::buttonColourId, Colour (0x503d338d));
    startButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    listenButton.reset (new TextButton ("listen button"));
    addAndMakeVisible (listenButton.get());
    listenButton->setButtonText (TRANS("LISTEN TO THE PRESET"));
    listenButton->addListener (this);
    listenButton->setColour (TextButton::buttonColourId, Colour (0x50747474));
    listenButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    finishedButton.reset (new TextButton ("finished button"));
    addAndMakeVisible (finishedButton.get());
    finishedButton->setButtonText (TRANS("OK !"));
    finishedButton->addListener (this);
    finishedButton->setColour (TextButton::buttonColourId, Colour (0xff097c2a));
    finishedButton->setColour (TextButton::buttonOnColourId, Colour (0xff252525));

    countLabel.reset (new Label ("count label",
                                 TRANS("Current preset: .../...")));
    addAndMakeVisible (countLabel.get());
    countLabel->setFont (Font (22.00f, Font::plain).withTypefaceStyle ("Regular"));
    countLabel->setJustificationType (Justification::centred);
    countLabel->setEditable (false, false, false);
    countLabel->setColour (TextEditor::textColourId, Colours::black);
    countLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    listenLabel.reset (new Label ("Listen label",
                                  TRANS("Listening to reference sound will start in:")));
    addAndMakeVisible (listenLabel.get());
    listenLabel->setFont (Font (26.00f, Font::plain).withTypefaceStyle ("Regular"));
    listenLabel->setJustificationType (Justification::centredRight);
    listenLabel->setEditable (false, false, false);
    listenLabel->setColour (Label::backgroundColourId, Colour (0x00757575));
    listenLabel->setColour (Label::textColourId, Colour (0xffb0b0b0));
    listenLabel->setColour (TextEditor::textColourId, Colours::black);
    listenLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    searchLabel.reset (new Label ("Search label",
                                  TRANS("Preset research will start in:")));
    addAndMakeVisible (searchLabel.get());
    searchLabel->setFont (Font (26.00f, Font::plain).withTypefaceStyle ("Regular"));
    searchLabel->setJustificationType (Justification::centredRight);
    searchLabel->setEditable (false, false, false);
    searchLabel->setColour (Label::textColourId, Colour (0xff81a5ff));
    searchLabel->setColour (TextEditor::textColourId, Colours::black);
    searchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    remainingTimeSlider.reset (new Slider ("Remaining Time slider"));
    addAndMakeVisible (remainingTimeSlider.get());
    remainingTimeSlider->setRange (0, 60, 1);
    remainingTimeSlider->setSliderStyle (Slider::LinearBar);
    remainingTimeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    remainingTimeSlider->setColour (Slider::backgroundColourId, Colour (0x00263238));
    remainingTimeSlider->setColour (Slider::trackColourId, Colours::white);
    remainingTimeSlider->setColour (Slider::textBoxOutlineColourId, Colours::white);

    listenCountdownLabel.reset (new Label ("Listen Countdown label",
                                           TRANS("3")));
    addAndMakeVisible (listenCountdownLabel.get());
    listenCountdownLabel->setFont (Font (26.00f, Font::plain).withTypefaceStyle ("Bold"));
    listenCountdownLabel->setJustificationType (Justification::centredLeft);
    listenCountdownLabel->setEditable (false, false, false);
    listenCountdownLabel->setColour (Label::backgroundColourId, Colour (0x00757575));
    listenCountdownLabel->setColour (TextEditor::textColourId, Colours::black);
    listenCountdownLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    searchCountdownLabel.reset (new Label ("Search Countdown label",
                                           TRANS("3")));
    addAndMakeVisible (searchCountdownLabel.get());
    searchCountdownLabel->setFont (Font (26.00f, Font::plain).withTypefaceStyle ("Bold"));
    searchCountdownLabel->setJustificationType (Justification::centredLeft);
    searchCountdownLabel->setEditable (false, false, false);
    searchCountdownLabel->setColour (Label::backgroundColourId, Colour (0x00757575));
    searchCountdownLabel->setColour (TextEditor::textColourId, Colours::black);
    searchCountdownLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    progressBarSlider.reset (new Slider ("Progress Bar slider"));
    addAndMakeVisible (progressBarSlider.get());
    progressBarSlider->setRange (0, 22, 1);
    progressBarSlider->setSliderStyle (Slider::LinearBar);
    progressBarSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    progressBarSlider->setColour (Slider::backgroundColourId, Colour (0x00757575));
    progressBarSlider->setColour (Slider::trackColourId, Colour (0xff757575));
    progressBarSlider->setColour (Slider::textBoxOutlineColourId, Colours::white);

    scoreLabel.reset (new Label ("Score label",
                                 TRANS("Score: 73/100 !")));
    addAndMakeVisible (scoreLabel.get());
    scoreLabel->setFont (Font (36.00f, Font::plain).withTypefaceStyle ("Bold"));
    scoreLabel->setJustificationType (Justification::centred);
    scoreLabel->setEditable (false, false, false);
    scoreLabel->setColour (Label::backgroundColourId, Colour (0x00ff0000));
    scoreLabel->setColour (Label::textColourId, Colour (0xff49ff08));
    scoreLabel->setColour (TextEditor::textColourId, Colours::black);
    scoreLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    infoLabel.reset (new Label ("info label",
                                TRANS("(noise)")));
    addAndMakeVisible (infoLabel.get());
    infoLabel->setFont (Font (18.00f, Font::italic));
    infoLabel->setJustificationType (Justification::centred);
    infoLabel->setEditable (false, false, false);
    infoLabel->setColour (TextEditor::textColourId, Colours::black);
    infoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    infoLabel2.reset (new Label ("info label",
                                 TRANS("Next step will start in a few seconds")));
    addAndMakeVisible (infoLabel2.get());
    infoLabel2->setFont (Font (18.00f, Font::plain).withTypefaceStyle ("Regular"));
    infoLabel2->setJustificationType (Justification::centred);
    infoLabel2->setEditable (false, false, false);
    infoLabel2->setColour (TextEditor::textColourId, Colours::black);
    infoLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1920, 1080);


    //[Constructor] You can add your own custom stuff here..

    // Many buttons are now removed (auto trigger of listen & search)
    // and disabled, such that they can still be used in algorithms (in re-activated
    // at some point...)
    listenButton->setEnabled(false);
    startButton->setEnabled(false);
    removeChildComponent(listenButton.get());
    removeChildComponent(startButton.get());

    // If the research time is longer, the main remainig time
    // bar becomes darker... (totally dark for 120s research)
    /*float barBrightness = 1.0f
    - ((float)(OSCRecorder::ResearchTimeMax_ms) / 1000.0f) / 120.0f;
     */
    //.... -> fixed brightness at the moment
    float barBrightness = 0.9f;
    remainingTimeSlider->setColour(Slider::ColourIds::trackColourId,
                                   Colour(0.0f, 0.0f, barBrightness, 1.0f));

    //[/Constructor]
}

OSCRecorderComponent::~OSCRecorderComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    startButton = nullptr;
    listenButton = nullptr;
    finishedButton = nullptr;
    countLabel = nullptr;
    listenLabel = nullptr;
    searchLabel = nullptr;
    remainingTimeSlider = nullptr;
    listenCountdownLabel = nullptr;
    searchCountdownLabel = nullptr;
    progressBarSlider = nullptr;
    scoreLabel = nullptr;
    infoLabel = nullptr;
    infoLabel2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCRecorderComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff313131));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCRecorderComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    startButton->setBounds ((getWidth() / 2) - (500 / 2), 200, 500, 120);
    listenButton->setBounds ((getWidth() / 2) - (500 / 2), 20, 500, 120);
    finishedButton->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 160 - 160, 500, 160);
    countLabel->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 100, 500, 32);
    listenLabel->setBounds ((getWidth() / 2) - 569, 80, 569, 32);
    searchLabel->setBounds ((getWidth() / 2) - 569, 160, 569, 32);
    remainingTimeSlider->setBounds ((getWidth() / 2) - ((getWidth() - 160) / 2), 200, getWidth() - 160, getHeight() - 550);
    listenCountdownLabel->setBounds ((getWidth() / 2) + 8, 80, 569, 32);
    searchCountdownLabel->setBounds ((getWidth() / 2) + 8, 160, 569, 32);
    progressBarSlider->setBounds ((getWidth() / 2) - (1200 / 2), getHeight() - 60, 1200, 40);
    scoreLabel->setBounds ((getWidth() / 2) - (500 / 2), getHeight() - 208, 500, 32);
    infoLabel->setBounds ((getWidth() / 2) - (300 / 2), (getHeight() / 2) + -20 - (24 / 2), 300, 24);
    infoLabel2->setBounds ((getWidth() / 2) - (600 / 2), (getHeight() / 2) + 20 - (24 / 2), 600, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCRecorderComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    if (recorderManager == 0)
    {
        // manager must be set at this point...
        assert(false);
        return;
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == startButton.get())
    {
        //[UserButtonCode_startButton] -- add your button handler code here..
        assert(false); // click on button is now deactivated (automatic listening trigger)
        //recorderManager->OnButtonClicked(ExperimentState::SearchingPreset);
        //[/UserButtonCode_startButton]
    }
    else if (buttonThatWasClicked == listenButton.get())
    {
        //[UserButtonCode_listenButton] -- add your button handler code here..
        assert(false); // click on button is now deactivated (automatic research trigger)
        //recorderManager->OnButtonClicked(ExperimentState::Listening);
        //[/UserButtonCode_listenButton]
    }
    else if (buttonThatWasClicked == finishedButton.get())
    {
        //[UserButtonCode_finishedButton] -- add your button handler code here..
        recorderManager->OnButtonClicked(ExperimentState::FinishedSearchingPreset);
        //[/UserButtonCode_finishedButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

bool OSCRecorderComponent::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    if (key.getKeyCode() == KeyPress::spaceKey
        || key.getKeyCode() == KeyPress::returnKey)
    {
        simulateClickOnDisplayedButton();
        return true;
    }
    else
    {
        return false;  // Return true if your handler uses this key event,
                        // or false to allow it to be passed-on.
    }

    //[/UserCode_keyPressed]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


void OSCRecorderComponent::DisplayNewState(ExperimentState newState, int presetStep, size_t presetsCount)
{
    setVisible(true); // visible par défaut

    // - - - - -  pour commencer : Boutons qui ne s'affichent que dans 1 seul mode - - - - - -
    listenButton->setVisible(newState == ExperimentState::ReadyToListen);
    listenLabel->setVisible(newState == ExperimentState::ReadyToListen);
    listenCountdownLabel->setVisible(newState == ExperimentState::ReadyToListen);
    startButton->setVisible(newState == ExperimentState::ReadyToSearchPreset);
    searchLabel->setVisible(newState == ExperimentState::ReadyToSearchPreset);
    searchCountdownLabel->setVisible(newState == ExperimentState::ReadyToSearchPreset);
    //progressBarSlider->setVisible(newState != ExperimentState::SearchingPreset
    //                              && newState != ExperimentState::Listening);
    finishedButton->setVisible(newState == ExperimentState::SearchingPreset);
    scoreLabel->setVisible(newState == ExperimentState::FinishedSearchingPreset);
    infoLabel->setVisible(newState == ExperimentState::FinishedSearchingPreset);
    //infoLabel2->setVisible(newState == ExperimentState::FinishedSearchingPreset);
    infoLabel2->setVisible(false);

    // - - - - - puis : Switch général pour affichage des cas plus complexes - - - - -
    switch(newState)
    {
        case ExperimentState::ConnectionLost:
        case ExperimentState::WaitingForTcpServerConnection:
        case ExperimentState::IntroDescriptionDisplayed:
        case ExperimentState::PostTrialDescriptionDisplayed:
        case ExperimentState::InitialQuestionsDisplayed:
        case ExperimentState::FinalQuestionsDisplayed:
        case ExperimentState::Finished:
            setVisible(false); // auto hiding
            break;

        case ExperimentState::NotInitialized:
            countLabel->setVisible(false);
            remainingTimeSlider->setVisible(false);
            break;

        case ExperimentState::ReadyToListen:
            countLabel->setVisible(true);
            remainingTimeSlider->setVisible(false);
            // auto-trigger of timer
            listenTimerValue_s = OSCRecorder::ListenAutoTriggerDelay_s;
            updateTimerLabels();
            Timer::callAfterDelay(1000, [this] { listenTimerCallback(); } );
            break;

        case ExperimentState::Listening:
            countLabel->setVisible(true);
            remainingTimeSlider->setVisible(false);
            break;

        case ExperimentState::ReadyToSearchPreset:
            countLabel->setVisible(true);
            UpdateRemainingTimeSlider(remainingTimeSlider->getMaximum(),
                                      remainingTimeSlider->getMaximum());
            remainingTimeSlider->setVisible(true); // updated just before
            // auto-trigger of timer
            searchTimerValue_s = OSCRecorder::SearchAutoTriggerDelay_s;
            updateTimerLabels();
            Timer::callAfterDelay(1000, [this] { searchTimerCallback(); } );
            break;

        case ExperimentState::SearchingPreset:
            countLabel->setVisible(true); // toujours visible... fait partie de la gamification...
            remainingTimeSlider->setVisible(true);
            break;

        case ExperimentState::FinishedSearchingPreset:
            countLabel->setVisible(true);
            remainingTimeSlider->setVisible(false);
            break;

        default:
            break;
    }



    // COUNT LABEL + PROGRESS BAR Display
    // On if the state if an actual experiment state
    if (ExperimentStateUtils::IsInteractiveExperimentState(newState))
    {
        int presetNumber = presetStep + 1;

        // label
        String trialPresetStr = TRANS("TRIAL preset");
        if (presetStep >= 0) // actual presets
            countLabel->setText(TRANS("Current preset: ") + String(presetNumber) + String("/") + String(presetsCount), NotificationType::dontSendNotification);
        else if (presetStep == -2)
            countLabel->setText(trialPresetStr + " 1.", NotificationType::dontSendNotification);
        else if (presetStep == -1)
            countLabel->setText(trialPresetStr + " 2.", NotificationType::dontSendNotification);
        else if (presetStep == -3)
            countLabel->setText(TRANS("trials not started yet"), NotificationType::dontSendNotification);
        else
            assert(false); // un planned case

        // progress bar (pour 2 trials...)
        progressBarSlider->setRange(0, presetsCount + 2, 1);
        progressBarSlider->setValue(presetStep + 2);
    }
    else
    {
        countLabel->setText(TRANS("Next questions will appear soon..."),
                            NotificationType::dontSendNotification);
    }

    // Self-focus
    if (isVisible())
        grabKeyboardFocus();
}

void OSCRecorderComponent::UpdateRemainingTimeSlider(double duration, double maxDuration)
{
    remainingTimeSlider->setRange(0.0, maxDuration, 0.0);
    remainingTimeSlider->setValue(duration);
}
void OSCRecorderComponent::SetPerformance(double performance)
{
    int scoreOutOf100 = (int) std::round(performance * 100.0);
    scoreLabel->setText(TRANS("Score : ").toStdString()
                        + boost::lexical_cast<std::string>(scoreOutOf100)
                        + std::string("/100"),
                        NotificationType::sendNotification);
    // couleur mise à jour également
    scoreLabel->setColour(Label::ColourIds::textColourId,
                          Colour(0.04f + (float)(performance) * 0.35f,
                                 1.0f, 0.8f, 1.0f));
}

void OSCRecorderComponent::simulateClickOnDisplayedButton()
{
    // double-trigger check
    if (! clickHappenedRecently)
    {
        clickHappenedRecently = true;
        Timer::callAfterDelay(doubleClickThreshold_ms,
                              [this] {
                                  clickHappenedRecently = false;
                              });
    }
    else // si appui récent : on oublie simplement cet évènement
    {
        return; // by-pass
        //std::cout << "double-appui !!!!" << std::endl;
    }

    // On vérifie qu'on n'aie bien qu'un seul bouton appuyé à la fois...
    size_t visibleButtonsCount = 0;
    visibleButtonsCount += (startButton->isVisible() && startButton->isEnabled()) ? 1 : 0;
    visibleButtonsCount += (startButton->isVisible() && startButton->isEnabled()) ? 1 : 0;
    visibleButtonsCount += (finishedButton->isVisible() && finishedButton->isEnabled()) ? 1 : 0;
    assert(visibleButtonsCount <= 1);

    // tout à fait possible d'appuyer quand rien n'est affiché
    if (visibleButtonsCount == 0)
        return;
    else if (visibleButtonsCount == 1)
    {
        if (startButton->isVisible() && startButton->isEnabled())
            buttonClicked(startButton.get());
        else if (startButton->isVisible() && startButton->isEnabled())
            buttonClicked(listenButton.get());
        else if (finishedButton->isVisible() && finishedButton->isEnabled())
            buttonClicked(finishedButton.get());
    }
}

void OSCRecorderComponent::listenTimerCallback()
{
    listenTimerValue_s--;
    // Countdown Still happening : update of label
    if (listenTimerValue_s > 0)
    {
        Timer::callAfterDelay(1000, [this] { listenTimerCallback(); } );
        updateTimerLabels();
    }
    // end of countdown : auto-trigger
    else
    {
        listenTimerValue_s = -1;
        recorderManager->OnButtonClicked(ExperimentState::Listening);
    }
}
void OSCRecorderComponent::searchTimerCallback()
{
    searchTimerValue_s--;
    // Countdown Still happening : update of label
    if (searchTimerValue_s > 0)
    {
        Timer::callAfterDelay(1000, [this] { searchTimerCallback(); } );
        updateTimerLabels();
    }
    // end of countdown : auto-trigger
    else
    {
        searchTimerValue_s = -1;
        recorderManager->OnButtonClicked(ExperimentState::SearchingPreset);
    }
}
void OSCRecorderComponent::updateTimerLabels()
{
    if (listenTimerValue_s >= 0)
        listenCountdownLabel->setText(boost::lexical_cast<std::string>(listenTimerValue_s),
                                      NotificationType::sendNotification);
    else
        listenCountdownLabel->setText("...", NotificationType::sendNotification);
    if (searchTimerValue_s >= 0)
        searchCountdownLabel->setText(boost::lexical_cast<std::string>(searchTimerValue_s),
                                      NotificationType::sendNotification);
    else
        searchCountdownLabel->setText("...", NotificationType::sendNotification);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCRecorderComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1920" initialHeight="1080">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff313131"/>
  <TEXTBUTTON name="start button" id="cb0e665733fa514e" memberName="startButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 200 500 120" bgColOff="503d338d"
              bgColOn="ff252525" buttonText="BEGIN PRESET RESEARCH" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="listen button" id="54994c165790472c" memberName="listenButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 20 500 120" bgColOff="50747474"
              bgColOn="ff252525" buttonText="LISTEN TO THE PRESET" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="finished button" id="ee4745ef80623612" memberName="finishedButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 160Rr 500 160"
              bgColOff="ff097c2a" bgColOn="ff252525" buttonText="OK !" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="count label" id="6112b78b8bf2731a" memberName="countLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cc 100R 500 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Current preset: .../..." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2.2e1" kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="Listen label" id="451b58f2b7f59deb" memberName="listenLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 80 569 32" bkgCol="757575"
         textCol="ffb0b0b0" edTextCol="ff000000" edBkgCol="0" labelText="Listening to reference sound will start in:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="2.6e1" kerning="0" bold="0"
         italic="0" justification="34"/>
  <LABEL name="Search label" id="7a6208a1c5ecc70d" memberName="searchLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cr 160 569 32" textCol="ff81a5ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Preset research will start in:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="2.6e1" kerning="0" bold="0"
         italic="0" justification="34"/>
  <SLIDER name="Remaining Time slider" id="a43e709f917b56b4" memberName="remainingTimeSlider"
          virtualName="" explicitFocusOrder="0" pos="0Cc 200 160M 550M"
          bkgcol="263238" trackcol="ffffffff" textboxoutline="ffffffff"
          min="0" max="6e1" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <LABEL name="Listen Countdown label" id="3a60263a4de41695" memberName="listenCountdownLabel"
         virtualName="" explicitFocusOrder="0" pos="8C 80 569 32" bkgCol="757575"
         edTextCol="ff000000" edBkgCol="0" labelText="3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2.6e1" kerning="0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <LABEL name="Search Countdown label" id="297cc669074c0214" memberName="searchCountdownLabel"
         virtualName="" explicitFocusOrder="0" pos="8C 160 569 32" bkgCol="757575"
         edTextCol="ff000000" edBkgCol="0" labelText="3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="2.6e1" kerning="0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <SLIDER name="Progress Bar slider" id="aa51755cb08ebea9" memberName="progressBarSlider"
          virtualName="" explicitFocusOrder="0" pos="0Cc 60R 1200 40" bkgcol="757575"
          trackcol="ff757575" textboxoutline="ffffffff" min="0" max="2.2e1"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <LABEL name="Score label" id="32c9ef90fa7faecd" memberName="scoreLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cc 208R 500 32" bkgCol="ff0000"
         textCol="ff49ff08" edTextCol="ff000000" edBkgCol="0" labelText="Score: 73/100 !"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="3.6e1" kerning="0" bold="1"
         italic="0" justification="36" typefaceStyle="Bold"/>
  <LABEL name="info label" id="2ceb4d4b04427bed" memberName="infoLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cc -20Cc 300 24"
         edTextCol="ff000000" edBkgCol="0" labelText="(noise)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.8e1" kerning="0" bold="0" italic="1" justification="36"
         typefaceStyle="Italic"/>
  <LABEL name="info label" id="a3f2ac820781a9b" memberName="infoLabel2"
         virtualName="" explicitFocusOrder="0" pos="0Cc 20Cc 600 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Next step will start in a few seconds"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.8e1" kerning="0" bold="0"
         italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

