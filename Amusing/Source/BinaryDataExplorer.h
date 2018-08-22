/*
  ==============================================================================

    BinaryDataExplorer.h
    Created: 1 Dec 2017 1:45:57pm
    Author:  ayup1

  ==============================================================================
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
