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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "Presenter.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainMenuComponent  : public Component,
                           public Button::Listener
{
public:
    //==============================================================================
    MainMenuComponent ();
    ~MainMenuComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void SetPresenter(Presenter* presenter_) {presenter = presenter_;}
    void ChangeAppMode(AppMode newAppMode);
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
    Presenter* presenter;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> sessionGroupComponent;
    ScopedPointer<TextButton> loadFromFileButton;
    ScopedPointer<ImageButton> playImageButton;
    ScopedPointer<ImageButton> playingImageButton;
    ScopedPointer<ImageButton> stopImageButton;
    ScopedPointer<ImageButton> stoppedImageButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainMenuComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
