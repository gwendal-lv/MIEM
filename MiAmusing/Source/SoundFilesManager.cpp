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

SoundFilesManager::SoundFilesManager(const int Nsamples,Colour colorCode[])
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

	for (int i = 0; i < Nsamples; ++i)
	{
		soundFileViewerArray.add(new SoundFileViewer());
		addAndMakeVisible(soundFileViewerArray.getLast());
		soundFileViewerArray.getLast()->completeInitialization(this);
		soundFileViewerArray.getLast()->setColourSample(colorCode[i]);
	}

	itemHeight = soundFileViewerArray.getFirst()->getHeight();
	spaceHeight = 2;
	repaint();
}

SoundFilesManager::~SoundFilesManager()
{
	for (int i = 0; i < soundFileViewerArray.size(); ++i)
		soundFileViewerArray[i]->release();
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
    //g.drawText ("soundFilesManager", getLocalBounds(),
    //            Justification::centred, true);   // draw some placeholder text
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
	
	Rectangle<int> buttonArea = r.removeFromBottom(itemHeight);
	buttonArea.reduce(100, 0);
	closeSoundFileManagerButton->setBounds(buttonArea);
	
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

void SoundFilesManager::setDefaultPath(String m_defaultPath)
{
	defaultPath = m_defaultPath;
	for (int i = 0; i < soundFileViewerArray.size(); ++i)
		soundFileViewerArray[i]->setSoundPath(defaultPath);
}

String SoundFilesManager::getDefaultPath()
{
	return defaultPath;
}

void SoundFilesManager::release()
{
	mainComponent = nullptr;
}

void SoundFilesManager::loadSoundFile(SoundFileViewer * soundFileViewer)
{
	for (int i = 0; i < soundFileViewerArray.size(); ++i)
		if (soundFileViewerArray[i] == soundFileViewer)
			sampleToSet = i;
	mainComponent->OpenSoundBrowser(sampleToSet,soundFileViewerArray[sampleToSet]->getSampleColour());
	//soundFileViewer->setSoundPath("myNewPath");
}

void SoundFilesManager::setCurrentSoundFilePath(String m_soundPath)
{
	soundFileViewerArray[sampleToSet]->setSoundPath(m_soundPath);
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

std::shared_ptr<bptree::ptree> SoundFilesManager::GetSoundTree()
{
	auto audioTree = std::make_shared<bptree::ptree>();
	for (int i = 0; i < soundFileViewerArray.size(); ++i)
	{
		bptree::ptree soundTree;
		soundTree.put("<xmlattr>.index", i);
		soundTree.put("<xmlattr>.soundFilePath", soundFileViewerArray[i]->getSoundPath().toStdString());
		soundTree.put("<xmlattr>.color", soundFileViewerArray[i]->getSampleColour().toString().toStdString());
		audioTree->add_child("sound",soundTree);
	}

	return audioTree;
}

void SoundFilesManager::SetSoundTree(bptree::ptree tree)
{
	bptree::ptree soundTree;
	soundTree = tree.get_child("soundTree");

	// mettre un while !empty quand on pourra avoir autant de son qu'on veut
	for (int i = 0; i < 4; ++i)
	{
		bptree::ptree currentSoundTree = soundTree.get_child("sound");
		auto index = currentSoundTree.get<size_t>("<xmlattr>.index");
		auto soundFilePath = currentSoundTree.get<std::string>("<xmlattr>.soundFilePath");
		auto color = currentSoundTree.get<std::string>("<xmlattr>.color");
		soundFileViewerArray[index]->setColourSample(Colour::fromString(StringRef(color)));
		soundFileViewerArray[index]->setSoundPath(soundFilePath);
	}

}