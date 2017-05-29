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
#include "JuceHeader.h"

#include "SpatStatesEditionManager.h"

#include "LabelledMatrixComponent.h"

#include "ISlidersMatrixListener.h"

#include "SpatMatrix.hpp"

namespace Miam {
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
                                    public ISlidersMatrixListener,
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

    /// \brief Empty function : we'll deal with this editor with full
    /// up-to-date matrices only (to prevent any error with notifying
    /// and being notified of modifications...)
    void OnSliderValueChanged(int row, int col, double value) override {}

    /// \brief Updates the combo box
    ///
    /// WARNING : Juce's items id's start from ONE and not from ZERO
    // But Juce's Indexes are OK (starting from zero)
    void UpdateStatesList(std::vector< std::shared_ptr<SpatState<double>> > &newSpatStates);

    // When a new state is selected (from the Presenter or from an internal event)
    void SelectAndUpdateState(int stateIndex, std::string infoText, std::shared_ptr<SpatMatrix> newSpatMatrix);

    /// \brief Whole update of the matrix : all coefficients, and active sliders
    private :
    void updateMatrix();

    public :
    void SetInsOutsCount(int _inputsCount, int _outputsCount);
    std::shared_ptr<SpatMatrix> GetDisplayedSpatMatrix();

    void AllowKeyboardEdition(bool allow);

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SpatStatesEditionManager* editionManager = 0;

    int previousStateIndex = -1; // nothing selected in Juce convention

    // For backing up the data (duplicated data from Model)
    int inputsCount = 0;
    int outputsCount = 0;
    // The last matrix coming from the Model
    std::shared_ptr<SpatMatrix> spatMatrix;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> stateEditorGroupComponent;
    ScopedPointer<Miam::LabelledMatrixComponent> labelledMatrixComponent;
    ScopedPointer<GroupComponent> spatStateGroupComponent;
    ScopedPointer<TextButton> addSpatStateTextButton;
    ScopedPointer<TextButton> deleteSpatStateTextButton;
    ScopedPointer<TextButton> stateUpTextButton;
    ScopedPointer<TextButton> stateDownTextButton;
    ScopedPointer<Label> linksInfoLabel;
    ScopedPointer<ComboBox> spatStatesComboBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpatStatesEditionComponent)
};

//[EndFile] You can add extra defines here...
} // namespace Miam
//[/EndFile]
