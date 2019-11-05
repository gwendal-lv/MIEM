/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include "LabelledMatrixComponent.h"

#include "ISlidersMatrixListener.h"
#include "IMatrixButtonListener.h"

#include "ControlMatrix.hpp"
#include "ControlState.hpp"

#include "AudioUtils.hpp"

namespace Miam {

    // forward declarations
    class StatesEditionManager;

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
class StatesEditionComponent  : public Component,
                                public ISlidersMatrixListener,
                                public IMatrixButtonListener,
                                public Button::Listener,
                                public ComboBox::Listener,
                                public Slider::Listener
{
public:
    //==============================================================================
    StatesEditionComponent ();
    ~StatesEditionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void CompleteInitialization(StatesEditionManager* _editionManager);

	void Reinit();

    /// - - - - - Events - - - - -

    private :
    /// \brief Called by any color slider that moved
    void colourSliderMoved();

    public :
    /// \brief Makes some graphical updates only
    ///
    /// Mostly empty function : we'll deal with this editor with full
    /// up-to-date matrices only (to prevent any error with notifying
    /// and being notified of modifications...)
    void OnSliderValueChanged(int row, int col, double value) override;
    virtual void OnMatrixButtonClicked(int row, int col, std::string matrixText, double matrixValue) override;
    /// \brief Makes some graphical updates only
    void OnMatrixZeroed() override;
    /// \brief When any param of the interp curve is being changed
    void OnInterpolationCurveChanged(int row, BasicInterpolationCurve<double> newInterpCurve) override;

    /// \brief Updates the combo box
    ///
    /// WARNING : Juce's items id's start from ONE and not from ZERO
    // But Juce's Indexes are OK (starting from zero)
    void UpdateStatesList(std::vector< std::shared_ptr<ControlState<double>> > &newSpatStates);

    /// \brief When a new state is selected (order from the Presenter only)
    void SelectAndUpdateState(int stateIndex, std::string infoText,
                              std::shared_ptr<ControlMatrix<double>> newSpatMatrix,
                              std::vector<size_t> defaultValuesIndexes,
                              const Colour& stateColour,
        std::shared_ptr<BasicInterpCurves> newInterpCurves);

    /// \brief Updates the links label
    void UpdateLinksLabel(const std::string& infoText);
    void UpdateMatrixData(const std::string& volume1, const std::string& volume2);
    void SetVisibleMatrixData(bool shouldBeVisible);

    /// \brief Whole update of the matrix : all coefficients, and active sliders
    private :
    void updateMatrix();


    /// - - - - - Helpers - - - - -
    private :
    void setParametersGroupEnabled(bool shouldBeEnabled);
    void setColour(const Colour& stateColour);
    /// \brief Tries to select the next state in list, going downward or upward
    /// \returns Wether another state could be selected or not
    bool trySelectNextState(bool shouldGoDownward = true);

    // - - - - - Setters and Getters - - - - -
    public :
    void SetInsOutsCount(int _inputsCount, int _outputsCount);
    void SetInOutNames(InOutChannelsName &channelsName);
    void SetInOutNamesDisplayed(bool areInputNamesVisible, bool areOutputNamesVisible);

    void SetAreaColourValue(juce::Colour colour);

    std::shared_ptr<ControlMatrix<double>> GetDisplayedSpatMatrix();
    LabelledMatrixComponent* GetLabelledMatrix() { return labelledMatrixComponent.get(); }

    void AllowKeyboardEdition(bool allow);


    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void visibilityChanged() override;
    bool keyPressed (const KeyPress& key) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    StatesEditionManager* editionManager = 0;

    int previousStateIndex = -1; // nothing selected in Juce convention

    // For backing up the data (duplicated data from Model)
    int inputsCount = 0;
    int outputsCount = 0;
    // The last matrix coming from the Model
    std::shared_ptr<ControlMatrix<double>> spatMatrix;

    String spatStatesListText;
    String spatStateEditorText;
    String genericStatesListText;
    String genericStateEditorText;

    // ====================== Keyboard SHORTCUTS =====================
    // codé vite et mal... à refactorer dans les boutons directement en les faisant
    // hériter d'une classe gestionnaire des raccourcis...
    // all working with Cmd (Ctrl) at the moment
    const char newStateCmdKey = 'N';
    const int deleteStateCmdKey = juce::KeyPress::deleteKey;
    const int previousStateCmdKey = juce::KeyPress::upKey;
    const int nextStateCmdKey = juce::KeyPress::downKey;
    const char sendStateCmdKey = 'E';
    // working with Cmd and Shift
    const char sendZerosCmdShiftKey = 'E';
    const int stateUpCmdShiftKey = juce::KeyPress::upKey;
    const int stateDownCmdShiftKey = juce::KeyPress::downKey;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> stateParametersGroupComponent;
    std::unique_ptr<GroupComponent> stateEditorGroupComponent;
    std::unique_ptr<Miam::LabelledMatrixComponent> labelledMatrixComponent;
    std::unique_ptr<GroupComponent> statesListGroupComponent;
    std::unique_ptr<TextButton> addStateTextButton;
    std::unique_ptr<TextButton> deleteStateTextButton;
    std::unique_ptr<TextButton> stateUpTextButton;
    std::unique_ptr<TextButton> stateDownTextButton;
    std::unique_ptr<ComboBox> statesComboBox;
    std::unique_ptr<Label> labelR;
    std::unique_ptr<Slider> sliderR;
    std::unique_ptr<Label> labelG;
    std::unique_ptr<Slider> sliderG;
    std::unique_ptr<Label> labelB;
    std::unique_ptr<Slider> sliderB;
    std::unique_ptr<Label> colourVisualisationLabel;
    std::unique_ptr<TextButton> sendStateTextButton;
    std::unique_ptr<TextButton> sendZerosTextButton;
    std::unique_ptr<Label> matrixInfoLabel1;
    std::unique_ptr<Label> matrixInfoLabel2;
    std::unique_ptr<Label> matrixInfoLabel3;
    std::unique_ptr<Label> oscAddressLabel;
    std::unique_ptr<Label> minLabel;
    std::unique_ptr<Label> maxLabel;
    std::unique_ptr<Label> interpolationCurveLabel;
    std::unique_ptr<Label> valueLabel;
    std::unique_ptr<Label> linksInfoLabel;
    std::unique_ptr<Label> defaultLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatesEditionComponent)
};

//[EndFile] You can add extra defines here...
} // namespace Miam
//[/EndFile]

