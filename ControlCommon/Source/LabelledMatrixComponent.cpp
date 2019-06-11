/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...

// not in the header, for cross-project usage
#include "JuceHeader.h"

#include <algorithm>

#include "MiamMath.h"
#include "TextUtils.h"

#include "InterpolationCurvesComboBox.h"
//[/Headers]

#include "LabelledMatrixComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#include "MatrixViewport.h" // after self header include, includes MatrixComponent

#include "MinMaxSlidersPair.h"

using namespace Miam;
//[/MiscUserDefs]

//==============================================================================
LabelledMatrixComponent::LabelledMatrixComponent (ISlidersMatrixListener* _listener, unsigned int _maxRowsCount, unsigned int _maxColsCount)
{
    //[Constructor_pre] You can add your own custom stuff here..
    listener = _listener;
    maxRowsCount = _maxRowsCount;
    maxColsCount = _maxColsCount;

    showInputsNames = true;
    showOutputsNames = true;
    showInputsNumbers = true;
    showOutputsNumbers = true;


    jucePaleBlueColour = Colour(54,145,188);
    juceLightPaleBlueColour = Colour(87,165,201);

    //[/Constructor_pre]

    matrixViewport.reset (new Miam::MatrixViewport (this, maxRowsCount, maxColsCount));
    addAndMakeVisible (matrixViewport.get());
    matrixViewport->setName ("Matrix Viewport");

    inputsOutputsLabel.reset (new Label ("Inputs Outputs Label",
                                         TRANS("inputs /\n"
                                         "    outputs")));
    addAndMakeVisible (inputsOutputsLabel.get());
    inputsOutputsLabel->setTooltip (TRANS("\n"));
    inputsOutputsLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    inputsOutputsLabel->setJustificationType (Justification::centredLeft);
    inputsOutputsLabel->setEditable (false, false, false);
    inputsOutputsLabel->setColour (Label::backgroundColourId, Colours::silver);
    inputsOutputsLabel->setColour (Label::textColourId, Colours::black);
    inputsOutputsLabel->setColour (TextEditor::textColourId, Colours::black);
    inputsOutputsLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    constructGuiObjects();
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

    // On va re-replacer les objets placés juste au-dessus... selon qu'on ait des labels
    // des noms des sorties, ou non
    auto matrixViewportBounds = matrixViewport->getBounds();

    // matrix viewport CUT from LEFT (space for ALL elements)
    if (GetDisplayPurpose() == AppPurpose::Spatialisation)
    {
        if (showInputsNames)
            removedFromLeftOfMatrix = std::min((getWidth() - 0)/2,
                                               300); // = découpe max
        else
            removedFromLeftOfMatrix = 0; // space is already substracted for input numbers
    }
    else if (GetDisplayPurpose() == AppPurpose::GenericController)
    {
        removedFromLeftOfMatrix = std::min((getWidth() - 0)/2,
                                           760); // = découpe max
        removedFromLeftOfMatrix += curveComboBoxW/4;
        removedFromLeftOfMatrix += 1*minMaxValueSlidersW;
    }
    matrixViewportBounds.removeFromLeft(removedFromLeftOfMatrix);
    removedFromLeftOfMatrix += 80; // because of the previous projucer resize (takes the i/o ID into account)

    // matrix viewport CUT from BOTTOM
    if (showOutputsNames)
        removedFromBottomOfMatrix = std::min(getHeight()/2 - 40, 360);
    else
        removedFromBottomOfMatrix = 0; // space is already substracted for output numbers
    matrixViewportBounds.removeFromBottom(removedFromBottomOfMatrix);
    removedFromBottomOfMatrix += 40; // because the previous projucer resize (takes the i/o ID into account)

    // matrix viewport CUT from RIGHT
    removedFromRightOfMatrix = 0;
    if (GetDisplayPurpose() == AppPurpose::GenericController)
    {
        removedFromRightOfMatrix += actionButtonW;
    }
    matrixViewportBounds.removeFromRight(removedFromRightOfMatrix);

    matrixViewport->setBounds(matrixViewportBounds);

    repositionLabelsAndButtons();

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
            if (0<=row && row < (int)maxRowsCount)
                newInputLabel = labels[row];
            Label* newOutputLabel = 0;
            if (0 <= col && col < (int)maxColsCount)
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
        || inputsOutputsLabel->contains(e.getEventRelativeTo(inputsOutputsLabel.get()).getPosition()))
        createAndManagePopupMenu();
    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void LabelledMatrixComponent::constructGuiObjects()
{
    // Pre-allocations for vector of scoped pointers
    labels.clear();
    labels.resize(maxColsCount+maxRowsCount);
    inputNameTextEditors.clear();
    inputNameTextEditors.resize(maxRowsCount);
    outputNameTextEditors.clear();
    outputNameTextEditors.resize(maxColsCount);
    rowTextButtons.clear();
    rowTextButtons.resize(maxRowsCount);
    rowComboBoxes.clear();
    rowComboBoxes.resize(maxRowsCount);
    curveImageComponents.clear();
    curveImageComponents.resize(maxRowsCount);
    minMaxSlidersPairs.clear();
    minMaxSlidersPairs.resize(maxRowsCount);

    // Actual creation of sliders, labels and buttons
    for (int i=0 ; i<(int)maxRowsCount ; i++)
    {
        auto iStr = boost::lexical_cast<std::string>(i);
        // - - - Label on each row - - -
        labels[i] = new Label("Input label " + iStr, "" + boost::lexical_cast<std::string>(i+1));
        addAndMakeVisible(labels[i]);
        inputNameTextEditors[i] = new ChannelNameTextEditor("Input Name text editor " + boost::lexical_cast<std::string>(i));
        inputNameTextEditors[i]->setComponentID("i" + iStr);
        addAndMakeVisible(inputNameTextEditors[i]);
        inputNameTextEditors[i]->addListener(this);
        // - - - Row's button - - -
        rowTextButtons[i].reset(new TextButton("[action " + iStr + "]",
                                           "Not initialized yet"));
        addAndMakeVisible(rowTextButtons[i].get());
        rowTextButtons[i]->setComponentID("bi" + iStr);
        rowTextButtons[i]->addListener(this);
        // - - - Row's interpolation curves and images - - -
        curveImageComponents[i].reset(new ImageComponent("Curve image component " + iStr));
        addAndMakeVisible(curveImageComponents[i].get());
        rowComboBoxes[i].reset(new InterpolationCurvesComboBox("Input Interpolation Curve combo box" +
                                                               iStr, this, i, curveImageComponents[i],
                                                               matItemH));
        rowComboBoxes[i]->setComponentID("cbi" + iStr);
        addAndMakeVisible(rowComboBoxes[i].get());
        // - - - Min and Max Sliders - - -
        minMaxSlidersPairs[i].reset(new MinMaxSlidersPair(this, "Min/Max Slider Pair " + iStr, i));
        minMaxSlidersPairs[i]->setComponentID("spi" + iStr);
        addAndMakeVisible(minMaxSlidersPairs[i].get());
    }
    for (int j=0 ; j<(int)maxColsCount ; j++)
    {
        // Column labels
        labels[maxRowsCount+j] = new Label("Output label " + boost::lexical_cast<std::string>(j),
                                           "" + boost::lexical_cast<std::string>(j+1));
        addAndMakeVisible(labels[maxRowsCount+j]);
        outputNameTextEditors[j] = new ChannelNameTextEditor("Output Name text editor " + boost::lexical_cast<std::string>(j));
        outputNameTextEditors[j]->setComponentID("o" + boost::lexical_cast<std::string>(j));
        addAndMakeVisible(outputNameTextEditors[j]);
        outputNameTextEditors[j]->addListener(this);
    }

    // Init des valeurs des courbes d'interp (pour mettre des valeurs légales, sinon bugs ensuite)
    auto interpCurves = std::make_shared<BasicInterpCurves>();
    for (size_t i=0 ; i<maxRowsCount ; i++)
        interpCurves->push_back(BasicInterpolationCurve<double>::GetDefault());
    SetInterpolationCurves(interpCurves);

    // Does not actually reinit objects values....
    ReinitGuiObjects();
}
void LabelledMatrixComponent::ReinitGuiObjects()
{
    // Suppression des objets temporaires et des références
    highlightedInputLabel = nullptr;
    highlightedOutputLabel = nullptr;

    // Mise en visible des objets qui conviennent
    inputsOutputsLabel->setVisible(currentDisplayPurpose != AppPurpose::GenericController);

    // réinit de tous les labels et text editors associés
    for (int i=0 ; i<(int)maxRowsCount ; i++)
    {
        // Label on each row
        initLabel(labels[i]);
        initNameTextEditor(inputNameTextEditors[i], false); // horizontal
        // Text action button, interp curves and min/max sliders for gen con
        if (GetDisplayPurpose() == AppPurpose::GenericController)
        {
            rowTextButtons[i]->setButtonText(TRANS("Send"));
            rowTextButtons[i]->setTooltip(TRANS("Click to send this parameter to the configured OSC device."));
			rowTextButtons[i]->setVisible(true);
            rowComboBoxes[i]->setVisible(true);
            curveImageComponents[i]->setVisible(true);
            minMaxSlidersPairs[i]->setVisible(true);
        }
		else
        {
			rowTextButtons[i]->setVisible(false);
            rowComboBoxes[i]->setVisible(false);
            curveImageComponents[i]->setVisible(false);
            minMaxSlidersPairs[i]->setVisible(false);
        }
    }
    for (int j=0 ; j<(int)maxColsCount ; j++)
    {
        // On each col
        initLabel(labels[maxRowsCount+j]);
        initNameTextEditor(outputNameTextEditors[j], true); // vertical
    }

    // Graphical placement
    resized();
}

