/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "MyPacaranaManager.h"
using namespace Miam;

//==============================================================================
MainComponent::MainComponent()
{
	// Add my OscConnector
	addAndMakeVisible(myOscConnector);
	myOscConnector.init(&pacaManager);

	// base stuff
	setSize(1110, 750);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	/*g.setFont(Font(16.0f));
	g.setColour(Colours::white);
	g.drawText("Hello World!", getLocalBounds(), Justification::centred, true);*/
}

void MainComponent::resized()
{
	// This is called when the MainComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
}
