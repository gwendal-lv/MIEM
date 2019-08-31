/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
	
}

// - - - - - Canvases & canvas group - - - - -
MultiSceneCanvasComponent* MultiCanvasComponentAmusing::AddCanvas()
{
	multiSceneCanvasComponents.push_back(new MultiSceneCanvasComponentAmusing()); //new MultiSceneCanvasComponentAmusing()
	addAndMakeVisible(multiSceneCanvasComponents.back());
	return multiSceneCanvasComponents.back();
}
