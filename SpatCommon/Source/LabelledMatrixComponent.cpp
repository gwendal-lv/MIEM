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

//[Headers] You can add your own extra header files here...

// not in the header, for cross-project usage
#include "JuceHeader.h"

#include <algorithm>
//[/Headers]

#include "LabelledMatrixComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#include "MatrixViewport.h" // after self header include, includes MatrixComponent

using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
LabelledMatrixComponent::LabelledMatrixComponent (ISlidersMatrixListener* _listener, unsigned int _maxRowsCount, unsigned int _maxColsCount)
{
    //[Constructor_pre] You can add your own custom stuff here..
    listener = _listener;
    maxRowsCount = _maxRowsCount;
    maxColsCount = _maxColsCount;
    //[/Constructor_pre]

    addAndMakeVisible (matrixViewport = new Miam::MatrixViewport (this, maxRowsCount, maxColsCount));
    matrixViewport->setName ("Matrix Viewport");

    addAndMakeVisible (inputsOutputsLabel = new Label ("Inputs Outputs Label",
                                                       TRANS("inputs /\n"
                                                       "    outputs")));
    inputsOutputsLabel->setTooltip (TRANS("\n"));
    inputsOutputsLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    inputsOutputsLabel->setJustificationType (Justification::centredLeft);
    inputsOutputsLabel->setEditable (false, false, false);
    inputsOutputsLabel->setColour (Label::backgroundColourId, Colours::silver);
    inputsOutputsLabel->setColour (Label::textColourId, Colours::black);
    inputsOutputsLabel->setColour (TextEditor::textColourId, Colours::black);
    inputsOutputsLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    ReconstructGuiObjects();
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    addMouseListener(this, true);
    //[/Constructor]
}

LabelledMatrixComponent::~LabelledMatrixComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    matrixViewport = nullptr;
    inputsOutputsLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LabelledMatrixComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::silver);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LabelledMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    matrixViewport->setBounds (80, 0, getWidth() - 80, getHeight() - 40);
    inputsOutputsLabel->setBounds (0, getHeight() - 40, 80, 40);
    //[UserResized] Add your own custom resize handling here..

    repositionLabels();

    //[/UserResized]
}

void LabelledMatrixComponent::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...

    /// \brief Highlighting of the corresponding in/out labels when mouse is
    /// over a particular matrix coefficient

    // Hover dynamic behavior for mouse single input only
    if (e.source.getIndex() == 0)
    {
        Point<int> componentPos = e.getEventRelativeTo(this).getPosition();
        if (componentPos.getX() > inputsOutputsLabel->getRight()
            && componentPos.getY() < inputsOutputsLabel->getPosition().getY())
        {
            // Detection of concerned row and col (relative to the real complete matrix)
            Point<int> matrixPos = e.getEventRelativeTo(matrixViewport->GetMatrixComponent()).getPosition();
            int row = (int)std::floor((double)(matrixPos.getY())/(double)(matItemH));
            int col = (int)std::floor((double)(matrixPos.getX())/(double)(matItemW));

            // Detection of concerned in/out labels
            Label* newInputLabel = 0;
            if (0<=row && row < maxRowsCount)
                newInputLabel = labels[row];
            Label* newOutputLabel = 0;
            if (0 <= col && col<maxColsCount)
                newOutputLabel = labels[maxColsCount+col];

            // Graphical updates for input
            if (highlightedInputLabel != newInputLabel)
            {
                if (highlightedInputLabel)
                    unhighlightLabel(highlightedInputLabel);
                if (newInputLabel)
                    highlightLabel(newInputLabel);

                highlightedInputLabel = newInputLabel;
            }

            // Graphical updates for output
            if (highlightedOutputLabel != newOutputLabel)
            {
                if (highlightedOutputLabel)
                    unhighlightLabel(highlightedOutputLabel);
                if (newOutputLabel)
                    highlightLabel(newOutputLabel);

                highlightedOutputLabel = newOutputLabel;
            }
        }
    }
    //[/UserCode_mouseMove]
}

