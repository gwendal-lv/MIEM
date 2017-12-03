/*
  ==============================================================================

    SoundBrowser.h
    Created: 30 Nov 2017 5:04:42pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "BinaryDataExplorer.h"

class BinaryDataExplorer;
class MainContentComponent;

//==============================================================================
/*
*/
class SoundBrowser    : public Component, public FileBrowserListener, public ButtonListener
{
public:
    SoundBrowser();
    ~SoundBrowser();

    void paint (Graphics&) override;
    void resized() override;

	void buttonClicked(Button* buttonThatWasClicked) override;

	void completeInitialisation(MainContentComponent *m_mainComponent);
	void release();

	String getPathToSound();

private:
	
	void selectionChanged() override;
	void fileClicked(const File&, const MouseEvent&) override {}
	void fileDoubleClicked(const File&) override {}
	void browserRootChanged(const File&) override {}

	WildcardFileFilter soundsWildcardFilter;
	TimeSliceThread directoryThread;
	DirectoryContentsList soundList;
	FileTreeComponent fileTree;

	bool filesExplorerOpen;
	ScopedPointer<TextButton> binaryDataExplorerButton;
	ScopedPointer<TextButton> filesExplorerButton;
	ScopedPointer<TextButton> closeButton;

	ScopedPointer<BinaryDataExplorer> binaryDataExplorer;

	String pathToSound;
	MainContentComponent *mainComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundBrowser)
};
