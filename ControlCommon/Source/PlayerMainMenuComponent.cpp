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

// direct, half-disgusting access to the static appPurpose info...
#include "AppPurpose.h"

#include "TextUtils.h"

#include "boost/core/ignore_unused.hpp"

//[/Headers]

#include "PlayerMainMenuComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
PlayerMainMenuComponent::PlayerMainMenuComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    transparentLookAndFeel.reset( new MiamLookAndFeel(true) );
    //[/Constructor_pre]

    sessionGroupComponent.reset (new GroupComponent ("Session group component",
                                                     TRANS("Session")));
    addAndMakeVisible (sessionGroupComponent.get());
    sessionGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff909090));
    sessionGroupComponent->setColour (GroupComponent::textColourId, Colour (0xff909090));

    loadFromFileButton.reset (new TextButton ("Load From File text button"));
    addAndMakeVisible (loadFromFileButton.get());
    loadFromFileButton->setButtonText (TRANS("Load from .mspat file"));
    loadFromFileButton->addListener (this);
    loadFromFileButton->setColour (TextButton::buttonColourId, Colour (0xff404040));

    playImageButton.reset (new ImageButton ("Play image button"));
    addAndMakeVisible (playImageButton.get());
    playImageButton->setButtonText (TRANS("new button"));
    playImageButton->addListener (this);

    playImageButton->setImages (false, true, true,
                                ImageCache::getFromMemory (play_png2, play_png2Size), 1.000f, Colour (0x00000000),
                                Image(), 0.600f, Colour (0x00000000),
                                Image(), 1.000f, Colour (0x00000000));
    playingImageButton.reset (new ImageButton ("Playing image button"));
    addAndMakeVisible (playingImageButton.get());
    playingImageButton->setButtonText (TRANS("new button"));
    playingImageButton->addListener (this);

    playingImageButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (playing_png, playing_pngSize), 1.000f, Colour (0x00000000),
                                   Image(), 1.000f, Colour (0x00000000),
                                   Image(), 1.000f, Colour (0x00000000));
    stopImageButton.reset (new ImageButton ("Stop image button"));
    addAndMakeVisible (stopImageButton.get());
    stopImageButton->setButtonText (TRANS("new button"));
    stopImageButton->addListener (this);

    stopImageButton->setImages (false, true, true,
                                ImageCache::getFromMemory (stop_png2, stop_png2Size), 1.000f, Colour (0x00000000),
                                Image(), 0.600f, Colour (0x00000000),
                                Image(), 1.000f, Colour (0x00000000));
    stoppedImageButton.reset (new ImageButton ("Stopped image button"));
    addAndMakeVisible (stoppedImageButton.get());
    stoppedImageButton->setButtonText (TRANS("new button"));
    stoppedImageButton->addListener (this);

    stoppedImageButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (stopped_png, stopped_pngSize), 1.000f, Colour (0x00000000),
                                   Image(), 1.000f, Colour (0x00000000),
                                   Image(), 1.000f, Colour (0x00000000));
    helpGroupComponent.reset (new GroupComponent ("Help group component",
                                                  TRANS("Help")));
    addAndMakeVisible (helpGroupComponent.get());
    helpGroupComponent->setColour (GroupComponent::outlineColourId, Colour (0xff909090));
    helpGroupComponent->setColour (GroupComponent::textColourId, Colour (0xff909090));

    helpButton.reset (new TextButton ("Help button"));
    addAndMakeVisible (helpButton.get());
    helpButton->setButtonText (TRANS("Show help"));
    helpButton->addListener (this);
    helpButton->setColour (TextButton::buttonColourId, Colour (0xff404040));

    infoTextEditor.reset (new TextEditor ("Info text editor"));
    addAndMakeVisible (infoTextEditor.get());
    infoTextEditor->setMultiLine (true);
    infoTextEditor->setReturnKeyStartsNewLine (true);
    infoTextEditor->setReadOnly (true);
    infoTextEditor->setScrollbarsShown (true);
    infoTextEditor->setCaretVisible (false);
    infoTextEditor->setPopupMenuEnabled (false);
    infoTextEditor->setColour (TextEditor::textColourId, Colour (0xff909090));
    infoTextEditor->setColour (TextEditor::backgroundColourId, Colour (0x00ffffff));
    infoTextEditor->setText (TRANS("Multi\n"
    "L\n"
    "I\n"
    "N\n"
    "E\n"
    "and scrollable information textbox for help contents"));

    miemProjectHyperlinkButton.reset (new HyperlinkButton (TRANS("Go to MIEM website: miem.laras.be"),
                                                           URL ("http://miem.laras.be")));
    addAndMakeVisible (miemProjectHyperlinkButton.get());
    miemProjectHyperlinkButton->setTooltip (TRANS("http://miem.laras.be"));
    miemProjectHyperlinkButton->setButtonText (TRANS("Go to MIEM website: miem.laras.be"));
    miemProjectHyperlinkButton->setColour (HyperlinkButton::textColourId, Colour (0xff3d6ed1));

    loadDefaultButton.reset (new TextButton ("Load Default button"));
    addAndMakeVisible (loadDefaultButton.get());
    loadDefaultButton->setButtonText (TRANS("Load default session"));
    loadDefaultButton->addListener (this);
    loadDefaultButton->setColour (TextButton::buttonColourId, Colour (0xff404040));

    additionnalStatusLabel.reset (new Label ("Additionnal Status Label",
                                             TRANS("Additionnal Status information")));
    addAndMakeVisible (additionnalStatusLabel.get());
    additionnalStatusLabel->setFont (Font (15.00f, Font::italic));
    additionnalStatusLabel->setJustificationType (Justification::centred);
    additionnalStatusLabel->setEditable (false, false, false);
    additionnalStatusLabel->setColour (Label::textColourId, Colour (0xff909090));
    additionnalStatusLabel->setColour (TextEditor::textColourId, Colours::black);
    additionnalStatusLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    fullscreenButton.reset (new TextButton ("Fullscreen button"));
    addAndMakeVisible (fullscreenButton.get());
    fullscreenButton->setButtonText (TRANS("Fullscreen"));
    fullscreenButton->addListener (this);
    fullscreenButton->setColour (TextButton::buttonColourId, Colour (0xff404040));

    udpPortLabel.reset (new Label ("UPD Port Label",
                                   TRANS("OSC device UDP port:")));
    addAndMakeVisible (udpPortLabel.get());
    udpPortLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredRight);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (Label::textColourId, Colours::white);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    udpPortTextEditor.reset (new TextEditor ("UDP Port Text Editor"));
    addAndMakeVisible (udpPortTextEditor.get());
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setColour (TextEditor::backgroundColourId, Colour (0xff404040));
    udpPortTextEditor->setText (TRANS("8001"));

    ipAddressLabel.reset (new Label ("Ip Address Label",
                                     TRANS("OSC device IP adress:")));
    addAndMakeVisible (ipAddressLabel.get());
    ipAddressLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    ipAddressLabel->setJustificationType (Justification::centredRight);
    ipAddressLabel->setEditable (false, false, false);
    ipAddressLabel->setColour (Label::textColourId, Colours::white);
    ipAddressLabel->setColour (TextEditor::textColourId, Colours::black);
    ipAddressLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ipAddressTextEditor.reset (new TextEditor ("IP Address Text Editor"));
    addAndMakeVisible (ipAddressTextEditor.get());
    ipAddressTextEditor->setMultiLine (false);
    ipAddressTextEditor->setReturnKeyStartsNewLine (false);
    ipAddressTextEditor->setReadOnly (false);
    ipAddressTextEditor->setScrollbarsShown (true);
    ipAddressTextEditor->setCaretVisible (true);
    ipAddressTextEditor->setPopupMenuEnabled (true);
    ipAddressTextEditor->setColour (TextEditor::backgroundColourId, Colour (0xff404040));
    ipAddressTextEditor->setText (TRANS("127.0.0.1"));

    changeConnectionButton.reset (new TextButton ("Change Connection button"));
    addAndMakeVisible (changeConnectionButton.get());
    changeConnectionButton->setButtonText (TRANS("Unlock IP/port"));
    changeConnectionButton->addListener (this);
    changeConnectionButton->setColour (TextButton::buttonColourId, Colour (0xff404040));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    loadFromFileButton->setButtonText (TRANS("Load from ") +
                                       PathUtils::GenerateAllowedFilePatterns({App::GetPurpose()},
                                                                              false,
                                                                              " " + (TRANS("or")).toStdString() + " ") +
                                       + " " + TRANS("file"));
    changeConnectionButtonProjucerText = changeConnectionButton->getButtonText();


    // Special transparent look and feel for the text editor
    infoTextEditor->setLookAndFeel(transparentLookAndFeel.get());
    // and thick scroll bar
    infoTextEditor->setScrollbarsShown(true); // automatically hidden if not necessary
#ifdef __MIAMOBILE
    infoTextEditor->setScrollBarThickness(18);
#else
    infoTextEditor->setScrollBarThickness(12);
#endif

    // Help URLs update (pages of the miem.laras.be website)
    if (App::GetPurpose() == AppPurpose::GenericController)
    {
        miemProjectHyperlinkButton->setURL(URL("http://miem.laras.be/controller"));
    }
    else if (App::GetPurpose() == AppPurpose::Spatialisation)
    {
        miemProjectHyperlinkButton->setURL(URL("http://miem.laras.be/spat"));
    }

    // Fullscreen button
#ifdef JUCE_WINDOWS
    const bool activateFullscreenButton = true;
#else
    const bool activateFullscreenButton = false;
#endif
    fullscreenButton->setEnabled(activateFullscreenButton);
    fullscreenButton->setVisible(activateFullscreenButton);

    // Manual registration for listening to events
    ipAddressTextEditor->addListener(this);
    udpPortTextEditor->addListener(this);

    // last updates
    updateOscConfigurationComponents();


#ifdef __MIEM_EXPERIMENTS
    additionnalStatusLabel->setText("Experiments' build (controlled by TCP connection).",
                                    NotificationType::dontSendNotification);
#else
    additionnalStatusLabel->setVisible(false);
#endif

    //[/Constructor]
}

PlayerMainMenuComponent::~PlayerMainMenuComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    sessionGroupComponent = nullptr;
    loadFromFileButton = nullptr;
    playImageButton = nullptr;
    playingImageButton = nullptr;
    stopImageButton = nullptr;
    stoppedImageButton = nullptr;
    helpGroupComponent = nullptr;
    helpButton = nullptr;
    infoTextEditor = nullptr;
    miemProjectHyperlinkButton = nullptr;
    loadDefaultButton = nullptr;
    additionnalStatusLabel = nullptr;
    fullscreenButton = nullptr;
    udpPortLabel = nullptr;
    udpPortTextEditor = nullptr;
    ipAddressLabel = nullptr;
    ipAddressTextEditor = nullptr;
    changeConnectionButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    transparentLookAndFeel = nullptr;
    //[/Destructor]
}

