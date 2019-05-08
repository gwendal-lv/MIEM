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
#include <vector>
#include <string>
#include "MyPacaranaManager.h"
//[/Headers]

#include "MyOscConnector.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

namespace Miam {

	void MyOscConnector::init(MyPacaranaManager* refPaca)
	{
		pacaManager = refPaca;

		currentInPort = -1;
		currentOutPort = -1;

		box_ip->setText("169.254.47.166");

		receiver.init(refPaca, messageReceived.get());

		updateButtonsAndText();
	}

	void MyOscConnector::connectIn()
	{

		auto portToOpen = portIn_line->getText().getIntValue();

		if (!isValidPortNumber(portToOpen))
		{
			handleInvalidPortNumberEntered();
			return;
		}

		if (receiver.connect(portToOpen))
			currentInPort = portToOpen;
		else
			handleConnectError(portToOpen);

	}

	void MyOscConnector::disconnectIn()
	{
		if (receiver.disconnect())
			currentInPort = -1;
		else
			handleDisconnectError();
	}

	void MyOscConnector::connectOut()
	{
		auto portToConnect = portOut_line->getText().getIntValue();
		String ipToConnect = box_ip->getText();

		if (!isValidPortNumber(portToConnect))
		{
			handleInvalidPortNumberEntered();
			return;
		}

		if (sender.connect(ipToConnect, portToConnect))
			currentOutPort = portToConnect;
		else
			handleConnectError(portToConnect);
	}

	void MyOscConnector::disconnectOut()
	{
		if (sender.disconnect())
			currentOutPort = -1;
		else
			handleDisconnectError();
	}

	void MyOscConnector::sendIntroMessage()
	{
		sendMessage("/osc/respond_to", portIn_line->getText().getIntValue());

	}
	void MyOscConnector::sendMessage(String oscAddress, int oscMessage)
	{
		sender.send(oscAddress, oscMessage);
		addToLog(messageSent.get(), oscAddress + " " + String(oscMessage));
	}
	void MyOscConnector::sendMessage(String oscAddress, String oscMessage)
	{
		sender.send(oscAddress, oscMessage);
		addToLog(messageSent.get(), oscAddress + " " + oscMessage);
	}
	void MyOscConnector::sendMessage(String oscAddress, int oscMessage1, float oscMessage2)
	{
		sender.send(oscAddress, oscMessage1, oscMessage2);
		addToLog(messageSent.get(), oscAddress + " " + String(oscMessage1) + " " + String(oscMessage2));
	}
	void MyOscConnector::sendMessage(String oscAddressAndMessage)
	{
		sender.send(oscAddressAndMessage);
		addToLog(messageSent.get(), oscAddressAndMessage);
	}


	//visual improvement
	void MyOscConnector::updateButtonsAndText()
	{
		Colour statusColor = Colours::white;
		String statusText = "empty";
		String ButtonText = "empty";

		// out
		if (isConnectedOut())
		{
			statusColor = Colours::green;
			statusText = "Connected to UPD port " + String(currentOutPort) + " at IP " + box_ip->getText();
			ButtonText = "Disconnect";
		}
		else {
			statusColor = Colours::red;
			statusText = "Disconnected";
			ButtonText = "Connect";
		}

		label_statusOut->setColour(Label::textColourId, statusColor);
		label_statusOut->setText(statusText, dontSendNotification);
		outButton->setButtonText(ButtonText);

		// in
		if (isConnectedIn())
		{
			statusColor = Colours::green;
			statusText = "Listening to port " + String(currentInPort);
			ButtonText = "Disconnect";
		}
		else {
			statusColor = Colours::red;
			statusText = "Disconnected";
			ButtonText = "Connect";
		}

		label_statusIn->setColour(Label::textColourId, statusColor);
		label_statusIn->setText(statusText, dontSendNotification);
		inButton->setButtonText(ButtonText);


	}
	void MyOscConnector::addToLog(TextEditor* tE, String str) {
		String oldText = (*tE).getText();
		(*tE).setText(str + "\n" + oldText);

	}

