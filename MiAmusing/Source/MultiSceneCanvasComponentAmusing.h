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

#pragma once

#include "JuceHeader.h"

#include "MultiSceneCanvasComponent.h"

#include "AreaOptionsComponent.h"

//class AreaOptionsComponent;
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

	void muteOtherAreas(int shouldMuteOtherAreas);

	void optionButtonClicked(OptionButtonClicked optionClicked);

private:
	//AreaOptions areaOptions;
	ScopedPointer<AreaOptionsComponent> areaOptionsComponent;
	OptionButtonClicked currentOptionClicked;
};

