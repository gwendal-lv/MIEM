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
#include "XmlUtils.h"
#include "boost/lexical_cast.hpp"

using namespace Miam;
//[/Headers]

#include "LoggerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LoggerComponent::LoggerComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor.get());
    textEditor->setMultiLine (true);
    textEditor->setReturnKeyStartsNewLine (true);
    textEditor->setReadOnly (true);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (false);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    textEditor->setColour (TextEditor::outlineColourId, Colours::white);
    textEditor->setText (String());

    ipTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (ipTextEditor.get());
    ipTextEditor->setMultiLine (false);
    ipTextEditor->setReturnKeyStartsNewLine (false);
    ipTextEditor->setReadOnly (false);
    ipTextEditor->setScrollbarsShown (true);
    ipTextEditor->setCaretVisible (true);
    ipTextEditor->setPopupMenuEnabled (true);
    ipTextEditor->setColour (TextEditor::backgroundColourId, Colour (0xff454545));
    ipTextEditor->setText (String());

    ipTextEditor->setBounds (40, 8, 112, 24);

    ipLabel.reset (new Label ("new label",
                              TRANS("IP")));
    addAndMakeVisible (ipLabel.get());
    ipLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    ipLabel->setJustificationType (Justification::centredLeft);
    ipLabel->setEditable (false, false, false);
    ipLabel->setColour (TextEditor::textColourId, Colours::black);
    ipLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ipLabel->setBounds (8, 8, 40, 24);

    portLabel.reset (new Label ("new label",
                                TRANS("TCP port\n")));
    addAndMakeVisible (portLabel.get());
    portLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    portLabel->setJustificationType (Justification::centredLeft);
    portLabel->setEditable (false, false, false);
    portLabel->setColour (TextEditor::textColourId, Colours::black);
    portLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    portLabel->setBounds (8, 48, 80, 24);

    portTextEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (portTextEditor.get());
    portTextEditor->setMultiLine (false);
    portTextEditor->setReturnKeyStartsNewLine (false);
    portTextEditor->setReadOnly (false);
    portTextEditor->setScrollbarsShown (true);
    portTextEditor->setCaretVisible (true);
    portTextEditor->setPopupMenuEnabled (true);
    portTextEditor->setColour (TextEditor::backgroundColourId, Colour (0xff454545));
    portTextEditor->setText (String());

    portTextEditor->setBounds (96, 48, 56, 24);

    connectButton.reset (new TextButton ("new button"));
    addAndMakeVisible (connectButton.get());
    connectButton->setButtonText (TRANS("Connect"));
    connectButton->addListener (this);
    connectButton->setColour (TextButton::buttonColourId, Colour (0xff858585));

    connectButton->setBounds (160, 8, 96, 24);

    disconnectButton.reset (new TextButton ("new button"));
    addAndMakeVisible (disconnectButton.get());
    disconnectButton->setButtonText (TRANS("Disconnect"));
    disconnectButton->addListener (this);
    disconnectButton->setColour (TextButton::buttonColourId, Colour (0xff858585));

    disconnectButton->setBounds (264, 8, 96, 24);

    clearButton.reset (new TextButton ("new button"));
    addAndMakeVisible (clearButton.get());
    clearButton->setButtonText (TRANS("Clear log"));
    clearButton->addListener (this);
    clearButton->setColour (TextButton::buttonColourId, Colour (0xff858585));
    clearButton->setColour (TextButton::textColourOffId, Colours::black);

    clearButton->setBounds (216, 48, 96, 24);

    statusLabel.reset (new Label ("new label",
                                  TRANS("connection status")));
    addAndMakeVisible (statusLabel.get());
    statusLabel->setFont (Font (15.00f, Font::italic));
    statusLabel->setJustificationType (Justification::centred);
    statusLabel->setEditable (false, false, false);
    statusLabel->setColour (TextEditor::textColourId, Colours::black);
    statusLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    statusLabel->setBounds (8, 88, 344, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    // quite large scrollbar for iOS/Android
    textEditor->setScrollBarThickness(16);

    textEditor->addListener(this);
    ipTextEditor->addListener(this);
    portTextEditor->addListener(this);

    // TEMP for debug
    ipTextEditor->setText("10.97.198.140");
    portTextEditor->setText(String(19192));

    update();
    //[/Constructor]
}

