/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "PlayerAppMode.h"
#include "PlayerPresenter.h"

namespace Miam {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PlayerMainMenuComponent  : public Component,
                                 public Button::Listener
{
public:
    //==============================================================================
    PlayerMainMenuComponent ();
    ~PlayerMainMenuComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void SetPresenter(PlayerPresenter* presenter_) {presenter = presenter_;}
    void ChangeAppMode(PlayerAppMode newAppMode);

    /// \brief Pour l'instant, fait simplement clignoter le bouton play
    void PrepareToPlay(int delayBeforeActualPlay_ms);
    bool IsPreparingToPlay() { return isPreparingToPlay; }
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* lecture_png;
    static const int lecture_pngSize;
    static const char* play_png;
    static const int play_pngSize;
    static const char* playOn_png;
    static const int playOn_pngSize;
    static const char* playOn_png2;
    static const int playOn_png2Size;
    static const char* stop_png;
    static const int stop_pngSize;
    static const char* stopOn_png;
    static const int stopOn_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    PlayerPresenter* presenter;

    bool isPreparingToPlay = false;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> sessionGroupComponent;
    std::unique_ptr<TextButton> loadFromFileButton;
    std::unique_ptr<ImageButton> playImageButton;
    std::unique_ptr<ImageButton> playingImageButton;
    std::unique_ptr<ImageButton> stopImageButton;
    std::unique_ptr<ImageButton> stoppedImageButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerMainMenuComponent)
};

//[EndFile] You can add extra defines here...
} // fin du namespace Miam
//[/EndFile]
