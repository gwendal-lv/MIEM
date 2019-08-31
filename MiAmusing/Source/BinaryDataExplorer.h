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

//==============================================================================
/*
*/
class BinaryDataExplorer    : public Component, public TableListBoxModel
{
public:
    BinaryDataExplorer();
    ~BinaryDataExplorer();

    void paint (Graphics&) override;
    void resized() override;

	int getNumRows() override;

	void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool) override;
	void paintRowBackground(Graphics & g, int rowNumber, int, int, bool rowIsSelected) override;
	void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;

	String getSelectedFileName();

private:
	String binaryDataName;
	TableListBox table;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinaryDataExplorer)
};