void LabelledMatrixComponent::initLabel(Label* label)
{
    label->setVisible(true);
    unhighlightLabel(label);
    label->setJustificationType(Justification::horizontallyCentred | Justification::verticallyCentred);
    label->toBack();
}
void LabelledMatrixComponent::initNameTextEditor(ChannelNameTextEditor* textEditor, bool isVertical)
{
    if (isVertical) // police avec + d'espacement
        textEditor->applyFontToAllText(Font().withExtraKerningFactor(0.10f));
    else
        textEditor->applyFontToAllText(Font().withExtraKerningFactor(0.05f));
    // Code spécifique selon OSC valide ou non
    try {
        TextUtils::ParseStringToJuceOscMessage(textEditor->getText().toStdString());
        textEditor->SetIsNameValid(true);
    }
    catch (ParseException&) {
        textEditor->SetIsNameValid(false);
    }
    // Dans tous les cas :
    textEditor->setMultiLine(false);
    textEditor->setJustification(Justification(Justification::Flags::horizontallyCentred
                                             | Justification::Flags::verticallyCentred));
}
void LabelledMatrixComponent::repositionLabelsAndButtons()
{
    // Info retrieval (ui controls' data from the projucer)
    const int inLabelsW = inputsOutputsLabel->getWidth();
    const int outLabelsH = inputsOutputsLabel->getHeight();
    const int inOutLabelY = inputsOutputsLabel->getY();
    // data from children
    const int matrixDeltaX = matrixViewport->getViewPositionX();
    const int matrixDeltaY = matrixViewport->getViewPositionY();
    viewportLX = matrixViewport->getX(); // left of matrix viewport
    viewportRX = matrixViewport->getRight(); // right of matrix viewport

    // Valeurs en X toujours calculées, même si composants pas visibles au final
    inputNamesX = inLabelsW;
    int inputNamesW = matrixViewport->getX() - inLabelsW - curveComboBoxW - curveImageW - 2*minMaxValueSlidersW - 4*5 - 2;
    rowButtonsX = viewportRX + 4;
    int curveImagesX = viewportLX - 4 - curveImageW;
    interpolationCurvesX = curveImagesX - 4 - curveComboBoxW;
    maximaX = interpolationCurvesX - 4 - minMaxValueSlidersW;
    minimaX = maximaX - 4 - minMaxValueSlidersW;

    // Labels dynamic positionning
    for (int i=0 ; i<(int)maxRowsCount ; i++)
    {
        // Label on each row
        labels[i]->setBounds(0, i*matItemH - matrixDeltaY, inLabelsW, matItemH);
        // On dessine les text editors d'entrée, si besoin
        if (showInputsNames)
        {
            inputNameTextEditors[i]->setVisible(true);
            inputNameTextEditors[i]->setBounds(inputNamesX, i*matItemH - matrixDeltaY,
                                               inputNamesW, matItemH);
        }
        else
            inputNameTextEditors[i]->setVisible(false);

        // On désactive les composants correspondant à des entrées désactivées
        inputNameTextEditors[i]->setEnabled( i < getN() );
        labels[i]->setEnabled( i < getN() );
        rowTextButtons[i]->setEnabled( i < getN() );
        minMaxSlidersPairs[i]->setEnabled( i < getN() );
        rowComboBoxes[i]->setEnabled( i < getN() );
        curveImageComponents[i]->setEnabled( i < getN() );

        // Comboboxes + Action buttons are drawn for the GenericController app purpose
        if (GetDisplayPurpose() == AppPurpose::GenericController)
        {
            // bouton à droite du slider
            rowTextButtons[i]->setVisible(true);
            rowTextButtons[i]->setBounds(rowButtonsX, i*matItemH - matrixDeltaY,
                                         actionButtonW - 4, matItemH);
            // Interp Curves et Min/Max à gauche
            rowComboBoxes[i]->setVisible(true);
            rowComboBoxes[i]->setBounds(interpolationCurvesX, i*matItemH - matrixDeltaY,
                                        curveComboBoxW, matItemH);
            curveImageComponents[i]->setVisible(true);
            curveImageComponents[i]->setBounds(curveImagesX, i*matItemH - matrixDeltaY,
                                        curveImageW, matItemH);
            minMaxSlidersPairs[i]->setVisible(true);
            minMaxSlidersPairs[i]->setBounds(minimaX, i*matItemH - matrixDeltaY,
                                             minMaxValueSlidersW*2 + 4, matItemH);
        }
        else
        {
            rowTextButtons[i]->setVisible(false);
            rowComboBoxes[i]->setVisible(false);
            curveImageComponents[i]->setVisible(false);
            minMaxSlidersPairs[i]->setVisible(false);
        }
        // On cache les éléments d'entrée plus bas que le viewport
        if (labels[i]->getBottom() > (matrixViewport->getBottom() + 8))
        {
            labels[i]->setVisible(false);
            inputNameTextEditors[i]->setVisible(false);
            rowTextButtons[i]->setVisible(false);
            rowComboBoxes[i]->setVisible(false);
            curveImageComponents[i]->setVisible(false);
            minMaxSlidersPairs[i]->setVisible(false);
        }
        else
        {
            labels[i]->setVisible(true);
            if (currentDisplayPurpose == AppPurpose::GenericController)
                rowTextButtons[i]->setVisible(true);
            if (showInputsNames)
                inputNameTextEditors[i]->setVisible(true);
        }
    }
    for (int j=0 ; j<(int)maxColsCount ; j++)
    {
        // Column labels : numbers
        labels[maxRowsCount+j]->setBounds(matrixViewport->getX() + j*matItemW - matrixDeltaX,
                                          inOutLabelY,
                                          matItemW, outLabelsH);
        // Les noms si besoin : juste en dessous du viewport
        if (showOutputsNames)
        {
            // transformation -90°. Effet : x' = -y et y = x
            outputNameTextEditors[j]->setTransform(AffineTransform::rotation((float)-M_PI_2, 0.0f, 0.0f));
            const int verticalTextEditorLength = getHeight() - outLabelsH - matrixViewport->getHeight();
            outputNameTextEditors[j]->setBounds(- matrixViewport->getBottom() - verticalTextEditorLength,
                                                matrixViewport->getX() + j*matItemW - matrixDeltaX + 6,
                                                verticalTextEditorLength,
                                                matItemW - 12);
        }
        else
            outputNameTextEditors[j]->setVisible(false);
        // On désactive les composants correspondant à des sorties désactivées
        if (j >= getM())
        {
            outputNameTextEditors[j]->setEnabled(false);
            labels[maxRowsCount + j]->setEnabled(false);
        }
        else
        {
            outputNameTextEditors[j]->setEnabled(true);
            labels[maxRowsCount + j]->setEnabled(true);
        }
        // Dans tous les cas : on vire les labels qui dépassent
        if (labels[maxRowsCount + j]->getX() < (matrixViewport->getX() - 12) )
        {
            outputNameTextEditors[j]->setVisible(false);
            labels[maxRowsCount + j]->setVisible(false);
        }
        else
        {
            labels[maxRowsCount + j]->setVisible(currentDisplayPurpose != AppPurpose::GenericController);
            if (showOutputsNames)
                outputNameTextEditors[j]->setVisible(true);
        }
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
#ifndef __MIAMOBILE
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
#else
    /* This function uses pop-ups and
     * must not be executed form a mobile platform.
     */
    assert(0);
#endif
}
void LabelledMatrixComponent::setMatrixToZero(bool notifyListener)
{
    // Without notification
    GetMatrixComponent()->SetSpatMatrix(std::make_shared<ControlMatrix<double>>());
	if (notifyListener)
		listener->OnMatrixZeroed(); // special unique notification to the listener
}
void LabelledMatrixComponent::setMatrixToIdentity()
{
    // At first : zeroing (supposed to be optimized)
    setMatrixToZero();

    // Then, actual setting to identity
    unsigned int smallestDimension = std::min(maxRowsCount, maxColsCount);
    for (int i=0 ; i<(int)smallestDimension ; i++)
        // each one will notify (not optimized but that's OK)
        GetMatrixComponent()->SetSliderValue_dB(i, i, 0.0, NotificationType::sendNotification);
}
size_t LabelledMatrixComponent::getN()
{
    return GetMatrixComponent()->GetActiveInputsCount();
}
size_t LabelledMatrixComponent::getM()
{
    return GetMatrixComponent()->GetActiveOutputsCount();
}

void LabelledMatrixComponent::OnViewportVisibleAreaChanged()
{
    repositionLabelsAndButtons();
}
void LabelledMatrixComponent::OnSliderValueChanged(int row, int col, double value)
{
    listener->OnSliderValueChanged(row, col, value);
}
void LabelledMatrixComponent::OnMinMaxValuesChanged(int row, double /*valueMin*/, double /*valueMax*/)
{
    matrixViewport->GetMatrixComponent()->SetHorizontalSliderInterpolationData(row,
        GetInterpolationCurve((size_t)row));
}
void LabelledMatrixComponent::OnInterpolationTypeChanged(int row, ParamInterpolationType /*newInterpolationType*/)
{
    matrixViewport->GetMatrixComponent()->SetHorizontalSliderInterpolationData(row,
        GetInterpolationCurve((size_t)row));
}
void LabelledMatrixComponent::textEditorTextChanged (TextEditor & textEditor)
{
    auto castedPtr = dynamic_cast<ChannelNameTextEditor*>(&textEditor);
    if (textEditor.getComponentID().startsWithChar('i')) // input text editor
    {
        assert(castedPtr != nullptr); // all text editors with ID starting with 'i' should be castable
        initNameTextEditor(castedPtr, false);
    }
    else if (textEditor.getComponentID().startsWithChar('o')) // input text editor
    {
        assert(castedPtr != nullptr); // all text editors with ID starting with 'o' should be castable
        initNameTextEditor(castedPtr, true);
    }
    else
        assert(false); // Cannot parse the ID of the calling text editor object
}
void LabelledMatrixComponent::buttonClicked (Button* _button)
{
    int integerID = -1;

    // We get its row index #, from its ID which must be 'bi#'
    auto buttonID = _button->getComponentID().toStdString();
    try {
        if (buttonID.length() < 3)
            throw BadIdException();
        if (buttonID[0] != 'b' || buttonID[1] != 'i')
            throw BadIdException();

        // At the moment, only 1 unique button on each row -> no valid col index will be given
        try {
            integerID = boost::lexical_cast<int>(buttonID.substr(2));
        } catch (boost::bad_lexical_cast&) {
            throw BadIdException();
        }
        if (integerID < 0 || (int)maxRowsCount < integerID)
            throw BadIdException();
    }
    catch (BadIdException& ) {
        assert(false); // a bad-referenced (bas ID) button has been clicked. This must not happen!
    }

    // Callback transmitted to the listener component
    buttonsListener->OnMatrixButtonClicked(integerID, -1,
                                           inputNameTextEditors[integerID]->getText().toStdString(),
                                           matrixViewport->GetMatrixComponent()->GetSliderValue(integerID, 0));
}


MatrixComponent* LabelledMatrixComponent::GetMatrixComponent()
{
    return matrixViewport->GetMatrixComponent();
}
void LabelledMatrixComponent::SetChannelsNames(InOutChannelsName &channelsName)
{
    for (size_t i=0 ; i<inputNameTextEditors.size() ; i++)
    {
        if (i < channelsName.Inputs.size())
            inputNameTextEditors[i]->setText(channelsName.Inputs[i]);
        else
            // On devrait toujours avoir suffisamment de données pour remplir tous les text editors !
            assert(false);
    }
    for (size_t j=0 ; j<outputNameTextEditors.size() ; j++)
    {
        if (j < channelsName.Outputs.size())
            outputNameTextEditors[j]->setText(channelsName.Outputs[j]);
        else
            // On devrait toujours avoir suffisamment de données pour remplir tous les text editors !
            assert(false);
    }
}
InOutChannelsName LabelledMatrixComponent::GetChannelsName()
{
    InOutChannelsName channelsName;

    channelsName.Inputs.reserve(maxRowsCount);
    for (unsigned int i=0 ; i<maxRowsCount ; i++)
        channelsName.Inputs.push_back(inputNameTextEditors[i]->getText().toStdString());
    channelsName.Outputs.reserve(maxColsCount);
    for (unsigned int j=0 ; j<maxColsCount ; j++)
        channelsName.Outputs.push_back(outputNameTextEditors[j]->getText().toStdString());

    return channelsName;
}
void LabelledMatrixComponent::SetInterpolationCurves(std::shared_ptr<BasicInterpCurves> interpCurvesPtr)
{
    BasicInterpCurves interpCurves = *(interpCurvesPtr.get());

    // Pas d'autre fonctionnement prévu pour l'instant...
    // Si condition n'est pas remplie (ce qui peut arriver au début), on ne fait juste rien
    if (interpCurves.size() != maxRowsCount)
        return;

    for (size_t i=0 ; i<interpCurves.size() ; i++)
    {
        // 1 - Direct application
        rowComboBoxes[i]->SetSelectedInterpolationType(interpCurves[i].GetInterpolationType());

        // to avoid conflicts (incoherent sliders on Juce side
        // we must check min/max values compared to the default values 0.0;1.0
        if (interpCurves[i].GetMaxY() <= 0.0 ) // if max is lower than default min
        {
            minMaxSlidersPairs[i]->SetMinValue(interpCurves[i].GetMinY());
            minMaxSlidersPairs[i]->SetMaxValue(interpCurves[i].GetMaxY());
        }
        // --> or normal order
        else
        {
            minMaxSlidersPairs[i]->SetMaxValue(interpCurves[i].GetMaxY());
            minMaxSlidersPairs[i]->SetMinValue(interpCurves[i].GetMinY());
        }
        // 2 - transmission of copies to the matrix
        GetMatrixComponent()->SetHorizontalSliderInterpolationData((int)i, interpCurves[i]);
    }
}
BasicInterpolationCurve<double> LabelledMatrixComponent::GetInterpolationCurve(size_t i)
{
    return
    BasicInterpolationCurve<double>(rowComboBoxes[i]->GetSelectedInterpolationType(),
                                    minMaxSlidersPairs[i]->GetMinValue(),
                                    minMaxSlidersPairs[i]->GetMaxValue()
                                    );
}
std::shared_ptr<BasicInterpCurves> LabelledMatrixComponent::GetInterpolationCurves()
{
    auto interpCurves = std::make_shared<BasicInterpCurves>();
    for (size_t i=0 ; i<maxRowsCount ; i++)
    {
        interpCurves->push_back(GetInterpolationCurve(i));
    }
    return interpCurves;
}
void LabelledMatrixComponent::SetInputNamesVisible(bool areVisible)
{
    if (showInputsNames != areVisible)
    {
        showInputsNames = areVisible;
        resized();
    }
}
void LabelledMatrixComponent::SetOutputNamesVisible(bool areVisible)
{
    if (showOutputsNames != areVisible)
    {
        showOutputsNames = areVisible;
        resized();
    }
}
void LabelledMatrixComponent::SetActiveSliders(int inputsCount, int outputsCount)
{
    // On sauvegarde les valeurs dans la matrice seulement....
    GetMatrixComponent()->SetActiveSliders(inputsCount, outputsCount);

    ReinitGuiObjects();
}
void LabelledMatrixComponent::SetDisplayPurpose(AppPurpose newSessionPurpose)
{
    // États internes indirects
    switch(newSessionPurpose)
    {
        case AppPurpose::GenericController :
            SetOutputNamesVisible(false); // actualisation si besoin
            break;

        default : break;
    }
    // États internes directs et actualisations
	// Seulement si on a vraiment changé de mode ! On ne re-force pas les valeurs
    if (currentDisplayPurpose != newSessionPurpose)
    {
        currentDisplayPurpose = newSessionPurpose;

		setMatrixToZero(false);

        ReinitGuiObjects();
        GetMatrixComponent()->RepositionGuiObjects();
        GetMatrixComponent()->resized(); // pour redonner la bonne taile au contenu du viewport
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LabelledMatrixComponent"
                 componentName="" parentClasses="public Component, public TextEditor::Listener, public Button::Listener"
                 constructorParams="ISlidersMatrixListener* _listener, unsigned int _maxRowsCount, unsigned int _maxColsCount"
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
         fontsize="1.5e1" kerning="0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

