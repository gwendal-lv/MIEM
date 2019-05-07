/*
  ==============================================================================

	MyPacaranaManager.cpp
	Created: 3 May 2019 8:40:33am
	Author:  Irisib

  ==============================================================================
*/

#include "MyPacaranaManager.h"
#include "MyOscConnector.h"
#include <vector>
#include <string>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/ptree_fwd.hpp>
//#include <boost/property_tree/json_parser.hpp>

#include "../boost/property_tree/ptree.hpp"
#include "../boost/property_tree/ptree_fwd.hpp"
#include "../boost/property_tree/json_parser.hpp"

#include <sstream>

using namespace std;

namespace pt = boost::property_tree;

namespace Miam {

	// ================================================================================

	MyPacaranaManager::MyPacaranaManager()
	{
	}


	MyPacaranaManager::~MyPacaranaManager()
	{
	}

	void MyPacaranaManager::init(MyOscConnector &oscConn)
	{
		DBG("=====init pacamanager=====");
		oscConnector = &oscConn;

		presetNbr = -1;
		totalWidgetsNbr = -1;
		usefulWidgetNbr = 0;

		allPresetsPrepared = false;
		allWidgetsPrepared = false;

		allMyPresets.begin();
		allMyWidgets.begin();
	}

	void MyPacaranaManager::lauch()
	{
		DBG("===========launch=========");
		if (!oscConnector->isConnectedIn())
			oscConnector->connectIn();
		if (!oscConnector->isConnectedOut())
			oscConnector->connectOut();
		oscConnector->sendIntroMessage();
	}

	void MyPacaranaManager::getWidgetsNumber()
	{
		DBG("======get widgets nbr=====");
		int getNotifs = 1;
		oscConnector->sendMessage("/osc/notify/vcs/PC", getNotifs);
	}

	void MyPacaranaManager::setWidgetsNumber(int nbr)
	{
		DBG("======set widgets nbr=====");
		totalWidgetsNbr = nbr;
		getWidgetInfo();
	}


	void MyPacaranaManager::getPresetsNumber()
	{
		DBG("======get presets nbr=====");
		int getNotifs = 1;
		oscConnector->sendMessage("/osc/notify/presets/PC", getNotifs);
	}

	void MyPacaranaManager::setPresetsNumber(int nbr)
	{
		DBG("======set presets nbr=====");
		presetNbr = nbr;
		createAllPresets();
	}

	void MyPacaranaManager::getWidgetInfo()
	{
		DBG("======get widget info=====");
		for (int i = 0; i < totalWidgetsNbr; i++)
		{
			oscConnector->sendMessage("/osc/widget", i);
		}
	}

	void MyPacaranaManager::treatWidgetInfo(int widgetId, std::string JSONFile)
	{
		DBG("PROCESSING WIDGET NBR " + std::to_string(widgetId));

		boost::property_tree::ptree root;
		std::stringstream stream;
		stream << JSONFile;

		try {
			pt::json_parser::read_json(stream, root); //  NEEDS TO BE STREAM, simple sring will not work
		}
		catch (std::exception & e)
		{
			DBG("====== ERROR PARSING FILE ======");
		}

		std::string type = root.get<std::string>("creationClass", "NOT FOUND");

		if (type == "VCSEventSourceComponent") // if not, discard
		{
			KymaWidget newWidget;
			newWidget.name = root.get<std::string>("label");
			newWidget.idInPaca = widgetId;
			newWidget.minVal = root.get<float>("minimum");
			newWidget.maxVal = root.get<float>("maximum");
			newWidget.scale = root.get<std::string>("taper");
			newWidget.concreteId = root.get<int>("concreteEventID");
			newWidget.value = newWidget.minVal;

			DBG(newWidget.to_string());

			usefulWidgetNbr++;

			allMyWidgets.push_back(newWidget);
			allMyWidgets.resize(usefulWidgetNbr);
		}
		else
			DBG("DISCARDED FOR TYPE = " + type);

		if (widgetId + 1 == totalWidgetsNbr) // si on est au dernier widget, prévenir
		{
			getPresetsNumber();
			allWidgetsPrepared = true;
		}
	}

	void MyPacaranaManager::createAllPresets()
	{
		DBG("====create all presets====");
		DBG("BUGGY, STILL : MyPacaranaManager::createAllPresets()");

		// Set everything to Zero
		setAllToZero();

		for (int i = 0; i < presetNbr; i++)
		{
			KymaPreset newPreset;
			newPreset.setAllPresets(allMyWidgets);

			allMyPresets.push_back(newPreset);
			allMyPresets.resize(i + 1);
			oscConnector->sendMessage("/osc/preset", i);
		}

	}

	void MyPacaranaManager::savePresetInfo(int presetId, std::string presetName)
	{
		if (allMyPresets.size() > presetId) {
			allMyPresets[presetId].setName(presetName);
			DBG(allMyPresets[presetId].to_string());
		}

		if (presetId == presetNbr - 1)
			allPresetsPrepared = true;
	}

	void MyPacaranaManager::setAllToZero()
	{
		for (int i = 0; i < allMyWidgets.size(); i++)
		{
			oscConnector->sendMessage("/vcs", allMyWidgets[i].concreteId, 0); // "/vcs" int float
		}
	}

	// ================================================================================

	KymaPreset::KymaPreset()
	{
		name = "NOT ASSIGNED";
	}

	KymaPreset::~KymaPreset()
	{
	}

	std::string KymaPreset::to_string()
	{
		std::string outString = "";
		outString = name + " :";
		for (int i = 0; i < allWidgets.size(); i++)
		{
			if (i != 0)
				outString += ",";
			outString += " " + allWidgets[i].name + " = " + std::to_string(allWidgets[i].value);
		}
		return outString;
	}


	void KymaPreset::setAllPresets(std::vector<KymaWidget> widgets)
	{
		allWidgets = widgets; // ON COPIE LE VECTEUR

		for (int i = 0; i < allWidgets.size(); i++)
		{
			allWidgets[i].value = 0.5;
		}
	}

	// ================================================================================
}





