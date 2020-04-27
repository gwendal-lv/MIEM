/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "PlayerAppMode.h"
#include "PlayerPresenter.h"

#include "MiamLookAndFeel.h"

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
                                 public TextEditor::Listener,
                                 public Button::Listener
{
public:
    //==============================================================================
    PlayerMainMenuComponent ();
    ~PlayerMainMenuComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void SetPresenter(PlayerPresenter* presenter_) {presenter = presenter_;}
    void ChangeAppMode(PlayerAppMode newAppMode);

    /// \brief Pour l'instant, fait simplement clignoter le bouton play
    void SetIsPlayEnabled(bool shouldBeEnabled);
    void PrepareToPlay(int delayBeforeActualPlay_ms);
    bool IsPreparingToPlay() { return isPreparingToPlay; }

    void SetIsHelpDisplayed(bool _displayHelp);
    bool GetIsHelpDisplayed() const {return displayHelp;}
    void SetHelpString(const String& helpString);

    void SetInfoLabelText(const String& text);
    void SetOscConfigurationFromTree(bptree::ptree& oscTree);

    virtual void textEditorTextChanged(TextEditor& editorThatHasChanged) override;
    virtual void textEditorFocusLost(TextEditor& editorThatHasChanged) override;

    /// \brief Uses a TextUtils:: static method
    std::string TryParseIpAddress();
    /// \brief Uses a TextUtils:: static method
    int TryParseUdpPort();


    // internal helpers
    private :
    void translateSessionGroup(int dX, int dY);
    void translateHelpGroup(int dX, int dY);
    void setIsOscConfigurationDisplayed(bool shouldBeDisplayed);
    void translateOscConfigurationGroup(int dX, int dY);
    void translateComponent(int dX, int dY, Component* component);
    void updateOscConfigurationComponents(bool resetConnectButtonText = true);
    public :

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* play_png2;
    static const int play_png2Size;
    static const char* playing_png;
    static const int playing_pngSize;
    static const char* stopped_png;
    static const int stopped_pngSize;
    static const char* stop_png2;
    static const int stop_png2Size;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    PlayerPresenter* presenter;

    bool isPreparingToPlay = false;

    bool displayHelp = true;

    // if true, the user is editing ip/udp but did not click on the "reconnect" button
    bool isOscConfigurationBeingEdited = false;
    // if true, a text editor is being changed -> on android, we must display
    // the text boxes a bit higher (the keyboard takes 50+ % of the screen..)
    bool areOscTextEditorsBeingEdited = false;
    String changeConnectionButtonProjucerText;

    std::unique_ptr<MiamLookAndFeel> transparentLookAndFeel;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> sessionGroupComponent;
    std::unique_ptr<TextButton> loadFromFileButton;
    std::unique_ptr<ImageButton> playImageButton;
    std::unique_ptr<ImageButton> playingImageButton;
    std::unique_ptr<ImageButton> stopImageButton;
    std::unique_ptr<ImageButton> stoppedImageButton;
    std::unique_ptr<GroupComponent> helpGroupComponent;
    std::unique_ptr<TextButton> helpButton;
    std::unique_ptr<TextEditor> infoTextEditor;
    std::unique_ptr<HyperlinkButton> miemProjectHyperlinkButton;
    std::unique_ptr<TextButton> loadDefaultButton;
    std::unique_ptr<Label> additionnalStatusLabel;
    std::unique_ptr<TextButton> fullscreenButton;
    std::unique_ptr<Label> udpPortLabel;
    std::unique_ptr<TextEditor> udpPortTextEditor;
    std::unique_ptr<Label> ipAddressLabel;
    std::unique_ptr<TextEditor> ipAddressTextEditor;
    std::unique_ptr<TextButton> changeConnectionButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerMainMenuComponent)
};

//[EndFile] You can add extra defines here...
} // fin du namespace Miam
//[/EndFile]

