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
#include "MainComponent.h"
#include "BinaryDataExplorer.h"

class BinaryDataExplorer;
class MainContentComponent;

//==============================================================================
/*
*/
class SoundBrowser    : public Component, public FileBrowserListener, public Button::Listener
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
	ScopedPointer<TextButton> cancelButton;

	ScopedPointer<BinaryDataExplorer> binaryDataExplorer;

	String pathToSound;
	MainContentComponent *mainComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundBrowser)
};
