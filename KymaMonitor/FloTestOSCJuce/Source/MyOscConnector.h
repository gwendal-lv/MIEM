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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <string>

namespace Miam {


	class MyPacaranaManager;

	class MyOscReceiver : public OSCReceiver,
		public OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback> // this kind of callback if realtime not important but with a lot of messages
	{
	public:
		//==============================================================================
		MyOscReceiver() {};
		~MyOscReceiver() {};

		void oscMessageReceived(const OSCMessage &message) override;

		void init(MyPacaranaManager* refPaca, TextEditor* box);

		String autoStringParsing(OSCArgument arg);

		//==============================================================================

	private:
		TextEditor* outBox;
		MyPacaranaManager* pacaManager;
		//==============================================================================

	};

	//[/Headers]



	//==============================================================================
	/**
																		//[Comments]
		An auto-generated component, created by the Projucer.

		Describe your class and how it works here!
																		//[/Comments]
	*/
	class MyOscConnector : public Component,
		public Button::Listener
	{
	public:
		//==============================================================================
		MyOscConnector();
		~MyOscConnector();

		//==============================================================================
		//[UserMethods]     -- You can add your own custom methods in this section.

		// simple verifications
		bool isValidPortNumber(int portNbr) { return portNbr > 0 && portNbr < 65536; }
		bool isConnectedIn() { return currentInPort != -1; }
		bool isConnectedOut() { return currentOutPort != -1; }

		// true functions

		void init(MyPacaranaManager* refPaca);

		void connectIn();
		void disconnectIn();

		void connectOut();
		void disconnectOut();

		void sendIntroMessage();
		void sendMessage(String oscAddress, int oscMessage);
		void sendMessage(String oscAddress, String oscMessage);
		void sendMessage(String oscAddress, int oscMessage1, float oscMessage2);
		void sendMessage(String oscAddressAndMessage);

		// Visual improvements
		void updateButtonsAndText();
		void addToLog(TextEditor* tE, String str);

		// Error handlers // Version from the tutorial for now
		void handleConnectError(int failedPort);
		void handleDisconnectError();
		void handleInvalidPortNumberEntered();

		//[/UserMethods]

		void paint(Graphics& g) override;
		void resized() override;
		void buttonClicked(Button* buttonThatWasClicked) override;



	private:
		//[UserVariables]   -- You can add your own custom variables in this section.
		MyOscReceiver receiver;
		OSCSender sender;
		MyPacaranaManager* pacaManager;

		int currentOutPort;
		int currentInPort;

		bool doTheWholeThing = false;

		//[/UserVariables]

		//==============================================================================
		std::unique_ptr<Label> title;
		std::unique_ptr<Label> label_portOut;
		std::unique_ptr<Label> label_statusIn;
		std::unique_ptr<Label> label_portIn;
		std::unique_ptr<TextButton> inButton;
		std::unique_ptr<TextButton> outButton;
		std::unique_ptr<Label> label_statusOut;
		std::unique_ptr<TextButton> sendMessageButton;
		std::unique_ptr<GroupComponent> group_sent;
		std::unique_ptr<GroupComponent> group_received;
		std::unique_ptr<TextEditor> messageSent;
		std::unique_ptr<TextEditor> messageReceived;
		std::unique_ptr<TextEditor> portIn_line;
		std::unique_ptr<TextEditor> portOut_line;
		std::unique_ptr<TextEditor> messageToSend;
		std::unique_ptr<Label> infoLabel;
		std::unique_ptr<TextEditor> box_ip;
		std::unique_ptr<TextButton> oscWidgetButtonSend;
		std::unique_ptr<TextEditor> oscWidgetInt;
		std::unique_ptr<TextButton> oscPresetButtonSend;
		std::unique_ptr<TextEditor> oscPresetInt;
		std::unique_ptr<TextButton> presetButtonSend;
		std::unique_ptr<TextEditor> presetInt;
		std::unique_ptr<TextButton> volumeButtonSend;
		std::unique_ptr<TextEditor> volumeInt;
		std::unique_ptr<GroupComponent> group_messages;
		std::unique_ptr<TextButton> notifPresetButtonSend;
		std::unique_ptr<TextEditor> notifPresetInt;
		std::unique_ptr<TextButton> notifVcsButtonSend;
		std::unique_ptr<TextEditor> notifVcsInt;
		std::unique_ptr<TextButton> comboButton;


		//==============================================================================
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyOscConnector)
	};

	//[EndFile] You can add extra defines here...

	/*
	class MyOscReceiver : public OSCReceiver,
		public OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback> // this kind of callback if realtime not important but with a lot of messages
	{
	public:
		//==============================================================================
		MyOscReceiver() {};
		~MyOscReceiver() {};

		void oscMessageReceived(const OSCMessage &message) override;

		void prepareReceiver();

		String autoStringParsing(OSCArgument arg);
		//==============================================================================
		TextEditor* outBox;
		//==============================================================================

	private:
		//==============================================================================

	}; */

}
//[/EndFile]