	// error lanagement, from tutorial
	void MyOscConnector::handleConnectError(int failedPort) {
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"OSC Connection error",
			"Error: could not connect to port " + String(failedPort),
			"OK");
	}
	void MyOscConnector::handleDisconnectError() {
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"Unknown error",
			"An unknown error occured while trying to disconnect from UDP port.",
			"OK");
	}
	void MyOscConnector::handleInvalidPortNumberEntered() {
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"Invalid port number",
			"Error: you have entered an invalid UDP port number.",
			"OK");
	}

	//[/MiscUserDefs]

	//==============================================================================
	MyOscConnector::MyOscConnector()
	{
		//[Constructor_pre] You can add your own custom stuff here..

		//[/Constructor_pre]

		title.reset(new Label("the title",
			TRANS("Osc connector")));
		addAndMakeVisible(title.get());
		title->setFont(Font(25.00f, Font::plain).withTypefaceStyle("Regular"));
		title->setJustificationType(Justification::centredLeft);
		title->setEditable(false, false, false);
		title->setColour(TextEditor::textColourId, Colours::black);
		title->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		title->setBounds(8, 8, 150, 24);

		label_portOut.reset(new Label("new label",
			TRANS("Out port : ")));
		addAndMakeVisible(label_portOut.get());
		label_portOut->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
		label_portOut->setJustificationType(Justification::centredLeft);
		label_portOut->setEditable(false, false, false);
		label_portOut->setColour(TextEditor::textColourId, Colours::black);
		label_portOut->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		label_portOut->setBounds(160, 48, 80, 24);

		label_statusIn.reset(new Label("the status",
			TRANS("Status : XXXX")));
		addAndMakeVisible(label_statusIn.get());
		label_statusIn->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Bold"));
		label_statusIn->setJustificationType(Justification::centredLeft);
		label_statusIn->setEditable(false, false, false);
		label_statusIn->setColour(TextEditor::textColourId, Colours::black);
		label_statusIn->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		label_statusIn->setBounds(424, 16, 368, 24);

		label_portIn.reset(new Label("new label",
			TRANS("In port : ")));
		addAndMakeVisible(label_portIn.get());
		label_portIn->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
		label_portIn->setJustificationType(Justification::centredLeft);
		label_portIn->setEditable(false, false, false);
		label_portIn->setColour(TextEditor::textColourId, Colours::black);
		label_portIn->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		label_portIn->setBounds(160, 16, 80, 24);

		inButton.reset(new TextButton("new button"));
		addAndMakeVisible(inButton.get());
		inButton->setButtonText(TRANS("StartListening"));
		inButton->addListener(this);
		inButton->setColour(TextButton::buttonColourId, Colour(0xff5c5da4));

		inButton->setBounds(296, 16, 120, 24);

		outButton.reset(new TextButton("new button"));
		addAndMakeVisible(outButton.get());
		outButton->setButtonText(TRANS("StartSending"));
		outButton->addListener(this);
		outButton->setColour(TextButton::buttonColourId, Colour(0xff5e5ca4));

		outButton->setBounds(296, 48, 120, 24);

		label_statusOut.reset(new Label("the status",
			TRANS("Status : XXXX")));
		addAndMakeVisible(label_statusOut.get());
		label_statusOut->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Bold"));
		label_statusOut->setJustificationType(Justification::centredLeft);
		label_statusOut->setEditable(false, false, false);
		label_statusOut->setColour(TextEditor::textColourId, Colours::black);
		label_statusOut->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		label_statusOut->setBounds(424, 48, 368, 24);

		sendMessageButton.reset(new TextButton("new button"));
		addAndMakeVisible(sendMessageButton.get());
		sendMessageButton->setButtonText(TRANS("Send"));
		sendMessageButton->addListener(this);
		sendMessageButton->setColour(TextButton::buttonColourId, Colour(0xff5ca488));

		sendMessageButton->setBounds(176, 120, 120, 24);

		group_sent.reset(new GroupComponent("new group",
			TRANS("Sent")));
		addAndMakeVisible(group_sent.get());

		group_sent->setBounds(0, 152, 300, 248);

		group_received.reset(new GroupComponent("new group",
			TRANS("Received")));
		addAndMakeVisible(group_received.get());

		messageSent.reset(new TextEditor("new text editor"));
		addAndMakeVisible(messageSent.get());
		messageSent->setMultiLine(true);
		messageSent->setReturnKeyStartsNewLine(true);
		messageSent->setReadOnly(true);
		messageSent->setScrollbarsShown(true);
		messageSent->setCaretVisible(false);
		messageSent->setPopupMenuEnabled(true);
		messageSent->setColour(TextEditor::backgroundColourId, Colour(0x00495358));
		messageSent->setText(TRANS("Here will be sent messages"));

		messageSent->setBounds(16, 176, 270, 208);

		messageReceived.reset(new TextEditor("new text editor"));
		addAndMakeVisible(messageReceived.get());
		messageReceived->setMultiLine(true);
		messageReceived->setReturnKeyStartsNewLine(true);
		messageReceived->setReadOnly(true);
		messageReceived->setScrollbarsShown(true);
		messageReceived->setCaretVisible(false);
		messageReceived->setPopupMenuEnabled(true);
		messageReceived->setColour(TextEditor::backgroundColourId, Colour(0x00495358));
		messageReceived->setText(TRANS("Here will be received messages"));

		portIn_line.reset(new TextEditor("new text editor"));
		addAndMakeVisible(portIn_line.get());
		portIn_line->setMultiLine(false);
		portIn_line->setReturnKeyStartsNewLine(false);
		portIn_line->setReadOnly(false);
		portIn_line->setScrollbarsShown(true);
		portIn_line->setCaretVisible(true);
		portIn_line->setPopupMenuEnabled(true);
		portIn_line->setText(TRANS("9000"));

		portIn_line->setBounds(240, 16, 48, 24);

		portOut_line.reset(new TextEditor("new text editor"));
		addAndMakeVisible(portOut_line.get());
		portOut_line->setMultiLine(false);
		portOut_line->setReturnKeyStartsNewLine(false);
		portOut_line->setReadOnly(false);
		portOut_line->setScrollbarsShown(true);
		portOut_line->setCaretVisible(true);
		portOut_line->setPopupMenuEnabled(true);
		portOut_line->setText(TRANS("8000"));

		portOut_line->setBounds(240, 48, 48, 24);

		messageToSend.reset(new TextEditor("new text editor"));
		addAndMakeVisible(messageToSend.get());
		messageToSend->setMultiLine(false);
		messageToSend->setReturnKeyStartsNewLine(false);
		messageToSend->setReadOnly(false);
		messageToSend->setScrollbarsShown(true);
		messageToSend->setCaretVisible(true);
		messageToSend->setPopupMenuEnabled(true);
		messageToSend->setText(TRANS("messageToSend"));

		messageToSend->setBounds(8, 120, 160, 24);

		infoLabel.reset(new Label("new label",
			TRANS("Info will go here")));
		addAndMakeVisible(infoLabel.get());
		infoLabel->setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
		infoLabel->setJustificationType(Justification::topLeft);
		infoLabel->setEditable(false, false, false);
		infoLabel->setColour(Label::backgroundColourId, Colour(0xbf272727));
		infoLabel->setColour(TextEditor::textColourId, Colours::black);
		infoLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		infoLabel->setBounds(440, 88, 150, 56);

		box_ip.reset(new TextEditor("the ip adress"));
		addAndMakeVisible(box_ip.get());
		box_ip->setMultiLine(false);
		box_ip->setReturnKeyStartsNewLine(false);
		box_ip->setReadOnly(false);
		box_ip->setScrollbarsShown(true);
		box_ip->setCaretVisible(true);
		box_ip->setPopupMenuEnabled(true);
		box_ip->setText(TRANS("localhost"));

		box_ip->setBounds(8, 48, 150, 24);

		oscWidgetButtonSend.reset(new TextButton("/osc/widget"));
		addAndMakeVisible(oscWidgetButtonSend.get());
		oscWidgetButtonSend->setTooltip(TRANS("/osc/widget"));
		oscWidgetButtonSend->addListener(this);

		oscWidgetButtonSend->setBounds(608, 175, 160, 24);

		oscWidgetInt.reset(new TextEditor("new text editor"));
		addAndMakeVisible(oscWidgetInt.get());
		oscWidgetInt->setMultiLine(false);
		oscWidgetInt->setReturnKeyStartsNewLine(false);
		oscWidgetInt->setReadOnly(false);
		oscWidgetInt->setScrollbarsShown(true);
		oscWidgetInt->setCaretVisible(true);
		oscWidgetInt->setPopupMenuEnabled(true);
		oscWidgetInt->setText(TRANS("0"));

		oscWidgetInt->setBounds(768, 175, 25, 24);

		oscPresetButtonSend.reset(new TextButton("/osc/preset"));
		addAndMakeVisible(oscPresetButtonSend.get());
		oscPresetButtonSend->setTooltip(TRANS("/osc/preset"));
		oscPresetButtonSend->addListener(this);

		oscPresetButtonSend->setBounds(608, 200, 160, 24);

		oscPresetInt.reset(new TextEditor("new text editor"));
		addAndMakeVisible(oscPresetInt.get());
		oscPresetInt->setMultiLine(false);
		oscPresetInt->setReturnKeyStartsNewLine(false);
		oscPresetInt->setReadOnly(false);
		oscPresetInt->setScrollbarsShown(true);
		oscPresetInt->setCaretVisible(true);
		oscPresetInt->setPopupMenuEnabled(true);
		oscPresetInt->setText(TRANS("0"));

		oscPresetInt->setBounds(768, 200, 25, 24);

		presetButtonSend.reset(new TextButton("/preset"));
		addAndMakeVisible(presetButtonSend.get());
		presetButtonSend->setTooltip(TRANS("/preset"));
		presetButtonSend->addListener(this);

		presetButtonSend->setBounds(608, 225, 160, 24);

		presetInt.reset(new TextEditor("new text editor"));
		addAndMakeVisible(presetInt.get());
		presetInt->setMultiLine(false);
		presetInt->setReturnKeyStartsNewLine(false);
		presetInt->setReadOnly(false);
		presetInt->setScrollbarsShown(true);
		presetInt->setCaretVisible(true);
		presetInt->setPopupMenuEnabled(true);
		presetInt->setText(TRANS("0"));

		presetInt->setBounds(768, 225, 25, 24);

		volumeButtonSend.reset(new TextButton("/volume"));
		addAndMakeVisible(volumeButtonSend.get());
		volumeButtonSend->setTooltip(TRANS("/volume"));
		volumeButtonSend->addListener(this);

		volumeButtonSend->setBounds(608, 250, 160, 24);

		volumeInt.reset(new TextEditor("new text editor"));
		addAndMakeVisible(volumeInt.get());
		volumeInt->setMultiLine(false);
		volumeInt->setReturnKeyStartsNewLine(false);
		volumeInt->setReadOnly(false);
		volumeInt->setScrollbarsShown(true);
		volumeInt->setCaretVisible(true);
		volumeInt->setPopupMenuEnabled(true);
		volumeInt->setText(TRANS("0"));

		volumeInt->setBounds(768, 250, 25, 24);

		group_messages.reset(new GroupComponent("groupButtons",
			TRANS("buttons")));
		addAndMakeVisible(group_messages.get());

		notifPresetButtonSend.reset(new TextButton("/osc/notify/presets/...,i"));
		addAndMakeVisible(notifPresetButtonSend.get());
		notifPresetButtonSend->setTooltip(TRANS("/osc/notify/presets/...,i"));
		notifPresetButtonSend->addListener(this);

		notifPresetButtonSend->setBounds(608, 275, 160, 24);

		notifPresetInt.reset(new TextEditor("new text editor"));
		addAndMakeVisible(notifPresetInt.get());
		notifPresetInt->setMultiLine(false);
		notifPresetInt->setReturnKeyStartsNewLine(false);
		notifPresetInt->setReadOnly(false);
		notifPresetInt->setScrollbarsShown(true);
		notifPresetInt->setCaretVisible(true);
		notifPresetInt->setPopupMenuEnabled(true);
		notifPresetInt->setText(TRANS("0"));

		notifPresetInt->setBounds(768, 275, 25, 24);

		notifVcsButtonSend.reset(new TextButton("/osc/notify/vcs/...,i"));
		addAndMakeVisible(notifVcsButtonSend.get());
		notifVcsButtonSend->setTooltip(TRANS("/osc/notify/vcs/...,i"));
		notifVcsButtonSend->addListener(this);

		notifVcsButtonSend->setBounds(608, 300, 160, 24);

		notifVcsInt.reset(new TextEditor("new text editor"));
		addAndMakeVisible(notifVcsInt.get());
		notifVcsInt->setMultiLine(false);
		notifVcsInt->setReturnKeyStartsNewLine(false);
		notifVcsInt->setReadOnly(false);
		notifVcsInt->setScrollbarsShown(true);
		notifVcsInt->setCaretVisible(true);
		notifVcsInt->setPopupMenuEnabled(true);
		notifVcsInt->setText(TRANS("0"));

		notifVcsInt->setBounds(768, 300, 25, 24);

		comboButton.reset(new TextButton("new button"));
		addAndMakeVisible(comboButton.get());
		comboButton->setButtonText(TRANS("Go !"));
		comboButton->addListener(this);
		comboButton->setColour(TextButton::buttonColourId, Colours::green);

		comboButton->setBounds(616, 72, 96, 72);

		configPresetidButton.reset(new TextButton("config preset"));
		addAndMakeVisible(configPresetidButton.get());
		configPresetidButton->setButtonText(TRANS("config Preset"));
		configPresetidButton->addListener(this);

		configPresetidButton->setBounds(608, 325, 160, 24);

		//[UserPreSize]

		//[/UserPreSize]

		setSize(800, 400);


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
		inButton = nullptr;
		outButton = nullptr;
		label_statusOut = nullptr;
		sendMessageButton = nullptr;
		group_sent = nullptr;
		group_received = nullptr;
		messageSent = nullptr;
		messageReceived = nullptr;
		portIn_line = nullptr;
		portOut_line = nullptr;
		messageToSend = nullptr;
		infoLabel = nullptr;
		box_ip = nullptr;
		oscWidgetButtonSend = nullptr;
		oscWidgetInt = nullptr;
		oscPresetButtonSend = nullptr;
		oscPresetInt = nullptr;
		presetButtonSend = nullptr;
		presetInt = nullptr;
		volumeButtonSend = nullptr;
		volumeInt = nullptr;
		group_messages = nullptr;
		notifPresetButtonSend = nullptr;
		notifPresetInt = nullptr;
		notifVcsButtonSend = nullptr;
		notifVcsInt = nullptr;
		comboButton = nullptr;
		configPresetidButton = nullptr;


		//[Destructor]. You can add your own custom destruction code here..
		//[/Destructor]
	}

	//==============================================================================
	void MyOscConnector::paint(Graphics& g)
	{
		//[UserPrePaint] Add your own custom painting code here..
		//[/UserPrePaint]

		g.fillAll(Colour(0xff323e44));

		//[UserPaint] Add your own custom painting code here..
		//[/UserPaint]
	}

	void MyOscConnector::resized()
	{
		//[UserPreResize] Add your own custom resize code here..
		//[/UserPreResize]

		group_received->setBounds(604 - 300, 152, 300, 248);
		messageReceived->setBounds(590 - 270, 176, 270, 208);
		group_messages->setBounds(800 - 192, 152, 192, 24);
		//[UserResized] Add your own custom resize handling here..
		//[/UserResized]
	}

	void MyOscConnector::buttonClicked(Button* buttonThatWasClicked)
	{
		//[UserbuttonClicked_Pre]
		//[/UserbuttonClicked_Pre]

		if (buttonThatWasClicked == inButton.get())
		{
			//[UserButtonCode_inButton] -- add your button handler code here..

			if (!isConnectedIn())
				connectIn();
			else
				disconnectIn();

			//[/UserButtonCode_inButton]
		}
		else if (buttonThatWasClicked == outButton.get())
		{
			//[UserButtonCode_outButton] -- add your button handler code here..
			if (!isConnectedOut()) {
				connectOut();
				sendIntroMessage();
			}
			else
				disconnectOut();

			//[/UserButtonCode_outButton]
		}
		else if (buttonThatWasClicked == sendMessageButton.get())
		{
			//[UserButtonCode_sendMessageButton] -- add your button handler code here..

			sendMessage(messageToSend->getText());

			//[/UserButtonCode_sendMessageButton]
		}
		else if (buttonThatWasClicked == oscWidgetButtonSend.get())
		{
			//[UserButtonCode_oscWidgetButtonSend] -- add your button handler code here..

			sender.send("/osc/widget", oscWidgetInt->getText().getIntValue());

			//[/UserButtonCode_oscWidgetButtonSend]
		}
		else if (buttonThatWasClicked == oscPresetButtonSend.get())
		{
			//[UserButtonCode_oscPresetButtonSend] -- add your button handler code here..

			sendMessage("/osc/preset", oscPresetInt->getText().getIntValue());

			//[/UserButtonCode_oscPresetButtonSend]
		}
		else if (buttonThatWasClicked == presetButtonSend.get())
		{
			//[UserButtonCode_presetButtonSend] -- add your button handler code here..

			sendMessage("/preset", presetInt->getText().getIntValue());

			//[/UserButtonCode_presetButtonSend]
		}
		else if (buttonThatWasClicked == volumeButtonSend.get())
		{
			//[UserButtonCode_volumeButtonSend] -- add your button handler code here..

			sendMessage("/volume", volumeInt->getText().getIntValue());

			//[/UserButtonCode_volumeButtonSend]
		}
		else if (buttonThatWasClicked == notifPresetButtonSend.get())
		{
			//[UserButtonCode_notifPresetButtonSend] -- add your button handler code here..

			sendMessage("/osc/notify/presets/PC", notifPresetInt->getText().getIntValue());

			//[/UserButtonCode_notifPresetButtonSend]
		}
		else if (buttonThatWasClicked == notifVcsButtonSend.get())
		{
			//[UserButtonCode_notifVcsButtonSend] -- add your button handler code here..

			sendMessage("/osc/notify/vcs/PC", notifVcsInt->getText().getIntValue());

			//[/UserButtonCode_notifVcsButtonSend]
		}
		else if (buttonThatWasClicked == comboButton.get())
		{
			//[UserButtonCode_comboButton] -- add your button handler code here..

			pacaManager->init(*this);
			pacaManager->lauch();

			comboButton->setEnabled(false);

			//[/UserButtonCode_comboButton]
		}
		else if (buttonThatWasClicked == configPresetidButton.get())
		{
			//[UserButtonCode_configPresetidButton] -- add your button handler code here..

			pacaManager->startPresetConfigurationLoop();
			configPresetidButton->setEnabled(false);

			//[/UserButtonCode_configPresetidButton]
		}

		//[UserbuttonClicked_Post]

		updateButtonsAndText();

		//[/UserbuttonClicked_Post]
	}



	//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


	void MyOscReceiver::oscMessageReceived(const OSCMessage & message)
	{

		//outBox->setText("Message received, size is " + size);
		//("Message received");
		String messages = "";
		String thePattern = message.getAddressPattern().toString();
		int messageSize = message.size();

		//DBG(std::string("Message de ") + std::to_string(messageSize) + std::string(" arguments OSC"));

		if (pacaManager != nullptr)
		{
			if (thePattern == "/osc/response_from") // gets 1 int
			{
				if (!(pacaManager->gotTotalWidget()))
					pacaManager->getWidgetsNumber();
			}
			else if (thePattern == "/osc/widget")
			{
				std::string file = message[1].getString().toStdString();
				if (!(pacaManager->gotAllUsefulWidget()))
					pacaManager->treatWidgetInfo(message[0].getInt32(), file);
			}
			else if (thePattern == "/osc/preset") // gets 1 int and 1 string
			{
				if (pacaManager->allPresetsPrepared())
					pacaManager->savePresetBaseInfo(message[0].getInt32(), autoStringParsing(message[1]).toStdString());
			}
			else if (thePattern == "/osc/notify/presets/PC") // gets 1 int
			{
				pacaManager->getNotif();
				if (!(pacaManager->gotAllPresets()))
					pacaManager->setPresetsNumber(message[0].getInt32());
			}
			else if (thePattern == "/osc/notify/vcs/PC") // gets 1 int
			{
				if (!(pacaManager->gotTotalWidget()))
					pacaManager->setWidgetsNumber(message[0].getInt32());
			}
			else if (thePattern == "/vcs") // gets 1 blob
			{
				MemoryBlock blob = message[0].getBlob(); // { byteCount, int_id0, float_value0, ... }
				pacaManager->treatBlobValue(blob);
			}
			else
			{
				DBG("ADDRESS NOT IMPLEMENTED : " + thePattern);
			}
		}

		for (int i = 0; i < messageSize; i++)
		{
			messages += " " + autoStringParsing(message[i]);
		}

		//MyOscConnector::addToLog(outBox, thePattern + messages);
		String log = thePattern + messages + "\n" + outBox->getText();
		outBox->setText(log);
	}

	String MyOscReceiver::autoStringParsing(OSCArgument arg)
	{
		String theStringArg;
		if (arg.isString())
		{
			auto receivedString = arg.getString();
			theStringArg = receivedString;
		}
		else if (arg.isInt32())
		{
			auto receivedData = arg.getInt32();
			theStringArg = std::to_string(receivedData);
		}
		else if (arg.isBlob())
		{
			auto receivedData = arg.getBlob(); // { byteCount, int_id0, float_value0, ... }
			int count = receivedData.getSize();
			//int id;
			//float value;
			//theStringArg = "{" + (String)count;
			//for (int i = 1; i < count; i += 2)
			//{
			//	id = receivedData[i];
			//	value = receivedData[i + 1];
			//	theStringArg += ", [" + (String)id + ", " + (String)value + "] ";
			//}

			theStringArg = "{ " + (String)receivedData[0];
			for (int i = 1; i < count; i++)
			{
				theStringArg += ", " + (String)receivedData[i];
			}

			theStringArg += " }";
		}
		else
		{
			theStringArg = "invalid type";
		}
		return theStringArg;
	}

	void MyOscReceiver::init(MyPacaranaManager* refPaca, TextEditor* box)
	{
		pacaManager = refPaca;
		outBox = box;

		addListener(this, "/osc/response_from");
		addListener(this, "/osc/widget"); // describe the widget
		addListener(this, "/osc/preset"); // describes the preset
		addListener(this, "/osc/notify/presets/PC"); // GIVES THE AMOUNTS OF EXISTING PRESETS
		addListener(this, "/osc/notify/vcs/PC"); // GIVES THE AMOUNTS OF EXISTING WIDGETS
		addListener(this, "/vcs"); // notif when widgets are modified
	}


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
				 fixedSize="1" initialWidth="800" initialHeight="400">
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
		 virtualName="" explicitFocusOrder="0" pos="424 16 368 24" edTextCol="ff000000"
		 edBkgCol="0" labelText="Status : XXXX" editableSingleClick="0"
		 editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
		 fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="33"
		 typefaceStyle="Bold"/>
  <LABEL name="new label" id="442527d42d97d347" memberName="label_portIn"
		 virtualName="" explicitFocusOrder="0" pos="160 16 80 24" edTextCol="ff000000"
		 edBkgCol="0" labelText="In port : " editableSingleClick="0" editableDoubleClick="0"
		 focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
		 kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="62b6b4d5f804aa75" memberName="inButton"
			  virtualName="" explicitFocusOrder="0" pos="296 16 120 24" bgColOff="ff5c5da4"
			  buttonText="StartListening" connectedEdges="0" needsCallback="1"
			  radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d2ad821a1bd67673" memberName="outButton"
			  virtualName="" explicitFocusOrder="0" pos="296 48 120 24" bgColOff="ff5e5ca4"
			  buttonText="StartSending" connectedEdges="0" needsCallback="1"
			  radioGroupId="0"/>
  <LABEL name="the status" id="feadc0f791915916" memberName="label_statusOut"
		 virtualName="" explicitFocusOrder="0" pos="424 48 368 24" edTextCol="ff000000"
		 edBkgCol="0" labelText="Status : XXXX" editableSingleClick="0"
		 editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
		 fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="33"
		 typefaceStyle="Bold"/>
  <TEXTBUTTON name="new button" id="399a08498c86cd94" memberName="sendMessageButton"
			  virtualName="" explicitFocusOrder="0" pos="176 120 120 24" bgColOff="ff5ca488"
			  buttonText="Send" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
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
			  virtualName="" explicitFocusOrder="0" pos="240 16 48 24" initialText="9000"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="ae78911f83a9acaf" memberName="portOut_line"
			  virtualName="" explicitFocusOrder="0" pos="240 48 48 24" initialText="8000"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="eb54649968143b7f" memberName="messageToSend"
			  virtualName="" explicitFocusOrder="0" pos="8 120 160 24" initialText="messageToSend"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <LABEL name="new label" id="8850c5adcb31a1ae" memberName="infoLabel"
		 virtualName="" explicitFocusOrder="0" pos="440 88 150 56" bkgCol="bf272727"
		 edTextCol="ff000000" edBkgCol="0" labelText="Info will go here"
		 editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
		 fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
		 italic="0" justification="9"/>
  <TEXTEDITOR name="the ip adress" id="d2e025885c4eb934" memberName="box_ip"
			  virtualName="" explicitFocusOrder="0" pos="8 48 150 24" initialText="localhost"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTBUTTON name="/osc/widget" id="77d5f897f0d312ed" memberName="oscWidgetButtonSend"
			  virtualName="" explicitFocusOrder="0" pos="608 175 160 24" tooltip="/osc/widget"
			  buttonText="/osc/widget" connectedEdges="0" needsCallback="1"
			  radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="b67574e2a678c787" memberName="oscWidgetInt"
			  virtualName="" explicitFocusOrder="0" pos="768 175 25 24" initialText="0"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTBUTTON name="/osc/preset" id="57d32dd3fbc429d" memberName="oscPresetButtonSend"
			  virtualName="" explicitFocusOrder="0" pos="608 200 160 24" tooltip="/osc/preset"
			  buttonText="/osc/preset" connectedEdges="0" needsCallback="1"
			  radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="79c6cc7310f4a7e1" memberName="oscPresetInt"
			  virtualName="" explicitFocusOrder="0" pos="768 200 25 24" initialText="0"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTBUTTON name="/preset" id="49a7ed7df05729ca" memberName="presetButtonSend"
			  virtualName="" explicitFocusOrder="0" pos="608 225 160 24" tooltip="/preset"
			  buttonText="/preset" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="cf22102d429d830e" memberName="presetInt"
			  virtualName="" explicitFocusOrder="0" pos="768 225 25 24" initialText="0"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTBUTTON name="/volume" id="2a706f51fc5df01d" memberName="volumeButtonSend"
			  virtualName="" explicitFocusOrder="0" pos="608 250 160 24" tooltip="/volume"
			  buttonText="/volume" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="862a569037d42cd2" memberName="volumeInt"
			  virtualName="" explicitFocusOrder="0" pos="768 250 25 24" initialText="0"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <GROUPCOMPONENT name="groupButtons" id="96bc023ad5a9ea70" memberName="group_messages"
				  virtualName="" explicitFocusOrder="0" pos="800r 152 192 24" title="buttons"/>
  <TEXTBUTTON name="/osc/notify/presets/...,i" id="653e4d93fd9cec01" memberName="notifPresetButtonSend"
			  virtualName="" explicitFocusOrder="0" pos="608 275 160 24" tooltip="/osc/notify/presets/...,i"
			  buttonText="/osc/notify/presets/...,i" connectedEdges="0" needsCallback="1"
			  radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="27cbed71e97ff44d" memberName="notifPresetInt"
			  virtualName="" explicitFocusOrder="0" pos="768 275 25 24" initialText="0"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTBUTTON name="/osc/notify/vcs/...,i" id="8e3e6cc3f1c801c5" memberName="notifVcsButtonSend"
			  virtualName="" explicitFocusOrder="0" pos="608 300 160 24" tooltip="/osc/notify/vcs/...,i"
			  buttonText="/osc/notify/vcs/...,i" connectedEdges="0" needsCallback="1"
			  radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="ae9beef47fa084c5" memberName="notifVcsInt"
			  virtualName="" explicitFocusOrder="0" pos="768 300 25 24" initialText="0"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="e02c10e6e8dac91f" memberName="comboButton"
			  virtualName="" explicitFocusOrder="0" pos="616 72 96 72" bgColOff="ff008000"
			  buttonText="Go !" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="config preset" id="eb34eb1e7062dd76" memberName="configPresetidButton"
			  virtualName="" explicitFocusOrder="0" pos="608 325 160 24" buttonText="config Preset"
			  connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="534d8d0c882ca35b" memberName="configPresetidInt"
			  virtualName="" explicitFocusOrder="0" pos="768 325 25 24" initialText="0"
			  multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
			  caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

}

//[/EndFile]

