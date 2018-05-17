/*
  ==============================================================================

    MultiSceneCanvasComponentAmusing.h
    Created: 10 Jul 2017 11:45:38am
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "MultiSceneCanvasComponent.h"

class AreaOptionsComponent;
namespace Amusing
{
	class MultiSceneCanvasManager;
}

class MultiSceneCanvasComponentAmusing : public MultiSceneCanvasComponent
{
public:
	MultiSceneCanvasComponentAmusing();
	~MultiSceneCanvasComponentAmusing();

	void resized() override;

	void setSamplesColor(int Nsamples, Colour colorCode[]);

	void addColourSample(int index, Colour colour);

	void showAreaOptions(bool shouldBeVisible);

	void optionsClosed();

private:
	//AreaOptions areaOptions;
	ScopedPointer<AreaOptionsComponent> areaOptionsComponent;
};

