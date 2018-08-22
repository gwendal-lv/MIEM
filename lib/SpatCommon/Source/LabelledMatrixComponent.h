/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --

// disabled for cross-project usage
//#include "../../MatrixRouter/JuceLibraryCode/JuceHeader.h"

//#include "MatrixComponent.h"

#include "ISlidersMatrixListener.h"

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
class LabelledMatrixComponent  : public Component
{
public:
    //==============================================================================
    LabelledMatrixComponent (ISlidersMatrixListener* _listener, unsigned int _maxRowsCount, unsigned int _maxColsCount);
    ~LabelledMatrixComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    public :

    void ReconstructGuiObjects();

    private :
    void initAndAddLabel(Label* label);
    void repositionLabels();
    void highlightLabel(Label* label);
    void unhighlightLabel(Label* label);
    
    void createAndManagePopupMenu();
    void setMatrixToZero();
    void setMatrixToIdentity();

    public :

    /// \brief Callback from the MatrixViewport
    void OnViewportVisibleAreaChanged();
    /// \brief Callback from the MatrixComponent
    void OnSliderValueChanged(int row, int col, double value)
    {listener->OnSliderValueChanged(row, col, value);}

    MatrixComponent* GetMatrixComponent();
    
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void mouseMove (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ISlidersMatrixListener* listener;
        unsigned int maxRowsCount, maxColsCount;

    // Graphical parameters from Miam::MatrixComponent
    const int matItemW = 40;
    const int matItemH = 20;

    /// \brief Labels precising input and outputs within the matrix
    ///
    /// Labels [0] to [n-1] are input labels,
    /// and labels [n] to [n+m-1] are output labels.
    std::vector<ScopedPointer<Label>> labels;
    Label* highlightedInputLabel = 0;
    Label* highlightedOutputLabel = 0;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Miam::MatrixViewport> matrixViewport;
    ScopedPointer<Label> inputsOutputsLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabelledMatrixComponent)
};

//[EndFile] You can add extra defines here...
} // namespace Miam
//[/EndFile]
