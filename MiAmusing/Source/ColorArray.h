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

#include "../JuceLibraryCode/JuceHeader.h"
#include "AreaOptions.h"

class AreaOptions;

//==============================================================================
/*
	This component contains de avalaible color for the Polygons.
	Each color is associated with a path to a sample by the SoundFileManager
*/
class ColorArray    : public Component, public Button::Listener
{
public:
    ColorArray(int size = 4);
    ~ColorArray();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button*) override;

	void completeInitialisation(AreaOptions *m_areaOption);
	void setSamplesColor(int Nsamples, Colour colorCode[]);
	void addColourSample(int index, Colour colour);
	void setCurrentColorSelected(int idx);
	//void addColor(Colour newColour);

private:
	int currentColor;
	bool wasClicked;
	OwnedArray<TextButton> buttonArray;

	AreaOptions *areaOptions;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorArray)
};
