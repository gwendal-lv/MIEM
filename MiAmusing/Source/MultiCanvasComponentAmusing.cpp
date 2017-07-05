/*
  ==============================================================================

    MultiCanvasComponentAmusing.cpp
    Created: 30 May 2017 3:19:47pm
    Author:  ayup1

  ==============================================================================
*/

#include "MultiCanvasComponentAmusing.h"
#include "IGraphicSessionManager.h"
#include "JuceHeader.h"

MultiCanvasComponentAmusing::MultiCanvasComponentAmusing(IGraphicSessionManager* _graphicSessionManager) :
	MultiCanvasComponent(_graphicSessionManager)
{
	
}

MultiCanvasComponentAmusing::~MultiCanvasComponentAmusing()
{
	//delete audioSetupComp;
}

//void MultiCanvasComponentAmusing::CreateDeviceSelector(AudioDeviceManager* deviceManager)
//{
//	audioSetupComp = new AudioDeviceSelectorComponent(*deviceManager,
//		0, 256, 0, 256, true, true, true, false);
//
//	options.content.setOwned(audioSetupComp);
//	Rectangle<int> area(0, 0, getWidth(), getHeight());
//	options.content->setSize(area.getWidth(), area.getHeight());
//	options.dialogTitle = "Dialog Window";
//	show = false;
//}

void MultiCanvasComponentAmusing::ShowDeviceOptionsDialog(AudioDeviceManager& deviceManager)
{

	DBG("showDeviceOptionDialog");
	show = true;
	
	//dialogWindow = options.launchAsync();

	/*if (dialogWindow != nullptr)
		dialogWindow->centreWithSize(getWidth(), getHeight());*/
}

void MultiCanvasComponentAmusing::resized()
{
	MultiCanvasComponent::resized();
	if (show == true)
	{
		DBG("resized with audioSetUpCOmp");
		Rectangle<int> rCanvas = getLocalBounds();
		rCanvas.reduce(8, 8);
		audioSetupComp->setBounds(rCanvas);
		addAndMakeVisible(audioSetupComp);
		audioSetupComp->setAlwaysOnTop(true);//toFront(true);
	}
}