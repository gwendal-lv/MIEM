/*
  ==============================================================================

    MultiCanvasComponentAmusing.cpp
    Created: 30 May 2017 3:19:47pm
    Author:  ayup1

  ==============================================================================
*/

#include "MultiCanvasComponentAmusing.h"
#include "IGraphicSessionManager.h"

MultiCanvasComponentAmusing::MultiCanvasComponentAmusing(IGraphicSessionManager* _graphicSessionManager) :
	MultiCanvasComponent(_graphicSessionManager)
{
}

MultiCanvasComponentAmusing::~MultiCanvasComponentAmusing()
{
}

void MultiCanvasComponentAmusing::ShowDeviceOptionsDialog(AudioDeviceManager& deviceManager)
{
	DialogWindow::LaunchOptions options;
	audioSetupComp = new AudioDeviceSelectorComponent(deviceManager,
		0, 256, 0, 256, true, true, true, false);
	options.content.setOwned(audioSetupComp);
	Rectangle<int> area(0, 0, getWidth(), getHeight());
	options.content->setSize(area.getWidth(), area.getHeight());
	options.dialogTitle = "Dialog Window";
	dialogWindow = options.launchAsync();

	if (dialogWindow != nullptr)
		dialogWindow->centreWithSize(getWidth(), getHeight());
}
