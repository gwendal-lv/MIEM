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

#ifndef __JUCE_HEADER_213F09986CBED4FE__
#define __JUCE_HEADER_213F09986CBED4FE__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include "SpatStatesEditionManager.h"
class SpatStateFadersDisplayComponent;

using namespace Miam;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

// \brief An important component that corresponds to an Miam::AppMode of the
// Miam::Presenter.
//
// An auto-generated component, created by the Projucer.
//

                                                                    //[/Comments]
*/
class SpatStatesEditionComponent  : public Component,
                                    public ButtonListener,
                                    public ComboBoxListener
{
public:
    //==============================================================================
    SpatStatesEditionComponent ();
    ~SpatStatesEditionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void CompleteInitialization(SpatStatesEditionManager* _editionManager);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SpatStatesEditionManager* editionManager = 0;
    
    ScopedPointer<SpatStateFadersDisplayComponent> fadersDisplayComponent;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> spatStateGroupComponent;
    ScopedPointer<TextButton> addSpatStateTextButton;
    ScopedPointer<TextButton> deleteSpatStateTextButton;
    ScopedPointer<TextButton> stateUpTextButton;
    ScopedPointer<TextButton> stateDownTextButton;
    ScopedPointer<Label> linksInfoLabel;
    ScopedPointer<Label> renameLabel;
    ScopedPointer<TextEditor> stateNameTextEditor;
    ScopedPointer<ComboBox> spatStatesComboBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpatStatesEditionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_213F09986CBED4FE__
