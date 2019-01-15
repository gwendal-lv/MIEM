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

#include "PlayerMainMenuComponent.h"
#include "MiemFileChoosers.h"

// Forward declarations
namespace Miam
{
    class PlayerPresenter;

    class MultiCanvasComponent;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PlayerBackgroundComponent  : public Component,
                                   public Button::Listener
{
public:
    //==============================================================================
    PlayerBackgroundComponent ();
    ~PlayerBackgroundComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    /// \brief Function called after both View and Presenter are contructed
    void CompleteInitialization(PlayerPresenter*);
    void CompleteInitialization(MultiCanvasComponent*);

    void DisplayInfo(const String& stringToDisplay, bool isImportant = false);
    void DisplayInfo2(const String& stringToDisplay);


    void ChangeAppMode(PlayerAppMode newAppMode);

    MultiCanvasComponent* GetMultiCanvasComponent() { return multiCanvasComponent; }

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
    PlayerPresenter* presenter;

    /// Simple GUIs, owned by the View directly
    ScopedPointer<PlayerMainMenuComponent> mainMenuComponent;

    /// \brief Interactive, Owned by the Presenter
    MultiCanvasComponent* multiCanvasComponent;

	/// \brief Sauvegarde la valeur correspondante choisie dans le Projucer
	Colour mainInfoLabelDefaultTextColour;
	Font mainInfoLabelDefaultFont;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> mainInfoLabel;
    std::unique_ptr<ImageButton> mainMenuImageButton;
    std::unique_ptr<Label> mainInfoLabel2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerBackgroundComponent)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]
