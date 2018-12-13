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

namespace Miam {

    class Presenter;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class StartupComponent  : public Component,
                          public Button::Listener
{
public:
    //==============================================================================
    StartupComponent ();
    ~StartupComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void CompleteInitialization(Presenter* presenter_) { presenter = presenter_; }
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Presenter* presenter;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> loadTextButton;
    std::unique_ptr<TextButton> createSpatTextButton;
    std::unique_ptr<TextButton> createDefaultTextButton;
    std::unique_ptr<TextButton> createGenericTextButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StartupComponent)
};

//[EndFile] You can add extra defines here...
} // fin de "namespace Miam"
//[/EndFile]
