/*
  ==============================================================================

	MyPacaranaManager.h
	Created: 3 May 2019 8:40:33am
	Author:  Irisib

  ==============================================================================
*/

#pragma once

//#include "MyOscConnector.h"
using namespace std;
#include <vector>
#include <string>
#include "../JuceLibraryCode/JuceHeader.h"

namespace Miam {

	struct KymaWidget {
		std::string name;
		int idInPaca;
		int concreteId;
		float value;
		float minVal;
		float maxVal;
		std::string scale;

		std::string to_string() { return name + "(" + std::to_string(idInPaca) + ") = " + std::to_string(value) + " [" + std::to_string(minVal) + ", " + std::to_string(maxVal) + "] - " + scale; }
	};

	class MyOscConnector;
	class KymaPreset;

	class MyPacaranaManager
	{
	public:
		MyPacaranaManager();
		~MyPacaranaManager();

		//void init(MyOscConnector* oscConn);
		void init(MyOscConnector& oscConn);

		void lauch();
		void getWidgetsNumber();
		void setWidgetsNumber(int nbr);
		void getPresetsNumber();
		void setPresetsNumber(int nbr);

		void getWidgetInfo();
		void createAllPresets();

		void savePresetBaseInfo(int presetId, std::string presetName);
		void startPresetConfigurationLoop();
		void nextPresetConfiguration();
		void endPresetconfiguration();
		void saveVcsInfo(int eventId, float newValue);

		void treatWidgetInfo(int widgetId, std::string JSONFile);
		void treatBlobValue(MemoryBlock blobInout);

		void rollDices();
		void setAllToZero();
		void getVcsNotif(bool yesOrNo);
		void getPresetNotif(bool yesOrNo);

		// useful booleans
		bool isFullyReady() { return gotAllPresets() && gotTotalWidget() && gotAllUsefulWidget() && !isConfiguringPreset() && allPresetsPrepared(); }
		bool gotAllPresets() { return presetNbr == allMyPresets.size(); }
		bool gotTotalWidget() { return presetNbr != -1; }
		bool gotAllUsefulWidget() { return allWidgetsPrepared; }
		bool isConfiguringPreset() { return presetBeingConfigurated > -1; }
		bool allPresetsPrepared() { return presetBeingConfigurated >= allMyPresets.size(); }

		void getNotif() { waitingForNotif = false; }

	private:
		int presetNbr;
		int totalWidgetsNbr;
		int usefulWidgetNbr;
		MyOscConnector* oscConnector;

		bool allWidgetsPrepared; // manually set to true, for now

		bool waitingForNotif;

		int presetBeingConfigurated;

		std::vector<KymaPreset> allMyPresets;
		std::vector<KymaWidget> allMyWidgets;
	};

	class KymaPreset {
	public:
		KymaPreset();
		~KymaPreset();

		void init(MyPacaranaManager* refPaca);

		std::string to_string();
		void KymaPreset::setName(std::string newName) { name = newName; }
		void setAllPresets(std::vector<KymaWidget> widgets);
		void setWidgetValue(int eventId, float newValue);

		bool isFullyConfigurated() { return (name != "NOT ASSIGNED" && widgetConfigurated == allWidgets.size()); }

	private:
		MyPacaranaManager* pacaManager;
		std::string name;
		std::vector<KymaWidget> allWidgets;
		int widgetConfigurated;
	};

}