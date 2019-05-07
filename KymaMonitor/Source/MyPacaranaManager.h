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
		void treatWidgetInfo(int widgetId, std::string JSONFile);
		void createAllPresets();
		void treatBlobValue(MemoryBlock blobInout);

		void savePresetInfo(int presetId, std::string presetName);

		void setAllToZero();

		// useful booleans
		bool isFullyReady() { return gotAllPresets() && gotTotalWidget() && gotAllUsefulWidget() && allPresetsPrepared; }
		bool gotAllPresets() { return presetNbr == allMyPresets.size(); }
		bool gotTotalWidget() { return presetNbr != -1; }
		bool gotAllUsefulWidget() { return allWidgetsPrepared; }

	private:
		int presetNbr;
		int totalWidgetsNbr;
		int usefulWidgetNbr;
		MyOscConnector* oscConnector;

		bool allPresetsPrepared; // manually set to true, for now
		bool allWidgetsPrepared; // manually set to true, for now

		std::vector<KymaPreset> allMyPresets;
		std::vector<KymaWidget> allMyWidgets;
	};

	class KymaPreset {
	public:
		KymaPreset();
		~KymaPreset();

		std::string to_string();
		void KymaPreset::setName(std::string newName) { name = newName; }
		void setAllPresets(std::vector<KymaWidget> widgets);

	private:
		std::string name;
		std::vector<KymaWidget> allWidgets;
	};

}