/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --

//#include "MatrixComponent.h"

#include "ISlidersMatrixListener.h"
#include "IMatrixButtonListener.h"
#include "AudioUtils.hpp"

#include <memory>

#include "AppPurpose.h"

namespace Miam
{

    // forward declarations
    class MatrixViewport;
    class MatrixComponent;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
     An auto-generated component, created by the Projucer.

     Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LabelledMatrixComponent  : public Component,
                                 public TextEditor::Listener,
                                 public Button::Listener
{
public:
    //==============================================================================
    LabelledMatrixComponent (ISlidersMatrixListener* _listener, unsigned int _maxRowsCount, unsigned int _maxColsCount);
    ~LabelledMatrixComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    private :
    void constructGuiObjects();
    public :
    void ReinitGuiObjects();

    private :
    void initLabel(Label* label);
    void initNameTextEditor(TextEditor* label, bool isVertical);
    void repositionLabelsAndButtons();
    void highlightLabel(Label* label);
    void unhighlightLabel(Label* label);

    void createAndManagePopupMenu();
    void setMatrixToZero(bool notifyListener = true);
    void setMatrixToIdentity();

    // Ces 2 fonctions vont chercher le nombre de lignes (ou colonnes)
    // vraiment actives. C'est une info dans le matrixcomponent enfant
    size_t getN();
    size_t getM();

    public :

    /// \brief Callback from the MatrixViewport
    void OnViewportVisibleAreaChanged();
    /// \brief Callback from the MatrixComponent.
    /// 'value' is a linear value (not given in decibels)
    void OnSliderValueChanged(int row, int col, double value);
    /// \brief Callback from any text editor. Display (in bold, or not) wether the input name
    /// can be parsed to a valid OSC message with address and arguments.
    ///
    /// Parse made by TextUtils::ParseStringToJuceOscMessage
    virtual void textEditorTextChanged (TextEditor &) override;
    /// \brief Callback from a generic action button (currently, the buttons
    /// displayed at the beginning of each line).
    virtual void buttonClicked (Button* ) override;

    // - - - - - - Getters and Setters - - - - -
    MatrixComponent* GetMatrixComponent();

    void SetChannelsNames(InOutChannelsName &channelsName);
    InOutChannelsName GetChannelsName();

    void SetInputNamesVisible(bool areVisible);
    void SetOutputNamesVisible(bool areVisible);
    void SetActiveSliders(int inputsCount, int outputsCount);

    void SetDisplayPurpose(AppPurpose newSessionPurpose);
    AppPurpose GetDisplayPurpose() {return currentDisplayPurpose;}

    void SetButtonsListener(IMatrixButtonListener* _listener) {buttonsListener = _listener;}

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void mouseMove (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AppPurpose currentDisplayPurpose;

    ISlidersMatrixListener* listener;
    IMatrixButtonListener* buttonsListener = nullptr;
    unsigned int maxRowsCount, maxColsCount;

    // Graphical parameters from Miam::MatrixComponent
    const int matItemW = 40;
    const int matItemH = 20;
    const int actionButtonW = 120;
    // Positionning variables
    int removedFromLeftOfMatrix = 100; // just in case, to avoid zero-sized buttons if it happens...
    int removedFromBottomOfMatrix = 100;

    /// \brief Labels precising input and outputs within the matrix
    ///
    /// Labels [0] to [n-1] are input labels,
    /// and labels [n] to [n+m-1] are output labels.
    std::vector<ScopedPointer<Label>> labels;
    Label* highlightedInputLabel = 0;
    Label* highlightedOutputLabel = 0;

    bool showInputsNames;
    bool showOutputsNames;
    bool showInputsNumbers;
    bool showOutputsNumbers;
    /// labels [0] to [m-1] for outputs' names
    // Si on voit les noms des sorties leur hauteur est this->getHeight()/2
    std::vector<ScopedPointer<TextEditor>> inputNameTextEditors; // component ID : 'i#', # is the row number
    std::vector<ScopedPointer<TextEditor>> outputNameTextEditors; // component ID : 'o#', # is the col number
    // These buttons will trigger a generic callback (the user will be able to
    // use it through a registered listener system). Shown in GenericController
    // mode only (at the moment ?)
    std::vector<ScopedPointer<TextButton>> rowTextButtons;  // component ID : 'bi#', # is the row number
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Miam::MatrixViewport> matrixViewport;
    std::unique_ptr<Label> inputsOutputsLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabelledMatrixComponent)
};

//[EndFile] You can add extra defines here...
} // namespace Miam
//[/EndFile]

