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
                   public Button::Listener,
                   public ComboBox::Listener,
                   public Slider::Listener
{
public:
    //==============================================================================
    EditScene ();
    ~EditScene();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void CompleteInitialization(GraphicSessionManager* _graphicSessionManager);//, MultiCanvasComponent* _multiCanvasComponent);
	void setMidiChannel(int chan);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* stopOn_png;
    static const int stopOn_pngSize;
    static const char* pauseOn_png;
    static const int pauseOn_pngSize;
    static const char* optionOn_png;
    static const int optionOn_pngSize;
    static const char* lectureOn_png;
    static const int lectureOn_pngSize;
    static const char* deleteOn_png;
    static const int deleteOn_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    GraphicSessionManager* graphicSessionManager = 0;
	//MultiCanvasComponent* multiCanvasComponent = 0;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> addCarreButton;
    ScopedPointer<TextButton> addTriangleButton;
    ScopedPointer<TextButton> completeButton;
    ScopedPointer<ComboBox> comboBoxMidi;
    ScopedPointer<Slider> timeSlider;
    ScopedPointer<ImageButton> imgPlayButton;
    ScopedPointer<ImageButton> imgStopButton;
    ScopedPointer<ImageButton> imgPauseButton;
    ScopedPointer<ImageButton> imgOptionButton;
    ScopedPointer<ImageButton> imgDeleteButton;
    ScopedPointer<TextButton> textButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditScene)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
