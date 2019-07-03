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
#include "../JuceLibraryCode/JuceHeader.h"

#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include "MyPacaranaManager.h"

#include "OscConnector.h"

//[/Headers]

#include "MyOscConnector.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

namespace Miam {

	void MyOscConnector::init(OscConnector * refConnect)
	{
		connector = refConnect;
	}

	//visual improvement
	void MyOscConnector::updateButtonsAndText()
	{
		Colour statusColor = Colours::white;
		String statusText = "empty";
		String ButtonText = "empty";

		// out
		if (connector->isConnectedOut())
		{
			statusColor = Colours::green;
			statusText = "Connected to UPD port " + String(connector->getOutPort()) + " at IP " + connector->getIp();
			ButtonText = "Disconnect";
		}
		else {
			statusColor = Colours::red;
			statusText = "Disconnected";
			ButtonText = "Connect";
		}

		label_statusOut->setColour(Label::textColourId, statusColor);
		label_statusOut->setText(statusText, dontSendNotification);

		// in
		if (connector->isConnectedIn())
		{
			statusColor = Colours::green;
			statusText = "Listening to port " + String(connector->getInPort());
			ButtonText = "Disconnect";
		}
		else {
			statusColor = Colours::red;
			statusText = "Disconnected";
			ButtonText = "Connect";
		}

		label_statusIn->setColour(Label::textColourId, statusColor);
		label_statusIn->setText(statusText, dontSendNotification);

	}
	void MyOscConnector::addToLog(TextEditor* tE, String str) {
		String oldText = (*tE).getText();
		(*tE).setText(str + "\n" + oldText);

	}