LoggerComponent::~LoggerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;
    ipTextEditor = nullptr;
    ipLabel = nullptr;
    portLabel = nullptr;
    portTextEditor = nullptr;
    connectButton = nullptr;
    disconnectButton = nullptr;
    clearButton = nullptr;
    statusLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LoggerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff232323));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LoggerComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textEditor->setBounds ((getWidth() / 2) - ((getWidth() - 40) / 2), 128, getWidth() - 40, getHeight() - 148);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LoggerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == connectButton.get())
    {
        //[UserButtonCode_connectButton] -- add your button handler code here..
        tryToConnect = true;
        connectionWasUserDisconnected = false;
        connectionWasLost = false;
        // very small delay...
        Timer::callAfterDelay(10, [this] { tryConnect(); });
        //[/UserButtonCode_connectButton]
    }
    else if (buttonThatWasClicked == disconnectButton.get())
    {
        //[UserButtonCode_disconnectButton] -- add your button handler code here..
        connectionWasUserDisconnected = true;
        disconnect();
        couldNotConnect = false;
        tryToConnect = false;
        //[/UserButtonCode_disconnectButton]
    }
    else if (buttonThatWasClicked == clearButton.get())
    {
        //[UserButtonCode_clearButton] -- add your button handler code here..
        couldNotConnect = false;
        connectionWasLost = false;
        receivedLog.clear();
        //[/UserButtonCode_clearButton]
    }

    //[UserbuttonClicked_Post]
    update();
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void LoggerComponent::textEditorTextChanged (TextEditor & textEditor)
{
    if (&textEditor == ipTextEditor.get())
    {
    }
    else if (&textEditor == portTextEditor.get())
    {
    }

    // for any text editor
    update();
}


void LoggerComponent::update()
{
    ipTextEditor->setEnabled( (!isConnected) && (!tryToConnect) );
    portTextEditor->setEnabled( (!isConnected) && (!tryToConnect) );

    bool isConnectionPossible;
    try {
        auto tcpPort = boost::lexical_cast<int>(portTextEditor->getText().toStdString());
        if (tcpPort < 0 || tcpPort > 65000)
            throw std::exception();
        isConnectionPossible = true;
    }
    catch (std::exception &) {
        isConnectionPossible = false;
    }
    isConnectionPossible = ( isConnectionPossible && XmlUtils::IsIpv4AddressValid(ipTextEditor->getText().toStdString()));

    connectButton->setEnabled( isConnectionPossible
                              && (! isConnected)
                              && (! tryToConnect));
    disconnectButton->setEnabled(isConnected || tryToConnect);

    if (tryToConnect)
    {
        if (connectionWasLost)
            statusLabel->setText("Connection lost! Trying to re-connect...", NotificationType::sendNotification);
        else
            statusLabel->setText("Trying to connect...", NotificationType::sendNotification);
    }
    else if (connectionWasLost)
            statusLabel->setText("Connection lost!", NotificationType::sendNotification);
    else if (couldNotConnect)
        statusLabel->setText("Cannot connect to server.", NotificationType::sendNotification);
    else if (isConnected)
        statusLabel->setText("Connected.", NotificationType::sendNotification);
    else
        statusLabel->setText("Not connected.", NotificationType::sendNotification);

    if (connectionWasLost)
        statusLabel->setColour(Label::ColourIds::textColourId, Colours::orangered);
    else if (isConnected)
        statusLabel->setColour(Label::ColourIds::textColourId, Colours::limegreen);
    else if ((!isConnected) && (! tryToConnect))
        statusLabel->setColour(Label::ColourIds::textColourId, Colours::grey);
    else
        statusLabel->setColour(Label::ColourIds::textColourId, Colours::white);

    textEditor->setText(receivedLog);
}


