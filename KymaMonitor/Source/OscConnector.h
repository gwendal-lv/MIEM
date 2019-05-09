/*
  ==============================================================================

	OscConnector.h
	Created: 9 May 2019 4:00:44pm
	Author:  F. Dawagne

  ==============================================================================
*/

#pragma once

using namespace std;

#include <vector>
#include <string>
#include <thread>
#include <iostream>

#include <boost/lockfree/spsc_queue.hpp>

#include "../JuceLibraryCode/JuceHeader.h"
#include "MyPacaranaManager.h"

namespace Miam {

	class OscConnector;
	class MyPacaranaManager;

	class MyOscReceiver : public OSCReceiver,
		public OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback> // this kind of callback if realtime not important but with a lot of messages
	{
	public:
		//==============================================================================
		MyOscReceiver() {};
		~MyOscReceiver() {};

		void oscMessageReceived(const OSCMessage &message) override;

		void init(MyPacaranaManager* refPaca, OscConnector* refConnect);

		String autoStringParsing(OSCArgument arg);

		//==============================================================================

	private:
		//TextEditor* outBox;
		OscConnector* connector;
		MyPacaranaManager* pacaManager;
		//==============================================================================

	};

	class OscConnector
	{
	public:

		OscConnector();
		~OscConnector();

		// ========== METHODS ===========================
		// simple verifications
		bool isValidPortNumber(int portNbr) { return portNbr > 0 && portNbr < 65536; }
		bool isConnectedIn() { return currentInPort != -1; }
		bool isConnectedOut() { return currentOutPort != -1; }

		int getInPort() { return currentInPort; }
		int getOutPort() { return currentOutPort; }
		std::string getIp() { return ip; }
		void setInPort(int newPort) { portToOpen = newPort; }
		void setOutPort(int newPort) { portToConnect = newPort; }
		void setIpAddress(std::string newIp) { ip = newIp; }

		// true functions
		void init(MyPacaranaManager* refPaca);
		void start();

		void connectIn();
		void disconnectIn();

		void connectOut();
		void disconnectOut();

		void addToReceivedQueue(OSCMessage messageToAdd);
		void addToSendingQueue(OSCMessage messageToAdd);
		void processMessages();
		void sendMessages();



		// Error handlers // Version from the tutorial for now
		void handleConnectError(int failedPort);
		void handleDisconnectError();
		void handleInvalidPortNumberEntered();

		// TEST threads
		void wait(int milliseconds);
		void myThread();
		void launchThread();

	private:
		MyOscReceiver receiver;
		OSCSender sender;
		MyPacaranaManager* pacaManager;

		std::string ip;
		int portToConnect;
		int portToOpen;

		int currentOutPort;
		int currentInPort;

		std::thread theThread;

		boost::lockfree::spsc_queue<juce::OSCMessage, boost::lockfree::capacity<16>> toSend;
		boost::lockfree::spsc_queue<juce::OSCMessage, boost::lockfree::capacity<16>> toProcess;

	};


}