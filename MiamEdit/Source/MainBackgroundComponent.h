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
#include "JuceHeader.h"

// #include "View.h" // génère un pb d'includes croisés

#include "FileMenu.h"
#include "ClearLabelTimer.h"


namespace Miam {

    // forward declarations
    class View;
    class Presenter;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

// \brief The background component with a few UI controls (always displayed).
//
// An auto-generated component, created by the Projucer.
//

                                                                    //[/Comments]
*/
class MainBackgroundComponent  : public Component,
                                 public Button::Listener
{
public:
    //==============================================================================
    MainBackgroundComponent ();
    ~MainBackgroundComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void CompleteInitialization(Presenter* _presenter);


    // SETTERS and GETTERS
    void setMiamView(Miam::View* _miamView) {miamView = _miamView;}

    // Orders from Presenter
    void DisplayInfo(const String& message);

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    View *miamView;
    std::unique_ptr<FileMenu> fileMenu;

    Miam::ClearLabelTimer clearLabelTimer;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> speakersTextButton;
    ScopedPointer<TextButton> spatStatesTextButtn;
    ScopedPointer<Label> mainInfoLabel;
    ScopedPointer<TextButton> scenesTextButton;
    ScopedPointer<TextButton> hardwareConfTextButton;
    ScopedPointer<TextButton> startTextButton;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> fileTextButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainBackgroundComponent)
};

//[EndFile] You can add extra defines here...

} // namespace Miam
//[/EndFile]
