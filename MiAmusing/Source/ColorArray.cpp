/*
  ==============================================================================

    ColorArray.cpp
    Created: 30 Nov 2017 11:54:40am
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ColorArray.h"

//==============================================================================
ColorArray::ColorArray(int /*size*/)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	//setSize(getLocalBounds().getWidth(),getLocalBounds().getHeight());
	Colour myColor[4] = { Colours::blue,Colours::red,Colours::yellow,Colours::white };

	currentColor = 0;
	wasClicked = false;
	/*for (int i = 0; i < size; ++i)
	{
		buttonArray.add(new TextButton(String(i)));
		buttonArray.getLast()->addListener(this);
		buttonArray.getLast()->setColour(TextButton::ColourIds::buttonColourId, myColor[i]);
		if (i == 0)
			addAndMakeVisible(buttonArray.getLast());
		else
			addChildComponent(buttonArray.getLast());


	}*/
}

ColorArray::~ColorArray()
{
}



void ColorArray::paint(Graphics &/*g*/)
{
	//g.fillAll(Colours::black);//getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    //g.setColour (Colours::grey);
  
}

void ColorArray::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	if (buttonArray.size() > 0)
	{
		Rectangle<int> r = getLocalBounds();


		if (getWidth() >= getHeight())
		{
			int buttonWidth = r.getWidth() / buttonArray.size();

			for (int i = 0; i < buttonArray.size(); ++i)
			{
				buttonArray[i]->setBounds(r.removeFromLeft(buttonWidth));
			}
		}
		else
		{
			int buttonHeight = r.getHeight() / buttonArray.size();

			for (int i = 0; i < buttonArray.size(); ++i)
			{
				buttonArray[i]->setBounds(r.removeFromTop(buttonHeight));
			}
		}

	}
	
}

void ColorArray::buttonClicked(Button *buttonThatWasClicked)
{

	for (int i = 0; i < buttonArray.size(); ++i)
	{
		if (buttonThatWasClicked == buttonArray[i])
		{
			
			if (wasClicked == false)
			{
				for (int j = 0; j < buttonArray.size(); ++j)
					if (j != i)
						buttonArray[j]->setVisible(true);
				wasClicked = true;
			}
			else
			{
				if (currentColor != i)
				{
					Rectangle<int> r(buttonArray[i]->getBounds());
					buttonArray[i]->setBounds(buttonArray[currentColor]->getBounds());
					buttonArray[currentColor]->setBounds(r);
					currentColor = i;
					areaOptions->OnColorChanged(buttonArray[currentColor]->findColour(TextButton::ColourIds::buttonColourId),currentColor);
				}
				for (int j = 0; j < buttonArray.size(); ++j)
					if (j != i)
						buttonArray[j]->setVisible(false);
				wasClicked = false;
			}
		}
	}
}


void ColorArray::completeInitialisation(AreaOptions * m_areaOption)
{
	areaOptions = m_areaOption;
}

void ColorArray::setSamplesColor(int Nsamples, Colour colorCode[])
{
	for (int i = 0; i < Nsamples; ++i)
	{
		buttonArray.add(new TextButton(String(i)));
		buttonArray.getLast()->addListener(this);
		buttonArray.getLast()->setColour(TextButton::ColourIds::buttonColourId, colorCode[i]);
		if (i == 0)
			addAndMakeVisible(buttonArray.getLast());
		else
			addChildComponent(buttonArray.getLast());


	}
	resized();
}

void ColorArray::addColourSample(int index, Colour colour)
{
	buttonArray.add(new TextButton(String(index)));
	buttonArray.getLast()->addListener(this);
	buttonArray.getLast()->setColour(TextButton::ColourIds::buttonColourId, colour);
	addChildComponent(buttonArray.getLast());
	resized();
}

void ColorArray::setCurrentColorSelected(int idx)
{
	// appeler cette fonction dans buttonClicked pour plus de clareté !
	Rectangle<int> r(buttonArray[idx]->getBounds());
	buttonArray[idx]->setBounds(buttonArray[currentColor]->getBounds());
	buttonArray[currentColor]->setBounds(r);
	buttonArray[currentColor]->setVisible(false);
	currentColor = idx;
	buttonArray[idx]->setVisible(true);
	
}


//void ColorArray::addColor(Colour newColour)
//{
//	buttonArray.add(new TextButton());
//}
