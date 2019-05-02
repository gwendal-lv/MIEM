/*
  ==============================================================================

    InterpolationCurvesComboBox.cpp
    Created: 9 Mar 2019 4:44:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InterpolationCurvesComboBox.h"

#include "BasicInterpolationCurve.hpp"

using namespace Miam;

std::vector<std::unique_ptr<ImageComponent>> InterpolationCurvesComboBox::curveImages;

InterpolationCurvesComboBox::InterpolationCurvesComboBox (const String &componentName,
                                                          LabelledMatrixComponent* _parentComponent,
                                                          std::shared_ptr<ImageComponent> _imageComponent,
                                                          int _height)
:
ComboBox(componentName),
parentComponent(_parentComponent),
associatedImageComponent(_imageComponent),
height(_height)
{
    // COnstruction des images, si nécessaire
    if (curveImages.size() == 0)
    {
        std::cout << "images à construire" << std::endl;
    }
    
    // pptés générales
    setEditableText(false);
    setTextWhenNoChoicesAvailable(TRANS("Custom interpolation not available"));
    setScrollWheelEnabled(false); // pour ne pas changer sans faire exprès....
    
    // ajout des items disponibles avec ID personnalisé
    auto availableTypes = BasicInterpolationCurve<double>::GetAvailableParamInterpolationTypes();
    for (int i = 0; i < availableTypes.size() ; i++)
    {
        addItem(ParamInterpolationTypes::GetInterpolationName(availableTypes[i], true, false),
                (int)availableTypes[i]);
    }
    
    // auto-listener
    addListener(this);
}


void InterpolationCurvesComboBox::SetSelectedInterpolationType(ParamInterpolationType newType)
{
    if (ParamInterpolationTypes::IsActualInterpolationType(newType))
        setSelectedId((int) newType, NotificationType::sendNotification);
    else if (newType == ParamInterpolationType::None)
        setSelectedId(0, NotificationType::sendNotification);
}


void InterpolationCurvesComboBox::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged != this)
    {
        assert(false); // self-listener only...
        return;
    }
    
    // translation of data, internal save
    lastActualChoice = (ParamInterpolationType) getSelectedId();
    if (ParamInterpolationTypes::IsActualInterpolationType(lastActualChoice))
    {
    }
    else
    {
        lastActualChoice = ParamInterpolationType::None;
        assert(false); // should not happen !!
    }
    // and short name writing
    setText(ParamInterpolationTypes::GetInterpolationName(lastActualChoice, true, true),
            NotificationType::dontSendNotification);
    
}