//==============================================================================
void PlayerMainMenuComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	boost::ignore_unused(g);
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PlayerMainMenuComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    sessionGroupComponent->setBounds ((getWidth() / 2) - ((getWidth() - 16) / 2), 8, getWidth() - 16, 56);
    loadFromFileButton->setBounds ((getWidth() / 2) - (200 / 2), 8 + 18, 200, 24);
    playImageButton->setBounds ((getWidth() / 2) + -16 - 62, getHeight() - 96, 62, 62);
    playingImageButton->setBounds ((getWidth() / 2) + -16 - 62, getHeight() - 96, 62, 62);
    stopImageButton->setBounds ((getWidth() / 2) + 16, getHeight() - 96, 62, 62);
    stoppedImageButton->setBounds ((getWidth() / 2) + 16, getHeight() - 96, 62, 62);
    helpGroupComponent->setBounds ((getWidth() / 2) - ((getWidth() - 16) / 2), 72, getWidth() - 16, getHeight() - 179);
    helpButton->setBounds (((getWidth() / 2) - ((getWidth() - 16) / 2)) + (getWidth() - 16) / 2 - (120 / 2), 72 + 18, 120, 24);
    infoTextEditor->setBounds (24, 128, (getWidth() - 16) - 32, (getHeight() - 179) - 96);
    miemProjectHyperlinkButton->setBounds ((getWidth() / 2) - ((getWidth() - 336) / 2), 72 + (getHeight() - 179) - 36, getWidth() - 336, 24);
    loadDefaultButton->setBounds (((getWidth() / 2) - ((getWidth() - 16) / 2)) + (getWidth() - 16) - 8 - 144, 72 + 18, 144, 24);
    additionnalStatusLabel->setBounds ((getWidth() / 2) - ((getWidth() - 80) / 2), getHeight() - 24, getWidth() - 80, 24);
    fullscreenButton->setBounds (getWidth() - 24 - 144, getHeight() - 76, 144, 24);
    udpPortLabel->setBounds ((getWidth() / 2) + -1 - 184, ((getHeight() / 2) + 28) + 0, 184, 24);
    udpPortTextEditor->setBounds ((getWidth() / 2), (getHeight() / 2) + 28, 64, 24);
    ipAddressLabel->setBounds ((getWidth() / 2) + -1 - 184, ((getHeight() / 2) + -4) + 0, 184, 24);
    ipAddressTextEditor->setBounds ((getWidth() / 2), (getHeight() / 2) + -4, 120, 24);
    changeConnectionButton->setBounds ((getWidth() / 2) - (200 / 2), (getHeight() / 2) + -40, 200, 24);
    //[UserResized] Add your own custom resize handling here..

    // Buttons (and the group itself) will be hidden if there is not enough height available
    helpGroupComponent->setVisible(true); // default, will be false only if very, very small screen

    // If there would be space for the "load session" category only.... we manage.
    // We are going to force everything on screen, because both Android and iOS run in
    // landscape mode only. -> same behavior on all platforms
    // -> only 2 modes : small height, or normal
    bool enableUltraSmallHeightMode = (helpGroupComponent->getHeight() < (helpButton->getHeight()+112));
    bool enableSmallHeightMode = getHeight() < 400;

    // much generic code now...
    helpButton->setVisible(true);
    loadDefaultButton->setVisible(displayHelp);
    miemProjectHyperlinkButton->setVisible(displayHelp);
    infoTextEditor->setVisible((! enableUltraSmallHeightMode) && displayHelp);

    // In case of very small width: small arrangements for iPhones...
    // // 400px est environ la limite entre iPhone et iPhone +
    // MAIS pour l'iphone Xr -> boutons se recouvrent... (écran large, mais pas assez...)
    // AUSSI pour small height -> bouton reste à gauche dans tous les cas
    if ((getWidth() < 440) || enableSmallHeightMode)
    {
        Rectangle<int> bounds = helpButton->getBounds();
        bounds.setX(16); // décalage forcé à gauche
        helpButton->setBounds(bounds);
    }

    // If help is not requested to be displayed:
    // 1 - Size of help group forced to a min value,
    // 2 - ip/port buttons and text are made visible, is there is enough vertical space
    // (no available on landscape smartphones, for example....)
    if (! displayHelp)
    {
        // help reduced at max
        Rectangle<int> newBounds = helpGroupComponent->getBounds();
        newBounds.setHeight(56);
        helpGroupComponent->setBounds(newBounds);

        // ip/port buttons are always activated,
        // but they might be moved to the right
        setIsOscConfigurationDisplayed(true);
        if (enableSmallHeightMode || enableUltraSmallHeightMode)
        {
            translateOscConfigurationGroup((getWidth()/2) - (ipAddressTextEditor->getWidth() + 24),
                                           -20);
            // and we make room for this IP/UDP port info :
            helpGroupComponent->setBounds(helpGroupComponent->getX(),
                                          helpGroupComponent->getY(),
                                          helpButton->getWidth() + 16,
                                          helpGroupComponent->getHeight());
        }
    }
    else
    {
        setIsOscConfigurationDisplayed(false);
        if (enableUltraSmallHeightMode)
        {
            // help reduced at max
            Rectangle<int> newBounds = helpGroupComponent->getBounds();
            newBounds.setHeight(56);
            helpGroupComponent->setBounds(newBounds);
            // link properly replaced
            miemProjectHyperlinkButton->setTopLeftPosition(miemProjectHyperlinkButton->getX(),
                                                           helpButton->getY());
        }
    }

#if defined(JUCE_ANDROID)
// Android met un clavier qui prend 50% de l'écran...
// 2 solutions : soit relever les éléments (mais l'utilisateur
// s'attend plutôt à pouvoir "dragger" les éléments, plutôt qu'un
// déplacement automatique
    // SOIT juste toujours avoir les boutons en haut... collés au groupe "session"
    if (!displayHelp) {
        if (enableSmallHeightMode)
            translateOscConfigurationGroup(0,
                                           -changeConnectionButton->getY() +
                                           sessionGroupComponent->getBottom() + 16);
    // ou collés ou groupe "help" si l'écran est assez haut
        else
            translateOscConfigurationGroup(0,
                                           -changeConnectionButton->getY() +
                                           helpGroupComponent->getBottom() + 16);
    }
    /*
    if (areOscTextEditorsBeingEdited)
    {
        translateSessionGroup(0, -getHeight()/3);
        translateHelpGroup(0, -getHeight()/3);
        translateOscConfigurationGroup(0, -getHeight()/3);
    }
    */
#endif

    //[/UserResized]
}

void PlayerMainMenuComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadFromFileButton.get())
    {
        //[UserButtonCode_loadFromFileButton] -- add your button handler code here..
        presenter->appModeChangeRequest(PlayerAppMode::LoadingFile);
        //[/UserButtonCode_loadFromFileButton]
    }
    else if (buttonThatWasClicked == playImageButton.get())
    {
        //[UserButtonCode_playImageButton] -- add your button handler code here..
        presenter->appModeChangeRequest(PlayerAppMode::Playing);
        //[/UserButtonCode_playImageButton]
    }
    else if (buttonThatWasClicked == playingImageButton.get())
    {
        //[UserButtonCode_playingImageButton] -- add your button handler code here..
        //[/UserButtonCode_playingImageButton]
    }
    else if (buttonThatWasClicked == stopImageButton.get())
    {
        //[UserButtonCode_stopImageButton] -- add your button handler code here..
        presenter->appModeChangeRequest(PlayerAppMode::Stopped);
        //[/UserButtonCode_stopImageButton]
    }
    else if (buttonThatWasClicked == stoppedImageButton.get())
    {
        //[UserButtonCode_stoppedImageButton] -- add your button handler code here..
        //[/UserButtonCode_stoppedImageButton]
    }
    else if (buttonThatWasClicked == helpButton.get())
    {
        //[UserButtonCode_helpButton] -- add your button handler code here..
        presenter->OnHelpButtonClicked(displayHelp);
        //[/UserButtonCode_helpButton]
    }
    else if (buttonThatWasClicked == loadDefaultButton.get())
    {
        //[UserButtonCode_loadDefaultButton] -- add your button handler code here..
        presenter->OnLoadDefaultSession();
        //[/UserButtonCode_loadDefaultButton]
    }
    else if (buttonThatWasClicked == fullscreenButton.get())
    {
        //[UserButtonCode_fullscreenButton] -- add your button handler code here..
        if (presenter->OnFullscreenButtonClicked()) // si fullscreen accepté
            fullscreenButton->setButtonText(TRANS("Reduce window"));
        else
            fullscreenButton->setButtonText(TRANS("Fullscreen"));
        //[/UserButtonCode_fullscreenButton]
    }
    else if (buttonThatWasClicked == changeConnectionButton.get())
    {
        //[UserButtonCode_changeConnectionButton] -- add your button handler code here..

        bool resetButtonText = true;
        // If not in edition mode yet, we ask for it to the parent, then allow edition there
        if (! isOscConfigurationBeingEdited)
            isOscConfigurationBeingEdited = presenter->OnOscConfigurationEditionRequest();
        // else if already editing, we transmit data to the parent manager
        else
        {
            // PROCEDURE SI VALEURS PAS OK ????
            isOscConfigurationBeingEdited = !
            presenter->OnOscConfigurationEditionFinished(TryParseIpAddress(),
                                                         TryParseUdpPort());
            // si probleme : on ne change pas le texte, et on en affiche un autre directement sur le
            // bouton (erreur connection...)
            if (isOscConfigurationBeingEdited)
            {
                resetButtonText = false;
                changeConnectionButton->setButtonText(TRANS("Cannot connect. Check and retry"));
            }
        }

        // Buttons enablement depends on manager's response
        updateOscConfigurationComponents(resetButtonText);

        //[/UserButtonCode_changeConnectionButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PlayerMainMenuComponent::ChangeAppMode(PlayerAppMode newAppMode)
{
    switch(newAppMode)
    {
        case PlayerAppMode::Playing : // le play est déjà lancé, on ne peut lancer que le stop
            SetIsPlayEnabled(true);
            playingImageButton->setVisible(true);
            playImageButton->setVisible(false);
            stoppedImageButton->setVisible(false);
            stopImageButton->setVisible(true);
            break;

        case PlayerAppMode::Stopped_NoPlay :
            SetIsPlayEnabled(false);
            playingImageButton->setVisible(false);
            playImageButton->setVisible(true);
            stoppedImageButton->setVisible(true);
            stopImageButton->setVisible(false);
            break;

        case PlayerAppMode::Stopped :
            SetIsPlayEnabled(true);
            playingImageButton->setVisible(false);
            playImageButton->setVisible(true);
            stoppedImageButton->setVisible(true);
            stopImageButton->setVisible(false);
            break;

        default :
            break;
    }
}

void PlayerMainMenuComponent::SetIsPlayEnabled(bool shouldBeEnabled)
{
    playingImageButton->setEnabled(shouldBeEnabled);
    playImageButton->setEnabled(shouldBeEnabled);
}

void PlayerMainMenuComponent::PrepareToPlay(int delayBeforeActualPlay_ms)
{
    presenter->OnViewIsPreparingToPlay(true);

    const int numberOfBlinks = 2;
    // on garde 1 demi-blink pour la fin
    const int halfBlinkDelays_ms = (int) std::round( (double)(delayBeforeActualPlay_ms) / (double)(numberOfBlinks*2 + 1) );
    // état initial
    playingImageButton->setVisible(true);
    playImageButton->setVisible(false);
    // Premiers clignotements, visible puis invisible
    for (int i=0 ; i < numberOfBlinks ; i++)
    {
        Timer::callAfterDelay( (2*i + 1)*halfBlinkDelays_ms,
                              [this] {
                                  playImageButton->setVisible(true);
                                  playingImageButton->setVisible(false);
                              });
        Timer::callAfterDelay( (2*i + 2)*halfBlinkDelays_ms,
                              [this] {
                                  playingImageButton->setVisible(true);
                                  playImageButton->setVisible(false);
                              });
    }
    // Dernier demi blink : on cache le play noir et on laisse seulement le vert
    Timer::callAfterDelay( (2*numberOfBlinks + 1)*halfBlinkDelays_ms, [this] {
        playImageButton->setVisible(false);
        playingImageButton->setVisible(true);
    });
}
void PlayerMainMenuComponent::SetIsHelpDisplayed(bool _displayHelp)
{
    displayHelp =  _displayHelp;

    // Elements hidden if not supposed to be displayed
    infoTextEditor->setVisible(displayHelp);
    miemProjectHyperlinkButton->setVisible(displayHelp);

    if (displayHelp)
        helpButton->setButtonText(TRANS("Hide help"));
    else
        helpButton->setButtonText(TRANS("Show help"));

    // For applying changes on the help group component
    resized();
}
void PlayerMainMenuComponent::SetHelpString(const String& helpString)
{
    infoTextEditor->setText(helpString);
}
void PlayerMainMenuComponent::SetInfoLabelText(const String& text)
{
    if (text.isEmpty())
    {
        additionnalStatusLabel->setVisible(false);
    }
    else
    {
        additionnalStatusLabel->setText(text, NotificationType::dontSendNotification);
#ifdef __MIEM_EXPERIMENTS
        additionnalStatusLabel->setVisible(true);
#endif
    }
}
void PlayerMainMenuComponent::SetOscConfigurationFromTree(bptree::ptree& oscTree)
{
    // we set all info, or nothing
    int udpPort = -1; // not a local anymore ; saved for future re-use
    std::string ipv4 = "";
    try {
        udpPort = oscTree.get<int>("udp.port");
        ipv4 = oscTree.get<std::string>("ip");
    }
    catch (bptree::ptree_error&){
        return;
    }
    ipAddressTextEditor->setText(ipv4, NotificationType::dontSendNotification);
    udpPortTextEditor->setText(boost::lexical_cast<std::string>(udpPort), NotificationType::dontSendNotification);
    // graphical updates
    TryParseIpAddress();
    TryParseUdpPort();
}
void PlayerMainMenuComponent::textEditorTextChanged(TextEditor& editorThatHasChanged)
{
    bool previousEditionState = areOscTextEditorsBeingEdited;
    if (&editorThatHasChanged == ipAddressTextEditor.get()) {
        TryParseIpAddress();
        areOscTextEditorsBeingEdited = true;
    }
    else if (&editorThatHasChanged == udpPortTextEditor.get())
    {
        TryParseUdpPort();
        areOscTextEditorsBeingEdited = true;
    }
    if (previousEditionState != areOscTextEditorsBeingEdited)
        resized(); // forced update, if necessary
}
void PlayerMainMenuComponent::textEditorFocusLost(TextEditor& editorThatHasChanged)
{
    if ( (&editorThatHasChanged == ipAddressTextEditor.get())
        || (&editorThatHasChanged == udpPortTextEditor.get()) )
    {
        areOscTextEditorsBeingEdited = false;
    }
    resized(); // forced update
}
std::string PlayerMainMenuComponent::TryParseIpAddress()
{
    return TextUtils::TryParseAndBoldenIpAddress(ipAddressTextEditor.get());
}
int PlayerMainMenuComponent::TryParseUdpPort()
{
    return TextUtils::TryParseAndBoldenUdpPort(udpPortTextEditor.get());
}

void PlayerMainMenuComponent::translateSessionGroup(int dX, int dY)
{
    translateComponent(dX, dY, sessionGroupComponent.get());
    translateComponent(dX, dY, loadFromFileButton.get());
}
void PlayerMainMenuComponent::translateHelpGroup(int dX, int dY)
{
    translateComponent(dX, dY, helpGroupComponent.get());
    translateComponent(dX, dY, helpButton.get());
    translateComponent(dX, dY, infoTextEditor.get());
    translateComponent(dX, dY, miemProjectHyperlinkButton.get());
    translateComponent(dX, dY, loadDefaultButton.get());
}

void PlayerMainMenuComponent::setIsOscConfigurationDisplayed(bool shouldBeDisplayed)
{
    changeConnectionButton->setVisible(shouldBeDisplayed);
    ipAddressLabel->setVisible(shouldBeDisplayed);
    ipAddressTextEditor->setVisible(shouldBeDisplayed);
    udpPortLabel->setVisible(shouldBeDisplayed);
    udpPortTextEditor->setVisible(shouldBeDisplayed);
}
void PlayerMainMenuComponent::translateOscConfigurationGroup(int dX, int dY)
{
    translateComponent(dX, dY, changeConnectionButton.get());
    translateComponent(dX, dY, ipAddressLabel.get());
    translateComponent(dX, dY, ipAddressTextEditor.get());
    translateComponent(dX, dY, udpPortLabel.get());
    translateComponent(dX, dY, udpPortTextEditor.get());
}
void PlayerMainMenuComponent::translateComponent(int dX, int dY, Component* component)
{
    component->setTopLeftPosition(component->getX() + dX,
                                  component->getY() + dY);
}
void PlayerMainMenuComponent::updateOscConfigurationComponents(bool resetConnectButtonText)
{
    ipAddressLabel->setEnabled(isOscConfigurationBeingEdited);
    ipAddressTextEditor->setEnabled(isOscConfigurationBeingEdited);
    udpPortLabel->setEnabled(isOscConfigurationBeingEdited);
    udpPortTextEditor->setEnabled(isOscConfigurationBeingEdited);
    if (resetConnectButtonText)
    {
        if (! isOscConfigurationBeingEdited)
            changeConnectionButton->setButtonText(changeConnectionButtonProjucerText);
        else
            changeConnectionButton->setButtonText(TRANS("Connect to new OSC device"));
    }
    // Update des fonts -> désactivé
    if (isOscConfigurationBeingEdited)
    {
        /*ipAddressTextEditor->setFont(Font());
        udpPortTextEditor->setFont(Font());*/
        ipAddressTextEditor->setColour(TextEditor::textColourId,
                                       juce::Colours::white);
        udpPortTextEditor->setColour(TextEditor::textColourId,
                                       juce::Colours::white);
        TryParseIpAddress();
        TryParseUdpPort();
    }
    else
    {
        const int grey = 90;
        ipAddressTextEditor->setColour(TextEditor::textColourId,
                                       juce::Colour::fromRGB(grey, grey, grey));
        udpPortTextEditor->setColour(TextEditor::textColourId,
                                     juce::Colour::fromRGB(grey, grey, grey));
        TryParseIpAddress();
        TryParseUdpPort();
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PlayerMainMenuComponent"
                 componentName="" parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
  <GROUPCOMPONENT name="Session group component" id="ee702f61e13ff830" memberName="sessionGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0.5Cc 8 16M 56" outlinecol="ff909090"
                  textcol="ff909090" title="Session"/>
  <TEXTBUTTON name="Load From File text button" id="2fe2a2c362ae91bd" memberName="loadFromFileButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 18 200 24" posRelativeY="ee702f61e13ff830"
              bgColOff="ff404040" buttonText="Load from .mspat file" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="Play image button" id="823680c6dd0a5a2e" memberName="playImageButton"
               virtualName="" explicitFocusOrder="0" pos="-16Cr 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="play_png2" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="0.6000000238418579" colourOver="0"
               resourceDown="" opacityDown="1.0" colourDown="0"/>
  <IMAGEBUTTON name="Playing image button" id="26f63b20519a1739" memberName="playingImageButton"
               virtualName="" explicitFocusOrder="0" pos="-16Cr 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="playing_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <IMAGEBUTTON name="Stop image button" id="9385dfb43a053dc6" memberName="stopImageButton"
               virtualName="" explicitFocusOrder="0" pos="16C 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="stop_png2" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="0.6000000238418579" colourOver="0"
               resourceDown="" opacityDown="1.0" colourDown="0"/>
  <IMAGEBUTTON name="Stopped image button" id="1b06a982c2d60ddc" memberName="stoppedImageButton"
               virtualName="" explicitFocusOrder="0" pos="16C 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="stopped_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <GROUPCOMPONENT name="Help group component" id="5beff948b653aff1" memberName="helpGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0.5Cc 72 16M 179M"
                  outlinecol="ff909090" textcol="ff909090" title="Help"/>
  <TEXTBUTTON name="Help button" id="87051e2f861a82a1" memberName="helpButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 18 120 24" posRelativeX="5beff948b653aff1"
              posRelativeY="5beff948b653aff1" bgColOff="ff404040" buttonText="Show help"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="Info text editor" id="a4539a25a9aebf1" memberName="infoTextEditor"
              virtualName="" explicitFocusOrder="0" pos="24 128 32M 96M" posRelativeY="97c294b92cbc0a85"
              posRelativeW="5beff948b653aff1" posRelativeH="5beff948b653aff1"
              textcol="ff909090" bkgcol="ffffff" initialText="Multi&#10;L&#10;I&#10;N&#10;E&#10;and scrollable information textbox for help contents"
              multiline="1" retKeyStartsLine="1" readonly="1" scrollbars="1"
              caret="0" popupmenu="0"/>
  <HYPERLINKBUTTON name="MIEM Project hyperlink button" id="fa7d05f849f2e1a1" memberName="miemProjectHyperlinkButton"
                   virtualName="" explicitFocusOrder="0" pos="0.5Cc 36R 336M 24"
                   posRelativeY="5beff948b653aff1" tooltip="http://miem.laras.be"
                   textCol="ff3d6ed1" buttonText="Go to MIEM website: miem.laras.be"
                   connectedEdges="0" needsCallback="0" radioGroupId="0" url="http://miem.laras.be"/>
  <TEXTBUTTON name="Load Default button" id="d025c06799a8b40f" memberName="loadDefaultButton"
              virtualName="" explicitFocusOrder="0" pos="8Rr 18 144 24" posRelativeX="5beff948b653aff1"
              posRelativeY="5beff948b653aff1" bgColOff="ff404040" buttonText="Load default session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Additionnal Status Label" id="bef02cde37454c59" memberName="additionnalStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="0.5Cc 24R 80M 24"
         textCol="ff909090" edTextCol="ff000000" edBkgCol="0" labelText="Additionnal Status information"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="1" justification="36" typefaceStyle="Italic"/>
  <TEXTBUTTON name="Fullscreen button" id="5a77ff389fbb58c9" memberName="fullscreenButton"
              virtualName="" explicitFocusOrder="0" pos="24Rr 76R 144 24" bgColOff="ff404040"
              buttonText="Fullscreen" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="UPD Port Label" id="8d369e08975b779c" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="-1Cr 0 184 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="e4ef4437203ce19e" textCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="OSC device UDP port:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="UDP Port Text Editor" id="e4ef4437203ce19e" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="0C 28C 64 24" bkgcol="ff404040"
              initialText="8001" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="Ip Address Label" id="2066f0f6ef12dcf0" memberName="ipAddressLabel"
         virtualName="" explicitFocusOrder="0" pos="-1Cr 0 184 24" posRelativeX="dfbb24a51fa3d6c0"
         posRelativeY="6997b5b4dc28675a" textCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="OSC device IP adress:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="IP Address Text Editor" id="6997b5b4dc28675a" memberName="ipAddressTextEditor"
              virtualName="" explicitFocusOrder="0" pos="0C -4C 120 24" bkgcol="ff404040"
              initialText="127.0.0.1" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Change Connection button" id="56c9310b25ab19de" memberName="changeConnectionButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc -40C 200 24" bgColOff="ff404040"
              buttonText="Unlock IP/port" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: play_png2, 5828, "../../Resources_LFS/Boutons/Exports/Play.png"
static const unsigned char resource_PlayerMainMenuComponent_play_png2[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,28,
233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,38,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,109,
108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,68,
70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,
62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,
116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,
102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,48,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,
111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,
58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,
110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,
121,68,97,116,101,62,50,48,49,57,45,48,54,45,49,50,84,48,57,58,48,54,58,51,54,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,56,46,52,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,
115,99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,107,198,50,2,0,0,18,55,73,68,65,84,120,1,237,157,7,144,21,85,22,134,
79,15,82,42,6,20,35,204,148,9,181,212,82,64,101,141,75,213,128,165,34,6,76,96,192,92,234,128,105,77,101,198,132,185,212,50,235,152,67,137,10,6,48,65,105,25,170,88,117,217,69,5,116,209,90,197,56,51,230,
65,180,196,128,204,219,254,238,204,255,184,211,60,96,112,120,51,221,239,221,83,245,186,251,158,27,222,125,231,191,255,185,221,183,187,207,139,172,13,114,215,93,119,245,143,139,13,141,114,185,1,102,21,
149,57,203,245,138,211,221,218,80,53,20,41,158,5,230,69,22,53,152,53,213,231,162,104,74,252,53,19,71,142,28,57,109,105,95,23,45,174,64,46,151,139,98,160,135,85,228,162,49,185,200,54,95,92,185,160,79,143,
5,162,156,253,175,41,202,141,142,129,31,31,69,113,170,128,20,4,188,182,182,182,119,174,41,55,54,46,191,67,129,58,65,149,118,11,228,108,106,212,37,26,81,83,83,51,59,217,213,69,0,191,243,206,59,7,197,174,
98,124,92,176,71,178,112,72,103,202,2,141,241,212,59,108,212,168,81,175,250,189,110,5,120,11,216,147,227,2,93,253,66,177,123,183,250,250,58,171,171,175,183,198,198,70,251,245,215,95,109,254,252,249,126,
145,112,220,193,22,232,218,181,171,173,188,242,202,214,163,71,15,171,170,172,178,202,202,74,139,221,120,178,23,243,99,208,7,251,160,231,75,180,184,241,127,199,53,90,49,187,62,6,121,250,140,233,246,243,
207,63,39,27,11,233,20,89,96,181,213,86,179,126,125,251,57,224,19,221,106,140,42,162,29,228,222,43,200,228,4,173,101,206,110,5,246,204,153,51,108,202,63,167,4,176,19,22,76,99,18,66,130,21,152,37,164,71,
110,65,238,81,48,70,223,133,77,207,117,123,30,98,145,253,131,99,9,21,103,125,240,129,146,97,159,17,11,124,247,253,247,214,212,180,192,214,91,111,253,133,61,142,172,106,218,180,105,179,94,120,225,133,255,
58,134,91,133,141,89,152,107,241,124,93,31,192,246,13,146,177,99,136,10,134,190,112,121,77,154,107,237,254,150,179,255,40,179,169,169,201,38,77,158,20,220,184,12,146,209,61,115,250,144,189,134,180,62,
145,139,236,111,48,124,168,255,155,26,26,234,3,216,190,65,50,122,204,156,206,149,85,66,134,86,52,47,151,46,84,215,213,181,118,5,11,115,194,81,214,44,192,101,180,47,96,93,145,179,168,202,87,54,206,105,
244,147,225,56,195,22,96,205,164,181,84,84,226,210,123,250,74,22,85,130,148,134,5,146,88,114,211,11,192,91,221,245,10,43,104,165,1,54,191,162,0,150,221,86,232,232,159,199,242,95,151,46,238,242,223,22,
44,88,192,162,79,71,119,161,172,191,175,67,1,175,168,168,136,23,5,154,236,207,63,255,204,27,93,235,191,1,248,188,73,138,122,208,161,128,3,246,170,171,174,106,59,237,180,147,113,157,56,101,202,20,251,62,
94,25,66,96,61,140,15,82,92,11,116,8,224,176,24,6,111,191,253,246,118,253,245,215,231,127,209,105,167,157,230,64,191,226,138,43,236,143,63,254,112,122,149,205,23,10,7,203,213,2,205,75,171,203,181,201,
214,141,249,0,10,108,92,186,220,250,128,1,3,108,210,164,73,182,199,30,123,184,138,12,12,92,191,92,125,235,214,66,170,189,22,40,58,224,58,65,171,174,174,118,125,197,109,175,176,194,10,238,131,2,55,15,192,
231,159,127,190,221,124,243,205,238,254,46,58,128,87,93,87,49,108,150,139,5,138,14,184,122,185,246,218,107,187,195,228,201,25,96,163,227,211,167,79,31,123,234,169,167,236,208,67,15,117,101,25,28,48,61,
176,93,86,108,255,190,195,0,151,11,47,212,101,129,10,179,145,248,102,189,61,244,208,67,182,201,38,155,228,7,3,3,35,72,251,45,208,97,86,108,11,75,197,118,128,223,96,131,13,236,190,251,238,179,83,78,57,
197,253,74,116,26,24,237,255,217,229,219,66,135,1,222,86,19,3,42,192,139,237,7,29,116,144,77,156,56,209,182,219,110,187,60,219,195,220,222,86,107,46,90,46,21,128,51,127,11,96,117,81,46,156,121,124,245,
213,87,183,27,110,184,193,46,190,248,98,119,178,167,235,245,182,120,13,181,23,246,205,22,72,5,224,73,86,251,224,192,102,13,134,129,3,7,218,228,201,147,109,183,221,118,115,69,24,40,228,7,224,125,139,45,
249,184,83,1,7,48,164,161,161,193,230,206,157,235,92,57,105,1,204,49,34,182,163,7,224,139,46,186,200,110,188,241,70,235,222,189,123,126,61,62,184,249,102,91,45,109,219,169,128,203,53,199,15,216,217,254,
251,239,111,111,189,245,150,235,47,0,43,207,255,1,232,25,36,124,182,221,118,91,155,48,97,130,13,27,54,204,21,161,60,76,15,108,247,45,182,232,113,167,2,174,238,172,181,214,90,238,240,130,11,46,176,243,
206,59,207,230,205,155,151,95,116,73,178,93,160,74,127,210,73,39,217,3,15,60,96,27,110,184,97,126,48,200,35,168,253,176,95,104,129,84,0,46,240,88,129,155,58,117,170,237,189,247,222,246,252,243,207,187,
94,2,30,249,114,255,234,186,216,78,222,70,27,109,100,15,62,248,160,197,111,88,184,108,116,26,24,42,31,246,205,22,72,5,224,62,24,98,39,103,229,0,248,245,215,95,187,57,28,0,53,48,84,30,29,229,229,254,135,
15,31,110,207,60,243,140,245,237,219,55,176,93,70,74,236,83,7,184,207,206,15,63,252,208,14,59,236,48,199,94,250,45,86,39,217,206,9,27,58,128,95,99,141,53,236,166,155,110,114,39,118,148,215,32,97,112,4,
137,109,152,70,35,0,30,31,177,157,101,86,214,215,25,0,114,213,2,82,253,71,15,240,210,115,233,198,37,92,117,117,181,43,66,123,225,76,62,165,128,11,68,177,29,224,191,249,230,27,231,226,113,245,136,6,131,
0,86,29,233,97,59,111,88,94,114,201,37,238,30,60,15,92,200,245,151,51,219,83,201,112,129,199,30,102,10,120,210,156,204,13,25,50,100,169,151,112,114,243,212,231,193,139,103,159,125,214,14,60,240,64,154,
200,123,143,114,4,62,245,128,59,132,90,64,226,24,32,121,252,150,75,184,11,47,188,208,126,251,237,183,188,171,78,178,61,233,254,79,61,245,84,187,247,222,123,173,170,170,202,13,34,127,218,208,247,148,250,
62,51,128,11,8,220,178,152,249,230,155,111,218,94,123,237,229,158,152,33,31,119,14,232,0,233,11,122,116,124,122,247,238,109,143,60,242,136,157,112,194,9,174,136,188,135,218,244,235,149,226,113,230,0,7,
4,1,170,249,250,186,235,174,179,147,79,62,217,190,253,246,91,7,58,224,105,190,22,104,98,187,244,135,31,126,184,61,249,228,147,182,205,54,219,228,7,131,218,83,157,82,220,103,18,112,1,225,179,115,214,172,
89,118,200,33,135,216,195,15,63,236,178,253,57,92,229,217,75,79,93,86,248,110,185,229,22,247,120,21,3,2,29,82,202,108,207,52,224,128,35,87,45,118,178,204,58,98,196,8,251,232,163,143,150,120,9,71,121,1,
204,3,148,60,72,201,3,149,106,147,129,81,138,146,121,192,5,138,216,14,144,220,125,59,241,196,19,221,2,12,249,26,12,2,88,117,164,199,205,175,184,226,138,118,249,229,151,219,53,215,92,99,171,172,178,74,
126,74,40,53,182,151,12,224,128,8,219,5,60,105,158,148,217,103,159,125,220,250,60,105,0,214,28,78,90,2,155,53,24,118,220,113,71,119,233,55,116,104,243,107,243,180,73,189,82,1,190,164,0,23,128,58,169,3,
200,95,126,249,197,221,129,227,105,153,223,127,255,125,177,151,112,98,187,128,63,253,244,211,237,238,187,239,182,158,61,123,186,193,32,224,245,29,89,221,151,36,224,2,3,54,139,153,188,214,196,37,220,75,
47,189,228,178,1,24,112,53,56,84,7,61,58,62,155,109,182,153,141,29,59,214,142,59,238,56,151,45,239,161,54,85,39,75,251,146,6,28,32,4,168,128,188,250,234,171,141,87,156,126,248,225,135,188,171,78,186,121,
0,229,35,182,31,121,228,145,246,196,19,79,216,86,91,109,149,31,12,242,8,89,2,155,190,150,60,224,2,196,103,231,123,239,189,103,7,31,124,176,99,47,249,186,84,211,224,80,29,13,18,234,174,187,238,186,118,
251,237,183,219,57,231,156,227,178,53,24,178,198,246,178,1,28,148,228,170,197,206,123,238,185,199,96,239,199,31,127,220,230,75,56,173,236,237,178,203,46,14,120,218,204,210,37,92,89,1,238,16,138,55,98,
59,192,215,213,213,185,101,214,91,111,189,213,101,139,213,133,216,78,1,220,255,74,43,173,100,87,94,121,165,251,16,239,84,83,66,22,216,94,150,128,3,28,128,10,120,210,79,63,253,180,237,183,223,126,246,246,
219,111,231,217,46,32,201,151,248,151,112,176,252,197,23,95,116,143,100,145,79,155,12,152,52,3,95,182,128,11,64,49,25,32,137,109,118,246,217,103,219,101,151,93,230,94,103,150,171,214,124,173,58,154,18,
164,167,206,29,119,220,17,135,187,92,47,127,230,175,50,170,147,150,125,217,3,46,32,96,179,152,249,250,235,175,219,158,123,238,105,47,191,252,178,203,6,60,242,53,56,84,7,61,58,62,91,110,185,165,61,254,
248,227,118,212,81,71,185,108,121,15,181,169,58,157,189,15,128,123,8,8,80,128,4,176,171,174,186,202,206,56,227,12,155,51,103,142,59,49,3,188,164,155,71,199,71,108,63,246,216,99,237,177,199,30,179,45,182,
216,34,63,24,210,196,246,0,184,7,184,14,125,118,78,159,62,221,61,41,195,117,56,162,57,92,131,67,117,196,118,234,174,191,254,250,22,255,217,128,157,121,230,153,46,91,131,33,13,108,15,128,11,177,196,94,
174,90,236,140,131,16,219,49,199,28,99,159,126,250,105,254,196,76,64,170,42,128,202,59,160,219,119,223,125,45,14,89,109,172,207,35,180,169,243,2,167,232,132,77,0,124,41,70,247,217,254,249,231,159,187,
101,86,22,96,16,177,186,16,219,201,199,253,119,235,214,205,221,129,35,112,17,119,228,52,37,116,22,219,3,224,32,179,20,17,219,5,18,79,202,112,55,237,221,119,223,109,243,37,220,174,187,238,234,30,155,30,
60,120,176,251,54,218,148,247,88,202,215,47,215,236,0,248,50,152,19,144,0,29,183,252,211,79,63,185,57,26,230,194,90,185,234,164,155,23,168,210,159,123,238,185,198,34,207,58,235,172,147,63,209,211,64,90,
134,174,252,229,162,1,240,101,52,29,160,3,176,64,122,229,149,87,12,214,190,250,106,243,191,69,1,48,249,148,243,197,119,255,91,111,189,181,141,27,55,206,61,153,67,25,13,36,191,124,177,142,3,224,127,209,
178,2,20,102,19,176,104,204,152,49,142,241,48,31,29,3,66,172,214,87,160,243,245,199,31,127,188,123,207,157,124,181,167,178,197,218,7,192,219,105,89,177,29,32,153,211,153,219,9,61,134,200,157,39,191,66,
108,103,160,240,158,251,17,71,28,225,138,240,246,108,177,37,0,190,28,44,12,59,249,8,224,219,110,187,205,14,56,224,0,123,255,253,247,93,235,73,166,163,100,128,168,60,171,122,8,131,167,216,18,0,47,182,133,
83,214,126,241,125,72,202,126,112,49,186,3,91,17,49,153,216,114,132,27,147,136,201,74,179,151,87,224,88,143,93,233,124,0,93,177,36,0,222,78,203,2,146,92,49,243,241,165,151,94,234,194,140,209,44,3,160,
16,216,210,83,151,219,177,188,250,132,44,41,90,165,43,176,28,54,1,240,191,104,68,88,13,75,1,155,147,45,130,3,15,26,52,200,181,134,14,160,147,96,83,30,145,158,23,27,31,125,244,81,167,83,123,46,81,196,77,
0,124,25,141,171,147,45,177,154,192,3,128,13,91,17,216,171,99,191,105,177,26,29,39,115,188,244,240,221,119,223,185,34,29,5,54,95,22,0,119,38,111,219,70,192,0,54,209,33,113,223,184,113,4,29,64,139,189,
106,81,64,75,127,237,181,215,186,37,86,242,209,145,47,230,171,78,49,247,1,240,54,88,23,160,17,1,195,19,175,188,173,234,235,10,177,90,131,128,114,111,188,241,134,91,156,225,101,8,132,54,1,187,163,37,0,
190,20,139,139,133,20,35,22,28,33,68,54,222,120,99,87,75,236,77,54,33,61,131,128,152,115,184,111,194,145,33,232,24,8,26,60,201,186,197,78,7,192,23,99,97,177,90,44,28,57,114,164,123,29,153,226,232,200,
151,155,86,19,128,200,71,250,231,158,123,46,191,116,74,25,234,0,118,103,74,0,188,128,245,125,86,247,235,215,207,69,113,94,115,205,53,93,73,223,77,251,85,197,106,64,37,182,28,15,66,18,117,10,81,123,157,
197,106,191,159,1,112,207,26,128,5,40,2,143,48,160,187,239,190,187,43,1,208,0,151,156,171,5,162,88,205,251,233,10,74,64,123,136,188,132,75,116,242,38,0,222,2,128,230,86,146,213,213,213,46,96,144,110,102,
0,88,18,104,202,105,96,112,252,65,252,39,237,176,154,240,98,136,88,237,18,41,218,148,61,224,98,53,12,38,150,27,39,101,132,249,66,228,190,197,94,225,38,160,165,231,239,185,120,118,13,65,39,47,161,242,105,
218,151,45,224,0,205,71,238,150,24,110,132,245,66,228,166,11,177,90,131,128,114,68,145,226,137,23,253,139,175,223,30,249,105,148,178,4,92,238,22,96,137,217,6,171,55,221,116,83,135,143,216,155,4,75,122,
6,1,177,225,120,224,1,192,17,77,7,26,40,201,186,105,74,151,21,224,48,16,17,171,137,213,70,248,46,68,96,201,77,59,101,139,158,60,233,9,254,67,152,48,9,109,194,250,172,72,217,0,46,86,3,12,177,217,96,181,
2,243,251,110,218,7,78,172,6,84,98,192,113,82,70,120,48,68,237,105,160,248,245,210,124,92,242,128,3,22,160,0,30,199,92,106,233,127,78,165,75,206,213,2,81,172,230,246,229,253,247,223,239,112,164,13,68,
94,194,37,50,180,41,105,192,53,183,130,7,49,216,136,205,202,203,0,136,216,235,18,222,198,215,19,235,13,79,240,213,87,95,185,18,98,181,87,60,115,135,37,9,184,88,141,171,38,230,218,232,209,163,243,175,251,
200,125,139,189,66,76,64,75,79,144,125,194,126,33,232,228,37,84,62,171,251,146,2,28,160,249,200,221,242,4,41,225,183,36,232,147,238,155,60,13,2,142,185,201,193,25,56,225,190,16,191,61,167,200,248,166,
100,0,151,187,133,137,189,122,245,114,247,170,9,187,133,36,217,43,204,164,103,16,112,219,146,48,30,132,247,66,52,29,104,62,87,157,172,239,51,15,56,12,68,196,106,222,207,214,75,249,2,75,110,90,96,161,231,
35,61,15,17,18,114,83,229,105,19,214,151,162,100,26,112,177,26,96,136,161,198,9,22,225,181,16,223,77,59,69,203,70,122,64,37,86,27,151,90,132,241,66,212,158,128,111,169,82,82,187,76,2,14,88,128,34,86,19,
59,141,112,90,8,58,242,147,115,181,64,148,158,8,139,132,237,66,40,143,168,61,151,40,209,77,230,0,215,220,10,30,68,81,226,12,156,48,90,8,128,201,77,59,69,203,198,215,207,158,61,219,205,239,132,235,66,196,
234,150,162,37,191,203,12,224,98,53,46,153,216,104,0,189,243,206,59,59,128,228,166,147,96,139,213,210,243,154,46,225,185,16,116,190,151,112,202,50,216,164,30,112,128,230,35,119,75,56,236,179,206,58,43,
15,13,122,185,233,188,50,62,208,32,64,199,195,254,204,213,132,229,66,252,246,156,162,140,54,169,6,92,238,22,38,18,3,141,199,130,137,142,132,200,77,139,189,194,76,122,6,193,252,249,243,93,36,38,194,112,
33,154,14,196,124,167,44,179,77,42,1,135,129,136,88,125,244,209,71,219,49,113,64,29,68,96,37,129,150,123,22,219,121,81,159,176,91,106,131,54,97,125,185,75,234,0,23,171,1,6,54,115,169,69,24,44,68,236,117,
9,111,35,247,13,216,63,254,248,163,243,4,51,102,204,112,37,212,158,6,138,87,173,44,15,83,7,184,24,201,60,205,124,141,160,131,161,133,88,13,144,98,53,97,52,136,143,134,36,189,132,83,134,77,58,94,53,18,
144,188,61,73,76,51,254,174,130,112,87,200,226,88,45,61,192,126,246,217,103,142,213,132,213,66,196,106,151,8,155,86,22,72,5,195,89,241,66,152,115,151,245,82,139,160,182,227,199,143,119,245,3,171,157,25,
150,184,233,84,192,229,138,251,247,239,111,19,38,76,176,238,221,187,187,206,194,94,229,249,189,23,171,209,17,79,133,75,173,185,115,231,186,34,148,15,39,101,190,181,10,31,119,42,224,98,36,119,183,16,1,
42,23,175,46,251,122,64,229,127,75,56,11,71,0,154,252,0,182,172,181,228,125,167,2,174,174,113,226,197,39,9,52,249,58,3,231,248,181,215,94,115,110,95,145,18,24,48,1,104,44,211,118,73,5,224,0,39,182,171,
235,98,53,12,38,246,25,238,251,157,119,222,113,217,114,223,225,82,75,214,106,251,62,21,128,251,221,77,178,157,152,103,132,193,66,52,40,2,171,125,139,45,219,113,170,0,23,171,1,246,139,47,190,112,172,254,
228,147,79,220,47,10,151,90,203,6,236,226,74,119,24,224,133,230,103,117,74,174,89,101,106,107,107,221,223,73,144,47,86,51,24,130,180,223,2,69,7,92,96,234,254,179,0,84,215,197,106,210,51,103,206,116,172,
110,108,108,116,217,154,171,85,54,236,219,111,129,162,3,46,102,78,155,54,45,15,162,206,178,121,29,87,174,154,96,55,10,80,135,142,129,18,230,234,246,3,156,108,161,232,161,55,1,14,86,3,30,127,24,131,0,180,
222,189,78,254,9,44,101,25,36,242,12,201,14,135,116,251,44,80,116,134,211,61,129,199,31,198,12,25,50,196,6,14,28,232,22,76,72,127,249,229,151,238,23,200,125,171,108,251,126,86,168,189,56,11,116,8,224,
250,114,216,203,187,212,252,155,159,47,242,0,190,46,28,23,199,2,29,10,184,220,59,108,70,72,227,234,3,171,139,3,110,161,86,59,20,112,129,172,147,182,66,29,10,186,226,90,128,147,182,121,254,87,116,237,218,
213,79,134,227,12,91,160,0,150,243,42,226,85,236,6,255,55,241,8,112,144,210,176,64,18,75,176,142,25,222,84,239,255,188,30,61,122,248,201,112,156,97,11,44,138,101,83,125,69,46,138,154,95,151,108,249,97,
85,149,149,25,254,137,161,235,190,5,170,42,171,252,164,129,53,115,120,243,91,239,45,89,189,122,85,186,120,101,173,74,134,68,230,44,64,204,185,202,69,201,59,177,162,166,166,230,237,248,215,124,164,95,196,
178,102,191,190,253,148,12,251,140,90,0,12,89,223,144,68,57,251,95,28,32,120,90,69,172,140,95,195,180,209,202,96,207,200,216,42,254,3,244,32,217,180,0,216,37,217,221,20,229,28,198,110,8,196,11,31,81,237,
93,181,255,138,127,222,14,254,79,156,57,115,134,205,138,99,136,6,201,142,5,0,187,79,159,190,173,59,156,179,169,53,163,106,118,134,220,238,230,9,7,81,69,68,132,186,230,251,146,45,197,169,56,224,239,3,194,
156,222,218,124,169,76,49,103,131,213,34,96,199,152,70,93,162,17,206,147,199,61,95,232,228,227,68,252,214,198,160,248,90,109,114,124,216,106,245,133,187,87,13,13,245,86,87,87,111,141,115,26,221,122,56,
47,234,5,233,60,11,176,168,194,117,54,151,94,92,89,85,198,103,228,49,168,201,14,205,143,31,15,29,60,106,212,168,87,149,177,72,137,22,208,121,178,63,92,144,203,74,217,220,55,198,96,15,243,193,230,103,44,
2,56,202,248,17,163,222,185,166,220,216,248,176,213,156,78,94,144,12,88,32,158,179,113,227,241,21,216,236,100,111,11,2,78,33,119,34,119,71,237,240,216,75,92,158,139,108,243,100,197,144,78,159,5,184,244,
138,111,64,94,92,115,82,205,56,205,217,201,94,46,22,112,21,116,192,215,214,18,49,126,104,148,203,13,136,95,213,171,140,93,5,175,138,52,191,237,167,130,97,223,209,22,152,215,124,31,164,169,190,101,181,
116,34,215,217,75,235,196,255,1,213,239,255,171,180,29,147,203,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::play_png2 = (const char*) resource_PlayerMainMenuComponent_play_png2;
const int PlayerMainMenuComponent::play_png2Size = 5828;

// JUCER_RESOURCE: playing_png, 6179, "../../Resources_LFS/Boutons/Exports/Playing.png"
static const unsigned char resource_PlayerMainMenuComponent_playing_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,
28,233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,38,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,
68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,
34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,
102,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,
34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,
47,47,112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,
47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,
105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,48,60,47,116,105,102,102,58,67,
111,109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,
105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,
32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,
105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,
101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,
32,32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,
102,121,68,97,116,101,62,50,48,49,57,45,48,54,45,49,50,84,48,57,58,48,54,58,48,57,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,
97,116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,56,46,52,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,
115,99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,185,26,17,19,0,0,19,150,73,68,65,84,120,1,237,93,9,116,85,197,25,254,
239,75,2,36,33,9,4,72,66,176,45,21,75,61,106,165,110,168,167,181,117,169,173,59,86,5,21,119,171,134,216,211,218,186,181,46,136,136,85,235,210,205,83,33,90,55,84,84,60,182,226,222,227,82,90,237,113,163,
22,169,82,69,177,90,27,32,16,2,33,100,129,36,239,246,255,230,189,239,101,222,53,33,225,144,188,220,247,222,252,28,238,157,153,59,115,223,220,255,155,239,255,103,238,242,199,147,62,200,188,121,243,246,
213,106,83,60,223,63,72,36,50,206,23,191,82,243,5,125,104,234,170,12,156,6,90,60,241,86,137,68,107,125,207,123,69,127,102,209,140,25,51,150,244,246,115,94,79,21,124,223,247,20,232,169,17,223,155,227,123,
50,177,167,122,174,60,60,26,240,124,89,17,245,252,153,10,252,99,158,167,185,110,164,91,192,107,106,106,38,248,81,127,129,214,159,220,77,27,87,20,118,13,248,242,134,151,227,157,86,85,85,181,50,216,213,
207,1,62,119,238,220,67,213,84,60,166,21,75,131,149,93,62,173,52,208,160,174,119,106,117,117,245,203,118,175,147,0,143,131,253,188,86,200,179,43,169,153,144,119,42,154,228,157,242,38,249,164,164,77,26,
135,117,72,107,94,167,93,197,165,83,172,129,252,246,28,41,105,203,149,241,141,195,100,210,154,98,153,84,55,92,212,253,6,123,209,174,160,31,97,131,158,168,17,55,227,111,106,139,36,102,3,228,199,119,171,
147,181,133,91,131,39,115,249,16,105,160,172,121,136,156,180,188,92,246,172,43,10,246,170,193,139,120,147,105,222,35,56,138,9,90,220,103,39,129,253,196,174,107,101,238,126,159,57,176,131,42,12,97,30,132,
188,67,177,2,102,1,41,245,59,253,135,128,49,202,115,176,25,91,54,246,100,241,228,34,164,41,104,248,252,46,245,204,186,125,154,104,224,163,210,22,233,136,248,178,107,125,97,87,143,61,217,105,201,146,37,
203,159,121,230,153,247,12,195,37,34,115,186,142,138,241,213,14,108,91,35,233,149,6,118,203,212,21,219,130,229,53,242,88,107,239,43,190,188,197,131,157,58,65,155,125,240,74,103,198,169,144,52,221,195,
167,95,187,120,66,242,68,206,147,253,192,240,41,246,53,45,211,217,184,155,160,217,26,73,207,52,48,196,202,42,32,83,34,177,219,165,93,197,75,3,166,160,235,136,75,165,155,6,176,194,178,5,88,171,123,247,
118,178,11,63,213,117,182,147,204,208,0,238,153,36,75,100,28,76,250,88,187,16,55,85,156,100,134,6,130,88,226,161,23,0,79,122,234,229,238,160,101,6,216,184,138,110,176,44,200,77,245,229,69,116,193,159,
227,197,86,131,157,126,84,162,186,68,112,146,58,13,164,20,240,28,93,240,119,42,196,81,191,235,62,188,62,168,49,87,171,230,38,117,87,157,197,191,148,82,192,1,246,136,72,190,28,61,108,119,25,169,251,63,
181,46,147,218,206,70,163,254,92,29,12,29,122,220,201,192,106,32,37,128,195,140,195,116,127,103,232,87,229,133,49,23,38,174,232,246,17,39,201,31,91,223,145,211,26,30,144,54,191,221,148,179,110,162,146,
75,244,171,6,98,206,180,95,79,153,124,50,152,108,250,105,130,189,85,77,122,123,220,172,159,144,63,73,154,199,221,34,103,20,236,103,26,162,46,76,63,77,125,242,217,92,110,71,53,48,224,128,231,198,39,104,
211,242,247,50,125,133,217,30,226,229,72,158,254,135,192,204,3,222,249,165,167,203,223,198,252,88,42,114,138,77,25,124,58,204,188,147,254,213,64,202,52,90,153,83,98,122,174,143,233,146,174,0,108,6,184,
248,119,208,208,9,178,122,236,28,185,188,232,48,83,7,131,3,76,119,108,79,82,217,14,101,82,6,120,187,242,182,39,33,168,96,59,228,151,37,199,201,191,43,174,148,61,243,42,19,131,1,3,195,201,142,107,32,101,
90,76,188,90,179,141,62,147,237,0,126,215,220,114,125,76,251,51,249,205,136,19,76,11,148,113,96,108,227,20,238,80,47,26,72,25,224,189,244,35,113,24,160,2,120,78,244,46,26,254,109,169,175,188,65,14,27,
58,49,193,118,231,219,19,234,218,238,68,40,0,135,255,38,192,188,2,76,228,32,240,227,163,34,133,242,226,152,31,202,35,165,103,155,201,30,215,235,172,195,54,110,223,187,6,66,1,56,88,13,240,232,195,237,110,
131,205,28,12,39,23,236,37,45,227,110,149,83,11,246,49,85,80,142,227,104,239,164,111,26,24,84,192,193,108,200,202,142,122,169,143,54,27,83,142,60,1,70,26,66,38,99,64,0,224,5,165,103,202,75,202,248,209,
202,124,176,29,231,113,102,62,166,171,222,182,131,10,120,135,62,60,129,188,176,229,3,25,179,234,74,121,186,237,61,147,7,192,52,219,166,32,190,225,164,14,0,31,170,62,125,157,250,246,139,139,14,49,71,81,
63,198,117,199,118,91,103,193,244,160,2,206,206,140,141,20,155,228,177,245,119,202,209,245,53,210,228,111,73,48,54,200,118,206,212,105,254,111,43,57,94,222,45,191,66,118,203,171,136,115,61,118,167,142,
231,118,251,100,13,132,2,112,130,138,59,112,207,182,45,151,226,218,203,229,174,230,215,76,79,193,90,128,75,243,207,238,147,237,56,178,187,130,253,158,130,126,171,130,15,65,125,14,12,214,119,251,152,6,
66,1,56,193,128,71,7,144,144,11,54,60,34,147,215,222,38,159,116,52,36,238,173,147,213,166,130,110,56,217,163,249,191,68,205,123,93,229,245,242,237,161,187,152,1,130,65,194,243,177,77,182,239,67,5,56,192,
176,217,249,214,214,255,202,151,215,204,150,107,55,61,103,112,34,171,131,108,199,132,13,101,0,190,44,82,36,139,199,252,72,30,210,137,29,234,115,144,112,226,231,0,15,161,6,0,30,254,145,157,179,55,61,47,
227,87,207,150,55,183,126,154,48,213,4,146,221,7,219,1,60,221,195,116,93,186,225,41,220,212,252,175,155,42,40,119,51,121,172,120,66,44,100,59,128,255,180,179,65,246,95,251,43,99,234,209,101,14,6,2,204,
203,32,147,193,246,161,94,174,44,28,117,142,121,6,63,50,82,144,152,249,179,14,219,100,211,62,212,128,3,8,48,29,192,19,36,76,230,138,116,82,215,219,18,142,102,30,237,241,226,69,67,229,141,242,163,225,223,
50,216,98,144,96,192,192,42,100,155,132,30,112,2,66,38,3,200,205,186,108,195,18,110,202,250,187,164,217,223,154,48,213,172,195,54,0,20,255,104,254,127,55,226,68,89,90,126,185,76,204,45,51,101,182,219,
96,155,76,223,167,13,224,4,130,55,88,144,127,178,245,93,25,94,123,153,220,211,252,186,57,12,206,2,92,0,105,11,216,140,50,252,155,148,55,78,62,168,184,74,110,44,57,214,84,161,219,200,22,182,167,29,224,
64,137,76,166,31,255,193,134,135,229,192,181,191,150,207,58,55,38,76,53,151,106,4,158,108,103,249,207,139,190,35,171,244,101,139,111,14,221,57,49,24,120,62,182,201,196,125,90,2,78,32,108,118,190,190,245,
19,249,226,234,89,50,103,211,159,205,97,219,135,179,62,246,44,71,219,177,250,58,213,43,99,46,146,251,245,245,42,219,244,115,190,96,183,203,148,116,90,3,14,16,104,170,201,206,107,54,61,43,187,172,153,35,
111,183,255,207,128,104,3,73,208,80,134,250,180,20,103,234,11,148,88,194,225,133,74,8,202,49,48,50,81,50,230,170,200,118,0,137,167,111,251,212,221,34,23,110,68,48,170,190,45,225,242,189,60,121,124,212,
185,242,236,232,42,41,209,119,230,105,250,51,141,237,25,3,56,128,5,219,1,60,65,154,187,249,85,25,177,234,231,242,156,222,159,135,160,156,64,154,130,248,6,108,38,219,143,28,182,155,108,172,188,73,170,135,
127,211,28,69,57,6,17,172,66,38,72,70,1,78,64,8,30,128,108,140,182,202,81,250,4,238,196,245,247,72,171,126,236,64,83,205,58,108,195,65,130,1,3,185,99,196,84,249,71,249,101,178,115,238,168,196,204,159,
110,131,109,210,113,159,145,128,19,8,176,153,64,226,11,151,66,93,194,205,111,137,69,55,65,57,192,133,85,176,5,160,114,94,176,119,222,78,178,178,226,26,153,83,124,148,169,66,183,145,206,108,207,104,192,
129,18,153,76,32,207,106,120,80,14,90,247,91,89,221,185,41,97,170,131,102,30,128,226,31,217,126,117,241,247,228,191,99,103,203,1,67,198,39,6,67,186,178,61,227,1,39,123,109,118,190,186,229,99,169,92,61,
83,110,106,122,209,28,134,153,39,171,89,31,123,14,18,180,253,66,206,8,121,173,236,167,114,247,200,83,77,21,14,6,90,16,187,93,152,211,89,3,56,64,32,168,100,231,21,141,79,201,196,53,215,203,210,246,218,
56,167,187,88,77,208,192,116,212,167,165,56,183,240,0,217,172,75,184,227,242,247,48,85,80,206,121,1,219,132,121,159,85,128,19,8,178,29,64,126,216,177,78,246,170,187,89,126,188,241,113,115,152,172,14,250,
118,50,25,230,191,208,27,34,139,70,157,47,79,142,62,95,134,123,67,19,51,127,214,225,239,132,113,159,149,128,3,8,0,10,224,9,210,237,155,255,38,165,171,174,144,23,245,133,74,250,240,160,111,71,59,176,153,
108,63,118,216,30,210,52,238,102,57,175,240,64,28,50,229,24,48,104,31,86,201,90,192,9,8,193,3,144,27,162,45,114,248,186,59,100,218,250,123,5,159,52,211,84,179,14,219,112,144,208,143,223,53,242,20,121,
189,236,98,249,82,78,105,98,230,15,224,195,40,225,236,213,32,104,202,94,194,61,214,186,84,10,106,47,149,7,91,98,127,81,2,0,227,120,208,204,219,230,127,255,33,95,146,79,198,206,146,107,138,143,48,189,167,
219,8,27,219,29,224,214,224,34,147,1,36,0,59,67,35,83,28,178,238,118,169,235,108,50,108,7,120,65,51,79,243,79,182,207,46,62,82,254,83,49,75,246,27,242,69,51,64,48,72,194,196,118,7,184,5,56,147,54,59,23,
111,249,72,42,86,95,45,183,54,189,108,14,211,135,247,196,118,180,29,159,91,42,111,150,93,34,243,70,78,51,109,56,24,232,10,248,59,131,177,119,128,247,160,117,0,106,179,243,178,198,69,178,123,221,141,242,
110,251,234,248,180,172,247,37,92,85,225,55,100,147,78,234,142,210,251,243,16,88,16,206,11,122,248,217,1,47,118,128,247,162,98,178,29,236,92,222,190,70,190,86,119,147,252,116,227,159,76,43,219,135,219,
167,33,147,97,254,139,116,217,246,140,62,129,123,98,212,121,82,160,203,57,186,4,214,177,219,165,34,237,0,239,131,150,193,116,176,147,32,253,102,243,98,25,173,223,194,189,188,101,133,14,131,216,63,2,105,
159,142,230,31,101,83,242,191,102,158,185,159,93,184,191,169,130,243,13,134,111,119,128,219,8,245,146,6,72,128,23,64,174,215,175,93,15,91,247,123,153,222,48,223,176,150,166,26,117,108,225,32,161,31,191,
119,228,116,121,181,236,34,217,73,111,213,178,12,231,76,149,56,192,183,83,211,96,59,216,76,32,31,110,249,135,89,194,61,210,242,182,57,19,202,113,28,245,108,177,205,255,55,134,236,44,159,233,195,152,43,
139,14,55,85,80,55,85,160,59,192,109,84,182,35,77,38,131,217,136,57,119,106,195,253,134,241,96,62,202,0,32,25,204,211,162,204,46,255,69,201,49,230,59,119,28,15,14,16,182,233,239,189,3,124,7,53,10,54,19,
72,248,116,248,246,223,110,254,171,57,107,79,62,154,108,199,64,193,119,238,87,21,127,215,212,103,236,186,29,236,210,54,155,59,192,183,169,158,190,29,4,59,241,143,0,255,100,227,31,165,124,213,213,242,247,
173,31,155,19,208,26,216,103,195,32,137,120,49,223,141,151,40,33,12,144,96,215,235,239,180,3,188,191,53,26,242,243,165,36,184,110,200,117,176,195,221,3,91,33,244,217,136,45,135,112,99,20,78,240,152,199,
62,102,19,98,37,15,196,239,217,35,76,41,99,208,218,117,251,51,237,0,223,65,109,98,130,198,53,56,252,241,194,81,103,155,48,99,56,45,6,0,205,188,253,51,44,207,213,129,130,199,177,215,199,63,158,24,104,176,
209,7,7,184,141,196,118,164,193,90,248,102,128,141,201,214,252,145,167,203,41,5,123,155,51,160,44,71,143,7,193,6,171,33,44,191,170,241,105,185,161,233,5,83,6,43,193,227,166,96,128,54,14,240,237,84,44,
128,1,152,100,53,98,198,33,18,180,125,227,133,105,251,212,100,53,202,48,153,59,101,253,253,242,63,253,22,14,146,42,176,241,91,14,112,104,161,143,210,197,106,223,152,109,152,111,152,113,8,6,0,128,14,250,
107,88,1,148,145,213,231,108,88,32,247,53,191,97,218,160,12,3,33,21,204,54,63,168,27,7,56,53,177,141,61,24,136,105,25,151,87,63,25,126,176,252,122,196,247,77,11,130,213,29,171,57,8,80,113,81,235,191,204,
109,216,22,253,158,29,130,65,0,176,83,45,14,240,94,52,222,197,66,49,177,224,30,213,120,175,123,228,197,254,212,155,109,166,237,211,144,213,24,4,136,57,119,202,250,251,76,56,50,212,65,25,6,2,7,143,221,
46,21,105,7,120,15,90,142,113,186,107,169,117,75,201,20,185,180,232,80,83,27,96,129,241,52,211,60,5,216,142,99,44,175,105,254,187,204,216,176,144,135,13,171,233,251,19,133,41,78,56,192,187,81,56,89,141,
67,7,107,204,55,68,113,46,207,41,50,53,109,51,109,55,37,219,49,161,67,108,185,105,13,247,10,194,142,65,120,190,193,98,181,221,79,7,184,165,13,78,202,8,222,125,165,167,201,233,5,251,154,26,0,26,96,6,125,
53,125,56,89,61,75,99,202,93,167,97,198,32,65,43,97,10,7,121,227,0,143,3,64,223,138,44,98,187,61,168,129,253,16,10,20,2,102,6,129,70,57,7,6,210,111,104,12,185,147,213,87,35,188,24,132,172,54,153,16,109,
178,30,112,178,26,12,70,44,55,44,181,16,230,11,66,243,141,58,182,96,0,160,140,172,62,95,195,132,254,33,30,27,22,101,56,142,193,16,70,201,90,192,97,110,1,26,129,65,12,55,132,245,130,208,76,119,199,106,
14,2,212,123,170,237,93,153,190,126,190,9,35,134,188,125,62,228,195,40,89,9,56,205,109,167,66,251,149,220,49,38,90,227,215,53,156,23,196,54,211,54,96,100,53,6,1,98,195,77,215,23,30,16,54,12,66,119,128,
58,97,151,172,2,60,56,137,66,172,54,132,239,130,144,213,52,211,4,46,184,212,66,76,56,132,9,163,128,213,96,125,186,72,214,0,78,86,3,24,196,102,91,88,122,142,9,219,133,188,109,166,145,167,144,237,152,157,
35,6,28,190,57,67,120,48,8,207,151,14,172,54,29,142,111,50,30,112,48,144,147,40,48,28,75,45,190,97,2,64,113,60,232,171,131,108,199,227,203,153,26,14,12,18,180,18,113,61,166,205,46,163,1,167,111,5,26,136,
193,246,96,233,25,130,240,92,16,12,130,160,249,70,57,89,141,52,98,189,77,213,96,64,31,119,172,71,54,193,106,147,73,211,77,70,2,78,86,195,84,35,230,218,195,186,166,70,56,46,8,205,55,234,216,130,1,128,50,
14,2,196,120,67,216,47,8,202,104,37,236,54,233,152,206,40,192,131,75,45,196,90,67,248,45,10,64,11,154,111,28,227,32,64,26,49,221,78,213,143,11,16,238,11,130,65,0,214,103,138,100,12,224,156,68,97,169,53,
33,119,180,89,106,33,236,22,132,102,186,39,86,99,16,32,134,219,233,250,121,48,194,123,65,232,14,48,72,50,73,210,30,240,224,36,234,58,141,169,54,83,195,108,65,130,147,47,2,23,92,106,33,118,219,217,13,15,
37,234,99,96,128,245,153,40,105,13,56,89,13,96,16,67,13,127,238,2,225,181,32,182,153,54,5,241,13,203,177,212,66,172,54,60,213,66,24,47,8,207,151,105,172,142,95,186,217,165,37,224,96,160,61,137,66,236,
52,132,211,130,244,182,212,162,15,71,140,54,132,237,130,4,173,132,41,204,208,77,218,1,78,223,10,60,16,43,109,65,233,89,38,140,22,242,24,4,96,105,80,232,195,81,254,142,198,100,155,166,79,181,86,116,172,
53,213,200,234,96,155,76,205,167,13,224,100,53,76,50,98,163,61,60,234,44,57,102,216,238,6,23,154,233,224,164,44,232,195,17,139,13,225,185,32,0,218,182,18,166,48,11,54,161,7,60,184,212,58,95,99,162,221,
169,97,178,40,0,141,102,154,101,216,115,16,32,141,151,253,193,106,132,229,130,96,96,100,210,82,203,92,84,31,55,161,6,156,230,22,75,45,196,64,195,179,234,201,26,30,11,66,51,29,100,53,6,0,202,48,8,182,248,
29,38,18,19,194,112,65,232,14,80,39,91,37,148,128,7,39,81,179,52,246,217,181,26,14,11,18,52,211,4,14,229,24,24,100,251,2,253,80,255,76,141,156,76,38,99,16,128,245,217,46,161,3,156,172,6,48,136,117,134,
167,90,8,131,5,33,171,77,198,218,208,124,227,91,173,181,209,38,99,190,255,170,225,182,32,60,95,54,179,218,82,149,18,34,68,130,187,219,100,36,252,52,252,53,4,101,96,40,192,179,5,172,134,113,38,171,111,
107,250,139,92,218,248,132,169,18,180,18,118,187,108,78,135,2,112,250,97,196,55,69,76,179,71,212,87,35,220,21,4,204,12,2,141,114,178,29,131,228,61,13,167,133,27,40,8,171,5,33,171,77,198,109,146,52,16,
10,192,87,71,55,153,78,61,53,250,130,237,94,106,93,162,140,254,149,50,27,130,129,3,198,211,74,152,66,183,73,210,192,160,2,142,15,224,33,135,235,91,162,235,42,111,144,209,145,66,147,7,171,105,166,77,65,
124,67,86,35,139,120,42,120,45,184,94,131,232,64,56,3,55,25,183,233,81,3,131,10,56,253,44,158,110,65,8,40,77,60,123,141,1,64,31,142,9,26,102,223,8,151,5,1,208,152,157,187,25,56,181,181,237,253,160,2,206,
174,113,242,213,157,175,230,12,28,117,31,109,249,167,156,177,225,129,68,88,12,12,2,7,52,181,216,183,125,40,0,7,211,147,223,63,137,77,214,0,40,24,140,216,103,48,223,47,169,25,135,208,124,187,165,86,223,
64,182,107,133,2,112,187,67,96,187,61,51,71,204,51,132,193,130,112,88,56,86,219,26,219,190,116,168,0,167,15,199,179,234,247,59,234,12,171,151,181,175,50,87,228,150,90,219,7,108,79,181,83,6,120,119,254,
153,157,138,221,62,137,173,159,81,246,179,198,39,229,230,166,151,204,97,178,218,45,181,168,173,29,219,15,56,224,190,31,123,80,177,66,255,92,20,196,139,71,31,100,183,201,106,228,95,217,178,82,111,160,220,
39,107,244,77,20,8,125,181,201,184,77,191,104,96,192,1,199,146,9,242,66,219,251,102,15,16,113,71,13,147,52,132,187,2,243,225,179,241,78,217,3,241,191,11,202,50,231,171,141,202,250,117,147,124,115,186,
95,79,29,59,25,204,53,151,79,248,131,49,16,124,119,205,64,178,252,35,176,4,27,117,193,122,154,249,216,89,220,182,191,52,48,224,12,71,71,185,124,194,31,140,41,170,189,92,78,46,216,75,205,117,142,44,222,
242,161,124,16,127,213,136,230,155,117,251,235,2,221,121,146,53,144,18,192,249,147,152,128,109,214,168,70,119,235,23,152,182,160,220,153,111,91,35,3,151,78,41,224,52,239,57,241,123,232,152,208,1,104,103,
190,7,14,224,224,153,83,10,56,126,28,240,70,117,210,230,100,112,52,128,73,91,236,205,190,248,239,231,183,199,2,217,12,78,119,220,175,246,167,6,186,193,178,37,162,254,51,118,43,43,254,75,37,109,41,39,125,
127,94,163,59,151,165,129,32,150,192,90,25,30,173,181,234,200,248,198,97,118,214,165,211,88,3,159,199,50,90,27,241,61,239,21,251,154,38,213,197,34,14,218,101,46,157,158,26,152,180,166,56,169,227,192,26,
62,124,145,93,186,231,154,34,41,107,30,98,23,185,116,26,106,0,24,78,170,27,30,236,249,162,72,85,85,21,94,29,249,144,71,114,124,79,78,92,94,206,172,219,167,169,6,78,82,12,213,124,39,122,239,249,178,98,
198,140,25,75,244,47,41,105,50,42,51,19,71,52,1,179,126,196,71,177,215,142,236,114,151,78,15,13,0,187,61,3,174,57,234,249,6,99,51,4,244,6,136,87,51,175,6,183,191,38,219,151,244,196,174,107,229,249,93,
234,237,34,151,14,185,6,0,246,241,239,151,37,247,210,151,55,170,170,171,14,4,185,205,195,19,36,188,136,55,93,107,197,34,195,198,171,163,97,245,91,95,112,62,61,89,125,161,204,193,103,95,168,88,125,14,108,
197,212,203,241,78,51,150,92,123,222,101,228,53,51,119,238,220,67,117,173,134,216,207,177,216,86,241,75,235,84,171,191,172,162,73,150,150,55,201,167,37,109,210,56,172,67,90,243,220,221,178,184,122,6,101,
135,155,42,88,103,99,233,5,23,60,73,39,219,182,207,142,119,170,93,111,91,31,81,93,93,253,50,59,153,4,56,10,227,160,63,166,201,216,7,93,172,233,246,233,166,129,6,5,123,170,13,54,46,224,115,128,163,176,
166,166,102,130,31,245,23,104,50,201,167,227,152,147,52,208,128,250,108,152,113,93,129,173,12,246,182,91,192,81,201,76,228,238,168,153,166,111,36,93,167,179,251,137,193,134,46,31,62,13,96,233,165,15,32,
175,169,186,176,106,33,125,118,176,151,61,2,206,138,6,248,154,154,125,52,63,197,243,253,131,244,11,174,113,106,42,42,53,95,192,58,110,63,40,26,104,137,61,7,137,214,198,239,150,46,194,58,187,183,158,252,
31,41,178,103,94,255,178,55,3,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::playing_png = (const char*) resource_PlayerMainMenuComponent_playing_png;
const int PlayerMainMenuComponent::playing_pngSize = 6179;

// JUCER_RESOURCE: stopped_png, 3466, "../../Resources_LFS/Boutons/Exports/Stopped.png"
static const unsigned char resource_PlayerMainMenuComponent_stopped_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,
28,233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,38,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,
68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,
34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,
102,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,
34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,
47,47,112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,
47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,
105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,48,60,47,116,105,102,102,58,67,
111,109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,
105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,
32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,
105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,
101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,
32,32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,
102,121,68,97,116,101,62,50,48,49,57,45,48,54,45,49,50,84,48,57,58,48,54,58,51,56,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,
97,116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,56,46,52,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,
115,99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,226,33,244,33,0,0,8,253,73,68,65,84,120,1,237,154,93,108,21,85,16,199,
231,236,189,173,165,181,208,80,40,242,145,42,130,4,163,86,162,72,148,104,34,68,163,36,198,26,8,26,245,77,31,110,139,15,62,250,226,39,62,249,104,162,180,247,129,104,76,212,68,226,71,85,226,71,140,162,38,
6,181,96,128,8,9,136,196,42,136,128,13,90,90,180,237,221,227,204,222,93,59,247,139,246,118,119,75,183,51,39,105,119,206,217,179,187,51,255,223,158,175,61,215,192,4,82,119,119,247,106,172,214,110,172,189,
13,192,89,108,193,46,194,124,253,4,46,213,42,241,41,48,100,192,156,0,112,143,91,99,190,198,199,244,116,116,116,244,142,247,56,83,169,130,181,214,32,232,205,142,53,207,91,3,43,42,213,211,242,233,163,128,
177,112,216,53,246,41,4,191,195,24,204,149,73,101,129,103,179,217,101,214,181,111,96,253,53,101,174,209,162,233,174,128,133,111,77,202,60,156,201,100,142,22,187,90,2,188,171,171,107,61,118,21,59,176,226,
220,226,202,154,79,148,2,253,56,244,110,238,236,236,252,156,123,93,0,220,135,253,49,86,168,41,168,228,186,208,120,108,159,247,55,235,116,31,164,7,207,66,106,248,60,175,162,246,20,43,144,171,157,5,163,
13,77,112,126,126,43,12,92,185,10,6,174,104,3,235,56,197,94,140,32,244,187,57,244,255,129,251,221,248,119,120,69,65,203,38,208,151,125,243,14,212,254,117,170,248,102,154,159,70,10,12,207,105,129,147,107,
55,193,192,210,182,98,175,250,141,99,214,4,221,187,247,74,208,4,205,31,179,25,108,11,11,118,191,7,173,31,117,43,236,98,9,167,97,158,26,100,235,71,93,30,51,128,130,249,218,92,155,179,175,19,99,114,59,69,
255,22,182,44,124,0,12,60,78,118,144,22,236,238,129,121,123,63,9,178,122,76,136,2,245,191,31,5,39,55,10,131,75,86,142,121,108,96,73,111,111,239,193,157,59,119,254,152,239,244,29,120,126,236,44,120,99,
181,194,230,138,36,203,38,118,141,199,246,23,56,77,203,107,42,160,181,246,106,236,1,190,15,206,26,55,7,203,223,220,170,221,120,32,72,66,143,52,166,255,244,224,51,133,19,57,3,55,81,11,111,231,49,209,155,
161,19,52,174,72,50,109,98,72,19,238,162,212,238,228,63,151,142,21,207,46,173,52,118,82,173,68,41,80,12,156,88,59,22,204,18,30,69,29,174,179,53,205,12,5,232,155,73,97,114,22,83,151,190,144,23,210,71,21,
77,51,67,129,98,150,180,233,69,192,11,118,189,244,11,218,204,128,77,81,148,97,89,159,142,52,60,131,95,225,29,92,218,227,17,108,193,226,63,210,199,136,185,153,175,163,197,117,117,84,41,58,224,49,56,23,
85,144,122,159,49,5,162,3,238,183,232,230,59,54,66,243,186,118,168,109,193,223,72,80,25,189,8,154,170,83,128,52,203,229,224,220,161,189,112,234,253,215,96,232,231,67,213,93,127,129,218,225,129,211,14,
13,238,166,57,151,212,193,117,175,236,130,198,107,117,11,253,2,122,87,117,170,233,150,59,97,201,35,79,64,223,182,103,161,175,123,107,85,215,86,170,28,30,184,223,178,219,94,253,10,46,189,6,63,218,141,142,
228,159,165,45,187,146,230,19,47,71,109,77,186,6,90,183,60,139,173,253,7,232,255,242,3,48,169,52,132,25,211,195,1,247,91,119,237,252,69,30,108,234,194,201,33,237,198,39,206,116,188,154,212,128,8,250,229,
143,61,231,1,183,214,29,239,146,11,158,167,101,217,164,147,55,35,199,171,103,223,112,171,119,15,239,205,211,150,61,105,61,203,93,232,53,32,60,209,176,114,85,254,52,14,159,97,26,84,184,22,238,123,152,110,
152,93,206,215,124,25,182,250,48,93,80,229,27,207,192,51,216,88,2,192,113,69,23,9,112,68,90,217,63,10,2,187,36,77,211,67,129,72,128,151,13,133,186,30,28,227,7,143,28,128,190,151,158,134,116,83,51,182,
244,92,217,170,210,11,241,39,197,224,226,88,157,106,104,132,229,79,110,203,203,65,147,225,24,134,199,216,128,91,220,87,55,8,252,159,95,142,192,159,95,244,72,103,58,225,248,255,7,62,225,43,170,171,24,27,
240,192,13,122,107,41,57,151,204,210,177,60,16,165,232,72,13,195,107,225,245,151,22,157,137,62,27,59,240,224,155,186,29,25,6,106,245,154,74,21,240,126,94,140,67,160,29,241,191,97,148,86,137,172,36,212,
178,172,42,47,98,24,143,170,122,126,18,42,79,129,70,83,7,60,9,130,11,240,81,129,11,128,204,67,84,224,92,13,1,182,2,23,0,153,135,168,192,185,26,2,108,5,46,0,50,15,81,129,115,53,4,216,10,92,0,100,30,162,
2,231,106,8,176,21,184,0,200,60,68,5,206,213,16,96,43,112,1,144,121,136,10,156,171,33,192,86,224,2,32,243,16,21,56,87,67,128,173,192,5,64,230,33,42,112,174,134,0,91,129,11,128,204,67,84,224,92,13,1,182,
2,23,0,153,135,168,192,185,26,2,108,5,46,0,50,15,81,129,115,53,4,216,10,92,0,100,30,162,2,231,106,8,176,21,184,0,200,60,68,5,206,213,16,96,43,112,1,144,121,136,10,156,171,33,192,86,224,2,32,243,16,21,
56,87,67,128,173,192,5,64,230,33,42,112,174,134,0,91,129,11,128,204,67,84,224,92,13,1,182,2,23,0,153,135,168,192,185,26,2,108,5,46,0,50,15,81,129,115,53,4,216,10,92,0,100,30,162,2,231,106,8,176,21,184,
0,200,60,68,5,206,213,16,96,43,112,1,144,121,136,10,156,171,33,192,86,224,2,32,243,16,21,56,87,67,128,173,192,5,64,230,33,42,112,174,134,0,91,129,11,128,204,67,84,224,92,13,1,182,2,23,0,153,135,168,192,
185,26,2,108,5,46,0,50,15,81,129,115,53,4,216,10,92,0,100,30,162,2,231,106,8,176,21,184,0,200,60,68,5,206,213,16,96,43,112,1,144,121,136,10,156,171,33,192,86,224,2,32,243,16,21,56,87,67,128,173,192,5,
64,230,33,42,112,174,134,0,91,129,11,128,204,67,84,224,92,13,1,246,212,1,183,86,128,156,33,67,156,2,141,210,33,93,28,255,114,99,188,58,166,166,22,32,55,58,126,125,129,53,140,227,128,59,58,2,166,166,38,
246,232,99,7,158,27,28,240,130,112,255,61,31,123,48,73,125,64,208,247,229,206,253,29,123,8,177,1,55,78,202,115,190,238,242,171,160,121,93,59,164,155,154,193,230,114,177,7,148,196,7,24,236,5,169,133,167,
26,26,99,119,63,54,224,128,221,20,165,134,171,174,131,171,95,124,55,246,64,102,220,3,252,161,48,234,184,34,1,110,32,63,78,151,117,14,39,34,86,199,238,178,210,148,20,34,100,147,138,4,73,201,173,131,130,
72,238,62,58,148,31,167,131,155,22,28,41,136,116,252,147,145,130,103,106,166,162,2,161,150,101,214,205,143,201,127,239,249,218,123,128,7,118,10,150,22,21,163,153,129,39,130,222,241,220,161,189,249,232,
104,168,12,161,113,184,22,238,186,0,216,130,135,79,159,128,191,190,223,5,115,110,186,29,188,151,128,28,138,105,12,154,129,76,43,135,132,58,6,189,227,111,219,95,240,234,209,100,216,146,238,147,76,225,128,
211,67,9,44,58,118,224,209,245,176,250,195,195,80,215,186,124,146,174,232,101,149,20,248,229,229,103,224,204,167,59,188,211,22,103,243,97,82,120,224,126,43,39,232,189,247,172,128,121,119,108,130,185,235,
238,133,218,150,69,249,174,71,91,122,245,124,72,51,92,194,158,59,184,7,78,190,179,29,254,249,245,104,245,247,168,112,69,120,224,116,227,160,11,199,227,153,207,222,246,254,42,60,79,139,39,163,0,141,219,
33,186,113,254,200,104,128,211,29,125,232,222,7,23,191,155,231,15,82,123,18,10,160,142,222,156,40,34,216,228,65,116,192,233,110,8,61,152,85,82,86,211,244,83,128,150,101,67,220,173,92,237,44,158,85,59,
193,10,148,97,57,228,224,103,145,19,60,166,209,134,38,158,85,59,193,10,20,179,36,214,52,27,56,206,99,58,63,191,149,103,213,78,176,2,165,44,221,227,142,53,38,255,153,204,15,108,96,233,245,9,14,81,93,231,
10,12,92,185,138,103,129,88,211,24,222,195,75,7,150,182,193,240,156,22,94,164,118,2,21,32,134,3,87,180,21,123,222,227,100,50,153,61,88,122,36,56,99,241,211,221,201,181,27,131,172,30,19,170,192,201,181,
155,192,250,91,212,20,130,177,112,184,163,163,163,215,193,205,119,11,46,60,197,227,162,110,253,204,13,119,241,34,181,19,164,0,177,163,158,154,39,215,88,143,177,183,145,109,173,53,217,238,236,110,172,176,
102,172,146,133,5,187,123,96,222,222,79,198,138,212,154,246,10,16,236,63,110,110,71,63,217,111,20,44,124,155,233,204,220,66,141,155,198,112,220,255,48,214,56,230,33,52,251,41,159,79,6,47,188,15,250,54,
116,232,152,30,72,50,141,143,52,102,247,109,232,244,152,21,192,70,166,38,101,30,246,122,114,244,159,189,6,0,93,93,93,235,113,173,246,49,150,23,252,98,193,224,190,119,227,177,253,48,251,216,62,168,59,221,
7,233,193,179,144,26,214,31,37,94,76,254,244,81,133,214,217,180,244,162,33,152,254,248,152,237,251,54,130,223,62,239,238,236,236,252,60,240,181,0,56,21,250,208,105,47,110,110,80,73,143,137,84,160,31,97,
111,230,176,41,138,18,224,84,152,205,102,151,89,215,190,129,38,27,211,233,140,166,68,40,128,99,54,117,227,184,2,43,217,87,45,11,156,130,242,38,114,219,178,247,227,134,205,86,107,96,69,34,2,21,238,36,45,
189,112,255,234,233,204,150,204,91,193,152,93,44,73,69,224,65,69,15,124,54,123,35,230,219,141,181,183,1,56,139,177,171,192,95,55,64,125,80,71,143,23,69,129,161,252,62,136,123,220,255,90,218,67,235,236,
241,60,249,15,187,79,81,135,16,129,93,78,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::stopped_png = (const char*) resource_PlayerMainMenuComponent_stopped_png;
const int PlayerMainMenuComponent::stopped_pngSize = 3466;

// JUCER_RESOURCE: stop_png2, 3215, "../../Resources_LFS/Boutons/Exports/Stop.png"
static const unsigned char resource_PlayerMainMenuComponent_stop_png2[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,28,
233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,38,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,109,
108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,68,
70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,
62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,
116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,
102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,48,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,
111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,
58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,
110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,
121,68,97,116,101,62,50,48,49,57,45,48,54,45,49,50,84,48,57,58,48,54,58,57,56,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,56,46,52,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,
115,99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,55,230,182,237,0,0,8,2,73,68,65,84,120,1,237,154,193,79,84,71,28,199,
127,243,22,52,172,129,3,146,90,89,78,106,212,54,81,73,74,77,26,193,131,39,235,133,147,109,172,94,60,45,244,210,240,7,216,67,245,31,208,67,97,47,146,24,203,161,158,56,16,245,98,140,84,83,91,154,32,73,53,
1,53,49,97,247,82,93,15,38,16,179,236,155,206,247,193,148,217,199,66,129,247,158,238,238,239,55,9,188,153,121,243,230,253,126,223,207,251,205,188,153,183,138,54,145,70,70,70,122,76,179,126,165,117,31,
145,151,209,164,59,77,57,189,137,75,165,73,114,10,44,40,82,5,34,63,175,149,154,52,183,25,31,24,24,152,250,191,219,169,245,26,104,173,149,1,125,214,211,234,178,86,116,112,189,118,82,95,59,10,40,77,179,
190,210,151,12,248,91,74,153,82,149,84,21,120,46,151,219,175,125,61,102,218,31,175,114,141,84,213,186,2,154,30,171,148,58,159,205,102,95,132,77,93,3,124,120,120,248,148,25,42,110,153,134,237,225,198,82,
174,43,5,138,102,234,61,59,56,56,120,207,181,186,2,248,10,236,59,166,65,179,219,200,12,239,148,207,207,211,124,62,79,197,98,145,22,23,23,169,84,42,185,77,36,255,129,21,104,110,110,166,150,150,22,106,111,
111,167,174,76,23,101,50,25,50,195,120,216,138,146,129,126,218,133,254,95,139,149,97,252,15,115,69,69,100,231,13,228,233,39,211,244,238,221,187,112,103,82,174,33,5,90,91,91,169,251,88,119,0,62,100,86,
81,121,234,184,29,222,61,156,196,11,218,202,156,93,1,123,102,230,9,77,254,54,41,176,67,10,214,98,17,1,9,86,96,22,74,237,186,172,127,1,99,212,167,240,111,239,39,123,191,37,69,63,32,111,19,46,124,250,236,
153,45,202,177,78,20,248,231,245,107,242,253,50,237,217,243,233,170,197,138,186,166,166,166,158,78,76,76,252,29,68,56,121,116,121,245,44,153,249,58,47,176,93,65,234,44,143,64,5,67,55,97,121,141,50,214,
218,61,164,233,79,123,210,247,125,186,125,231,182,12,227,86,144,58,61,98,78,63,243,245,153,202,23,57,69,95,34,194,251,93,159,10,133,188,192,118,5,169,211,60,230,116,172,172,66,169,223,91,222,46,93,173,
158,159,175,28,10,86,207,72,174,222,20,192,50,218,77,96,237,105,82,93,110,101,241,109,209,45,74,190,142,21,192,158,73,101,242,50,24,210,247,186,149,216,84,145,212,24,10,132,89,226,163,23,128,87,124,245,
146,29,180,198,128,13,47,170,176,76,55,197,233,30,182,246,60,207,11,222,12,177,29,43,41,154,2,208,19,58,150,203,229,104,29,57,87,199,6,60,9,227,28,59,37,27,147,2,177,1,183,17,221,215,215,71,189,189,189,
212,209,209,17,60,157,85,54,244,99,50,189,113,187,129,102,136,234,185,185,57,186,123,247,46,189,122,245,42,54,103,35,3,183,145,189,99,199,14,186,122,245,42,29,62,124,56,54,227,184,119,212,211,211,67,231,
206,157,163,209,209,81,186,113,227,70,44,114,68,6,110,173,184,118,237,26,29,58,116,136,150,150,150,130,42,137,108,171,204,246,143,24,53,155,154,154,232,226,197,139,244,252,249,115,122,244,232,17,165,82,
169,72,115,122,36,224,54,186,119,239,222,29,192,134,129,48,72,96,111,31,114,248,74,4,144,133,14,224,216,250,142,146,176,44,219,118,2,92,164,35,71,142,4,71,204,59,2,59,144,34,182,127,86,227,3,7,14,4,125,
34,168,162,104,28,41,194,173,87,187,118,237,178,217,53,71,24,24,231,178,98,205,13,26,168,2,32,45,224,164,220,138,5,56,160,174,151,224,4,134,36,73,181,161,64,98,36,48,215,96,19,230,229,203,151,116,253,
250,117,106,107,107,139,60,255,212,134,100,241,91,129,160,192,92,157,78,167,105,104,104,40,184,65,212,161,123,61,43,19,7,142,15,241,15,31,62,92,239,254,82,31,82,192,2,15,85,199,86,76,12,184,181,16,191,
172,68,218,185,115,167,204,229,86,148,208,209,70,184,213,42,116,58,214,98,226,192,173,181,216,200,143,186,164,176,125,53,218,17,192,63,212,203,109,164,101,217,86,132,135,83,146,62,190,2,31,12,248,199,
119,85,44,128,2,2,156,217,115,32,192,5,56,51,5,152,185,43,17,46,192,153,41,192,204,93,137,112,1,206,76,1,102,238,74,132,11,112,102,10,48,115,87,34,92,128,51,83,128,153,187,18,225,2,156,153,2,204,220,149,
8,23,224,204,20,96,230,174,68,184,0,103,166,0,51,119,37,194,5,56,51,5,152,185,43,17,46,192,153,41,192,204,93,137,112,1,206,76,1,102,238,74,132,11,112,102,10,48,115,87,34,92,128,51,83,128,153,187,18,225,
2,156,153,2,204,220,149,8,23,224,204,20,96,230,174,68,184,0,103,166,0,51,119,37,194,5,56,51,5,152,185,43,17,46,192,153,41,192,204,93,137,112,1,206,76,1,102,238,74,132,11,112,102,10,48,115,87,34,92,128,
51,83,128,153,187,18,225,2,156,153,2,204,220,149,8,23,224,204,20,96,230,174,68,184,0,103,166,0,51,119,37,194,5,56,51,5,152,185,43,17,46,192,153,41,192,204,93,137,112,1,206,76,1,102,238,74,132,11,112,102,
10,48,115,87,34,92,128,51,83,128,153,187,18,225,2,156,153,2,204,220,149,8,23,224,204,20,96,230,174,68,184,0,103,166,0,51,119,37,194,5,120,50,10,104,173,147,233,88,122,221,146,2,77,91,106,29,161,113,115,
115,51,149,203,229,8,61,52,238,165,74,41,90,90,90,162,84,42,149,184,147,137,3,95,92,92,12,156,120,255,254,125,226,206,212,251,13,22,22,22,18,119,33,49,224,158,183,252,122,144,201,100,232,196,137,19,212,
214,214,70,190,239,39,238,80,61,222,192,70,120,58,157,78,220,252,196,129,239,219,183,143,174,92,185,146,184,35,141,118,3,60,4,73,164,88,128,111,100,28,94,214,100,238,222,28,58,232,152,244,60,30,11,240,
141,230,30,56,209,212,20,203,109,54,167,154,180,218,80,129,72,235,112,27,185,51,51,51,193,77,0,86,150,95,27,234,189,229,147,86,227,185,185,185,224,90,4,80,20,141,35,133,30,110,12,3,222,188,121,67,211,
211,211,212,221,221,29,188,152,217,250,45,123,39,23,84,40,0,29,237,232,56,54,54,22,156,195,144,143,37,220,118,83,36,224,238,77,135,134,134,232,230,205,155,132,183,114,73,241,42,48,58,58,74,247,239,223,
15,58,141,2,27,29,68,6,110,163,25,199,11,23,46,208,201,147,39,131,101,88,71,71,71,48,244,96,4,144,180,53,5,160,25,134,242,217,217,89,154,152,152,160,66,161,176,181,14,54,104,29,25,56,250,118,161,63,120,
240,128,240,39,41,62,5,162,206,219,174,37,177,0,71,135,22,58,54,92,226,52,208,53,150,91,222,70,58,180,141,43,197,6,28,6,193,48,251,86,25,151,129,210,79,188,10,96,89,86,177,129,139,143,28,146,26,67,129,
42,44,23,60,69,170,226,141,160,165,165,165,49,188,21,47,40,204,18,172,77,132,251,121,87,155,246,246,118,183,40,249,58,86,96,45,75,63,239,105,165,38,93,159,186,100,29,237,202,81,215,249,174,76,87,133,253,
96,141,57,124,220,173,237,236,204,80,107,107,171,91,37,249,58,84,0,12,171,108,130,141,123,217,108,246,47,227,207,242,70,173,201,96,89,213,125,172,187,14,93,20,147,93,5,192,16,203,58,155,148,166,217,129,
129,129,41,207,84,106,242,233,146,61,129,35,158,140,207,63,251,204,173,146,124,29,41,0,118,225,232,246,149,14,24,7,143,128,89,63,171,220,72,238,119,227,211,113,215,175,153,153,39,244,244,217,51,183,74,
242,53,174,0,96,31,61,122,172,210,74,77,143,179,131,217,175,16,220,152,195,17,250,90,121,234,59,147,45,186,45,113,97,95,111,159,204,233,174,40,53,154,199,156,13,86,107,96,27,166,42,165,206,7,35,185,177,
125,117,144,55,133,225,225,225,83,102,173,118,199,100,43,118,95,240,91,180,66,33,79,243,243,121,42,190,45,18,126,152,88,42,149,106,212,117,30,102,97,83,5,235,108,44,189,176,178,202,152,55,114,3,53,236,
124,73,147,62,61,56,56,120,207,158,88,211,98,5,250,45,211,64,22,228,86,165,250,60,22,13,236,179,46,108,184,177,6,56,42,115,185,220,126,237,107,124,113,175,152,211,113,78,82,29,40,96,230,108,12,227,102,
5,246,34,108,109,85,224,104,20,188,200,253,156,251,198,140,18,63,105,69,7,195,23,74,185,246,20,192,210,203,124,191,250,49,251,125,246,87,59,103,135,173,92,23,184,109,24,128,207,229,190,48,229,126,165,
117,159,89,169,103,204,80,209,105,202,201,255,136,218,26,33,199,106,10,44,44,127,7,241,243,43,187,165,227,88,103,87,107,232,214,253,11,191,221,102,200,240,118,106,173,0,0,0,0,73,69,78,68,174,66,96,130,
0,0};

const char* PlayerMainMenuComponent::stop_png2 = (const char*) resource_PlayerMainMenuComponent_stop_png2;
const int PlayerMainMenuComponent::stop_png2Size = 3215;


//[EndFile] You can add extra defines here...
//[/EndFile]

