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
#include "MouseSimulator.h"

class MultiSceneCanvasComponentAmusing : public MultiSceneCanvasComponent
{
public:
	MultiSceneCanvasComponentAmusing();
	~MultiSceneCanvasComponentAmusing();

	void resized() override;

	void LinkToManager(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_);

	void LinkToMouseSimulator();

	void LinkToMouseSimulator(StringArray opt);

	void TryToClose();

private:
	//AreaOptions areaOptions;
	std::shared_ptr<MouseSimulator> mouseSimulator;
};

