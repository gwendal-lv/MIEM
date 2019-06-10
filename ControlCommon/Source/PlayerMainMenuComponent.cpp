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
                                ImageCache::getFromMemory (play_png, play_pngSize), 1.000f, Colour (0x00000000),
                                Image(), 0.600f, Colour (0x00000000),
                                Image(), 1.000f, Colour (0x00000000));
    playingImageButton.reset (new ImageButton ("Playing image button"));
    addAndMakeVisible (playingImageButton.get());
    playingImageButton->setButtonText (TRANS("new button"));
    playingImageButton->addListener (this);

    playingImageButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (playOn_png2, playOn_png2Size), 1.000f, Colour (0x00000000),
                                   Image(), 1.000f, Colour (0x00000000),
                                   Image(), 1.000f, Colour (0x00000000));
    stopImageButton.reset (new ImageButton ("Stop image button"));
    addAndMakeVisible (stopImageButton.get());
    stopImageButton->setButtonText (TRANS("new button"));
    stopImageButton->addListener (this);

    stopImageButton->setImages (false, true, true,
                                ImageCache::getFromMemory (stop_png, stop_pngSize), 1.000f, Colour (0x00000000),
                                Image(), 0.600f, Colour (0x00000000),
                                Image(), 1.000f, Colour (0x00000000));
    stoppedImageButton.reset (new ImageButton ("Stopped image button"));
    addAndMakeVisible (stoppedImageButton.get());
    stoppedImageButton->setButtonText (TRANS("new button"));
    stoppedImageButton->addListener (this);

    stoppedImageButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (stopOn_png, stopOn_pngSize), 1.000f, Colour (0x00000000),
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
    changeConnectionButton->setButtonText (TRANS("Change IP/port"));
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
                buttonThatWasClicked->setButtonText(TRANS("Cannot connect. Check and retry"));
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
    ipAddressTextEditor->setText(ipv4);
    udpPortTextEditor->setText(boost::lexical_cast<std::string>(udpPort));
}
void PlayerMainMenuComponent::textEditorTextChanged(TextEditor& editorThatHasChanged)
{
    if (&editorThatHasChanged == ipAddressTextEditor.get())
        TryParseIpAddress();
    else if (&editorThatHasChanged == udpPortTextEditor.get())
        TryParseUdpPort();
}
std::string PlayerMainMenuComponent::TryParseIpAddress()
{
    return TextUtils::TryParseAndBoldenIpAddress(ipAddressTextEditor.get());
}
int PlayerMainMenuComponent::TryParseUdpPort()
{
    return TextUtils::TryParseAndBoldenUdpPort(udpPortTextEditor.get());
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
            changeConnectionButton->setButtonText(TRANS("Change IP/port"));
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
                  virtualName="" explicitFocusOrder="0" pos="0Cc 8 16M 56" outlinecol="ff909090"
                  textcol="ff909090" title="Session"/>
  <TEXTBUTTON name="Load From File text button" id="2fe2a2c362ae91bd" memberName="loadFromFileButton"
              virtualName="" explicitFocusOrder="0" pos="0Cc 18 200 24" posRelativeY="ee702f61e13ff830"
              bgColOff="ff404040" buttonText="Load from .mspat file" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="Play image button" id="823680c6dd0a5a2e" memberName="playImageButton"
               virtualName="" explicitFocusOrder="0" pos="-16Cr 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="play_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="0.6000000238418579" colourOver="0"
               resourceDown="" opacityDown="1.0" colourDown="0"/>
  <IMAGEBUTTON name="Playing image button" id="26f63b20519a1739" memberName="playingImageButton"
               virtualName="" explicitFocusOrder="0" pos="-16Cr 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="playOn_png2" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <IMAGEBUTTON name="Stop image button" id="9385dfb43a053dc6" memberName="stopImageButton"
               virtualName="" explicitFocusOrder="0" pos="16C 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="stop_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="0.6000000238418579" colourOver="0"
               resourceDown="" opacityDown="1.0" colourDown="0"/>
  <IMAGEBUTTON name="Stopped image button" id="1b06a982c2d60ddc" memberName="stoppedImageButton"
               virtualName="" explicitFocusOrder="0" pos="16C 96R 62 62" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="stopOn_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <GROUPCOMPONENT name="Help group component" id="5beff948b653aff1" memberName="helpGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0Cc 72 16M 179M" outlinecol="ff909090"
                  textcol="ff909090" title="Help"/>
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
                   virtualName="" explicitFocusOrder="0" pos="0Cc 36R 336M 24" posRelativeY="5beff948b653aff1"
                   tooltip="http://miem.laras.be" textCol="ff3d6ed1" buttonText="Go to MIEM website: miem.laras.be"
                   connectedEdges="0" needsCallback="0" radioGroupId="0" url="http://miem.laras.be"/>
  <TEXTBUTTON name="Load Default button" id="d025c06799a8b40f" memberName="loadDefaultButton"
              virtualName="" explicitFocusOrder="0" pos="8Rr 18 144 24" posRelativeX="5beff948b653aff1"
              posRelativeY="5beff948b653aff1" bgColOff="ff404040" buttonText="Load default session"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Additionnal Status Label" id="bef02cde37454c59" memberName="additionnalStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="0Cc 24R 80M 24" textCol="ff909090"
         edTextCol="ff000000" edBkgCol="0" labelText="Additionnal Status information"
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
              buttonText="Change IP/port" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: lecture_png, 2530, "../../MiamCommon/Resources/Copies_temp/Lecture.png"
static const unsigned char resource_PlayerMainMenuComponent_lecture_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,80,0,0,0,80,8,6,0,0,0,142,17,242,173,0,0,0,9,112,72,89,115,0,0,11,19,0,
0,11,19,1,0,154,156,24,0,0,5,241,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,
61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,
58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,48,32,55,57,46,49,54,48,52,53,49,44,32,50,48,49,55,47,48,53,47,48,54,
45,48,49,58,48,56,58,50,49,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,
57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,
109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,
58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,
46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,
46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,
46,99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,
32,40,77,97,99,105,110,116,111,115,104,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,120,109,112,58,
77,101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,34,50,48,49,55,45,
49,49,45,48,57,84,48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,52,101,56,48,98,50,97,45,49,97,57,102,45,
52,55,54,55,45,56,53,99,53,45,50,56,53,51,97,98,55,101,56,50,57,102,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,
115,104,111,112,58,99,101,52,97,101,48,100,101,45,98,100,100,101,45,54,101,52,97,45,56,97,98,56,45,101,48,99,99,97,101,57,53,53,50,97,54,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,
99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,97,100,55,48,51,100,52,45,98,97,52,102,45,52,101,56,55,45,97,99,98,55,45,53,97,48,48,52,100,98,57,57,52,51,57,34,32,100,99,58,102,111,
114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,111,112,58,73,67,67,80,
114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,
108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,100,97,100,55,
48,51,100,52,45,98,97,52,102,45,52,101,56,55,45,97,99,98,55,45,53,97,48,48,52,100,98,57,57,52,51,57,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,48,57,84,48,56,58,53,49,58,
51,50,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,77,97,99,105,110,116,
111,115,104,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,
112,46,105,105,100,58,57,52,101,56,48,98,50,97,45,49,97,57,102,45,52,55,54,55,45,56,53,99,53,45,50,56,53,51,97,98,55,101,56,50,57,102,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,
49,45,48,57,84,48,56,58,53,49,58,51,50,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,
67,67,32,40,77,97,99,105,110,116,111,115,104,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,
111,114,121,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,
32,101,110,100,61,34,114,34,63,62,222,251,161,205,0,0,3,151,73,68,65,84,120,156,237,220,61,136,93,69,20,192,241,223,91,54,133,46,18,139,164,88,109,130,77,162,18,3,34,178,130,162,54,193,88,4,148,109,140,
136,213,70,176,16,181,177,208,78,133,128,160,177,209,132,109,68,48,130,44,22,41,220,96,37,42,186,126,32,68,81,34,24,72,227,7,104,17,21,69,48,230,89,156,60,72,132,176,119,222,184,111,238,188,157,127,185,
48,123,207,249,51,103,238,220,153,121,51,24,14,135,26,227,51,83,58,128,218,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,105,2,51,153,29,163,205,0,139,88,194,109,24,226,252,255,25,212,
4,153,17,249,124,140,101,172,136,124,58,51,72,252,148,187,10,199,112,13,158,195,187,248,35,229,31,244,144,57,236,197,51,248,1,7,240,123,215,198,41,2,7,56,142,95,68,239,59,151,20,102,255,153,21,189,112,
27,246,235,216,19,83,198,192,69,209,243,14,154,62,121,68,78,75,34,199,197,174,141,82,4,46,137,178,253,59,45,174,170,56,39,114,92,234,218,32,165,132,127,195,188,250,199,188,245,152,195,79,98,188,95,151,
212,105,204,180,203,35,114,236,220,171,82,4,214,58,85,25,135,206,185,182,137,116,38,77,96,38,77,96,38,77,96,38,77,96,38,77,96,38,77,96,38,37,5,62,37,225,155,179,175,148,20,184,15,135,241,30,246,20,140,
35,139,210,37,188,31,111,226,4,142,96,123,217,112,210,41,45,240,60,142,98,23,254,196,55,120,28,91,10,198,148,68,105,129,35,126,197,147,184,93,172,14,127,37,74,188,247,244,69,224,136,111,113,47,158,192,
75,120,7,59,139,70,180,14,125,19,56,98,21,187,197,158,203,7,120,17,91,139,70,116,25,250,42,144,88,249,62,140,27,197,34,231,41,60,162,103,49,247,42,152,203,240,179,16,119,15,30,192,23,184,179,104,68,23,
81,131,192,17,39,113,151,216,179,120,13,111,97,71,185,112,130,154,4,142,88,193,245,66,232,231,120,86,148,120,17,106,20,8,127,225,121,241,5,179,67,188,189,31,20,123,215,19,165,86,129,35,190,199,67,226,
155,250,49,124,132,91,39,25,64,237,2,71,172,97,1,175,226,109,49,70,206,79,226,193,211,34,144,216,138,124,93,124,22,254,136,47,241,232,70,63,116,154,4,22,97,154,4,14,240,176,152,112,207,227,38,188,178,
209,15,29,231,124,96,31,89,192,203,248,7,247,227,211,73,61,184,118,129,215,226,16,238,22,43,220,199,36,30,144,204,165,214,18,190,2,79,139,201,244,25,177,98,243,134,9,203,163,206,30,184,136,23,240,25,110,
17,2,139,81,147,192,61,98,156,187,90,188,44,222,47,26,205,5,106,40,225,237,98,217,255,132,216,63,185,89,79,228,209,111,129,91,196,202,244,215,226,204,222,46,33,178,87,199,236,250,90,194,251,196,146,254,
105,220,33,22,11,122,73,223,4,238,20,226,174,19,189,111,181,108,56,235,211,151,18,222,42,246,61,62,20,251,32,187,85,32,143,242,2,103,196,114,253,41,92,137,27,196,62,72,53,191,4,40,93,194,199,241,157,216,
239,56,89,56,150,177,40,41,112,85,188,36,86,10,198,144,77,202,239,68,206,138,73,236,102,224,172,142,185,150,30,3,171,167,9,204,164,9,204,164,9,204,164,9,204,164,9,204,164,9,204,36,69,224,102,146,221,57,
215,84,41,197,14,241,76,144,57,9,103,108,82,4,174,137,243,203,211,206,94,113,13,74,39,82,4,46,139,171,65,170,57,65,63,6,179,34,199,229,174,13,198,189,246,228,160,138,150,156,58,178,225,215,158,12,197,
165,52,219,68,57,223,103,58,198,196,57,145,203,39,34,183,3,18,246,151,83,111,46,226,210,171,159,22,46,252,173,87,27,61,9,140,58,208,154,9,93,253,212,248,15,155,105,110,183,33,52,129,153,52,129,153,52,
129,153,52,129,153,52,129,153,52,129,153,52,129,153,52,129,153,252,11,241,216,160,159,249,219,245,255,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::lecture_png = (const char*) resource_PlayerMainMenuComponent_lecture_png;
const int PlayerMainMenuComponent::lecture_pngSize = 2530;

// JUCER_RESOURCE: play_png, 5814, "../../MiamCommon/Resources/HighlyVisible/Play.png"
static const unsigned char resource_PlayerMainMenuComponent_play_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,28,
233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,36,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,109,
108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,68,
70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,
62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,
116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,
102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,
111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,
58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,
110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,
121,68,97,116,101,62,50,48,49,56,45,48,49,45,49,49,84,49,49,58,48,49,58,56,56,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,99,
114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,69,220,158,213,0,0,18,43,73,68,65,84,120,1,237,157,119,172,21,69,20,198,135,
106,71,108,88,98,193,130,17,81,81,140,45,118,132,63,236,137,196,70,196,130,45,152,216,163,160,18,43,42,70,77,20,18,21,197,46,214,216,91,98,1,69,141,45,98,137,13,187,98,65,80,177,160,134,250,156,223,121,
247,91,230,237,187,247,21,223,123,251,118,239,157,147,236,206,238,217,153,189,115,231,155,239,76,217,217,179,93,92,73,234,234,234,186,120,169,227,212,31,247,244,193,21,126,27,225,183,62,126,139,82,172,
18,152,230,179,59,206,227,57,149,108,123,60,187,250,227,165,28,119,41,41,66,176,71,122,221,100,93,227,186,151,197,126,179,202,96,103,113,151,215,18,232,230,51,214,53,200,220,115,254,248,0,15,238,34,129,
222,37,197,236,9,62,194,169,165,4,128,76,98,42,133,85,140,146,62,6,249,46,1,136,185,196,111,128,15,110,243,252,54,208,131,62,203,176,22,224,62,28,239,47,140,246,155,81,223,135,97,77,241,167,81,10,88,2,
144,182,187,223,22,248,173,151,7,125,161,76,250,16,175,120,222,111,128,221,136,209,75,151,46,117,108,81,242,89,2,158,172,206,131,233,186,117,235,102,97,42,151,2,253,93,31,103,144,0,135,246,189,253,6,170,
13,152,189,100,201,18,187,81,234,38,241,52,167,37,80,1,47,225,58,140,54,124,184,207,251,20,191,169,38,52,250,43,127,254,249,167,155,49,99,134,91,188,120,113,185,26,212,40,126,84,100,91,2,88,223,245,214,
91,207,13,24,48,160,210,15,11,240,217,116,217,167,251,13,89,88,31,52,220,143,29,59,150,78,64,220,10,80,6,253,251,247,175,155,51,103,142,1,232,153,222,16,200,210,25,12,215,112,139,208,76,188,204,194,77,
55,221,228,70,141,26,101,181,166,66,251,80,169,70,69,125,198,37,0,102,64,185,198,26,107,184,95,126,249,197,126,157,115,218,246,146,44,242,97,15,34,73,150,114,224,205,131,206,235,122,246,236,105,204,238,
222,189,123,100,120,1,24,46,188,238,189,247,94,195,112,225,194,6,70,219,78,26,116,208,168,9,62,166,85,136,191,255,254,219,249,4,118,76,237,137,146,255,18,16,118,111,189,245,86,197,204,54,2,92,49,23,45,
194,2,68,41,98,9,8,248,114,121,175,8,120,215,174,21,47,149,187,79,212,21,164,4,34,170,5,1,170,189,178,25,1,111,175,146,44,200,125,152,103,205,84,104,42,24,226,33,116,6,227,148,109,166,197,111,19,235,153,
253,34,64,167,65,214,56,177,169,142,70,102,25,172,129,31,202,148,225,128,221,187,119,111,183,255,254,251,187,213,86,91,205,61,250,232,163,238,135,31,126,176,98,246,99,125,155,186,173,129,50,239,220,191,
232,153,37,177,25,23,15,138,157,255,241,199,31,201,100,139,103,97,114,236,115,219,234,99,111,198,45,205,144,33,67,244,91,73,248,240,195,15,215,45,191,252,242,201,61,21,247,255,252,78,173,167,233,209,163,
135,149,227,233,167,159,110,229,219,162,137,151,246,174,126,152,108,181,211,207,63,207,19,88,103,19,58,26,231,31,114,200,33,142,73,158,17,35,88,77,229,31,215,249,7,1,113,26,215,138,162,67,118,29,222,75,
199,84,35,135,29,118,152,133,60,113,243,83,128,206,215,70,59,199,204,211,145,187,235,174,187,220,244,233,211,221,58,235,172,99,237,188,175,162,78,105,45,98,220,181,75,9,116,56,224,202,37,143,239,16,128,
12,5,54,163,99,219,125,247,221,221,79,63,253,228,206,61,247,92,139,162,199,177,234,216,133,233,226,241,255,43,129,204,0,151,9,47,151,77,0,101,131,237,200,85,87,93,229,62,249,228,19,183,205,54,219,36,149,
65,67,185,114,233,163,174,229,37,144,25,224,45,97,169,216,14,240,91,108,177,133,123,255,253,247,221,117,215,93,103,255,6,157,42,70,203,255,94,140,153,46,129,204,0,79,255,112,165,115,64,5,120,117,244,124,
143,211,158,239,238,179,207,62,9,219,99,219,94,169,244,154,215,231,2,112,218,111,1,172,44,211,145,67,104,199,121,168,255,194,11,47,184,251,239,191,223,58,123,232,16,197,177,147,184,107,81,9,228,2,112,
88,13,120,106,195,195,156,195,102,85,134,195,15,63,220,253,243,207,63,238,200,35,143,180,40,232,185,222,146,230,34,188,103,45,31,119,42,224,234,177,127,249,229,151,102,182,213,49,19,192,2,70,76,166,66,
0,176,95,209,225,94,124,241,69,183,230,154,107,154,5,224,62,209,204,171,180,154,14,59,21,112,153,102,38,100,214,90,107,45,247,212,83,79,89,110,1,88,215,194,236,171,83,7,192,131,7,15,118,115,231,206,117,
103,157,117,150,69,33,62,233,34,219,195,18,107,124,220,169,128,43,59,235,174,187,174,29,30,120,224,129,54,207,254,215,95,127,37,140,77,179,29,64,217,100,254,175,189,246,90,247,225,135,31,186,45,183,220,
210,76,63,149,65,150,66,247,143,225,178,18,200,5,224,2,149,25,184,103,158,121,198,245,234,213,203,221,114,203,45,150,75,181,237,50,255,202,186,216,78,90,214,99,127,244,209,71,238,154,107,174,177,203,113,
8,167,82,106,28,230,2,112,101,43,100,231,73,39,157,228,118,220,113,71,247,205,55,223,36,115,235,98,181,226,195,244,208,252,159,125,246,217,238,231,159,127,118,123,238,185,103,50,132,139,108,87,105,213,
135,185,2,156,44,133,236,124,251,237,183,221,198,27,111,236,46,190,248,98,203,173,88,157,102,59,29,54,116,180,227,125,250,244,113,47,189,244,146,155,50,101,138,85,20,85,18,42,70,148,212,123,100,121,41,
16,192,11,217,126,201,37,151,184,190,125,251,58,150,223,166,219,112,229,25,61,192,171,121,24,62,124,184,61,133,59,244,208,67,45,10,250,216,147,207,41,224,2,81,108,135,217,223,126,251,173,219,105,167,157,
28,166,30,145,169,22,192,74,35,38,195,246,229,150,91,206,61,248,224,131,142,81,0,11,46,212,243,87,28,165,169,165,48,247,118,14,166,3,188,64,162,51,183,202,42,171,52,59,132,147,153,39,189,95,120,225,126,
251,237,55,119,234,169,245,190,14,168,36,84,24,172,66,173,73,238,1,23,32,98,50,64,206,159,63,223,49,132,59,248,224,131,205,108,203,84,43,142,210,164,205,255,132,9,19,220,123,239,189,231,54,223,124,115,
171,68,97,179,161,52,213,30,22,6,112,1,161,9,22,206,159,120,226,9,183,242,202,43,187,219,110,187,205,46,99,5,176,6,0,25,74,216,217,27,56,112,160,155,57,115,166,187,242,202,43,45,138,154,141,90,97,123,
225,0,7,37,49,89,237,248,241,199,31,239,118,217,101,23,55,107,214,172,196,84,171,189,22,240,98,187,244,99,198,140,113,63,254,248,163,219,109,183,221,106,106,8,87,72,192,5,98,200,206,55,222,120,195,109,
184,225,134,238,178,203,46,179,203,97,27,174,248,132,210,147,150,25,190,87,94,121,197,221,121,231,157,13,102,239,212,95,8,211,85,203,113,161,1,7,4,204,119,216,22,95,120,225,133,110,179,205,54,51,143,21,
98,53,224,134,130,30,235,32,75,113,244,209,71,91,95,128,5,149,8,122,245,11,194,116,213,112,92,120,192,5,130,216,14,144,60,125,219,126,251,237,221,41,167,156,98,151,101,250,5,176,210,136,201,152,249,21,
86,88,193,249,37,211,54,181,187,234,170,171,86,237,16,174,106,0,7,68,152,14,240,2,242,198,27,111,180,23,31,158,125,246,89,195,24,189,218,112,129,78,8,155,85,25,246,221,119,95,247,251,239,191,39,158,47,
208,83,97,170,165,83,87,85,128,11,68,129,7,144,254,133,10,183,223,126,251,185,97,195,134,185,127,255,253,55,49,213,138,163,52,170,36,50,255,55,220,112,131,123,231,157,119,220,38,155,108,146,244,252,101,
41,148,166,136,97,85,2,46,32,96,179,128,124,228,145,71,220,74,43,173,100,235,223,185,142,30,112,155,26,194,13,26,52,200,154,7,117,4,213,108,20,153,237,85,13,56,192,138,201,176,19,112,143,57,230,152,100,
253,187,76,117,218,204,167,59,123,222,147,149,251,238,187,239,220,206,59,239,108,247,8,59,137,252,70,145,164,234,1,23,24,33,59,95,125,245,85,243,107,54,126,60,222,70,151,13,213,42,177,157,180,27,108,176,
129,123,253,245,215,221,173,183,222,106,105,208,33,178,32,118,82,128,93,205,0,14,22,0,26,178,243,188,243,206,179,105,86,166,91,211,172,22,118,232,177,4,178,20,35,71,142,180,169,221,131,14,58,200,162,160,
47,210,16,174,166,0,23,136,98,59,64,126,254,249,231,110,187,237,182,115,167,157,118,154,93,150,233,79,179,93,76,198,252,211,23,120,252,241,199,147,169,93,53,9,138,163,223,201,99,88,147,128,3,4,128,2,188,
64,154,56,113,162,91,125,245,213,109,253,187,216,46,32,67,224,194,33,28,15,112,88,127,119,194,9,39,88,20,216,174,126,65,152,38,79,199,53,11,184,64,144,169,6,200,121,243,230,185,161,67,135,218,155,174,
248,168,147,169,86,28,165,81,37,81,59,206,35,91,166,118,55,218,104,163,164,231,15,240,121,148,154,7,92,160,192,102,1,249,208,67,15,185,21,87,92,209,221,115,207,61,118,25,61,215,211,102,62,52,255,44,206,
96,253,29,83,187,136,154,13,172,69,158,36,2,30,160,33,38,3,36,128,225,164,96,239,189,247,182,133,145,176,29,240,210,102,94,230,95,108,103,57,214,215,95,127,237,118,216,97,135,70,157,196,224,167,58,237,
48,2,94,166,232,67,118,178,32,18,39,5,90,2,173,54,188,18,219,73,219,183,180,254,14,231,196,136,42,131,44,72,153,159,204,76,21,1,175,80,212,0,202,166,182,248,156,115,206,177,245,239,188,244,0,112,48,91,
64,234,22,232,136,47,75,113,242,201,39,59,124,205,51,181,139,160,87,191,64,105,178,14,35,224,205,148,184,216,14,200,31,127,252,177,219,122,235,173,221,153,103,158,105,169,194,54,60,188,141,152,140,249,
103,253,221,211,79,63,237,30,123,236,49,235,23,168,73,80,156,48,93,22,199,17,240,22,148,50,76,135,157,2,9,39,5,188,200,56,117,234,212,100,194,70,64,134,183,147,249,71,167,245,119,199,30,123,172,69,225,
126,178,30,166,200,104,23,1,111,69,65,3,18,102,27,32,127,253,245,87,135,147,2,214,191,3,182,76,53,113,66,81,37,145,249,191,253,246,219,29,83,187,235,175,191,126,210,36,112,207,172,36,2,222,202,146,134,
237,0,44,32,239,187,239,62,51,213,56,43,64,208,115,157,120,161,132,230,127,215,93,119,181,245,119,231,159,127,190,69,33,110,86,160,71,192,67,84,90,113,44,38,195,108,28,22,225,164,0,198,195,124,116,0,40,
86,235,182,232,66,253,229,151,95,110,239,185,115,61,93,65,148,166,189,195,8,120,27,75,20,54,11,72,218,116,218,246,235,175,191,222,238,90,169,141,22,219,169,40,131,253,123,238,23,92,112,129,197,151,239,
186,54,102,169,201,228,17,240,38,139,167,101,23,97,39,155,0,62,227,140,51,220,218,107,175,237,94,123,237,53,187,129,172,65,120,55,42,137,154,5,22,81,34,84,158,142,150,8,120,71,151,112,206,238,31,1,111,
7,64,100,210,213,102,51,108,227,61,117,58,103,136,152,28,254,148,172,2,186,187,239,190,219,46,169,167,31,198,107,239,227,122,71,168,237,125,215,26,186,31,32,201,20,211,30,243,182,42,110,198,16,42,128,
204,124,88,36,210,147,22,119,100,227,198,141,179,203,77,121,171,12,211,183,229,56,50,252,127,150,158,88,11,216,116,182,24,158,225,89,10,176,209,133,109,186,126,66,172,86,37,160,179,198,227,88,4,43,145,
133,68,134,183,178,148,1,6,192,196,106,134,99,120,130,150,57,166,131,166,227,240,214,98,53,58,58,115,71,28,113,132,251,254,251,239,45,10,247,164,50,100,33,145,225,173,40,101,88,13,48,128,13,147,97,52,
62,227,66,179,46,230,235,182,234,161,139,213,199,29,119,156,189,192,8,216,210,101,5,54,121,138,128,11,153,38,66,24,8,144,2,143,97,23,223,247,164,205,150,153,46,199,106,42,134,42,0,107,224,88,11,119,199,
29,119,216,47,161,87,39,175,137,159,110,247,75,209,164,55,83,164,176,16,96,0,22,95,112,15,60,240,128,219,106,171,173,44,85,104,166,195,219,80,49,0,148,74,192,154,55,204,55,238,200,16,89,3,85,158,48,93,
22,199,145,225,21,74,25,86,179,137,133,87,95,125,181,249,130,3,108,192,162,2,200,36,235,22,232,136,47,86,79,154,52,201,124,206,9,108,244,106,251,149,38,235,48,50,188,76,137,139,213,92,218,107,175,189,
204,139,51,51,103,8,128,149,51,223,98,59,105,89,219,198,39,63,112,59,134,232,126,157,197,106,203,68,105,23,25,30,148,134,152,41,240,152,16,153,54,109,154,77,147,2,52,12,78,131,141,46,100,251,69,23,93,
100,190,229,0,59,109,37,130,159,234,180,195,200,240,82,209,171,109,229,20,223,110,172,88,197,21,40,2,51,211,64,163,87,197,224,248,205,55,223,116,184,247,198,189,24,34,86,219,73,142,118,53,207,112,177,
26,6,227,203,13,159,110,204,150,1,182,218,91,197,17,110,50,205,128,138,156,120,226,137,246,162,33,96,163,11,219,126,165,201,75,88,179,128,3,10,224,8,60,124,184,225,203,13,159,110,50,211,229,88,77,37,80,
5,120,242,201,39,109,205,218,228,201,147,13,79,244,176,158,244,121,149,154,52,233,50,183,128,211,175,95,63,99,244,182,219,110,107,24,133,102,58,4,141,138,1,160,84,2,62,172,199,210,38,220,134,33,106,14,
84,121,194,116,121,59,174,41,134,167,59,81,248,106,251,236,179,207,28,96,139,213,50,211,2,10,61,149,64,172,198,39,28,190,225,4,54,122,153,126,165,201,115,88,51,12,23,171,1,3,223,108,180,211,114,204,15,
96,229,204,183,216,78,90,124,192,49,212,226,29,50,68,247,43,2,171,45,195,165,93,213,51,92,204,4,60,24,142,79,54,124,179,1,54,58,24,156,6,59,205,118,30,95,226,3,14,176,211,86,34,44,204,34,28,87,53,195,
213,182,2,4,62,216,24,106,225,158,11,129,153,105,243,141,94,172,230,120,198,140,25,54,68,251,234,171,175,56,77,88,109,39,5,221,85,37,195,197,106,76,53,62,215,152,218,196,7,27,96,171,189,85,28,225,38,211,
172,74,128,143,55,124,189,1,54,58,152,77,101,40,186,84,21,224,128,2,56,2,111,212,168,81,230,115,13,223,107,8,250,180,249,70,79,37,80,5,192,167,27,31,181,199,199,27,130,94,166,223,20,5,223,85,141,73,7,
104,128,97,219,116,211,77,173,83,134,219,45,4,29,215,5,170,48,163,2,160,163,18,224,195,237,168,163,142,114,184,247,66,212,28,168,242,40,77,209,195,194,51,60,221,137,186,244,210,75,221,23,95,124,225,0,
59,221,249,18,88,233,161,22,43,86,120,86,45,176,169,4,50,253,74,83,45,97,161,25,46,86,3,6,62,212,24,106,225,94,11,1,176,74,230,27,61,105,249,86,57,67,45,222,245,66,116,191,106,99,181,253,185,210,174,144,
12,151,105,198,84,35,248,78,195,135,26,96,163,107,106,168,165,74,128,143,54,62,98,15,216,105,43,81,42,155,170,12,10,199,112,181,173,160,129,175,52,214,148,97,142,17,152,9,75,211,162,54,28,61,223,36,135,
213,204,176,33,98,181,157,212,192,174,48,12,23,171,49,213,76,109,242,224,66,235,196,212,222,42,142,112,75,183,225,248,98,99,26,21,176,1,186,90,134,90,250,191,45,9,115,207,112,64,1,72,153,111,30,69,222,
124,243,205,201,127,131,213,50,211,137,210,31,132,109,56,139,253,97,53,110,185,144,240,126,166,168,161,93,174,25,14,11,213,163,198,7,26,139,12,4,182,42,64,154,213,234,112,81,9,22,44,88,96,64,179,216,31,
176,85,49,20,167,134,112,78,254,106,46,1,79,119,162,88,54,196,58,49,190,69,154,54,211,250,39,232,97,181,42,128,218,118,124,174,33,232,101,250,149,166,22,195,220,153,244,176,19,133,175,51,134,90,125,189,
27,44,36,236,124,153,162,180,147,249,134,193,115,230,204,49,86,191,252,242,203,118,85,247,171,101,86,135,101,149,43,134,135,157,40,76,55,223,26,5,108,128,134,193,128,23,10,186,176,13,231,91,226,172,46,
5,236,180,149,8,211,213,242,113,46,24,46,51,140,127,83,124,154,225,47,5,119,87,8,128,166,129,70,47,182,3,44,223,14,167,83,134,91,45,68,172,182,147,184,107,80,2,185,96,56,51,94,8,67,45,124,154,1,182,218,
91,85,6,229,58,221,134,243,205,112,94,14,0,108,226,134,86,66,105,98,184,172,4,58,149,225,234,53,211,139,158,59,119,174,249,71,33,107,161,153,94,150,213,101,172,70,135,63,21,150,5,243,142,23,194,189,84,
73,76,17,119,101,75,160,83,1,135,141,8,79,183,16,153,233,52,171,169,0,232,48,213,128,138,79,20,222,199,70,0,154,116,17,108,43,142,102,119,157,10,184,114,151,54,211,210,19,2,164,44,1,47,242,225,225,88,
158,18,168,4,17,232,176,180,154,63,206,5,224,234,81,135,217,21,171,1,27,223,103,152,111,222,199,70,100,190,137,19,165,117,37,144,139,78,91,152,101,216,142,137,150,89,199,231,25,190,207,0,91,21,35,178,
58,44,177,214,29,231,130,225,202,178,218,112,218,234,79,63,253,212,88,253,193,7,31,216,229,56,212,82,41,181,45,204,140,225,0,86,73,210,109,248,232,209,163,93,255,254,253,29,96,139,213,84,134,40,109,47,
129,14,103,56,96,34,122,254,172,158,185,178,46,86,115,206,122,113,38,80,102,207,158,109,151,213,86,43,110,12,219,94,2,29,206,112,49,147,183,50,17,64,100,70,77,61,109,152,79,231,139,161,214,30,123,236,
97,96,163,163,98,196,182,186,237,0,167,239,208,225,128,195,112,13,159,248,96,12,194,171,184,114,36,171,143,192,202,27,33,113,169,36,178,12,233,12,199,243,182,149,64,135,155,116,178,167,225,19,31,140,97,
218,148,33,22,76,231,124,230,204,153,246,15,100,190,21,183,109,127,43,166,174,84,2,153,0,174,31,199,76,207,159,63,63,249,96,107,168,143,230,91,165,209,177,97,166,128,203,188,171,199,206,57,64,71,243,221,
177,32,135,119,207,20,112,126,24,147,29,205,118,8,65,182,199,29,222,105,203,246,239,196,95,107,174,4,34,224,205,149,80,149,93,175,8,120,52,187,85,134,116,233,239,84,4,92,227,228,234,252,219,213,253,175,
210,179,153,225,191,109,4,184,34,243,250,142,28,211,169,87,29,38,140,199,249,43,1,97,199,114,238,74,210,197,15,137,228,84,140,208,150,160,104,126,155,175,227,242,82,61,162,233,78,59,137,187,220,149,128,
102,39,241,227,174,101,95,64,171,74,224,51,188,200,111,61,24,3,79,7,116,47,11,235,131,134,251,177,99,199,82,17,226,86,128,50,240,79,24,235,252,186,124,3,208,87,128,134,64,150,206,96,248,112,15,232,20,
191,241,209,172,178,227,114,62,137,140,131,27,38,73,130,26,227,163,71,201,67,9,208,193,230,213,231,1,3,6,84,202,14,75,131,104,190,103,155,9,247,160,243,150,93,111,191,233,130,63,172,23,153,119,157,199,
48,223,37,80,1,47,225,58,76,128,15,241,127,131,231,151,92,64,103,122,31,154,196,217,49,149,68,62,67,79,88,179,188,21,250,89,178,220,239,122,235,60,8,147,238,67,235,187,141,247,127,103,180,223,0,29,105,
212,131,175,87,199,125,129,74,64,96,47,240,121,238,229,113,94,152,128,234,79,198,120,229,68,191,161,99,35,50,224,171,23,239,15,163,20,160,4,192,11,236,8,233,147,209,92,247,3,108,35,183,63,161,167,238,
207,187,24,176,254,120,164,87,77,246,91,104,214,117,3,162,71,201,111,9,176,112,48,33,177,63,126,206,111,7,120,108,23,121,92,187,250,112,105,2,106,10,116,62,5,112,133,223,70,248,173,143,223,162,20,171,
4,166,249,236,142,243,0,79,37,219,2,155,227,255,0,55,41,170,175,73,28,10,139,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::play_png = (const char*) resource_PlayerMainMenuComponent_play_png;
const int PlayerMainMenuComponent::play_pngSize = 5814;

// JUCER_RESOURCE: playOn_png, 6097, "../../MiamCommon/Resources/HighlyVisible/PlayOn.png"
static const unsigned char resource_PlayerMainMenuComponent_playOn_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,28,
233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,36,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,109,
108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,68,
70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,
62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,
116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,
102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,
111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,
58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,
110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,
121,68,97,116,101,62,50,48,49,56,45,48,49,45,49,49,84,49,49,58,48,49,58,52,53,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,99,
114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,125,38,139,223,0,0,19,70,73,68,65,84,120,1,237,93,11,156,77,229,22,95,103,60,
42,202,35,143,194,173,46,174,18,149,30,55,228,145,200,43,70,72,165,72,137,48,163,146,110,137,74,229,74,232,73,42,227,217,67,73,47,228,153,71,145,68,244,146,138,148,171,174,36,164,146,46,201,12,51,119,
253,215,236,181,231,155,61,231,152,51,154,57,103,159,115,190,53,191,253,250,246,247,237,249,246,250,127,255,245,173,181,207,62,235,4,200,145,172,172,172,0,75,22,14,121,191,36,111,70,242,210,131,151,202,
188,88,137,45,13,44,231,238,142,96,60,151,161,219,140,103,18,239,103,98,63,224,20,152,96,247,226,178,41,122,14,231,89,14,241,34,131,65,142,236,202,175,26,40,198,29,75,50,58,183,132,247,147,25,220,12,5,
61,224,97,246,56,174,112,171,211,0,32,163,49,6,133,12,12,167,220,110,252,173,1,16,243,48,47,0,31,184,237,225,165,30,131,190,77,176,86,192,121,59,154,79,12,230,69,168,207,91,115,164,240,161,149,24,212,
0,72,91,156,151,131,188,148,97,208,211,213,164,183,228,130,165,188,0,236,60,140,206,202,202,196,60,192,167,172,248,81,3,89,60,219,6,240,151,148,36,91,79,31,21,244,117,12,248,249,10,56,104,95,142,23,0,
158,139,217,153,12,118,82,32,87,145,231,122,246,208,79,26,8,129,151,226,218,5,115,120,55,238,240,116,94,116,36,228,233,255,129,244,253,180,117,215,38,202,204,228,42,1,25,35,121,234,216,130,232,105,0,214,
183,92,233,138,84,173,98,205,80,157,80,192,119,194,190,167,56,181,130,218,236,89,239,167,209,188,53,83,66,93,200,150,251,72,3,85,43,84,167,33,93,39,211,9,165,202,243,20,156,201,220,116,45,179,238,156,
12,134,43,208,216,10,125,213,44,44,95,255,6,77,91,58,74,110,73,204,186,101,183,143,224,205,221,149,172,76,246,179,248,239,248,227,202,210,83,55,47,147,147,58,183,59,53,51,120,91,2,12,207,37,168,164,115,
246,203,203,30,147,115,73,73,197,216,156,179,167,175,67,35,87,11,123,224,23,13,20,47,86,130,246,29,216,75,107,54,45,162,134,181,219,10,102,197,146,114,67,172,84,207,233,179,67,248,131,25,7,232,208,97,
12,10,198,153,71,143,21,255,107,64,141,245,119,59,54,132,236,108,94,192,157,170,135,225,160,89,137,73,13,40,240,193,58,31,18,112,99,194,15,214,206,150,197,168,6,66,2,30,163,247,99,187,157,143,6,44,224,
249,40,40,222,78,231,118,225,34,112,119,152,42,52,10,64,248,135,120,209,74,228,52,16,81,192,1,52,64,62,108,128,140,103,192,16,132,131,86,138,94,3,17,5,28,96,151,58,230,4,170,87,163,9,149,58,182,12,125,
186,121,57,237,217,247,147,220,165,27,235,23,253,61,39,244,127,136,8,224,48,227,48,221,117,78,107,64,131,174,26,239,42,252,186,75,239,162,79,54,47,163,137,11,238,165,140,67,233,82,174,117,221,74,118,167,
80,53,80,228,78,27,76,182,206,211,10,54,30,232,104,156,127,65,173,22,52,241,182,85,212,168,78,123,185,49,212,133,233,87,83,95,168,119,107,47,150,251,163,208,162,208,7,76,53,164,254,25,173,100,139,71,180,
120,4,168,143,252,96,230,193,234,62,237,134,211,221,215,76,161,178,165,43,200,60,47,143,120,157,182,210,208,174,10,69,3,69,206,112,237,101,185,227,43,201,174,215,57,3,155,81,134,191,211,255,118,30,141,
77,93,66,237,234,223,32,117,49,56,192,116,203,118,213,226,95,223,70,12,112,53,225,193,186,172,160,130,237,144,171,46,30,64,35,123,205,164,83,42,213,114,7,131,134,114,193,218,219,178,240,53,16,49,192,157,
79,94,143,216,51,101,59,128,175,114,226,223,105,248,13,175,80,183,22,119,74,27,49,253,150,237,71,212,95,56,39,35,8,120,56,221,193,176,8,136,211,166,142,94,171,243,175,149,207,119,235,156,90,63,135,237,
118,110,15,79,153,65,106,249,2,112,153,193,29,115,174,125,132,35,7,193,60,142,15,245,7,93,157,70,169,201,163,196,217,147,207,230,249,156,214,145,138,118,21,150,6,124,1,184,204,225,12,176,206,225,102,207,
225,229,43,219,235,215,110,77,147,6,174,166,6,252,225,62,68,66,56,62,111,157,58,83,99,71,222,143,42,224,96,54,228,167,223,126,224,55,53,126,115,159,177,43,192,218,117,101,50,6,4,6,64,74,242,67,116,215,
213,19,152,249,229,196,2,224,58,26,254,105,27,187,13,174,129,168,2,174,166,121,195,214,53,116,235,51,151,210,250,45,43,165,151,0,88,207,153,221,86,167,14,0,159,121,234,133,60,183,191,67,109,254,121,157,
84,65,125,180,179,108,55,53,150,119,63,170,128,107,119,240,138,45,100,236,236,129,52,102,214,0,250,51,253,15,151,177,121,216,46,144,6,92,243,127,205,37,183,211,136,158,175,81,213,10,53,196,196,11,219,
157,249,95,175,111,183,57,26,240,5,224,153,206,123,116,120,2,247,249,183,171,40,117,92,83,90,241,249,108,233,165,176,157,77,185,154,127,237,186,203,118,62,135,247,177,31,186,241,117,234,202,224,67,108,
8,167,90,202,187,245,5,224,218,45,188,139,165,15,88,158,95,50,130,134,191,116,61,253,188,247,71,247,217,186,215,169,115,157,61,54,231,144,182,108,222,159,236,191,148,206,56,229,2,25,32,150,237,170,217,
156,173,175,0,71,183,76,118,126,183,115,3,13,154,220,129,222,92,61,81,122,236,178,154,225,52,69,60,121,46,195,60,94,166,212,137,252,50,254,36,234,215,254,33,25,40,58,72,212,241,51,219,37,226,190,239,0,
7,8,96,166,201,206,57,171,39,209,157,147,146,233,91,126,253,86,88,205,107,5,82,65,67,185,25,194,53,60,179,45,77,28,184,138,46,60,163,165,84,209,16,78,235,39,234,214,151,128,43,24,202,118,48,251,151,223,
119,208,131,211,175,39,152,122,136,154,254,60,78,157,227,176,129,237,197,139,149,164,254,29,30,150,207,224,75,243,11,23,234,249,39,50,219,125,13,56,128,5,211,5,120,7,72,56,115,41,79,54,205,63,132,195,
3,27,231,15,47,94,60,125,203,114,106,121,254,53,184,100,246,3,27,190,94,34,134,112,190,7,92,16,114,64,194,62,204,246,193,140,63,36,132,27,55,251,95,188,127,32,236,16,174,123,139,65,252,129,204,12,58,185,
252,105,50,136,204,105,67,255,79,188,111,99,6,112,5,66,31,176,224,120,221,150,21,204,246,38,180,242,139,57,114,58,223,16,142,25,127,74,165,211,105,84,239,89,116,101,211,236,204,38,58,109,36,10,219,99,
14,112,32,171,243,182,206,227,207,46,30,78,35,166,247,164,95,255,183,43,39,132,115,66,53,25,9,188,114,157,61,167,188,125,131,158,52,38,117,49,213,170,118,174,107,250,245,122,218,38,30,183,49,9,184,2,97,
178,115,203,142,47,232,142,137,237,104,238,7,217,223,101,215,80,13,102,219,20,45,71,91,60,225,187,231,218,169,116,211,101,255,150,1,129,50,72,60,59,117,49,13,56,192,81,199,76,217,57,123,85,26,13,158,210,
81,50,86,184,172,118,128,68,125,8,202,81,95,45,69,227,186,201,52,129,67,56,188,80,9,137,231,16,46,230,1,23,132,120,165,108,7,144,248,244,109,216,139,221,105,218,219,70,50,3,7,72,173,143,173,50,25,126,
65,201,226,199,208,45,29,31,165,219,187,140,163,227,142,57,62,110,67,184,184,1,28,0,130,237,2,188,19,194,45,255,236,13,234,255,84,51,250,252,187,85,56,45,0,107,44,46,5,206,202,124,96,115,78,245,198,52,
254,214,21,212,252,220,43,229,172,176,61,142,66,184,184,2,92,65,84,83,13,32,15,28,220,71,99,102,14,160,167,231,12,162,244,67,7,67,135,112,206,32,209,121,252,250,150,119,211,176,30,211,169,82,217,106,113,
21,194,197,37,224,10,188,25,194,225,27,46,41,99,27,211,170,13,243,229,116,56,33,220,105,39,213,166,71,250,204,165,43,154,164,74,27,157,54,224,3,196,170,196,53,224,0,197,101,59,51,24,38,127,202,91,15,208,
200,25,189,233,183,253,63,231,31,194,57,206,94,135,134,55,209,227,253,22,82,205,42,103,203,53,112,29,117,18,99,13,248,184,7,92,1,49,217,185,121,251,103,116,123,90,27,90,176,246,121,57,173,161,26,128,52,
69,60,121,46,67,219,19,79,56,137,134,118,127,158,122,181,185,95,170,168,233,87,199,207,108,231,231,253,132,1,28,32,0,80,147,157,111,172,124,138,134,76,237,76,223,255,244,181,132,106,48,213,10,164,130,
230,13,225,154,158,221,145,38,220,246,62,157,87,179,153,84,137,181,16,46,161,0,87,16,149,237,96,240,174,61,223,211,3,211,186,209,244,101,143,202,105,101,181,151,237,202,100,248,5,199,148,56,142,6,116,
126,130,110,235,60,134,247,75,197,84,8,151,144,128,3,89,0,42,192,59,222,249,219,159,190,66,183,60,221,156,54,110,93,155,195,118,207,227,89,180,51,67,184,115,107,94,204,108,95,73,23,159,221,9,167,178,31,
216,240,245,252,236,212,37,44,224,130,144,3,18,246,1,228,254,63,127,167,71,95,239,79,227,231,13,150,28,117,40,131,168,227,39,7,188,114,217,238,56,117,55,182,185,143,238,235,254,2,85,40,83,197,247,33,92,
194,3,174,32,154,33,220,71,95,191,77,253,198,54,162,15,54,46,148,211,18,194,49,219,189,102,222,52,255,53,170,156,69,143,245,157,79,29,47,234,35,109,116,218,240,27,219,45,224,138,56,111,149,201,0,18,128,
77,90,120,31,61,252,106,95,250,253,143,95,197,2,136,83,231,49,243,40,147,114,135,237,157,26,167,208,163,125,230,81,245,147,235,202,0,49,157,68,227,95,69,109,215,2,30,68,245,38,59,55,109,251,132,110,27,
223,138,22,125,244,162,212,212,57,60,20,219,209,182,98,217,170,116,255,117,211,232,134,86,247,72,27,148,65,116,42,144,131,40,173,44,224,33,20,15,64,77,118,190,186,98,44,221,251,220,85,180,253,231,45,2,
156,201,106,189,4,202,196,204,59,0,95,82,175,11,165,13,88,73,231,212,104,44,85,252,16,194,89,192,21,173,16,91,151,237,108,230,127,252,229,91,26,250,252,213,52,99,249,227,82,219,156,195,205,230,202,100,
248,5,199,150,44,69,183,95,49,142,6,116,122,156,74,150,56,54,234,33,156,5,220,68,42,196,190,112,157,89,171,64,46,249,228,101,254,46,92,11,250,234,251,143,156,25,156,31,216,120,230,118,92,74,205,63,246,
207,251,199,37,146,188,168,201,89,29,112,232,134,112,114,16,193,149,5,188,0,202,134,73,22,179,205,225,26,242,146,63,242,90,10,77,152,127,175,128,29,110,8,215,187,237,48,126,203,230,89,42,207,143,106,221,
185,157,175,26,41,177,128,23,80,211,96,187,25,194,173,229,100,244,125,57,132,91,187,105,177,92,41,156,16,174,86,181,122,244,4,127,24,147,220,160,151,180,193,53,49,144,34,33,22,240,163,212,178,27,194,49,
219,145,176,104,194,252,123,132,241,96,62,216,30,202,169,51,203,187,52,189,89,190,231,142,46,0,244,72,136,5,252,47,106,89,216,46,240,6,100,78,199,220,190,244,211,25,114,85,56,117,193,68,157,61,12,20,124,
207,189,67,195,222,82,77,115,215,5,107,83,88,101,193,123,84,88,87,79,144,235,136,83,199,12,85,128,241,91,49,136,221,55,111,95,47,26,80,107,96,170,3,76,87,39,176,81,221,236,44,148,193,28,63,179,77,97,236,
91,192,11,67,139,49,116,141,136,36,215,141,33,125,28,85,87,193,86,136,122,221,200,45,135,116,99,42,202,100,61,198,86,230,108,36,66,224,166,171,55,100,63,179,199,220,127,164,4,134,102,251,163,221,183,128,
31,173,230,156,118,0,73,77,49,230,99,124,91,21,105,198,32,24,0,106,230,157,234,185,202,3,220,22,31,199,234,15,1,22,53,216,248,231,22,112,19,137,2,236,131,181,152,155,1,54,156,45,36,7,110,80,187,141,92,
65,28,57,254,225,87,47,216,234,137,107,249,204,149,207,208,252,181,207,74,27,88,9,61,95,128,110,20,184,170,5,188,128,42,19,103,139,193,84,86,35,103,92,95,6,219,124,240,162,251,230,165,77,182,195,153,75,
155,127,55,237,225,239,194,65,34,5,54,254,151,5,28,90,8,83,148,213,89,204,106,152,109,152,111,152,113,8,6,0,128,246,206,215,242,116,142,173,129,178,122,234,162,97,244,254,151,243,164,13,202,48,16,34,193,
108,249,135,188,178,128,171,38,142,176,21,167,140,127,119,85,195,171,214,23,116,163,107,155,223,33,45,20,172,160,172,118,6,1,42,174,251,207,187,52,129,127,249,33,61,227,79,105,135,129,161,78,158,20,68,
104,101,1,207,71,209,202,66,70,91,114,193,245,239,48,218,253,217,102,211,76,155,151,113,89,205,140,71,206,185,180,249,67,36,29,25,234,96,96,192,26,232,224,49,219,69,98,223,2,30,66,203,194,106,62,167,44,
236,218,108,32,181,189,176,135,212,22,176,152,241,106,166,245,18,96,187,153,122,236,221,245,51,233,133,165,35,245,180,152,123,157,251,221,194,8,239,88,192,131,40,220,101,53,159,171,205,57,223,82,153,213,
72,7,6,209,185,90,14,140,149,178,61,192,3,1,185,229,198,207,27,66,72,59,6,209,235,69,139,213,70,55,237,28,110,42,67,157,50,5,15,137,2,46,170,211,78,170,0,232,0,66,45,54,201,166,184,115,56,207,201,144,
55,87,77,160,57,31,76,150,125,175,149,144,194,40,175,44,195,29,0,116,110,197,33,114,187,245,109,55,66,126,140,7,199,96,166,23,104,148,235,192,192,254,183,59,190,20,86,35,189,24,68,89,45,7,62,90,37,60,
224,46,171,153,193,200,229,6,167,12,105,190,32,106,190,243,11,181,158,91,252,32,189,247,197,155,210,6,64,99,30,215,185,95,10,125,180,74,88,192,97,110,49,223,42,48,200,225,134,180,94,16,215,76,123,204,
55,206,233,32,192,254,103,91,222,147,55,94,144,70,12,18,173,80,75,254,121,152,171,132,4,92,205,45,152,120,82,249,83,133,213,167,86,62,67,84,102,154,105,83,135,48,235,0,20,166,29,185,225,38,242,171,77,
72,27,6,209,233,192,15,78,153,217,231,96,251,9,5,184,215,137,66,174,54,164,239,130,184,172,118,156,47,41,116,202,205,80,11,57,225,144,38,76,69,88,205,211,65,172,72,194,0,174,172,6,48,200,205,214,255,242,
135,37,109,23,142,77,51,141,99,21,101,59,76,63,114,192,141,159,59,152,144,30,12,162,215,139,5,86,235,253,96,27,247,128,131,129,0,5,224,129,225,189,47,27,70,72,211,5,145,50,60,64,241,204,213,94,182,227,
227,203,89,239,167,73,27,175,149,144,194,24,90,197,53,224,58,183,2,15,228,96,235,219,126,132,164,231,194,49,6,1,88,234,21,101,53,202,183,238,218,68,207,204,189,139,118,239,221,46,213,148,213,222,54,177,
116,28,151,128,187,172,230,185,21,57,215,82,146,71,18,210,113,65,212,124,163,142,41,24,0,40,211,65,128,28,111,72,251,5,65,153,159,67,45,243,62,242,219,143,43,192,97,110,205,80,11,185,214,144,126,75,69,
88,237,49,223,56,167,131,0,251,200,233,134,87,142,145,238,11,130,65,0,214,199,139,196,13,224,106,110,193,196,202,229,254,38,159,85,35,237,22,68,205,116,72,86,243,32,64,14,183,73,11,134,202,15,216,163,
141,78,7,24,36,241,36,49,15,184,215,137,234,220,56,149,46,191,232,38,193,200,235,124,41,112,40,55,67,45,228,110,155,250,214,48,55,52,19,86,199,80,168,165,247,21,206,54,166,1,87,86,227,70,145,67,13,161,
22,210,107,65,76,51,45,5,206,74,203,97,250,145,171,13,161,22,210,120,65,244,122,241,198,106,231,214,101,19,147,128,131,129,0,69,231,86,228,78,67,58,45,8,202,0,102,200,80,203,153,195,145,163,13,105,187,
32,94,43,33,133,113,186,138,57,192,117,110,5,30,200,149,214,143,127,135,20,105,180,32,226,148,241,96,240,138,206,225,40,223,182,251,27,102,245,16,218,185,103,171,84,83,86,123,219,196,235,113,204,0,238,
178,154,231,86,228,70,75,229,80,171,94,205,166,130,75,142,153,206,13,182,119,14,71,46,54,164,231,130,0,232,120,9,181,228,134,194,92,249,30,112,111,168,213,236,156,206,212,179,245,80,247,246,132,213,249,
132,90,120,217,31,111,160,32,45,23,68,156,178,56,243,190,93,133,228,179,227,107,192,213,220,130,137,200,129,134,215,130,107,84,169,43,183,164,102,26,224,153,130,1,128,50,152,254,67,135,211,37,19,19,210,
112,65,116,58,64,157,68,21,95,2,238,117,162,58,54,234,75,157,26,245,19,140,188,102,90,129,147,80,43,51,231,205,148,53,95,45,162,201,156,118,75,29,59,97,117,156,134,90,170,131,112,182,190,3,92,89,141,206,
35,215,25,222,64,65,26,44,136,178,90,14,140,149,59,135,51,171,145,83,13,230,251,107,78,183,5,209,235,37,50,171,13,85,249,235,211,50,132,83,135,153,165,16,204,211,152,175,33,0,90,66,45,175,249,230,71,37,
236,121,185,33,216,162,143,95,162,87,223,29,35,109,188,86,66,10,237,202,31,128,39,49,208,144,67,135,51,36,167,89,106,242,104,73,119,133,50,113,202,60,64,163,220,101,59,183,69,238,52,176,26,105,181,32,
202,106,57,176,171,92,26,240,133,73,199,19,47,200,192,206,99,11,28,106,189,194,140,94,204,204,134,136,3,199,140,215,121,91,10,237,42,151,6,162,10,184,62,13,171,123,90,67,122,234,230,119,248,11,122,229,
164,115,194,234,96,161,22,155,118,176,23,130,28,105,96,245,190,3,191,201,177,122,224,114,96,87,33,53,16,85,192,117,158,197,167,91,16,53,211,33,67,45,6,27,14,218,164,133,247,115,154,172,69,210,6,64,103,
241,188,143,114,43,249,107,32,170,128,107,247,36,212,130,243,21,108,174,102,32,213,18,124,184,105,137,196,213,154,41,193,134,90,170,193,240,183,190,0,92,152,238,56,110,218,117,152,117,0,10,176,145,251,
44,141,205,247,198,239,63,148,211,106,190,109,168,165,218,10,127,235,11,192,205,238,202,3,20,131,237,200,121,134,52,88,16,157,2,172,249,54,53,86,176,125,95,1,158,51,135,7,104,199,175,255,21,86,111,219,
189,89,238,200,134,90,5,3,54,84,237,136,1,30,108,126,214,78,121,31,151,190,254,222,56,90,248,225,11,114,218,101,117,2,63,255,86,61,21,198,182,200,1,87,48,241,115,81,16,5,80,59,175,172,198,241,55,63,172,
227,80,107,48,237,221,255,139,156,214,185,90,235,218,237,95,215,64,209,3,238,60,42,253,114,235,26,23,68,60,81,195,163,82,164,187,2,243,225,124,77,225,119,202,86,111,92,144,93,71,202,178,179,22,255,245,
91,180,87,48,53,144,251,179,69,243,76,33,237,131,225,26,62,225,7,99,32,197,139,149,16,176,177,143,31,129,237,247,100,99,23,108,169,203,3,64,45,3,234,88,41,60,13,20,57,195,209,85,13,159,240,131,49,41,79,
54,229,4,118,173,37,220,218,180,237,99,218,249,171,243,170,17,135,95,240,190,181,110,225,221,162,189,146,169,129,136,0,174,255,16,243,55,190,75,173,95,158,55,203,109,168,165,218,40,218,109,68,1,87,243,
174,30,59,142,133,213,248,152,211,74,68,52,16,81,192,113,71,48,217,135,109,136,21,17,112,131,253,147,34,119,218,130,253,83,91,22,61,13,88,192,163,167,251,168,252,231,144,128,91,111,57,42,120,20,249,63,
13,9,120,36,126,112,165,200,239,46,65,255,1,30,106,133,146,188,128,59,149,241,245,29,60,32,129,32,3,161,21,255,107,64,129,174,238,188,187,31,172,199,121,188,116,196,202,250,124,27,191,221,49,109,233,40,
9,157,36,148,58,194,200,9,118,113,91,22,57,13,224,173,31,60,178,70,30,247,134,156,180,31,162,47,142,152,189,0,224,43,121,193,151,180,14,241,34,148,214,56,185,121,189,43,57,107,255,110,249,77,14,12,2,27,
46,179,134,124,44,85,43,84,167,33,93,179,243,188,194,7,195,99,106,67,4,219,0,127,141,167,27,23,78,231,5,128,231,97,60,26,28,72,223,47,9,110,50,249,135,213,248,42,40,178,226,35,13,224,171,88,229,74,87,
116,243,184,7,233,26,94,246,7,250,59,5,61,110,176,135,15,240,202,168,158,224,221,108,81,243,174,199,118,235,111,13,132,192,75,113,237,162,128,183,228,219,88,202,11,78,160,44,23,141,97,30,48,138,172,248,
83,3,242,200,154,33,131,115,237,125,223,128,123,172,150,123,29,59,117,231,195,164,243,22,155,172,209,124,114,48,47,0,29,146,107,2,200,46,178,235,24,211,128,130,125,144,251,93,134,113,78,119,65,229,131,
33,92,136,28,24,40,195,130,202,0,223,82,155,149,16,67,2,188,128,29,182,240,201,48,93,215,2,216,66,110,62,128,185,230,227,128,0,203,251,189,184,104,10,47,166,89,215,11,160,186,21,255,106,0,63,215,224,146,
152,247,151,240,146,204,216,102,48,174,252,21,190,64,166,11,170,7,244,146,92,17,191,206,210,131,151,202,188,88,137,45,13,44,231,238,142,96,128,151,161,219,10,54,246,255,15,100,21,128,176,42,75,85,3,0,
0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::playOn_png = (const char*) resource_PlayerMainMenuComponent_playOn_png;
const int PlayerMainMenuComponent::playOn_pngSize = 6097;

// JUCER_RESOURCE: playOn_png2, 6044, "../../MiamCommon/Resources/HighlyVisible/PlayOn.png"
static const unsigned char resource_PlayerMainMenuComponent_playOn_png2[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,
28,233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,36,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,
68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,
34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,
102,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,
34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,
47,47,112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,
47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,
105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,
111,109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,
105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,
32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,
105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,
101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,
32,32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,
102,121,68,97,116,101,62,50,48,49,56,45,48,49,45,49,49,84,49,49,58,48,49,58,51,48,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,
97,116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,
99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,85,165,248,31,0,0,19,17,73,68,65,84,120,1,237,157,9,152,93,101,121,199,223,
201,204,36,100,38,147,76,50,147,61,85,234,66,171,209,34,88,65,31,165,42,75,91,17,164,149,18,32,16,4,42,10,246,1,149,125,71,36,66,202,226,198,163,69,65,136,16,22,241,113,1,44,182,2,1,4,31,17,43,139,5,43,
88,44,202,100,159,201,190,206,218,247,247,221,251,191,57,115,50,55,153,152,153,59,231,222,251,189,247,57,219,123,190,239,220,115,190,255,251,127,191,229,156,243,158,26,203,75,95,95,95,141,75,31,155,190,
62,218,23,87,251,52,207,167,41,62,69,41,175,18,120,212,79,119,190,227,185,152,211,118,60,71,249,122,47,235,53,121,69,18,236,83,93,119,139,246,177,223,165,219,167,96,12,97,43,206,178,90,2,181,126,98,163,
18,39,247,19,95,63,194,193,237,18,232,53,41,102,127,213,19,156,153,207,0,200,100,198,40,130,97,228,245,113,145,237,18,128,152,61,62,1,62,184,173,241,105,95,7,253,181,128,181,0,247,229,2,223,113,129,79,
129,250,190,76,90,138,111,70,41,195,18,128,180,117,62,109,243,105,188,131,222,41,151,126,168,43,30,242,9,176,119,96,116,175,123,115,166,40,217,44,1,39,171,57,152,78,233,154,240,75,157,165,64,127,214,211,
236,47,192,161,125,179,79,0,222,143,217,61,174,170,237,175,74,29,47,110,102,169,4,138,224,37,92,143,166,14,159,235,39,124,167,79,178,132,29,206,127,125,239,86,123,166,171,205,186,251,122,6,178,160,29,
210,71,69,105,75,0,239,59,163,118,130,205,174,159,86,236,143,5,248,114,252,251,233,249,84,3,250,236,203,214,61,104,243,215,255,103,177,3,69,125,134,74,224,45,245,83,237,241,41,103,217,228,81,227,66,21,
60,42,212,206,225,4,229,181,167,193,112,1,205,50,184,120,185,133,155,54,254,204,206,88,115,111,200,129,91,15,59,51,116,129,241,84,182,151,64,143,179,156,95,203,168,70,107,159,201,16,10,253,104,175,219,
183,163,214,229,170,250,29,0,79,38,26,211,118,182,117,186,27,175,115,176,187,67,245,30,142,19,103,25,45,129,209,53,181,1,175,187,90,78,178,227,27,222,105,93,142,93,189,235,242,18,0,23,213,165,116,155,
200,201,166,190,206,144,153,45,172,39,74,246,75,64,40,61,221,249,199,162,39,187,3,224,74,137,117,68,41,207,18,192,75,23,147,162,128,199,26,187,88,145,149,183,190,40,224,229,125,89,241,236,139,149,64,4,
188,88,201,84,168,158,126,120,73,133,170,162,182,38,103,103,61,125,189,161,191,88,210,19,168,242,63,43,41,224,244,229,233,227,247,38,26,132,234,39,238,172,161,81,229,24,13,233,229,151,20,112,192,110,30,
53,214,62,188,215,108,155,232,203,31,108,249,181,45,233,89,23,46,40,246,245,135,20,215,162,7,43,9,224,184,113,198,123,15,221,235,47,236,161,201,159,42,156,204,141,19,255,201,190,191,229,121,59,161,227,
14,219,218,199,184,0,119,110,114,105,11,137,226,202,144,150,192,176,55,218,112,217,186,181,42,176,25,189,83,63,255,163,99,247,181,77,179,174,179,121,141,239,10,23,70,218,220,48,110,28,200,29,82,164,243,
7,27,118,192,235,242,13,180,57,13,251,133,191,100,136,150,33,64,13,249,225,230,97,245,237,147,78,180,159,250,192,255,180,218,241,161,158,167,78,199,205,71,25,218,18,40,89,137,114,251,14,217,126,175,38,
119,33,176,25,112,249,29,52,230,141,182,108,198,85,118,126,211,33,97,39,198,145,187,165,31,217,158,43,173,61,159,151,12,112,185,240,129,78,89,160,194,118,228,95,155,63,98,255,51,237,98,251,171,250,25,
5,99,192,48,162,236,121,9,148,172,20,7,195,81,177,29,224,255,210,239,237,62,63,237,2,251,114,243,71,195,85,162,147,97,236,249,101,87,239,17,74,6,248,96,139,24,80,1,94,13,189,79,55,189,63,220,223,61,100,
175,125,10,108,143,117,251,96,75,115,199,116,153,0,156,250,91,0,235,20,105,200,33,212,227,220,212,127,120,242,191,216,61,45,39,135,198,158,238,205,43,141,242,196,229,174,75,32,19,128,195,106,192,83,29,
158,60,109,216,44,99,56,214,91,250,155,103,93,31,110,238,147,6,61,251,201,31,101,112,37,48,162,128,195,108,228,149,238,118,107,239,221,84,120,58,86,0,235,18,196,100,12,2,128,121,162,227,17,103,124,171,
51,31,182,115,156,232,230,85,90,59,95,142,40,224,221,126,243,4,121,104,235,75,54,121,201,197,246,163,45,47,134,109,0,150,219,14,138,252,76,141,58,0,62,216,235,244,85,254,236,214,217,77,31,12,123,73,159,
227,122,100,123,178,204,210,235,35,10,184,78,102,186,15,182,32,71,182,127,211,62,188,234,27,182,161,111,91,129,177,105,182,171,165,46,247,127,67,243,63,216,11,211,46,178,183,250,35,186,57,174,231,70,234,
116,236,184,236,95,2,153,0,92,160,50,2,247,224,214,223,216,248,182,243,237,230,77,63,15,103,10,107,1,87,238,95,167,47,182,179,135,231,177,95,116,208,175,119,240,17,210,203,48,148,62,46,115,37,144,9,192,
5,6,53,58,64,34,159,88,125,143,29,176,226,6,123,181,123,117,97,108,93,172,14,9,124,166,198,158,220,255,57,238,222,87,204,152,111,239,31,243,166,96,32,24,137,142,167,60,213,190,204,20,224,128,145,100,231,
47,253,233,203,63,95,118,165,125,110,221,143,3,78,98,117,154,237,52,216,208,1,252,148,218,38,123,108,202,153,118,167,55,236,72,47,35,81,195,47,2,158,193,18,0,60,126,98,231,149,235,255,195,246,118,224,
159,238,252,67,193,85,11,72,157,62,108,7,120,85,15,115,253,185,108,238,194,29,211,240,142,144,4,125,108,201,167,94,28,84,225,101,101,41,182,3,252,31,220,181,31,184,226,139,193,213,115,126,50,6,1,172,115,
22,147,97,251,152,154,58,187,183,229,148,112,15,126,226,168,134,66,203,95,105,148,167,154,150,153,115,233,233,194,135,233,0,47,144,104,204,53,121,163,110,87,93,56,185,121,242,243,224,197,234,153,215,216,
153,227,254,38,28,30,35,193,96,240,10,213,38,153,7,92,128,136,201,0,185,209,187,109,116,225,142,106,191,217,120,67,70,174,90,105,148,7,64,249,201,253,127,117,226,209,246,220,180,243,109,159,186,41,65,
151,172,54,148,167,210,151,101,3,184,128,208,0,11,219,247,111,121,193,198,181,157,103,183,110,122,42,236,134,179,128,11,144,73,129,205,232,248,237,91,63,211,94,154,126,137,93,51,225,200,144,68,213,70,
181,176,189,236,0,7,37,49,89,245,248,63,175,190,219,222,179,226,75,246,90,207,218,130,171,86,87,77,192,139,237,210,95,56,254,80,91,234,15,91,188,111,204,27,10,198,160,227,41,79,37,46,203,18,112,1,145,
100,231,83,157,175,218,235,150,94,97,87,229,223,101,79,214,225,74,207,82,122,242,50,194,247,196,148,79,219,183,253,241,170,164,235,87,123,33,153,175,82,214,203,26,112,64,144,171,22,59,47,247,0,6,111,90,
118,149,61,211,217,22,64,76,2,41,208,208,145,94,158,226,36,127,128,146,46,28,15,84,34,232,213,46,80,158,74,89,150,61,224,2,66,108,7,72,238,190,189,115,197,117,246,169,53,223,13,187,101,12,2,88,121,196,
100,220,252,216,154,122,251,94,235,169,246,224,228,79,218,4,127,102,94,174,95,105,148,167,220,151,21,3,56,64,192,118,128,23,72,255,182,241,73,107,94,114,161,253,216,199,231,17,244,2,50,40,242,51,216,44,
99,248,208,94,111,181,181,51,23,216,25,227,222,23,246,162,199,96,240,10,149,32,21,5,184,0,17,120,0,185,174,119,139,29,238,119,224,142,110,191,213,182,248,203,14,114,213,74,163,60,50,18,12,6,249,250,196,
99,236,87,83,207,179,55,212,181,20,90,254,242,20,202,83,142,203,138,4,92,64,192,102,1,201,27,46,141,222,133,187,125,211,47,195,110,244,128,139,87,72,10,160,170,93,176,255,232,89,246,202,244,203,237,170,
9,135,135,36,170,54,202,153,237,21,13,56,40,137,201,2,242,99,171,23,217,65,43,191,98,203,122,214,23,92,117,218,205,3,40,63,177,253,210,241,127,103,127,156,113,165,189,123,244,222,5,99,40,87,182,87,60,
224,98,111,146,157,79,110,251,189,205,88,122,153,45,88,255,112,216,141,155,23,171,149,158,165,140,132,188,127,86,219,108,63,159,250,89,251,214,164,227,67,18,25,131,60,72,50,95,150,215,171,6,112,64,16,
168,98,231,69,235,30,176,125,150,205,183,231,186,150,228,57,189,157,213,2,13,166,147,94,158,226,212,198,119,219,70,239,194,125,100,236,219,66,18,244,106,23,40,79,150,151,85,5,184,128,16,219,1,242,119,
221,171,108,191,229,215,218,89,107,190,23,118,139,213,233,186,93,76,198,253,55,214,140,182,251,90,79,179,251,125,26,87,51,166,208,242,87,26,253,79,22,151,85,9,56,64,0,40,192,11,164,27,55,254,212,38,45,
185,200,30,246,7,42,85,135,167,235,118,242,193,102,177,253,72,103,249,134,89,215,218,199,27,223,195,174,160,199,96,200,159,85,169,90,192,5,136,192,3,200,53,189,155,237,176,85,95,183,57,29,183,21,2,18,
146,78,105,148,71,70,162,122,252,230,73,199,217,83,83,207,182,215,215,77,42,180,252,1,62,139,146,205,179,26,129,146,74,118,225,190,187,249,57,107,104,59,215,22,109,254,175,112,38,0,204,254,180,155,79,
186,255,3,71,191,222,94,157,126,133,93,62,254,239,67,30,85,27,89,99,123,4,60,97,92,98,50,64,2,216,60,143,76,241,193,149,55,218,138,158,13,193,149,3,94,218,205,203,253,139,237,87,78,248,144,253,159,3,255,
174,209,175,11,6,130,145,100,137,237,17,240,4,224,90,77,178,243,177,109,255,107,211,150,94,106,215,111,88,28,118,171,14,47,198,118,242,238,237,174,253,233,169,231,216,77,19,231,132,60,50,6,85,5,250,159,
145,88,70,192,139,148,58,128,38,217,121,222,218,251,108,246,242,107,236,133,174,101,249,102,217,174,187,112,159,28,247,94,91,239,141,186,195,125,124,30,193,131,140,116,23,46,2,94,4,112,169,197,118,216,
249,155,174,229,246,246,229,11,236,179,107,127,16,118,39,235,112,165,103,41,38,227,254,155,188,219,246,239,126,7,238,135,173,31,183,6,239,206,169,74,80,154,100,190,82,172,71,192,7,81,202,48,29,118,10,
164,47,111,120,204,90,253,93,184,197,91,95,118,51,200,253,4,100,242,112,114,255,232,142,26,251,246,112,207,253,228,198,3,67,18,142,55,18,117,123,4,60,137,208,46,214,1,9,120,1,178,195,223,118,61,100,213,
215,108,110,199,237,129,181,114,213,164,73,138,140,68,245,248,109,147,230,218,147,254,148,205,44,31,170,149,142,99,150,74,34,224,187,89,210,176,29,54,11,200,187,55,255,42,116,225,238,217,252,76,56,18,
122,246,147,46,41,73,247,255,94,127,142,238,53,191,25,115,241,248,195,66,18,210,150,10,244,8,120,18,149,221,88,23,147,97,54,1,139,142,239,248,182,29,178,242,107,129,249,232,0,80,12,214,97,209,37,245,95,
152,112,68,120,207,157,253,105,3,81,158,161,94,70,192,247,176,68,97,179,128,92,188,237,229,80,183,127,101,195,227,225,168,197,234,104,177,29,67,225,61,247,75,198,255,109,72,175,216,117,123,120,74,59,205,
30,1,223,105,241,12,110,39,236,228,39,128,63,179,246,251,54,117,201,165,246,51,191,13,139,200,27,36,143,134,145,248,23,96,131,138,135,40,17,5,72,8,27,195,52,139,128,15,83,193,102,245,176,37,9,174,155,
213,139,31,170,243,130,173,136,234,108,98,203,17,110,76,162,6,158,182,89,230,124,66,78,115,199,166,220,152,61,97,74,119,22,192,48,153,255,79,93,143,128,255,169,37,151,207,71,3,77,125,240,131,199,236,99,
247,182,158,28,194,140,177,27,3,144,155,79,254,141,244,117,110,40,220,142,213,135,0,135,27,108,206,33,2,158,68,98,55,214,97,45,117,51,96,211,216,34,56,240,113,13,251,135,35,160,227,195,175,105,176,97,
53,34,253,37,235,126,100,87,175,231,27,191,185,104,22,218,31,20,195,52,139,128,239,102,193,226,190,1,83,172,230,131,112,183,183,156,88,24,35,199,8,52,8,147,60,180,88,141,142,198,220,113,222,141,107,243,
119,225,16,142,89,10,176,249,175,8,56,165,48,72,217,206,234,220,39,31,239,245,200,144,116,171,16,12,0,160,211,245,53,6,128,78,172,62,101,245,93,182,112,211,47,66,30,116,24,66,169,192,230,79,35,224,161,
232,119,62,131,129,52,203,128,6,249,76,211,7,236,75,205,255,24,214,5,214,64,172,150,17,144,240,190,45,255,29,134,97,55,251,251,236,8,70,0,216,165,150,8,248,46,74,124,59,11,45,196,130,251,142,179,250,109,
245,211,67,174,164,155,78,30,70,172,198,8,136,57,119,92,251,194,16,142,140,52,232,48,4,25,79,50,95,41,214,35,224,69,74,57,199,233,237,93,173,235,154,143,178,115,155,14,14,169,1,11,198,203,77,235,16,176,
157,125,210,127,195,191,206,124,122,254,235,204,164,129,213,170,251,149,167,212,203,8,248,0,37,46,86,179,235,3,30,243,141,40,206,83,61,28,24,146,116,211,65,145,159,137,237,52,232,136,45,199,131,144,132,
29,67,116,188,145,98,117,254,20,195,34,2,158,40,13,24,8,40,2,111,97,203,9,118,98,195,95,135,20,0,13,152,233,186,90,117,184,88,125,133,199,148,251,188,135,25,67,210,94,34,40,71,120,22,1,207,3,160,186,149,
77,98,187,45,154,116,82,248,24,15,219,24,65,26,104,244,50,12,214,127,225,49,228,142,237,88,24,194,139,177,45,86,179,158,37,169,122,192,197,106,24,76,44,55,186,90,132,249,66,228,190,73,147,20,12,0,157,
88,125,154,135,9,189,37,31,27,22,157,188,68,50,79,86,214,171,22,112,220,45,160,193,82,132,24,110,132,245,66,228,166,7,98,181,140,128,116,15,120,20,41,158,120,33,140,24,146,60,94,80,100,112,86,149,128,
203,221,246,56,180,111,174,155,236,227,223,167,216,59,60,156,23,146,116,211,73,188,196,106,140,128,216,112,115,125,164,140,176,97,136,170,3,210,100,93,170,10,240,116,35,138,88,109,132,239,66,196,106,185,
105,1,151,238,106,17,19,142,48,97,18,88,13,235,203,69,170,6,112,177,26,96,136,205,70,12,86,5,230,79,186,233,36,112,98,59,173,115,98,192,205,105,191,205,8,15,134,232,120,229,192,234,112,194,249,89,197,
3,14,3,213,136,130,225,11,39,157,96,122,194,4,64,217,159,174,171,211,108,231,246,229,101,30,14,12,73,123,137,124,57,150,205,162,162,1,87,221,10,26,196,96,91,212,50,47,132,231,98,27,35,72,187,111,244,98,
53,235,196,122,59,166,227,86,251,125,119,7,155,5,86,135,141,50,157,85,36,224,98,53,174,154,152,107,119,123,176,124,194,113,33,114,223,164,73,10,6,128,78,70,64,140,55,194,126,33,232,228,37,146,121,202,
113,189,162,0,79,119,181,136,181,70,248,45,9,160,165,221,55,251,100,4,172,19,211,237,120,239,106,17,238,11,193,8,96,125,165,72,197,0,174,70,20,93,173,55,214,181,134,70,25,97,183,16,185,233,98,172,198,
8,136,225,118,162,191,30,76,120,47,68,213,1,70,82,73,82,246,128,167,27,81,159,247,152,106,151,121,152,45,36,221,248,18,112,233,174,22,177,219,78,94,125,103,33,61,134,1,235,43,81,202,26,112,177,26,96,136,
161,198,0,10,225,181,144,164,155,14,138,252,76,122,186,90,196,106,227,174,22,97,188,16,29,175,210,88,157,191,244,176,40,75,192,97,96,178,17,69,236,52,194,105,33,187,234,106,169,14,39,70,27,97,187,144,
180,151,8,202,10,157,149,29,224,170,91,193,131,88,105,119,181,124,44,132,209,98,27,35,128,165,105,81,29,142,254,121,143,201,54,199,159,64,121,185,123,101,72,38,86,167,243,84,234,118,217,0,46,86,227,146,
137,141,118,183,3,125,196,216,217,1,23,185,233,116,163,44,93,135,19,139,141,240,92,8,64,39,189,68,80,86,193,44,243,128,167,187,90,167,121,76,180,111,122,152,44,9,160,201,77,75,199,82,70,192,58,15,251,
207,241,123,213,132,229,66,48,140,74,234,106,133,139,26,228,44,211,128,203,221,210,213,34,6,26,247,170,15,240,240,88,136,220,116,154,213,24,0,58,140,96,91,95,183,205,91,125,135,17,134,11,81,117,64,154,
106,149,76,2,158,110,68,93,225,177,207,62,231,225,176,144,180,155,22,112,232,49,12,177,253,46,127,81,255,164,142,69,5,38,99,4,176,190,218,37,115,128,139,213,0,67,172,51,238,106,17,6,11,17,171,195,70,98,
38,247,205,187,90,43,61,166,26,238,251,113,15,183,133,232,120,213,204,234,68,81,57,33,50,36,140,110,171,110,165,158,166,190,70,208,193,80,192,75,10,172,198,57,139,213,55,108,120,212,206,93,251,195,144,
36,237,37,146,249,170,121,61,19,128,171,30,238,244,136,8,196,52,187,199,223,192,36,220,21,2,51,211,64,163,23,219,49,146,23,61,156,22,3,40,132,213,66,196,234,176,17,103,253,74,32,19,128,51,226,133,60,208,
250,137,221,238,106,157,227,140,254,162,51,27,193,112,96,188,188,68,80,198,89,191,18,24,81,192,121,1,30,57,204,159,18,93,53,243,106,107,29,213,24,182,97,181,220,116,80,228,103,98,53,155,196,72,227,177,
224,118,15,159,133,168,5,30,54,226,172,104,9,140,40,224,170,103,185,187,133,8,80,185,120,157,53,6,160,58,156,6,26,173,111,194,101,33,0,77,235,60,182,192,85,90,59,95,142,40,224,58,53,53,190,6,170,171,213,
2,39,237,119,54,63,27,250,213,138,148,128,17,68,160,85,138,131,91,102,2,112,152,222,255,249,147,92,99,13,64,97,48,81,15,113,223,143,184,27,71,228,190,99,87,107,112,32,39,83,101,2,240,228,9,193,246,100,
203,156,152,103,132,193,66,100,22,145,213,201,18,219,189,245,76,1,174,58,156,123,213,191,237,90,17,88,253,235,174,165,225,138,98,87,107,247,128,45,150,186,100,128,215,230,91,228,3,157,72,110,248,36,215,
127,102,255,5,107,239,183,107,55,60,18,146,138,213,177,171,53,80,201,237,190,110,216,1,239,235,203,221,168,120,185,107,85,14,192,124,244,65,157,170,88,205,246,19,219,94,9,195,162,203,243,253,114,213,213,
74,27,151,123,94,2,195,14,56,93,38,228,161,173,191,13,75,64,100,68,141,70,26,225,174,112,213,212,217,60,83,118,71,254,187,160,210,197,186,58,20,217,144,206,250,15,78,15,233,161,115,7,195,93,171,251,196,
7,99,144,209,14,180,2,201,234,35,176,2,155,180,176,94,110,62,119,148,56,31,170,18,24,118,134,115,162,234,62,241,193,152,166,182,243,237,216,134,253,172,206,65,127,108,235,239,236,165,252,163,70,114,223,
74,59,84,23,24,143,211,191,4,74,2,184,254,146,6,24,239,82,127,203,223,192,76,10,250,232,190,147,37,50,124,235,37,5,92,238,93,45,118,26,116,0,29,221,247,240,1,156,62,114,73,1,231,207,129,183,215,27,109,
81,70,166,4,134,189,209,54,50,151,21,255,181,88,9,68,192,139,149,76,133,234,139,2,30,91,203,149,137,120,81,192,213,79,174,204,203,174,236,171,210,112,244,64,87,185,3,224,186,77,217,232,159,77,100,128,
4,225,102,70,148,236,151,128,80,58,192,159,246,45,38,53,222,53,202,141,125,242,200,55,119,32,93,52,190,125,147,7,135,61,35,31,28,150,225,78,29,176,216,193,162,126,228,74,128,33,108,126,45,254,152,88,187,
63,46,134,176,157,96,123,151,171,234,233,150,61,225,211,65,62,117,163,240,169,240,148,232,233,254,117,220,37,61,235,194,55,57,226,221,42,74,38,219,242,150,250,169,246,248,148,179,194,73,210,6,75,81,52,
96,11,195,231,122,138,59,125,2,240,1,251,229,235,123,183,218,51,93,109,254,93,45,110,122,68,158,135,18,205,208,12,112,103,212,78,176,217,245,211,138,157,21,175,220,80,125,47,15,232,57,232,107,124,131,
55,233,181,195,87,115,34,247,174,237,184,204,118,9,20,193,75,184,30,45,192,15,245,203,224,243,58,236,64,215,143,198,97,116,204,173,40,74,54,75,128,102,88,141,63,103,64,227,122,0,15,44,207,253,172,167,
217,31,151,238,203,208,118,91,224,151,115,129,79,128,142,236,208,130,207,169,227,188,140,74,64,96,19,253,119,188,227,220,89,0,213,55,46,116,37,55,172,209,49,145,24,240,35,181,189,16,202,72,192,11,236,
88,210,38,163,186,126,51,96,7,114,251,134,137,229,249,245,83,125,121,139,79,73,183,174,3,144,36,74,118,75,128,129,147,2,137,125,253,39,62,29,225,96,119,57,198,254,109,219,154,222,2,168,41,208,71,123,66,
58,115,243,124,154,226,83,148,242,42,129,71,253,116,231,59,192,139,57,109,129,205,250,255,3,53,88,69,109,226,140,242,50,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::playOn_png2 = (const char*) resource_PlayerMainMenuComponent_playOn_png2;
const int PlayerMainMenuComponent::playOn_png2Size = 6044;

// JUCER_RESOURCE: stop_png, 3348, "../../MiamCommon/Resources/HighlyVisible/Stop.png"
static const unsigned char resource_PlayerMainMenuComponent_stop_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,28,
233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,36,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,109,
108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,68,
70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,
62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,
116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,
102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,
111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,
58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,
110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,
121,68,97,116,101,62,50,48,49,56,45,48,49,45,49,49,84,49,49,58,48,49,58,49,48,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,99,
114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,144,108,187,1,0,0,8,137,73,68,65,84,120,1,237,155,79,104,84,87,20,198,239,204,
36,49,65,163,129,108,36,21,186,169,133,82,66,13,100,145,150,44,116,215,110,84,116,81,164,133,110,236,34,221,149,110,20,140,93,180,17,41,138,203,108,164,33,208,125,13,46,186,172,16,44,21,73,16,44,10,13,
174,36,65,112,35,113,97,77,50,51,61,223,53,103,230,204,63,51,233,188,55,157,201,249,46,220,185,103,238,187,239,190,123,190,223,187,127,39,201,4,19,138,197,98,70,66,81,179,228,251,103,98,127,46,241,61,
137,135,53,159,105,199,41,0,102,207,36,254,45,241,23,97,248,187,182,176,154,105,198,92,200,74,193,2,190,75,161,41,73,174,75,28,192,119,134,174,83,96,93,90,252,173,240,252,25,45,23,158,37,182,17,184,190,
5,146,246,201,245,63,37,142,161,160,132,188,68,188,4,40,151,149,200,208,153,10,196,142,42,77,67,79,239,145,168,29,249,174,64,159,68,147,21,122,70,13,73,15,72,254,83,137,67,18,181,2,66,22,49,186,48,88,
126,171,210,254,119,5,124,30,172,75,93,93,50,151,36,2,246,166,68,128,38,108,17,161,75,131,242,219,146,246,191,35,17,163,118,192,148,141,238,143,238,126,69,146,247,37,162,64,175,196,82,144,107,97,107,107,
43,228,114,57,220,80,202,167,209,57,10,128,81,62,159,15,61,61,61,213,140,192,23,76,199,165,204,119,194,239,58,134,244,65,201,192,36,143,128,57,128,84,163,20,221,249,1,248,85,29,211,50,205,225,13,192,138,
28,1,67,121,69,239,70,38,222,156,11,23,46,132,197,197,197,240,250,245,235,88,25,42,101,248,255,21,80,176,253,253,253,225,196,137,19,97,102,102,166,26,54,26,137,14,140,94,14,214,95,99,56,127,36,17,97,243,
77,82,44,10,228,104,174,175,175,23,135,134,134,64,151,177,11,52,24,25,25,41,202,244,27,217,41,195,109,166,202,246,15,80,255,64,34,66,238,77,82,254,60,119,238,92,120,241,226,69,216,183,111,95,156,199,245,
141,42,151,160,213,9,10,8,212,56,127,175,173,173,133,243,231,207,135,185,185,57,116,100,219,52,93,128,127,132,57,92,175,32,69,247,215,180,52,60,0,116,185,152,173,135,118,167,40,144,205,102,67,161,80,8,
189,189,189,97,99,99,67,155,165,44,53,109,188,245,218,220,196,148,206,208,45,10,104,135,220,137,155,118,245,26,191,56,124,215,72,178,39,50,26,2,223,19,222,209,137,26,5,8,188,70,146,189,157,129,85,122,
98,1,211,128,158,200,233,156,146,88,229,14,43,210,197,50,78,58,147,10,137,1,79,163,113,73,57,201,122,202,10,36,6,92,123,244,153,51,103,194,233,211,167,131,28,2,196,173,28,23,127,101,177,155,181,160,25,
78,56,151,151,151,195,252,252,124,120,252,248,113,179,183,238,92,78,64,105,40,108,27,49,149,229,61,246,110,49,74,3,74,182,230,105,42,251,191,120,77,142,247,138,247,238,221,211,186,152,38,168,192,229,203,
151,27,234,175,28,44,35,243,232,10,166,200,111,120,240,130,121,3,155,120,4,188,113,82,54,218,213,31,122,237,254,253,251,97,124,124,60,232,62,16,249,12,173,41,0,205,149,193,201,147,39,195,237,219,183,227,
137,90,189,57,93,57,224,137,134,21,160,85,30,166,201,69,37,89,113,177,25,224,122,186,131,225,123,117,117,181,244,32,194,110,13,180,189,27,29,8,208,31,60,120,16,198,198,198,130,106,110,203,192,110,22,120,
75,219,50,172,200,17,38,39,39,99,138,151,132,176,163,20,137,125,224,55,110,132,99,199,142,197,20,199,167,173,104,156,200,162,237,224,193,131,177,49,245,62,48,128,212,27,130,234,149,245,158,7,144,10,56,
45,45,18,1,94,158,21,106,155,9,39,116,30,170,189,202,156,118,43,144,8,240,122,141,198,208,131,249,230,225,195,135,97,122,122,58,12,15,15,199,173,70,189,178,222,243,208,41,48,87,15,14,14,134,217,217,217,
40,7,58,81,43,67,119,35,77,83,3,142,125,36,128,175,172,172,132,133,133,133,70,207,103,126,149,2,10,188,42,59,177,175,169,1,215,22,226,173,69,24,24,24,224,92,174,162,84,165,232,24,232,225,7,14,224,47,197,
211,13,169,3,215,249,29,63,202,163,215,51,212,42,160,91,45,61,195,168,45,145,92,78,75,219,178,221,52,35,141,249,104,55,207,239,134,178,237,208,168,109,192,187,65,112,15,109,36,112,15,148,141,143,4,110,
196,240,96,18,184,7,202,198,71,2,55,98,120,48,9,220,3,101,227,35,129,27,49,60,152,4,238,129,178,241,145,192,141,24,30,76,2,247,64,217,248,72,224,70,12,15,38,129,123,160,108,124,36,112,35,134,7,147,192,
61,80,54,62,18,184,17,195,131,73,224,30,40,27,31,9,220,136,225,193,36,112,15,148,141,143,4,110,196,240,96,18,184,7,202,198,71,2,55,98,120,48,9,220,3,101,227,35,129,27,49,60,152,4,238,129,178,241,145,192,
141,24,30,76,2,247,64,217,248,72,224,70,12,15,38,129,123,160,108,124,36,112,35,134,7,147,192,61,80,54,62,18,184,17,195,131,73,224,30,40,27,31,9,220,136,225,193,36,112,15,148,141,143,4,110,196,240,96,18,
184,7,202,198,71,2,55,98,120,48,9,220,3,101,227,35,129,27,49,60,152,4,238,129,178,241,145,192,141,24,30,76,2,247,64,217,248,72,224,70,12,15,38,129,123,160,108,124,36,112,35,134,7,147,192,61,80,54,62,18,
184,17,195,131,73,224,30,40,27,31,9,220,136,225,193,36,112,15,148,141,143,4,110,196,240,96,18,184,7,202,198,71,2,55,98,120,48,9,220,3,101,227,35,129,27,49,60,152,4,238,129,178,241,145,192,141,24,30,76,
2,247,64,217,248,72,224,70,12,15,38,129,123,160,108,124,36,112,35,134,7,147,192,61,80,54,62,18,184,17,195,131,73,224,30,40,27,31,9,220,136,225,193,36,112,15,148,141,143,4,110,196,240,96,182,13,120,177,
88,244,160,103,75,62,182,67,163,158,150,90,216,196,205,153,76,38,150,234,235,235,11,91,91,91,77,220,225,175,72,54,155,13,155,155,155,161,183,183,55,117,231,83,7,254,242,229,203,232,196,171,87,175,82,119,
166,219,31,176,190,190,158,186,11,169,1,207,229,114,177,241,71,143,30,13,167,78,157,10,195,195,195,33,159,207,167,238,80,55,62,0,163,32,122,248,224,224,96,234,205,79,13,56,134,41,132,209,209,209,112,235,
214,173,212,29,217,107,15,208,169,48,105,191,18,1,254,182,198,97,33,194,185,187,57,108,208,177,167,39,17,36,13,31,152,72,237,58,79,215,123,10,156,104,199,98,164,222,179,153,87,171,64,75,219,50,157,147,
23,23,23,99,205,0,219,142,173,69,173,27,123,55,71,71,199,229,229,229,232,36,166,202,86,52,110,169,135,23,10,133,128,30,188,182,182,22,238,220,185,19,142,31,63,30,23,102,104,208,219,134,249,189,139,39,
89,207,160,163,142,142,87,175,94,141,149,99,49,12,221,255,115,144,74,53,20,182,141,152,202,170,17,39,37,49,10,188,146,173,121,154,202,27,87,186,182,178,178,162,117,49,77,80,129,233,233,233,146,198,170,
123,117,106,25,153,71,87,48,69,126,6,31,114,51,2,82,156,146,196,20,67,137,190,93,232,173,229,98,40,90,25,236,245,179,103,207,198,109,216,200,200,72,188,7,215,24,118,167,0,52,195,116,185,180,180,20,110,
222,188,25,158,60,121,178,99,5,150,129,97,85,193,20,149,36,2,60,86,180,195,75,177,99,139,89,160,161,2,152,183,119,26,198,155,5,222,210,28,110,91,136,183,10,15,197,28,99,31,110,203,208,222,157,2,208,17,
61,125,39,216,187,169,53,49,224,120,40,160,235,170,114,55,141,96,217,246,41,208,210,182,172,125,205,228,147,146,82,128,192,147,82,178,75,234,33,240,46,1,149,84,51,27,2,215,31,63,146,122,16,235,233,12,
5,222,10,124,255,254,253,177,149,250,83,103,103,52,153,173,168,167,128,254,232,114,228,200,145,120,217,236,197,43,138,99,149,254,143,196,126,137,186,73,207,96,165,141,10,174,93,187,22,166,166,166,226,
202,91,15,97,42,238,230,151,142,80,0,112,241,123,58,194,141,27,55,98,138,237,156,48,212,83,47,101,251,12,91,169,5,137,8,27,111,146,202,207,139,23,47,162,48,99,23,104,48,51,51,83,9,175,252,77,217,206,227,
164,237,83,1,250,155,68,156,200,215,29,226,159,63,127,30,143,249,244,215,49,41,199,208,65,10,96,52,158,152,152,8,135,14,29,106,212,42,237,225,31,199,46,47,208,159,74,73,12,254,248,43,195,138,195,24,185,
198,95,190,26,201,216,97,249,13,88,41,211,191,228,228,110,84,129,127,34,109,191,187,221,126,125,27,172,59,232,253,252,131,52,171,72,231,217,248,35,194,234,17,218,178,252,80,128,63,194,144,46,105,252,13,
229,146,220,240,195,182,31,13,135,247,206,243,147,45,106,160,128,101,248,141,48,158,21,214,242,51,204,118,144,140,31,197,252,105,251,43,242,49,20,224,38,188,37,12,221,161,0,88,129,25,216,41,219,75,128,
141,230,75,90,208,101,59,190,199,32,111,193,151,98,204,73,180,115,57,42,32,248,168,80,199,126,128,165,101,134,127,4,248,66,32,255,106,91,92,3,28,23,209,245,37,185,34,241,43,137,135,37,50,116,143,2,88,
128,207,9,232,239,235,53,249,95,157,2,254,41,3,92,169,141,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::stop_png = (const char*) resource_PlayerMainMenuComponent_stop_png;
const int PlayerMainMenuComponent::stop_pngSize = 3348;

// JUCER_RESOURCE: stopOn_png, 3436, "../../MiamCommon/Resources/HighlyVisible/StopOn.png"
static const unsigned char resource_PlayerMainMenuComponent_stopOn_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,124,0,0,0,124,8,6,0,0,0,171,30,212,190,0,0,0,1,115,82,71,66,0,174,206,28,
233,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,4,36,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,120,58,120,109,112,109,101,116,97,32,120,109,
108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,88,77,80,32,67,111,114,101,32,53,46,52,46,48,34,62,10,32,32,32,60,114,100,102,58,82,68,
70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,
62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,101,120,105,102,61,34,104,
116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,
112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,
102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,67,111,109,112,114,101,115,115,105,111,110,62,53,60,47,116,105,102,102,58,67,111,
109,112,114,101,115,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,
111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,
32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,55,50,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,
58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,
102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,
110,115,105,111,110,62,49,50,52,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,
32,32,32,32,32,32,32,32,32,60,114,100,102,58,66,97,103,47,62,10,32,32,32,32,32,32,32,32,32,60,47,100,99,58,115,117,98,106,101,99,116,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,
121,68,97,116,101,62,50,48,49,56,45,48,49,45,49,49,84,49,49,58,48,49,58,55,51,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,62,80,105,120,101,108,109,97,116,111,114,32,51,46,55,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,99,
114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,79,130,136,134,0,0,8,225,73,68,65,84,120,1,237,155,127,136,21,85,20,199,207,
188,183,111,221,117,119,85,92,48,177,112,163,220,72,100,73,201,164,212,63,50,50,138,34,163,162,254,40,40,130,32,19,132,40,162,63,52,201,95,69,36,66,63,176,63,164,69,232,191,136,146,144,192,254,208,18,
53,168,93,255,208,214,72,12,220,205,50,11,173,117,253,181,251,222,155,206,185,239,222,183,179,235,155,221,243,118,102,214,55,167,115,225,238,125,239,206,153,59,231,124,63,115,127,205,155,245,32,144,124,
223,247,48,249,174,10,191,63,132,159,159,198,60,15,243,108,87,175,101,205,41,64,204,206,96,254,5,243,167,200,112,159,243,112,52,83,47,112,32,131,134,69,250,142,70,171,177,216,134,185,145,190,107,74,157,
2,253,232,241,43,200,243,19,242,28,121,150,217,26,224,238,46,192,178,30,143,127,143,121,17,25,98,42,96,166,155,128,236,50,152,53,213,166,2,166,163,162,107,212,211,235,48,187,142,124,16,161,47,39,151,29,
116,207,125,192,178,25,235,251,48,207,192,236,26,80,200,40,70,10,83,144,223,105,244,191,13,193,23,136,117,185,171,99,101,23,102,130,61,132,153,64,43,108,20,33,165,201,241,203,163,255,55,98,166,81,27,104,
202,166,238,79,221,125,43,22,183,97,38,131,28,230,225,228,251,224,23,242,224,101,178,116,198,112,189,126,170,29,5,136,81,177,0,94,22,113,142,100,68,124,137,233,98,100,252,42,2,223,70,67,122,11,86,208,
36,79,137,230,0,165,106,164,72,233,31,132,63,10,122,144,41,221,18,64,43,114,74,52,148,143,236,221,88,65,119,206,169,237,111,64,127,247,1,40,14,94,45,53,70,141,106,186,254,10,216,222,156,169,111,128,233,
75,86,64,219,218,45,163,97,147,143,212,129,169,151,19,235,23,169,135,247,224,135,249,129,74,92,178,225,156,159,201,64,225,226,5,248,113,229,92,200,95,248,7,15,107,170,117,5,234,103,205,129,197,223,244,
150,166,95,203,208,250,236,128,31,38,224,174,187,14,119,125,107,220,179,230,17,56,255,221,30,200,212,79,1,63,143,231,140,156,31,106,61,254,255,143,127,136,208,171,171,51,35,240,172,85,207,67,251,230,206,
210,156,78,235,174,82,162,85,59,45,228,46,85,2,94,6,127,176,195,78,231,4,186,124,95,216,38,180,168,45,5,112,68,166,145,217,171,203,193,210,35,131,206,55,199,210,149,225,91,47,63,79,83,186,166,212,40,96,
59,228,120,220,194,247,218,58,124,167,134,117,53,142,134,3,175,166,21,181,77,141,2,10,60,53,168,226,113,148,246,102,241,37,156,6,202,79,228,116,145,23,93,87,187,88,166,39,157,113,165,248,128,39,224,92,
92,65,106,59,195,10,196,7,220,246,232,214,251,31,135,153,247,61,6,244,16,192,108,229,116,241,55,172,54,243,19,142,147,102,31,61,112,188,27,206,238,222,5,151,127,61,206,60,115,124,179,208,125,56,13,35,
135,22,218,39,173,182,247,86,108,206,238,255,50,83,26,160,163,243,91,104,238,88,82,209,76,43,39,174,64,239,71,27,160,239,227,141,99,55,16,96,180,236,40,109,187,77,114,251,111,87,154,231,171,238,224,196,
74,219,179,59,118,29,128,230,5,139,241,137,156,221,191,107,207,158,152,158,193,179,80,91,122,144,50,119,205,91,112,241,231,35,112,110,255,87,230,23,177,40,115,122,180,33,221,246,110,26,190,9,54,13,225,
21,126,162,11,134,160,159,171,84,128,58,80,9,250,70,3,220,247,221,187,13,85,54,100,205,35,109,203,204,138,28,27,154,182,104,185,105,206,220,121,218,179,39,70,34,228,44,211,129,240,88,211,237,11,75,22,
244,59,71,4,141,163,245,112,235,100,182,121,90,136,187,88,141,189,62,202,16,20,222,176,192,35,8,210,1,78,42,186,88,128,143,249,195,10,5,129,243,144,166,218,80,32,30,224,149,98,177,63,177,94,58,113,20,
78,125,176,30,114,51,90,205,86,163,146,169,214,225,54,12,231,234,108,83,11,220,186,126,71,73,14,90,12,71,24,186,195,52,77,12,184,121,199,10,23,117,151,79,157,128,115,251,118,135,93,95,235,71,41,80,6,62,
170,62,174,175,137,1,119,14,210,93,75,41,51,165,81,231,114,39,202,168,210,195,142,81,164,30,62,149,222,20,79,54,37,14,220,205,239,254,208,160,14,233,33,44,241,109,113,243,242,130,63,148,252,59,8,145,182,
101,33,254,87,174,78,96,62,170,124,161,20,215,78,130,70,147,7,60,197,28,36,185,174,192,37,209,100,196,162,192,25,34,73,50,81,224,146,104,50,98,81,224,12,145,36,153,40,112,73,52,25,177,40,112,134,72,146,
76,20,184,36,154,140,88,20,56,67,36,73,38,10,92,18,77,70,44,10,156,33,146,36,19,5,46,137,38,35,22,5,206,16,73,146,137,2,151,68,147,17,139,2,103,136,36,201,68,129,75,162,201,136,69,129,51,68,146,100,162,
192,37,209,100,196,162,192,25,34,73,50,81,224,146,104,50,98,81,224,12,145,36,153,40,112,73,52,25,177,40,112,134,72,146,76,20,184,36,154,140,88,20,56,67,36,73,38,10,92,18,77,70,44,10,156,33,146,36,19,5,
46,137,38,35,22,5,206,16,73,146,137,2,151,68,147,17,139,2,103,136,36,201,68,129,75,162,201,136,69,129,51,68,146,100,162,192,37,209,100,196,162,192,25,34,73,50,81,224,146,104,50,98,81,224,12,145,36,153,
40,112,73,52,25,177,40,112,134,72,146,76,20,184,36,154,140,88,20,56,67,36,73,38,10,92,18,77,70,44,10,156,33,146,36,19,5,46,137,38,35,22,5,206,16,73,146,137,2,151,68,147,17,139,2,103,136,36,201,68,129,
75,162,201,136,69,129,51,68,146,100,162,192,37,209,100,196,162,192,25,34,73,50,81,224,146,104,50,98,81,224,12,145,36,153,40,112,73,52,25,177,40,112,134,72,146,76,20,184,36,154,140,88,20,56,67,36,73,38,
10,92,18,77,70,44,10,156,33,146,36,19,5,46,137,38,35,22,5,206,16,73,146,137,2,151,68,147,17,139,2,103,136,36,201,68,129,75,162,201,136,69,129,51,68,146,100,162,192,37,209,100,196,162,192,25,34,73,50,153,
60,224,190,47,73,183,100,98,153,4,141,234,146,241,60,208,170,231,153,47,94,174,30,160,144,15,28,208,143,78,1,47,147,129,98,126,8,188,92,206,85,37,86,38,14,188,112,241,130,113,190,120,245,114,98,65,164,
189,97,55,246,21,6,250,19,15,37,49,224,94,38,107,156,111,108,107,135,153,43,86,65,110,70,43,248,197,66,226,1,165,243,2,30,248,216,195,179,77,45,137,187,159,24,112,192,97,138,210,212,246,14,152,255,254,
151,137,7,34,238,2,118,42,140,59,174,120,128,143,229,28,46,68,124,157,187,121,220,80,71,47,27,15,146,176,11,198,210,186,155,167,43,94,132,130,168,75,126,49,82,241,218,90,121,141,2,145,182,101,110,78,238,
239,62,96,26,54,96,39,97,107,113,77,20,130,43,220,232,56,112,188,187,20,37,77,149,17,52,142,214,195,139,69,0,236,193,131,103,127,135,127,127,216,15,211,239,186,183,180,48,35,135,198,26,230,5,3,138,53,
52,212,209,141,142,167,119,190,99,154,166,197,176,79,186,79,48,69,3,78,23,37,176,232,216,177,23,86,192,157,123,78,64,195,220,121,19,116,69,79,11,83,160,247,195,55,225,239,189,159,153,195,180,154,143,146,
162,3,183,189,156,160,119,61,220,14,173,43,159,48,219,176,250,89,115,74,67,143,246,244,170,249,224,170,199,140,148,3,61,93,240,231,231,59,225,74,223,201,170,219,8,59,193,243,49,217,131,84,210,99,49,83,
210,220,113,104,161,93,108,217,94,28,214,136,169,231,216,140,217,128,30,12,85,128,230,237,241,134,241,128,254,203,142,58,164,37,150,216,174,99,11,209,123,184,243,146,238,27,188,168,121,224,18,184,184,
59,172,229,4,20,64,29,205,194,120,60,216,85,52,29,31,112,186,40,66,119,171,202,42,124,80,211,73,84,32,210,182,108,18,253,212,75,197,164,128,2,143,73,200,180,52,163,192,211,66,42,38,63,67,129,123,94,232,
161,152,46,173,205,92,15,5,194,169,226,86,32,219,216,100,124,114,63,117,94,15,7,245,154,60,5,220,143,46,245,55,220,84,58,161,188,219,30,121,62,173,210,175,96,110,192,236,246,106,30,173,180,169,129,155,
95,123,15,78,110,90,109,86,222,238,17,223,200,211,245,91,77,40,64,187,35,251,4,238,150,215,183,27,151,104,59,135,12,75,175,27,13,179,61,67,192,247,98,126,20,51,189,157,96,122,188,187,91,102,63,245,18,
92,253,163,23,126,219,249,118,185,65,180,209,84,163,10,180,173,221,2,173,15,60,105,188,115,12,173,171,142,237,94,122,210,246,32,86,126,141,153,158,200,87,28,226,135,206,255,5,3,63,117,161,133,190,177,
98,5,172,169,130,224,182,220,113,55,100,155,167,135,249,229,70,239,123,76,151,71,232,125,104,73,131,63,189,101,56,242,97,140,125,130,22,214,146,214,215,144,2,149,89,57,166,199,60,207,235,112,192,151,162,
219,7,173,235,238,110,8,70,66,189,95,187,119,80,145,218,251,76,47,17,142,30,161,131,44,23,32,240,30,26,210,177,52,191,161,172,195,19,54,217,56,66,135,247,218,139,83,61,10,81,32,200,240,101,100,188,3,89,
219,55,13,241,12,172,216,140,197,187,246,100,186,83,104,40,160,147,232,46,209,148,14,5,136,21,49,35,118,174,183,175,35,216,228,62,150,69,183,108,167,239,38,225,93,240,44,126,232,196,28,156,203,169,1,5,
111,20,170,217,63,196,50,200,140,254,17,224,25,132,252,69,208,227,107,128,211,65,234,250,88,108,197,252,28,230,217,152,53,165,71,1,90,128,119,34,232,13,149,92,254,15,115,204,62,254,225,220,237,132,0,0,
0,0,73,69,78,68,174,66,96,130,0,0};

const char* PlayerMainMenuComponent::stopOn_png = (const char*) resource_PlayerMainMenuComponent_stopOn_png;
const int PlayerMainMenuComponent::stopOn_pngSize = 3436;


//[EndFile] You can add extra defines here...
//[/EndFile]