	//// error lanagement, from tutorial
	//void MyOscConnector::handleConnectError(int failedPort) {
	//	AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
	//		"OSC Connection error",
	//		"Error: could not connect to port " + String(failedPort),
	//		"OK");
	//}
	//void MyOscConnector::handleDisconnectError() {
	//	AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
	//		"Unknown error",
	//		"An unknown error occured while trying to disconnect from UDP port.",
	//		"OK");
	//}
	//void MyOscConnector::handleInvalidPortNumberEntered() {
	//	AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
	//		"Invalid port number",
	//		"Error: you have entered an invalid UDP port number.",
	//		"OK");
	//}



//[/MiscUserDefs]

//==============================================================================
MyOscConnector::MyOscConnector ()
{
    //[Constructor_pre] You can add your own custom stuff here..

    //[/Constructor_pre]

    title.reset (new Label ("the title",
                            TRANS("Osc connector")));
    addAndMakeVisible (title.get());
    title->setFont (Font (25.00f, Font::plain).withTypefaceStyle ("Regular"));
    title->setJustificationType (Justification::centredLeft);
    title->setEditable (false, false, false);
    title->setColour (TextEditor::textColourId, Colours::black);
    title->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    title->setBounds (8, 8, 150, 24);

    label_portOut.reset (new Label ("new label",
                                    TRANS("Out port : ")));
    addAndMakeVisible (label_portOut.get());
    label_portOut->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label_portOut->setJustificationType (Justification::centredLeft);
    label_portOut->setEditable (false, false, false);
    label_portOut->setColour (TextEditor::textColourId, Colours::black);
    label_portOut->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label_portOut->setBounds (160, 48, 80, 24);

    label_statusIn.reset (new Label ("the status",
                                     TRANS("Status : XXXX")));
    addAndMakeVisible (label_statusIn.get());
    label_statusIn->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    label_statusIn->setJustificationType (Justification::centredLeft);
    label_statusIn->setEditable (false, false, false);
    label_statusIn->setColour (TextEditor::textColourId, Colours::black);
    label_statusIn->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label_statusIn->setBounds (288, 16, 312, 24);

    label_portIn.reset (new Label ("new label",
                                   TRANS("In port : ")));
    addAndMakeVisible (label_portIn.get());
    label_portIn->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label_portIn->setJustificationType (Justification::centredLeft);
    label_portIn->setEditable (false, false, false);
    label_portIn->setColour (TextEditor::textColourId, Colours::black);
    label_portIn->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label_portIn->setBounds (160, 16, 80, 24);

    label_statusOut.reset (new Label ("the status",
                                      TRANS("Status : XXXX")));
    addAndMakeVisible (label_statusOut.get());
    label_statusOut->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    label_statusOut->setJustificationType (Justification::centredLeft);
    label_statusOut->setEditable (false, false, false);
    label_statusOut->setColour (TextEditor::textColourId, Colours::black);
    label_statusOut->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label_statusOut->setBounds (288, 48, 312, 24);

    group_sent.reset (new GroupComponent ("new group",
                                          TRANS("Sent")));
    addAndMakeVisible (group_sent.get());

    group_sent->setBounds (0, 152, 300, 248);

    group_received.reset (new GroupComponent ("new group",
                                              TRANS("Received")));
    addAndMakeVisible (group_received.get());

    messageSent.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (messageSent.get());
    messageSent->setMultiLine (true);
    messageSent->setReturnKeyStartsNewLine (true);
    messageSent->setReadOnly (true);
    messageSent->setScrollbarsShown (true);
    messageSent->setCaretVisible (false);
    messageSent->setPopupMenuEnabled (true);
    messageSent->setColour (TextEditor::backgroundColourId, Colour (0x00495358));
    messageSent->setText (TRANS("Here will be sent messages"));

    messageSent->setBounds (16, 176, 270, 208);

    messageReceived.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (messageReceived.get());
    messageReceived->setMultiLine (true);
    messageReceived->setReturnKeyStartsNewLine (true);
    messageReceived->setReadOnly (true);
    messageReceived->setScrollbarsShown (true);
    messageReceived->setCaretVisible (false);
    messageReceived->setPopupMenuEnabled (true);
    messageReceived->setColour (TextEditor::backgroundColourId, Colour (0x00495358));
    messageReceived->setText (TRANS("Here will be received messages"));

    portIn_line.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (portIn_line.get());
    portIn_line->setMultiLine (false);
    portIn_line->setReturnKeyStartsNewLine (false);
    portIn_line->setReadOnly (false);
    portIn_line->setScrollbarsShown (true);
    portIn_line->setCaretVisible (true);
    portIn_line->setPopupMenuEnabled (true);
    portIn_line->setText (TRANS("9000"));

    portIn_line->setBounds (232, 16, 48, 24);

    portOut_line.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (portOut_line.get());
    portOut_line->setMultiLine (false);
    portOut_line->setReturnKeyStartsNewLine (false);
    portOut_line->setReadOnly (false);
    portOut_line->setScrollbarsShown (true);
    portOut_line->setCaretVisible (true);
    portOut_line->setPopupMenuEnabled (true);
    portOut_line->setText (TRANS("8000"));

    portOut_line->setBounds (232, 48, 48, 24);

    infoLabel.reset (new Label ("new label",
                                TRANS("Info will go here")));
    addAndMakeVisible (infoLabel.get());
    infoLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    infoLabel->setJustificationType (Justification::topLeft);
    infoLabel->setEditable (false, false, false);
    infoLabel->setColour (Label::backgroundColourId, Colour (0xbf272727));
    infoLabel->setColour (TextEditor::textColourId, Colours::black);
    infoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    infoLabel->setBounds (8, 88, 150, 56);

    box_ip.reset (new TextEditor ("the ip adress"));
    addAndMakeVisible (box_ip.get());
    box_ip->setMultiLine (false);
    box_ip->setReturnKeyStartsNewLine (false);
    box_ip->setReadOnly (false);
    box_ip->setScrollbarsShown (true);
    box_ip->setCaretVisible (true);
    box_ip->setPopupMenuEnabled (true);
    box_ip->setText (TRANS("localhost"));

    box_ip->setBounds (8, 48, 150, 24);

    comboButton.reset (new TextButton ("new button"));
    addAndMakeVisible (comboButton.get());
    comboButton->setButtonText (TRANS("Go !"));
    comboButton->addListener (this);
    comboButton->setColour (TextButton::buttonColourId, Colours::green);

    comboButton->setBounds (344, 72, 96, 72);

    StopButton.reset (new TextButton ("new button"));
    addAndMakeVisible (StopButton.get());
    StopButton->setButtonText (TRANS("Stop"));
    StopButton->addListener (this);
    StopButton->setColour (TextButton::buttonColourId, Colour (0xffa40000));

    StopButton->setBounds (448, 72, 96, 72);


    //[UserPreSize]

    //[/UserPreSize]

    setSize (605, 400);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

MyOscConnector::~MyOscConnector()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    //[/Destructor_pre]

    title = nullptr;
    label_portOut = nullptr;
    label_statusIn = nullptr;
    label_portIn = nullptr;
    label_statusOut = nullptr;
    group_sent = nullptr;
    group_received = nullptr;
    messageSent = nullptr;
    messageReceived = nullptr;
    portIn_line = nullptr;
    portOut_line = nullptr;
    infoLabel = nullptr;
    box_ip = nullptr;
    comboButton = nullptr;
    StopButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MyOscConnector::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MyOscConnector::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    group_received->setBounds (604 - 300, 152, 300, 248);
    messageReceived->setBounds (590 - 270, 176, 270, 208);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MyOscConnector::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == comboButton.get())
    {
        //[UserButtonCode_comboButton] -- add your button handler code here..

			box_ip->setText("169.254.119.246");
			connector->setIpAddress(box_ip->getText().toStdString());
			connector->setInPort(portIn_line->getTextValue().getValue());
			connector->setOutPort(portOut_line->getTextValue().getValue());

			connector->start();

			comboButton->setEnabled(false);
			StopButton->setEnabled(true);

        //[/UserButtonCode_comboButton]
    }
    else if (buttonThatWasClicked == StopButton.get())
    {
        //[UserButtonCode_StopButton] -- add your button handler code here..

			connector->disconnectIn();
			connector->disconnectOut();

			comboButton->setEnabled(true);
			StopButton->setEnabled(false);
        //[/UserButtonCode_StopButton]
    }

