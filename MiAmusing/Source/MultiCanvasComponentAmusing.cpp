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
#include "MultiSceneCanvasComponentAmusing.h"

MultiCanvasComponentAmusing::MultiCanvasComponentAmusing(IGraphicSessionManager* _graphicSessionManager) :
	MultiCanvasComponent(_graphicSessionManager)
{
	
}

MultiCanvasComponentAmusing::~MultiCanvasComponentAmusing()
{
	//delete audioSetupComp;
}

void MultiCanvasComponentAmusing::setSamplesColor(int Nsamples, Colour colorCode[])
{
	MultiSceneCanvasComponent* compo = multiSceneCanvasComponents[0];
	if (MultiSceneCanvasComponentAmusing* compoAmusing = (MultiSceneCanvasComponentAmusing*)compo)
	{
		compoAmusing->setSamplesColor(Nsamples, colorCode);
		compoAmusing = nullptr;
	}
}

void MultiCanvasComponentAmusing::addColourSample(int index, Colour colour)
{
	MultiSceneCanvasComponent* compo = multiSceneCanvasComponents[0];
	if (MultiSceneCanvasComponentAmusing* compoAmusing = (MultiSceneCanvasComponentAmusing*)compo)
	{
		compoAmusing->addColourSample(index, colour);
		compoAmusing = nullptr;
	}
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

// - - - - - Canvases & canvas group - - - - -
MultiSceneCanvasComponent* MultiCanvasComponentAmusing::AddCanvas()
{
	multiSceneCanvasComponents.push_back(new MultiSceneCanvasComponentAmusing()); //new MultiSceneCanvasComponentAmusing()
	addAndMakeVisible(multiSceneCanvasComponents.back());
	return multiSceneCanvasComponents.back();
}