void LabelledMatrixComponent::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    
    // Pop-up menu appears on Right-click anywhere,
    // Or it may appear on a click on the inputs/outputs label (not editable)
    if (e.mods.isRightButtonDown()
        || inputsOutputsLabel->contains(e.getEventRelativeTo(inputsOutputsLabel).getPosition()))
        createAndManagePopupMenu();
    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void LabelledMatrixComponent::ReconstructGuiObjects()
{
    // Pre-allocations for vector of scoped pointers
    labels.clear();
    labels.resize(maxColsCount+maxRowsCount);

    // Actual creation of sliders and labels
    for (int i=0 ; i<maxRowsCount ; i++)
    {
        // Label on each row
        labels[i] = new Label("Input label " + std::to_string(i), "" + std::to_string(i+1));
        initAndAddLabel(labels[i]);
    }
    for (int j=0 ; j<maxColsCount ; j++)
    {
        // Column labels
        labels[maxRowsCount+j] = new Label("Output label " + std::to_string(j), "" + std::to_string(j+1));
        initAndAddLabel(labels[maxRowsCount+j]);
    }

    // Graphical placement
    resized();
}

void LabelledMatrixComponent::initAndAddLabel(Label* label)
{
    unhighlightLabel(label);
    label->setJustificationType(Justification::horizontallyCentred | Justification::verticallyCentred);
    addAndMakeVisible(label);
    label->toBack();
}
void LabelledMatrixComponent::repositionLabels()
{
    // Info retrieval (ui controls' data from the projucer)
    const int inLabelsW = inputsOutputsLabel->getWidth();
    const int outLabelsH = inputsOutputsLabel->getHeight();
    const int inOutLabelY = inputsOutputsLabel->getY();
    // data from children
    const int matrixDeltaX = matrixViewport->getViewPositionX();
    const int matrixDeltaY = matrixViewport->getViewPositionY();

    // Labels dynamic positionning
    for (int i=0 ; i<maxRowsCount ; i++)
    {
        // Label on each row
        labels[i]->setBounds(0, i*matItemH - matrixDeltaY, inLabelsW, matItemH);
    }
    for (int j=0 ; j<maxColsCount ; j++)
    {
        // Column labels
        labels[maxRowsCount+j]->setBounds(inLabelsW+j*matItemW - matrixDeltaX, inOutLabelY, matItemW, outLabelsH);
    }
}
void LabelledMatrixComponent::highlightLabel(Label* label)
{
    label->setColour(Label::textColourId, Colours::white);
    label->setColour(Label::backgroundColourId, Colours::black);
}
void LabelledMatrixComponent::unhighlightLabel(Label* label)
{
    label->setColour(Label::textColourId, Colours::black);
    label->setColour(Label::backgroundColourId, Colours::transparentWhite);
}
void LabelledMatrixComponent::createAndManagePopupMenu()
{
    PopupMenu menu;
    menu.addItem (1, "Reset to Zero matrix");
    menu.addItem (2, "Set to Identity matrix");
    const int userChoice = menu.show();
    if (userChoice == 0) // user dismissed the menu without picking anything
    {}
    else if (userChoice == 1)
        setMatrixToZero();
    else if (userChoice == 2)
        setMatrixToIdentity();
}
void LabelledMatrixComponent::setMatrixToZero()
{
    // Without notification
    GetMatrixComponent()->SetSpatMatrix(std::make_shared<SpatMatrix>());
    listener->OnMatrixZeroed(); // special unique notification to the listener
}
void LabelledMatrixComponent::setMatrixToIdentity()
{
    // At first : zeroing (supposed to be optimized)
    setMatrixToZero();
    
    // Then, actual setting to identity
    unsigned int smallestDimension = std::min(maxRowsCount, maxColsCount);
    for (int i=0 ; i<smallestDimension ; i++)
        // each one will notify (not optimized but that's OK)
        GetMatrixComponent()->SetSliderValue(i, i, 0.0, NotificationType::sendNotification);
}

void LabelledMatrixComponent::OnViewportVisibleAreaChanged()
{
    repositionLabels();
}

MatrixComponent* LabelledMatrixComponent::GetMatrixComponent()
{
    return matrixViewport->GetMatrixComponent();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LabelledMatrixComponent"
                 componentName="" parentClasses="public Component" constructorParams="ISlidersMatrixListener* _listener, unsigned int _maxRowsCount, unsigned int _maxColsCount"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffc0c0c0"/>
  <GENERICCOMPONENT name="Matrix Viewport" id="371018f99bc87dd2" memberName="matrixViewport"
                    virtualName="" explicitFocusOrder="0" pos="80 0 80M 40M" class="Miam::MatrixViewport"
                    params="this, maxRowsCount, maxColsCount"/>
  <LABEL name="Inputs Outputs Label" id="e417dac4d5a27562" memberName="inputsOutputsLabel"
         virtualName="" explicitFocusOrder="0" pos="0 40R 80 40" tooltip="&#10;"
         bkgCol="ffc0c0c0" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="inputs /&#10;    outputs" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