void LoggerComponent::tryConnect()
{
    if (tryToConnect)
    {
        couldNotConnect = ! connectToSocket(ipTextEditor->getText(),
                                            boost::lexical_cast<int>(portTextEditor->getText()),
                                            timeOut_ms); // timeout
        if (couldNotConnect)
            Timer::callAfterDelay(timeOut_ms, [this] { tryConnect(); });
    }
    update();
}


void LoggerComponent::connectionMade ()
{
    std::cout << "connection made" << std::endl;
    connectionWasLost = false;
    isConnected = true;
    couldNotConnect = false;
    tryToConnect = false;
    update();
}
void LoggerComponent::connectionLost ()
{
    std::cout << "connection lost" << std::endl;
    isConnected = false;
    couldNotConnect = false;
    tryToConnect = ! connectionWasUserDisconnected;
    connectionWasLost = ! connectionWasUserDisconnected;
    Timer::callAfterDelay(10, [this] { tryConnect(); }); // very short delay
    update();
}
void LoggerComponent::messageReceived (const MemoryBlock &message)
{
    // re-transformation du message en std::string via char*
    std::string lastString;
    lastString.resize(message.getSize());
    // copie des caractères 1 par 1...
    // Au cas où l'on a pas de caractère de fin de chaîne...
    for (size_t i=0 ; i<message.getSize() ; i++)
        lastString[i] = (char) message[i];

    receivedLog += lastString;
    receivedLog += '\n';

    // ajout du texte et update graphique
    textEditor->setText(receivedLog);
    textEditor->applyFontToAllText(Font(Font::getDefaultMonospacedFontName(),
                                        16.0,
                                        Font::plain));
    textEditor->scrollEditorToPositionCaret(0, 1000000000);
    update();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LoggerComponent" componentName=""
                 parentClasses="public Component, public TextEditor::Listener, public InterprocessConnection"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff232323"/>
  <TEXTEDITOR name="new text editor" id="c334ceb0628275d" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="0Cc 128 40M 148M"
              bkgcol="0" outlinecol="ffffffff" initialText="" multiline="1"
              retKeyStartsLine="1" readonly="1" scrollbars="1" caret="0" popupmenu="0"/>
  <TEXTEDITOR name="new text editor" id="ae28c7f281366300" memberName="ipTextEditor"
              virtualName="" explicitFocusOrder="0" pos="40 8 112 24" bkgcol="ff454545"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="87207a51d1c3cef7" memberName="ipLabel" virtualName=""
         explicitFocusOrder="0" pos="8 8 40 24" edTextCol="ff000000" edBkgCol="0"
         labelText="IP" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="1ab36cd4f26b1824" memberName="portLabel"
         virtualName="" explicitFocusOrder="0" pos="8 48 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="TCP port&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="69536ead7adf5efa" memberName="portTextEditor"
              virtualName="" explicitFocusOrder="0" pos="96 48 56 24" bkgcol="ff454545"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="7d9d7b0070a5b9ec" memberName="connectButton"
              virtualName="" explicitFocusOrder="0" pos="160 8 96 24" bgColOff="ff858585"
              buttonText="Connect" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="686f67c00ff9865f" memberName="disconnectButton"
              virtualName="" explicitFocusOrder="0" pos="264 8 96 24" bgColOff="ff858585"
              buttonText="Disconnect" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="823f15096c8b976d" memberName="clearButton"
              virtualName="" explicitFocusOrder="0" pos="216 48 96 24" bgColOff="ff858585"
              textCol="ff000000" buttonText="Clear log" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="3b7188284f2c6258" memberName="statusLabel"
         virtualName="" explicitFocusOrder="0" pos="8 88 344 24" edTextCol="ff000000"
         edBkgCol="0" labelText="connection status" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="1" justification="36"
         typefaceStyle="Italic"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

