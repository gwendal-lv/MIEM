/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_B6F0B542616105F0__
#define __JUCE_HEADER_B6F0B542616105F0__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"



namespace Amusing
{
	class GraphicSessionManager;
	
}
using namespace Amusing;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EditScene  : public Component,
                   public ButtonListener
{
public:
    //==============================================================================
    EditScene ();
    ~EditScene();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void CompleteInitialization(GraphicSessionManager* _graphicSessionManager);//, MultiCanvasComponent* _multiCanvasComponent);
    
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    GraphicSessionManager* graphicSessionManager = 0;
	
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> playButton;
    ScopedPointer<TextButton> pauseButton;
    ScopedPointer<TextButton> stopButton;
    ScopedPointer<TextButton> addCarreButton;
    ScopedPointer<TextButton> addTriangleButton;
    ScopedPointer<TextButton> addCircleButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditScene)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B6F0B542616105F0__
