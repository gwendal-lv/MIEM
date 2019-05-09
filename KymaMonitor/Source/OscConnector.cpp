/*
  ==============================================================================

	OscConnector.cpp
	Created: 9 May 2019 4:00:44pm
	Author:  F. Dawagne

  ==============================================================================
*/

#include "OscConnector.h"

#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <boost/lockfree/spsc_queue.hpp>

#include "MyPacaranaManager.h"


namespace Miam {
	OscConnector::OscConnector()
	{
		currentInPort = -1;
		currentOutPort = -1;

		ip = "localhost";
		portToConnect = 6464;
		portToOpen = 6464;
	}
	OscConnector::~OscConnector()
	{
		if (isConnectedIn())
			disconnectIn();
		if (isConnectedOut())
			disconnectOut();
		if (theThread.joinable())
			theThread.join();
	}
	void OscConnector::init(MyPacaranaManager* refPaca)
	{
		pacaManager = refPaca;

		currentInPort = -1;
		currentOutPort = -1;

		ip = "localhost";
		portToConnect = 6464;
		portToOpen = 6464;

		receiver.init(refPaca, this);
	}

	void OscConnector::start()
	{
		pacaManager->init(this);
		pacaManager->lauch();
	}

	void OscConnector::connectIn()
	{

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

	void OscConnector::disconnectIn()
	{
		if (receiver.disconnect())
			currentInPort = -1;
		else
			handleDisconnectError();
	}

	void OscConnector::connectOut()
	{
		String ipToConnect = ip;

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

	void OscConnector::disconnectOut()
	{
		if (sender.disconnect())
			currentOutPort = -1;
		else
			handleDisconnectError();
	}

	void OscConnector::addToReceivedQueue(OSCMessage messageToAdd)
	{
		toProcess.push(messageToAdd);
	}

	void OscConnector::addToSendingQueue(OSCMessage messageToAdd)
	{
		toSend.push(messageToAdd);
	}

	void OscConnector::processMessages()
	{
		OSCMessage message = toProcess.front();

		String thePattern = message.getAddressPattern().toString();

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
					pacaManager->savePresetBaseInfo(message[0].getInt32(), message[1].getString().toStdString());
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

		toProcess.pop();
	}

	void OscConnector::sendMessages()
	{
		OSCMessage message = toSend.front();
		toSend.pop();

		sender.send(message);
	}

	void OscConnector::wait(int milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	void OscConnector::myThread()
	{
		while (isConnectedIn())
		{
			if (!toProcess.empty())
				processMessages();
			if (!toSend.empty())
				sendMessages();
			wait(100); // temps d'attente minimum pour éviter des problème de tailles de vector...
		}
	}

	void OscConnector::launchThread()
	{
		DBG(" -> lauching thread");
		theThread = std::thread(&OscConnector::myThread, this);
	}


	// error management, from tutorial
	void OscConnector::handleConnectError(int failedPort) {
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"OSC Connection error",
			"Error: could not connect to port " + String(failedPort),
			"OK");
	}
	void OscConnector::handleDisconnectError() {
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"Unknown error",
			"An unknown error occured while trying to disconnect from UDP port.",
			"OK");
	}
	void OscConnector::handleInvalidPortNumberEntered() {
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"Invalid port number",
			"Error: you have entered an invalid UDP port number.",
			"OK");
	}

	// ================================

	void MyOscReceiver::oscMessageReceived(const OSCMessage & message)
	{
		String messages = "";
		String thePattern = message.getAddressPattern().toString();
		int messageSize = message.size();

		connector->addToReceivedQueue(message);

		for (int i = 0; i < messageSize; i++)
		{
			messages += " " + autoStringParsing(message[i]);
		}

		////MyOscConnector::addToLog(outBox, thePattern + messages);
		//String log = thePattern + messages + "\n" + outBox->getText();
		//outBox->setText(log);
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

	void MyOscReceiver::init(MyPacaranaManager* refPaca, OscConnector* refConnect)
	{
		connector = refConnect;
		pacaManager = refPaca;

		addListener(this, "/osc/response_from");
		addListener(this, "/osc/widget"); // describe the widget
		addListener(this, "/osc/preset"); // describes the preset
		addListener(this, "/osc/notify/presets/PC"); // GIVES THE AMOUNTS OF EXISTING PRESETS
		addListener(this, "/osc/notify/vcs/PC"); // GIVES THE AMOUNTS OF EXISTING WIDGETS
		addListener(this, "/vcs"); // notif when widgets are modified
	}
}