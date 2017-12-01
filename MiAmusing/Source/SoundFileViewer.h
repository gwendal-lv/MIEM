/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundFilesManager.h"

class SoundFilesManager;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SoundFileViewer  : public Component,
                         public Button::Listener
{
public:
    //==============================================================================
    SoundFileViewer ();
    ~SoundFileViewer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void completeInitialization(ScopedPointer<SoundFilesManager> m_manager);
	void setSoundPath(String m_path);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	ScopedPointer<SoundFilesManager> manager;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> sampleLabel;
    ScopedPointer<Label> colorLabel;
    ScopedPointer<TextButton> deleteButton;
    ScopedPointer<TextButton> loadButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundFileViewer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
