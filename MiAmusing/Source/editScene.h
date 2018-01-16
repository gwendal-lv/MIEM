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
                   public ComboBox::Listener,
                   public Slider::Listener,
                   public Button::Listener
{
public:
    //==============================================================================
    EditScene ();
    ~EditScene();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void CompleteInitialization(GraphicSessionManager* _graphicSessionManager);//, MultiCanvasComponent* _multiCanvasComponent);
	void setMidiChannel(int chan);
	void hideAddPolygon();
	void showAddPolygon();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

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
    static const char* lecture_png;
    static const int lecture_pngSize;
    static const char* pause_png;
    static const int pause_pngSize;
    static const char* stop_png;
    static const int stop_pngSize;
    static const char* delete_png;
    static const int delete_pngSize;
    static const char* option_png;
    static const int option_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    GraphicSessionManager* graphicSessionManager = 0;
	bool isAddEnabled;
	//MultiCanvasComponent* multiCanvasComponent = 0;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> groupComponent;
    ScopedPointer<ComboBox> comboBoxMidi;
    ScopedPointer<Slider> timeSlider;
    ScopedPointer<ImageButton> imgPlayButton;
    ScopedPointer<ImageButton> imgStopButton;
    ScopedPointer<ImageButton> imgPauseButton;
    ScopedPointer<ImageButton> imgOptionButton;
    ScopedPointer<ImageButton> imgDeleteButton;
    ScopedPointer<TextButton> textButton;
    ScopedPointer<Label> label;
    ScopedPointer<ShapeButton> addCarreShapeButton;
    ScopedPointer<ShapeButton> addTriangleShapeButton2;
    ScopedPointer<ShapeButton> addHexaShapeButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditScene)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
