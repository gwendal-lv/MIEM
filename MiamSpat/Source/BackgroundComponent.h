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

// Forward declarations
namespace Miam
{
    class Presenter;
    class TouchMainMenu;
}


//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class BackgroundComponent  : public Component,
                             public Button::Listener
{
public:
    //==============================================================================
    BackgroundComponent ();
    ~BackgroundComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    /// \brief Function called after both View and Presenter are contructed
    void CompleteInitialization(Presenter*);
    void CompleteInitialization(MultiCanvasComponent*);

    void DisplayInfo(const String& stringToDisplay);

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* menu_icon_png;
    static const int menu_icon_pngSize;
    static const char* menu_activated_icon_png;
    static const int menu_activated_icon_pngSize;
    static const char* menu_icon_png2;
    static const int menu_icon_png2Size;
    static const char* menu_activated_icon_png2;
    static const int menu_activated_icon_png2Size;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    /// \brief To send commands such as "load", "quit", ...
    Presenter* presenter;

    /// \brief Owned by the Presenter
    MultiCanvasComponent* multiCanvasComponent;

    ScopedPointer<TouchMainMenu> touchMainMenu;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> mainInfoLabel;
    ScopedPointer<ImageButton> imageButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BackgroundComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
