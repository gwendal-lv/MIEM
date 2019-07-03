/*
  ==============================================================================

	MyPacaranaManager.cpp
	Created: 3 May 2019 8:40:33am
	Author:  F. Dawagne

  ==============================================================================
*/

#include <vector>
#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "MyPacaranaManager.h"
#include "OscConnector.h"

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

	void MyPacaranaManager::init(OscConnector* refConn)
	{
		DBG("=====init pacamanager=====");

		oscConnector = refConn;

		presetNbr = -1;
		totalWidgetsNbr = -1;
		presetBeingConfigurated = -1;
		usefulWidgetNbr = 0;

		allWidgetsPrepared = false;

		waitingForNotif = false;

		allMyPresets.begin();
		allMyWidgets.begin();

		DBG("  => init pacaMa done ====");
	}


	void MyPacaranaManager::lauch()
	{
		DBG("===========launch=========");
		if (!oscConnector->isConnectedIn())
			oscConnector->connectIn();
		if (!oscConnector->isConnectedOut())
			oscConnector->connectOut();

		getVcsNotif(false);
		getPresetNotif(false);

		oscConnector->addToSendingQueue(OSCMessage("/osc/respond_to", oscConnector->getInPort())); // sends port to respond to

		oscConnector->launchThread();
		DBG("  => launch done =========");
	}

	void MyPacaranaManager::getWidgetsNumber()
	{
		DBG("======get widgets nbr=====");
		getVcsNotif(true);
		DBG("  => get widgets nbr ====");
	}

	void MyPacaranaManager::setWidgetsNumber(int nbr)
	{
		DBG("======set widgets nbr=====");
		totalWidgetsNbr = nbr;
		getWidgetInfo();
		DBG("  => set wid nbr done ====");
	}

	void MyPacaranaManager::getPresetsNumber()
	{
		DBG("======get presets nbr=====");
		getPresetNotif(true);
		DBG("  => ==== done ===========");
	}

	void MyPacaranaManager::setPresetsNumber(int nbr)
	{
		DBG("======set presets nbr=====");
		presetNbr = nbr;
		createAllPresets();
		DBG("  => set pres nbr done ===");
	}


	void MyPacaranaManager::getWidgetInfo()
	{
		DBG("======get widget info=====");
		for (int i = 0; i < totalWidgetsNbr; i++)
		{
			DBG("OSC : ASKING FOR WIDGET NBR " + std::to_string(i) + "/" + std::to_string(totalWidgetsNbr));
			oscConnector->addToSendingQueue(OSCMessage("/osc/widget", i));
		}
		DBG("  => get wid info done ===");
	}

	void MyPacaranaManager::createAllPresets()
	{
		DBG("====create all presets====");

		getVcsNotif(false);

		// We roll the dices, to avoid having no changes in the values later when configurating...
		rollDices();

		getVcsNotif(true);

		for (int i = 0; i < presetNbr; i++)
		{
			KymaPreset newPreset;
			newPreset.init(this);
			newPreset.setAllPresets(allMyWidgets);

			allMyPresets.push_back(newPreset);
			allMyPresets.resize(i + 1);
			oscConnector->addToSendingQueue(OSCMessage("/osc/preset", i));
			DBG("    -> preset done !======");
		}

		DBG("  => creat all pres done =");
	}


	void MyPacaranaManager::savePresetBaseInfo(int presetId, std::string presetName)
	{
		if (allMyPresets.size() > presetId) {
			allMyPresets[presetId].setName(presetName);
			DBG(allMyPresets[presetId].to_string());
		}

		if (presetId == presetNbr - 1)
			startPresetConfigurationLoop();
	}

	void MyPacaranaManager::startPresetConfigurationLoop()
	{
		presetBeingConfigurated = 0;
		nextPresetConfiguration();
	}

	void MyPacaranaManager::nextPresetConfiguration()
	{
		getVcsNotif(false);
		waitingForNotif = true;
		rollDices();
		getVcsNotif(true); // ICI CA VA TROP VITE, IL FAUT RALENTIR LE TRUC
		for (int i = 0; i < 100000; i++)
		{
			;
		}
		oscConnector->addToSendingQueue(OSCMessage("/preset", presetBeingConfigurated + 1)); // on doit faire +1 parce que dans le kyma les ID démarrent à 1
	}

	void MyPacaranaManager::endPresetconfiguration()
	{
		presetBeingConfigurated++;
		if (presetBeingConfigurated < allMyPresets.size())
			nextPresetConfiguration();
		else {

			DBG(" -> finished presets config");
		}
	}

	void MyPacaranaManager::saveVcsInfo(int eventId, float newValue)
	{
		if (!isConfiguringPreset())
			return;

		allMyPresets[presetBeingConfigurated].setWidgetValue(eventId, newValue);
	}


	void MyPacaranaManager::treatWidgetInfo(int widgetId, std::string JSONFile)
	{
		DBG("PROCESSING WIDGET NBR " + std::to_string(widgetId) + "/" + std::to_string(totalWidgetsNbr));

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

	void MyPacaranaManager::treatBlobValue(MemoryBlock blobInout)
	{

		//// OTHER WAY BELOW, KEEPING THIS JUST IN CASE
		//std::vector<unsigned char> idTab;
		//std::vector<unsigned char> valTab;

		//idTab.begin();
		//valTab.begin();

		//int count = blobInout.getSize();
		//int temp;
		//for (int i = 0; i < count / 2; i++)
		//{
		//	temp = blobInout[i];
		//	idTab.push_back(temp);
		//}
		//for (int i = count / 2; i < count; i++)
		//{
		//	temp = blobInout[i];
		//	valTab.push_back(temp);
		//}

		//int id = int((unsigned char)idTab[0] << 24 | (unsigned char)idTab[1] << 16 | (unsigned char)idTab[2] << 8 | (unsigned char)idTab[3]); // correspond au ConcreteID du widget


		//THIS ONE WORKS, FOR NOW

		//le nombre de bytes
		int byteCount = blobInout.getSize();
		int pairCount = byteCount / 8;

		//les outils de conversion
		union IntFromByte
		{
			int unionInt;
			unsigned char byteArray[4];
		} iFB;

		union FloatFromByte
		{
			float unionFloat;
			unsigned char byteArray[4];
		} fFB;

		//pour chaque paire
		for (int i = 0; i < pairCount; i++)
		{
			iFB.byteArray[0] = blobInout[i * 8 + 3];
			iFB.byteArray[1] = blobInout[i * 8 + 2];
			iFB.byteArray[2] = blobInout[i * 8 + 1];
			iFB.byteArray[3] = blobInout[i * 8 + 0];

			int id = iFB.unionInt;

			fFB.byteArray[0] = blobInout[i * 8 + 7];
			fFB.byteArray[1] = blobInout[i * 8 + 6];
			fFB.byteArray[2] = blobInout[i * 8 + 5];
			fFB.byteArray[3] = blobInout[i * 8 + 4];

			float value = fFB.unionFloat;

			if (isConfiguringPreset())
				saveVcsInfo(id, value);
		}
		DBG("  -> finished blob reading");
	}


	void MyPacaranaManager::rollDices()
	{
		;
		oscConnector->addToSendingQueue(OSCMessage("/preset", 128));
	}

	void MyPacaranaManager::setAllToZero()
	{
		for (int i = 0; i < allMyWidgets.size(); i++)
		{
			oscConnector->addToSendingQueue(OSCMessage("/vcs", allMyWidgets[i].concreteId, 0)); // "/vcs" int float
		}
	}

	void MyPacaranaManager::getVcsNotif(bool yesOrNo)
	{
		int getNotifs = yesOrNo ? 1 : 0;
		oscConnector->addToSendingQueue(OSCMessage("/osc/notify/vcs/PC", getNotifs));
	}

	void MyPacaranaManager::getPresetNotif(bool yesOrNo)
	{
		int getNotifs = yesOrNo ? 1 : 0;
		oscConnector->addToSendingQueue(OSCMessage("/osc/notify/presets/PC", getNotifs));
	}

	// ================================================================================

	KymaPreset::KymaPreset()
	{
		name = "NOT ASSIGNED";
		widgetConfigurated = 0;
	}

	KymaPreset::~KymaPreset()
	{
	}

	void KymaPreset::init(MyPacaranaManager * refPaca)
	{
		pacaManager = refPaca;
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
			allWidgets[i].value = -1;
		}
	}

	void KymaPreset::setWidgetValue(int eventId, float newValue)
	{
		for (int i = 0; i < allWidgets.size(); i++)
		{
			if (eventId == allWidgets[i].concreteId)
			{
				allWidgets[i].value = newValue;
				widgetConfigurated++;
				break;
			}
		}

		if (isFullyConfigurated())
		{
			DBG("Config done : " + to_string());
			pacaManager->endPresetconfiguration();
		}
	}

	// ================================================================================
}





