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

#include "boost/lexical_cast.hpp"

#include "MiamExceptions.h"
#include "TextUtils.h"

//[/Headers]

#include "MainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    virtualMidiDeviceName = "MIEM bridge - OSC to MIDI";
    //[/Constructor_pre]

    listenLabel1.reset (new Label ("new label",
                                   TRANS("LISTEN OR NOT INFO MESSAGE")));
    addAndMakeVisible (listenLabel1.get());
    listenLabel1->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    listenLabel1->setJustificationType (Justification::centredLeft);
    listenLabel1->setEditable (false, false, false);
    listenLabel1->setColour (TextEditor::textColourId, Colours::black);
    listenLabel1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    listenLabel2.reset (new Label ("new label",
                                   TRANS("i (as a part of the OSC address) is a number between 0 and 127, and will be the MIDI CC number")));
    addAndMakeVisible (listenLabel2.get());
    listenLabel2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    listenLabel2->setJustificationType (Justification::centredLeft);
    listenLabel2->setEditable (false, false, false);
    listenLabel2->setColour (Label::textColourId, Colour (0xffc1c1c1));
    listenLabel2->setColour (TextEditor::textColourId, Colours::black);
    listenLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    listenLabel3.reset (new Label ("new label",
                                   TRANS("f is a float between 0.0 and 1.0, and will be transformed into a MIDI CC value between 0 and 127")));
    addAndMakeVisible (listenLabel3.get());
    listenLabel3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    listenLabel3->setJustificationType (Justification::centredLeft);
    listenLabel3->setEditable (false, false, false);
    listenLabel3->setColour (Label::textColourId, Colour (0xffc1c1c1));
    listenLabel3->setColour (TextEditor::textColourId, Colours::black);
    listenLabel3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    connectButton.reset (new TextButton ("Connect button"));
    addAndMakeVisible (connectButton.get());
    connectButton->setButtonText (TRANS("Restart bridge"));
    connectButton->addListener (this);
    connectButton->setColour (TextButton::buttonColourId, Colour (0xff616161));

    connectButton->setBounds (320, 40, 150, 24);

    udpPortLabel.reset (new Label ("udp port label",
                                   TRANS("UDP port for receiving OSC:")));
    addAndMakeVisible (udpPortLabel.get());
    udpPortLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    udpPortLabel->setJustificationType (Justification::centredLeft);
    udpPortLabel->setEditable (false, false, false);
    udpPortLabel->setColour (TextEditor::textColourId, Colours::black);
    udpPortLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    udpPortLabel->setBounds (40, 40, 192, 24);

    notConnectedLabel.reset (new Label ("new label",
                                        TRANS("Not connected!")));
    addAndMakeVisible (notConnectedLabel.get());
    notConnectedLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    notConnectedLabel->setJustificationType (Justification::centredRight);
    notConnectedLabel->setEditable (false, false, false);
    notConnectedLabel->setColour (Label::textColourId, Colour (0xffff9c50));
    notConnectedLabel->setColour (TextEditor::textColourId, Colours::black);
    notConnectedLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    udpPortTextEditor.reset (new TextEditor ("udp port text editor"));
    addAndMakeVisible (udpPortTextEditor.get());
    udpPortTextEditor->setMultiLine (false);
    udpPortTextEditor->setReturnKeyStartsNewLine (false);
    udpPortTextEditor->setReadOnly (false);
    udpPortTextEditor->setScrollbarsShown (true);
    udpPortTextEditor->setCaretVisible (true);
    udpPortTextEditor->setPopupMenuEnabled (true);
    udpPortTextEditor->setColour (TextEditor::backgroundColourId, Colour (0xff616161));
    udpPortTextEditor->setColour (TextEditor::outlineColourId, Colours::white);
    udpPortTextEditor->setText (TRANS("28012"));

    udpPortTextEditor->setBounds (224, 40, 79, 24);

    displayLogButton.reset (new TextButton ("display log button"));
    addAndMakeVisible (displayLogButton.get());
    displayLogButton->setButtonText (TRANS("Display OSC-MIDI"));
    displayLogButton->addListener (this);
    displayLogButton->setColour (TextButton::buttonColourId, Colour (0xff616161));

    logTextEditor.reset (new TextEditor ("log text editor"));
    addAndMakeVisible (logTextEditor.get());
    logTextEditor->setMultiLine (true);
    logTextEditor->setReturnKeyStartsNewLine (true);
    logTextEditor->setReadOnly (true);
    logTextEditor->setScrollbarsShown (true);
    logTextEditor->setCaretVisible (false);
    logTextEditor->setPopupMenuEnabled (true);
    logTextEditor->setColour (TextEditor::textColourId, Colour (0xffc1c1c1));
    logTextEditor->setColour (TextEditor::backgroundColourId, Colour (0xff202020));
    logTextEditor->setText (String());

    forwardingLabel.reset (new Label ("forwarding label",
                                      TRANS("Forwarding messages as MIDI CC to virtual device ")));
    addAndMakeVisible (forwardingLabel.get());
    forwardingLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    forwardingLabel->setJustificationType (Justification::centredLeft);
    forwardingLabel->setEditable (false, false, false);
    forwardingLabel->setColour (TextEditor::textColourId, Colours::black);
    forwardingLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    baseOscListeningString = " to '/miem/i f' OSC messages on port ";
    baseOscForwardingString = forwardingLabel->getText().toStdString();

    forwardingLabel->setText(baseOscForwardingString + "'" + virtualMidiDeviceName + "'",
                             NotificationType::sendNotification);

    //[/UserPreSize]

    setSize (800, 400);


    //[Constructor] You can add your own custom stuff here..

    // simulation de clic sur le bouton
    // (pour forcer une tentative de connection dès le démarrage)
    buttonClicked(connectButton.get());

    // display caché pour l'instant...
    logTextEditor->setVisible(false);
    displayLogButton->setVisible(false);

    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    listenLabel1 = nullptr;
    listenLabel2 = nullptr;
    listenLabel3 = nullptr;
    connectButton = nullptr;
    udpPortLabel = nullptr;
    notConnectedLabel = nullptr;
    udpPortTextEditor = nullptr;
    displayLogButton = nullptr;
    logTextEditor = nullptr;
    forwardingLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff303030));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    // forced updates...
    listenLabel1->setVisible(isConnected);
    listenLabel2->setVisible(isConnected);
    listenLabel3->setVisible(isConnected);
    notConnectedLabel->setVisible(!isConnected);



    //[/UserPreResize]

    listenLabel1->setBounds (40, 88, getWidth() - 120, 24);
    listenLabel2->setBounds (40, 144, getWidth() - 80, 24);
    listenLabel3->setBounds (40, 168, getWidth() - 80, 24);
    notConnectedLabel->setBounds (getWidth() - 40 - 400, 88, 400, 24);
    displayLogButton->setBounds (40, (getHeight() / 2) + 85, 150, 24);
    logTextEditor->setBounds (240, 240, getWidth() - 280, getHeight() - 254);
    forwardingLabel->setBounds (40, 112, getWidth() - 94, 24);
    //[UserResized] Add your own custom resize handling here..

    //[/UserResized]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == connectButton.get())
    {
        //[UserButtonCode_connectButton] -- add your button handler code here..

        // D'abord, tentative de parse de l'info de port UDP
        int udpPort = -1;
        try {
            udpPort = TextUtils::ParseUdpPortNumber(udpPortTextEditor->getText().toStdString());
        }
        catch (Miam::ParseException&) {
            udpPort = -1;
            udpPortTextEditor->setText(""); // vidage forcé du contenu erroné
        }

        // Ensuite, tentative de connection (avec déconnection dans tous les cas...)
        oscListenerForwarder.reset(nullptr);
        OSCListenerForwarder* newListenerForwarder = nullptr;
        if (udpPort >= 0)
        {
            try {
                newListenerForwarder = new OSCListenerForwarder(udpPort,
                                                                MiemClock::now(),
                                                                virtualMidiDeviceName,
                                                                this);
            }
            catch (std::runtime_error& e) {
                newListenerForwarder = nullptr;
            }
        }
        isConnected = (newListenerForwarder != nullptr);
        oscListenerForwarder.reset(newListenerForwarder);

        // Affichages et mises à jour au final
        //connectButton->setEnabled(! isConnected);
        listenLabel1->setText((isConnected ? String("Listening") : String("Not listening..."))
                              + baseOscListeningString
                              + boost::lexical_cast<std::string>(udpPort),
                              NotificationType::sendNotification);

        resized();

        //[/UserButtonCode_connectButton]
    }
    else if (buttonThatWasClicked == displayLogButton.get())
    {
        //[UserButtonCode_displayLogButton] -- add your button handler code here..
        //[/UserButtonCode_displayLogButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void  MainComponent::OnMessageThroughBridge(const MiemExpeSample& oscSample,
                                    const MiemMidiSample& midiSample)
{
    std::string bridgeTransformation = "/miem/";
    bridgeTransformation += boost::lexical_cast<std::string>(oscSample.parameterIndex);
    bridgeTransformation += " ";
    bridgeTransformation += boost::lexical_cast<std::string>(oscSample.value);
    bridgeTransformation += "   -->   MIDI CC ";
    bridgeTransformation += boost::lexical_cast<std::string>(midiSample.parameterIndex);
    bridgeTransformation += " value ";
    bridgeTransformation += boost::lexical_cast<std::string>(midiSample.value);
    std::cout << bridgeTransformation << std::endl;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public Component, public OSCListenerForwarder::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="800"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff303030"/>
  <LABEL name="new label" id="10cef6d217b0bd64" memberName="listenLabel1"
         virtualName="" explicitFocusOrder="0" pos="40 88 120M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="LISTEN OR NOT INFO MESSAGE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="3de9cbbfa9060fd1" memberName="listenLabel2"
         virtualName="" explicitFocusOrder="0" pos="40 144 80M 24" textCol="ffc1c1c1"
         edTextCol="ff000000" edBkgCol="0" labelText="i (as a part of the OSC address) is a number between 0 and 127, and will be the MIDI CC number"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="2a6ba04889c45e9d" memberName="listenLabel3"
         virtualName="" explicitFocusOrder="0" pos="40 168 80M 24" textCol="ffc1c1c1"
         edTextCol="ff000000" edBkgCol="0" labelText="f is a float between 0.0 and 1.0, and will be transformed into a MIDI CC value between 0 and 127"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="Connect button" id="e06f44f463dfb38d" memberName="connectButton"
              virtualName="" explicitFocusOrder="0" pos="320 40 150 24" bgColOff="ff616161"
              buttonText="Restart bridge" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="udp port label" id="7567e69b878a823b" memberName="udpPortLabel"
         virtualName="" explicitFocusOrder="0" pos="40 40 192 24" edTextCol="ff000000"
         edBkgCol="0" labelText="UDP port for receiving OSC:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="9dcfef8854cae5e5" memberName="notConnectedLabel"
         virtualName="" explicitFocusOrder="0" pos="40Rr 88 400 24" textCol="ffff9c50"
         edTextCol="ff000000" edBkgCol="0" labelText="Not connected!"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="1"
         italic="0" justification="34" typefaceStyle="Bold"/>
  <TEXTEDITOR name="udp port text editor" id="1e7dc36d6b22994b" memberName="udpPortTextEditor"
              virtualName="" explicitFocusOrder="0" pos="224 40 79 24" bkgcol="ff616161"
              outlinecol="ffffffff" initialText="28012" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="display log button" id="fd6f8dbc16ea7993" memberName="displayLogButton"
              virtualName="" explicitFocusOrder="0" pos="40 85C 150 24" bgColOff="ff616161"
              buttonText="Display OSC-MIDI" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTEDITOR name="log text editor" id="d4304e642d982fa2" memberName="logTextEditor"
              virtualName="" explicitFocusOrder="0" pos="240 240 280M 254M"
              textcol="ffc1c1c1" bkgcol="ff202020" initialText="" multiline="1"
              retKeyStartsLine="1" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <LABEL name="forwarding label" id="89727b1b4fbebdd0" memberName="forwardingLabel"
         virtualName="" explicitFocusOrder="0" pos="40 112 94M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Forwarding messages as MIDI CC to virtual device "
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

