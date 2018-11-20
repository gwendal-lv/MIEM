/*
  ==============================================================================

    BinaryDataExplorer.cpp
    Created: 1 Dec 2017 1:45:57pm
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "BinaryDataExplorer.h"

//==============================================================================
BinaryDataExplorer::BinaryDataExplorer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	addAndMakeVisible(table);
	table.setModel(this);

	// give it a border
	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);

	//table.getHeader().setColumnName(0,"sample");
	Rectangle<int> r(getLocalBounds());
	table.getHeader().addColumn("sample", 1, 100);
}

BinaryDataExplorer::~BinaryDataExplorer()
{
}

void BinaryDataExplorer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("BinaryDataExplorer", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void BinaryDataExplorer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	//table.setBoundsInset(BorderSize<int>(8));
	Rectangle<int> r(getBounds());
	Rectangle<int> rLocal(getLocalBounds());
	table.setBounds(getLocalBounds());
	//table.getHeader().setStretchToFitActive(getWidth());
	table.getHeader().setColumnWidth(1, getWidth());
}

int BinaryDataExplorer::getNumRows()
{
	return 5;//BinaryData::namedResourceListSize;
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void BinaryDataExplorer::paintCell(Graphics& g, int rowNumber, int /*columnId*/,
	int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		g.setColour(Colours::black);
	else
		g.setColour(Colours::lightgrey);
	if(rowNumber < BinaryData::namedResourceListSize)
		g.drawText(BinaryData::namedResourceList[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
	else
		g.drawText("pouley", 2, 0, width - 4, height, Justification::centredLeft, true);
}

void BinaryDataExplorer::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
	const Colour alternateColour(getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f));
	if (rowIsSelected)
		g.fillAll(Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll(alternateColour);
}

void BinaryDataExplorer::cellClicked(int rowNumber, int /*columnId*/, const MouseEvent &)
{
	binaryDataName = BinaryData::namedResourceList[rowNumber];
}

String BinaryDataExplorer::getSelectedFileName()
{
	return binaryDataName;
}
