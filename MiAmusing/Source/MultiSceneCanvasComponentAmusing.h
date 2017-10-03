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


class MultiSceneCanvasComponentAmusing : public MultiSceneCanvasComponent
{
public:
	MultiSceneCanvasComponentAmusing();
	~MultiSceneCanvasComponentAmusing();

	void resized() override;

private:
	//AreaOptions areaOptions;
};

