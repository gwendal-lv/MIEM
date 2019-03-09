/*
  ==============================================================================

    InterpolationCurvesComboBox.h
    Created: 9 Mar 2019 3:03:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <vector>
#include <memory>

#include "JuceHeader.h"

#include "InterpolationTypes.h"

#include "LabelledMatrixComponent.h" // for direct callbacks

namespace Miam
{
    class InterpolationCurvesComboBox : public ComboBox {
        
        
        // =========== ATTRIBUTES ==========
        private :
        LabelledMatrixComponent* parentComponent;
        std::shared_ptr<ImageComponent> associatedImageComponent;
        const int height;
        
        // Internal "singleton"
        static std::vector<std::unique_ptr<ImageComponent>> curveImages;
        
        // =========== METHODS ==========
        // ction and dtion
        public :
        InterpolationCurvesComboBox (const String &componentName, LabelledMatrixComponent* _parentComponent, std::shared_ptr<ImageComponent> _imageComponent, int _height);
        virtual ~InterpolationCurvesComboBox() {}
        
        
    };
}


