/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_12B80BD2AA08A168__
#define __JUCE_HEADER_12B80BD2AA08A168__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

// #include "View.h" // génère un pb d'includes croisés
namespace Miam { class View; } // sufficient declaration, as we just declare pointers

#include "ClearLabelTimer.h"
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
                                 public ButtonListener
{
public:
    //==============================================================================
    MainBackgroundComponent ();
    ~MainBackgroundComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void CompleteInitialization(/*Presenter* _presenter*/);


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

    Miam::View *miamView;

    Miam::ClearLabelTimer clearLabelTimer;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> speakersTextButton;
    ScopedPointer<TextButton> speakersGroupsTextButton;
    ScopedPointer<Label> mainInfoLabel;
    ScopedPointer<TextButton> scenesTextButton;
    ScopedPointer<TextButton> hardwareConfTextButton;
    ScopedPointer<TextButton> startTextButton;
    ScopedPointer<Label> label;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainBackgroundComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_12B80BD2AA08A168__
