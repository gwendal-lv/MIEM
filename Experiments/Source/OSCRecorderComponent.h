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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "ExperimentState.h"

class OSCRecorder;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
   Classe qui gère l'affichage graphique du contrôleur principal de
 l'expérience,
 mais qui déclenche aussi des évènements après des attentes
 pour simuler des clics automatiques
 (pour + de simplicité...)
                                                                    //[/Comments]
*/
class OSCRecorderComponent  : public Component,
                              public Button::Listener
{
public:
    //==============================================================================
    OSCRecorderComponent ();
    ~OSCRecorderComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void SetRecorderManager(OSCRecorder* _recorder) {recorderManager = _recorder;}

    void DisplayNewState(ExperimentState newState, int presetStep, size_t presetsCount);

    /// \brief Might also change the color depending on remaining time
    void UpdateRemainingTimeSlider(double duration, double maxDuration);
    /// \brief Might also change the color of the label, depending on the normalized performance
    void SetPerformance(double performance);

    /// \brief Simule un clic sur le bouton affiché et activé,
    /// s'il y en a bien un d'affiché et activé. Avec protection contre double-clic
    void simulateClickOnDisplayedButton();

    private :

    void listenTimerCallback();
    void searchTimerCallback();
    void updateTimerLabels();

    public :
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    bool keyPressed (const KeyPress& key) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    OSCRecorder* recorderManager = 0;

    // permet de détecter/emêcher les doubles-appuis sur la barre d'espace,
    // double-clic, ou bien 2 évènements trop proches de manière générale
    bool clickHappenedRecently = false;
    const int doubleClickThreshold_ms = 500;

    // Pour faire visuellement le décompte du temps restant
    // avant auto-déclenchement de l'écoute ou de la recherche de preset
    int listenTimerValue_s = -1 ; ///< Duration before auto-triggering
    int searchTimerValue_s = -1; ///< Duration before auto-triggering

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> startButton;
    std::unique_ptr<TextButton> listenButton;
    std::unique_ptr<TextButton> finishedButton;
    std::unique_ptr<Label> countLabel;
    std::unique_ptr<Label> listenLabel;
    std::unique_ptr<Label> searchLabel;
    std::unique_ptr<Slider> remainingTimeSlider;
    std::unique_ptr<Label> listenCountdownLabel;
    std::unique_ptr<Label> searchCountdownLabel;
    std::unique_ptr<Slider> progressBarSlider;
    std::unique_ptr<Label> scoreLabel;
    std::unique_ptr<Label> infoLabel;
    std::unique_ptr<Label> infoLabel2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCRecorderComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