    //[UserbuttonClicked_Post]

		updateButtonsAndText();

    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...





//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MyOscConnector" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="605" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="the title" id="bed4f37846c2d3c" memberName="title" virtualName=""
         explicitFocusOrder="0" pos="8 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Osc connector" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="25.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="ecfc959010f22c7f" memberName="label_portOut"
         virtualName="" explicitFocusOrder="0" pos="160 48 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Out port : " editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="the status" id="b980354f8649bb1a" memberName="label_statusIn"
         virtualName="" explicitFocusOrder="0" pos="288 16 312 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Status : XXXX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="442527d42d97d347" memberName="label_portIn"
         virtualName="" explicitFocusOrder="0" pos="160 16 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="In port : " editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="the status" id="feadc0f791915916" memberName="label_statusOut"
         virtualName="" explicitFocusOrder="0" pos="288 48 312 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Status : XXXX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
  <GROUPCOMPONENT name="new group" id="26237c2a292af9b7" memberName="group_sent"
                  virtualName="" explicitFocusOrder="0" pos="0 152 300 248" title="Sent"/>
  <GROUPCOMPONENT name="new group" id="f0414c1aa013031a" memberName="group_received"
                  virtualName="" explicitFocusOrder="0" pos="604r 152 300 248"
                  title="Received"/>
  <TEXTEDITOR name="new text editor" id="c96c36e8cf071256" memberName="messageSent"
              virtualName="" explicitFocusOrder="0" pos="16 176 270 208" bkgcol="495358"
              initialText="Here will be sent messages" multiline="1" retKeyStartsLine="1"
              readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="2a0d8e0d3fc235e7" memberName="messageReceived"
              virtualName="" explicitFocusOrder="0" pos="590r 176 270 208"
              bkgcol="495358" initialText="Here will be received messages"
              multiline="1" retKeyStartsLine="1" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="ee1df21b3c9d37e" memberName="portIn_line"
              virtualName="" explicitFocusOrder="0" pos="232 16 48 24" initialText="9000"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="ae78911f83a9acaf" memberName="portOut_line"
              virtualName="" explicitFocusOrder="0" pos="232 48 48 24" initialText="8000"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="new label" id="8850c5adcb31a1ae" memberName="infoLabel"
         virtualName="" explicitFocusOrder="0" pos="8 88 150 56" bkgCol="bf272727"
         edTextCol="ff000000" edBkgCol="0" labelText="Info will go here"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="9"/>
  <TEXTEDITOR name="the ip adress" id="d2e025885c4eb934" memberName="box_ip"
              virtualName="" explicitFocusOrder="0" pos="8 48 150 24" initialText="localhost"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="e02c10e6e8dac91f" memberName="comboButton"
              virtualName="" explicitFocusOrder="0" pos="344 72 96 72" bgColOff="ff008000"
              buttonText="Go !" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f920b6f62b075037" memberName="StopButton"
              virtualName="" explicitFocusOrder="0" pos="448 72 96 72" bgColOff="ffa40000"
              buttonText="Stop" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

}

//[/EndFile]

