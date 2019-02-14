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

//[Headers] You can add your own extra header files here...

// not in the header, for cross-project usage
#include "JuceHeader.h"

#include <algorithm>

#include "MiamMath.h"
#include "TextUtils.h"
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

    showInputsNames = true;
    showOutputsNames = true;
    showInputsNumbers = true;
    showOutputsNumbers = true;
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

    if (showInputsNames)
        removedFromLeftOfMatrix = std::min(getWidth()/2 - 120, 320);
    else
        removedFromLeftOfMatrix = 0; // space is already substracted for input numbers
    if (GetDisplayPurpose() == AppPurpose::GenericController)
        removedFromLeftOfMatrix += actionButtonW; // for the action button
    matrixViewportBounds.removeFromLeft(removedFromLeftOfMatrix);
    removedFromLeftOfMatrix += 80; // because the previous projucer resize (takes the i/o ID into account)

    if (showOutputsNames)
        removedFromBottomOfMatrix = std::min(getHeight()/2 - 40, 360);
    else
        removedFromBottomOfMatrix = 0; // space is already substracted for output numbers
    matrixViewportBounds.removeFromBottom(removedFromBottomOfMatrix);
    removedFromBottomOfMatrix += 40; // because the previous projucer resize (takes the i/o ID into account)

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

    // Actual creation of sliders, labels and buttons
    for (int i=0 ; i<(int)maxRowsCount ; i++)
    {
        // Label on each row
        labels[i] = new Label("Input label " + boost::lexical_cast<std::string>(i), "" + boost::lexical_cast<std::string>(i+1));
        addAndMakeVisible(labels[i]);
        inputNameTextEditors[i] = new TextEditor("Input Name text editor " + boost::lexical_cast<std::string>(i));
        inputNameTextEditors[i]->setComponentID("i" + boost::lexical_cast<std::string>(i));
        addAndMakeVisible(inputNameTextEditors[i]);
        inputNameTextEditors[i]->addListener(this);
        // And the row's button
        rowTextButtons[i] = new TextButton("[action " + boost::lexical_cast<std::string>(i) + "]",
                                           "Not initialized yet");
        addAndMakeVisible(rowTextButtons[i]);
        rowTextButtons[i]->setComponentID("bi" + boost::lexical_cast<std::string>(i));
        rowTextButtons[i]->addListener(this);
    }
    for (int j=0 ; j<(int)maxColsCount ; j++)
    {
        // Column labels
        labels[maxRowsCount+j] = new Label("Output label " + boost::lexical_cast<std::string>(j),
                                           "" + boost::lexical_cast<std::string>(j+1));
        addAndMakeVisible(labels[maxRowsCount+j]);
        outputNameTextEditors[j] = new TextEditor("Output Name text editor " + boost::lexical_cast<std::string>(j));
        outputNameTextEditors[j]->setComponentID("o" + boost::lexical_cast<std::string>(j));
        addAndMakeVisible(outputNameTextEditors[j]);
        outputNameTextEditors[j]->addListener(this);
    }

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
        if (GetDisplayPurpose() == AppPurpose::GenericController)
        {
            rowTextButtons[i]->setButtonText(TRANS("Send"));
            rowTextButtons[i]->setTooltip(TRANS("Click to send this parameter to the configured OSC device."));
			rowTextButtons[i]->setVisible(true);
        }
		else
			rowTextButtons[i]->setVisible(false);
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
void LabelledMatrixComponent::initNameTextEditor(TextEditor* textEditor, bool isVertical)
{
    if (isVertical) // police avec + d'espacement
        textEditor->applyFontToAllText(Font().withExtraKerningFactor(0.10f));
    else
        textEditor->applyFontToAllText(Font().withExtraKerningFactor(0.05f));
    // Code spécifique selon OSC valide ou non
    try {
        TextUtils::ParseStringToJuceOscMessage(textEditor->getText().toStdString());

        Font textEditorFont = textEditor->getFont(); // copie
        textEditorFont.setBold(true); // si pas d'exception, on met en gras
        textEditor->applyFontToAllText(textEditorFont, false); // ne devient pas la font actuelle
        textEditor->applyColourToAllText(Colours::palegreen);
    }
    catch (ParseException&) {
        textEditor->applyFontToAllText(textEditor->getFont()); // sinon on laisse normal
        textEditor->applyColourToAllText(Colours::white);
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

    // Labels dynamic positionning
    for (int i=0 ; i<(int)maxRowsCount ; i++)
    {
        // Label on each row
        labels[i]->setBounds(0, i*matItemH - matrixDeltaY, inLabelsW, matItemH);
        // Action button are drawn for the GenericController app purpose
        if (GetDisplayPurpose() == AppPurpose::GenericController)
        {
            rowTextButtons[i]->setVisible(true);
            rowTextButtons[i]->setBounds(matrixViewport->getX() - actionButtonW + 4,
                                         i*matItemH - matrixDeltaY,
                                         actionButtonW - 4,
                                         matItemH);
        }
        else
            rowTextButtons[i]->setVisible(false);
        // On dessine les text editors d'entrée, si besoin
        if (showInputsNames)
        {
            inputNameTextEditors[i]->setVisible(true);
            if (rowTextButtons[i]->isVisible())
                inputNameTextEditors[i]->setBounds(inLabelsW, i*matItemH - matrixDeltaY,
                                                   matrixViewport->getX() - inLabelsW - actionButtonW,
                                                   matItemH);
            else
                inputNameTextEditors[i]->setBounds(inLabelsW, i*matItemH - matrixDeltaY,
                                                   matrixViewport->getX() - inLabelsW,
                                                   matItemH);
        }
        else
            inputNameTextEditors[i]->setVisible(false);
        // On désactive les composants correspondant à des entrées désactivées
        if (i >= getN())
        {
            inputNameTextEditors[i]->setEnabled(false);
            labels[i]->setEnabled(false);
            rowTextButtons[i]->setEnabled(false);
        }
        else
        {
            inputNameTextEditors[i]->setEnabled(true);
            labels[i]->setEnabled(true);
            rowTextButtons[i]->setEnabled(true);
        }
        // On cache les éléments d'entrée plus bas que le viewport
        if (labels[i]->getBottom() > (matrixViewport->getBottom() + 8))
        {
            labels[i]->setVisible(false);
            inputNameTextEditors[i]->setVisible(false);
            rowTextButtons[i]->setVisible(false);
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
void LabelledMatrixComponent::textEditorTextChanged (TextEditor & textEditor)
{
    if (textEditor.getComponentID().startsWithChar('i')) // input text editor
        initNameTextEditor(&textEditor, false);
    else if (textEditor.getComponentID().startsWithChar('i')) // input text editor
        initNameTextEditor(&textEditor, true);
    else
        throw std::runtime_error("Cannot parse the ID of the calling text editor object");
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
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

