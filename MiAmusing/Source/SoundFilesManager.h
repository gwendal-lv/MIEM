/*
  ==============================================================================

    soundFilesManager.h
    Created: 30 Nov 2017 7:03:48pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundFileViewer.h"
#include "MainComponent.h"

class SoundFileViewer;
class MainContentComponent;

//==============================================================================
/*
*/
class SoundFilesManager    : public Component, public Button::Listener
{
public:
    SoundFilesManager();
	SoundFilesManager(const int Nsamples, Colour colorCode[]);
    ~SoundFilesManager();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* buttonThatWasClicked) override;

	void completeInitialisation(MainContentComponent *m_mainComponent);
	void setDefaultPath(String m_defaultPath);
	String getDefaultPath();
	void release();

	void loadSoundFile(SoundFileViewer* SoundFileViewer);
	void setCurrentSoundFilePath(String m_soundPath);
	void addSoundFileViewer();

	std::shared_ptr<bptree::ptree> GetSoundTree();

	void SetSoundTree(bptree::ptree tree);

private:
	int Nmax = 7;
	int itemHeight, spaceHeight;
	String defaultPath;
	int sampleToSet;
	ScopedPointer<TextButton> closeSoundFileManagerButton;
	ScopedPointer<TextButton> addSoundFileViewerButton;
	OwnedArray<SoundFileViewer> soundFileViewerArray;

	MainContentComponent* mainComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundFilesManager)
};
