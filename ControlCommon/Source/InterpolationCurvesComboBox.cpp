/*
  ==============================================================================

    InterpolationCurvesComboBox.cpp
    Created: 9 Mar 2019 4:44:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InterpolationCurvesComboBox.h"

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
}
