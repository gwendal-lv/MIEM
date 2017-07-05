/*
  ==============================================================================

    OptionWindow.cpp
    Created: 5 Jul 2017 11:00:48am
    Author:  ayup1

  ==============================================================================
*/

#include "OptionWindow.h"
#include "MainComponent.h"
using namespace Amusing;
OptionWindow::OptionWindow(MainContentComponent* _mainComponent)
{
	OKbutton = new TextButton();
	OKbutton->setButtonText("OK");
	OKbutton->addListener(this);
	addAndMakeVisible(OKbutton);

	mainComponent = _mainComponent;
}

OptionWindow::~OptionWindow()
{
	delete OKbutton;
}

void OptionWindow::CompleteInitialization(AudioDeviceManager& deviceManager)
{
	audioSetupComp = new AudioDeviceSelectorComponent(deviceManager,
		0, 256, 0, 256, true, true, true, false);
	addAndMakeVisible(audioSetupComp);
}

void OptionWindow::removeDeviceManager()
{
	audioSetupComp->deviceManager.removeAllChangeListeners();
}

void OptionWindow::paint(Graphics& g)
{

}

void OptionWindow::resized()
{
	if(audioSetupComp)
		audioSetupComp->setBounds(0,0,getWidth(),getHeight()-20);
	OKbutton->setBounds(getWidth()/2 - 25, getHeight()-20,50,20);
}

void OptionWindow::buttonClicked(Button * button)
{
	if (button == OKbutton)
		mainComponent->CloseOptionWindow();//DBG("OK clicked"); // dire au mainContentComponent de revenir aux scenes
}