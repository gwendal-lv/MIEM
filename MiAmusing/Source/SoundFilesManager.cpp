/*
  ==============================================================================

    soundFilesManager.cpp
    Created: 30 Nov 2017 7:03:48pm
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundFilesManager.h"

//==============================================================================
SoundFilesManager::SoundFilesManager()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	addSoundFileViewerButton = new TextButton();
	addSoundFileViewerButton->setButtonText("+");
	addAndMakeVisible(addSoundFileViewerButton);
	addSoundFileViewerButton->addListener(this);

	closeSoundFileManagerButton = new TextButton();
	closeSoundFileManagerButton->setButtonText("OK");
	addAndMakeVisible(closeSoundFileManagerButton);
	closeSoundFileManagerButton->addListener(this);

	for (int i = 0; i < 4; ++i)
	{
		soundFileViewerArray.add(new SoundFileViewer());
		addAndMakeVisible(soundFileViewerArray.getLast());
		soundFileViewerArray.getLast()->completeInitialization(this);
	}

	itemHeight = soundFileViewerArray.getFirst()->getHeight();
	spaceHeight = 2;
}

SoundFilesManager::~SoundFilesManager()
{
}

void SoundFilesManager::paint (Graphics& g)
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
    g.drawText ("soundFilesManager", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void SoundFilesManager::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	Rectangle<int> r(getLocalBounds());
	for (int i = 0; i < soundFileViewerArray.size(); ++i)
	{
		soundFileViewerArray[i]->setBounds(r.removeFromTop(itemHeight));
		r.removeFromTop(spaceHeight);
	}
	r.removeFromTop(itemHeight);
	
	closeSoundFileManagerButton->setBounds(r.removeFromBottom(itemHeight));
	r.removeFromBottom(spaceHeight);
	addSoundFileViewerButton->setBounds(r.removeFromBottom(itemHeight));
	
}

void SoundFilesManager::buttonClicked(Button * buttonThatWasClicked)
{
	if (buttonThatWasClicked == addSoundFileViewerButton)
		addSoundFileViewer();
	else if (buttonThatWasClicked == closeSoundFileManagerButton)
		mainComponent->CloseSoundFileManager();
}

void SoundFilesManager::completeInitialisation(MainContentComponent * m_mainComponent)
{
	mainComponent = m_mainComponent;
}

void SoundFilesManager::loadSoundFile(SoundFileViewer * soundFileViewer)
{
	soundFileViewer->setSoundPath("myNewPath");
}

void SoundFilesManager::addSoundFileViewer()
{
	if (soundFileViewerArray.size() < Nmax - 1)
	{
		
		soundFileViewerArray.add(new SoundFileViewer());
		
		addAndMakeVisible(soundFileViewerArray.getLast());
		soundFileViewerArray.getLast()->completeInitialization(this);
		soundFileViewerArray.getLast()->setBounds(soundFileViewerArray[soundFileViewerArray.size()-2]->getX(), soundFileViewerArray[soundFileViewerArray.size() - 2]->getY() + itemHeight + spaceHeight,
			soundFileViewerArray[soundFileViewerArray.size() - 2]->getWidth(), soundFileViewerArray[soundFileViewerArray.size() - 2]->getHeight());
		
	}
}
