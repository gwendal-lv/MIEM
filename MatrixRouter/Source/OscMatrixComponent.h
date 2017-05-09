/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "ISlidersMatrixListener.h"
#include "SpatCommon/Source/LabelledMatrixComponent.h"

namespace Miam {

    // Forward declarations
    class Presenter;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OscMatrixComponent  : public Component,
                            public ISlidersMatrixListener
{
public:
    //==============================================================================
    OscMatrixComponent (Presenter* _presenter);
    ~OscMatrixComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void OnSliderValueChanged(int row, int col, double value) override;
    void SetSliderValue(int row, int col, double value);
    void SetUdpPortAndMessage(int udpPort, bool isConnected);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Presenter* presenter;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> oscGroupComponent;
    ScopedPointer<TextEditor> udpPortTextEditor;
    ScopedPointer<Label> udpPortLabel;
    ScopedPointer<Label> udpStatusLabel;
    ScopedPointer<Miam::LabelledMatrixComponent> slidersMatrix;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscMatrixComponent)
};

//[EndFile] You can add extra defines here...
} // namespace Miam
//[/EndFile]
