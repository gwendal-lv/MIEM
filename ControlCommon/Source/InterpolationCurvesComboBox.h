/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <vector>
#include <memory>

#include "JuceHeader.h"

#include "InterpolationTypes.h"

#include "LabelledMatrixComponent.h" // for direct callbacks

namespace Miam
{
    class InterpolationCurvesComboBox : public ComboBox,
                                        public ComboBox::Listener // auto-listens and re-sends events
    {
        /// Internal singleton class for managing images
        class ImagesManager : public DeletedAtShutdown {
            private :
            static ImagesManager* mainInstance;
            int height;
            std::vector<std::unique_ptr<Image>> curveImages;
            ImagesManager(int _imagesHeight);
            public :
            static Image& GetCurveImage(ParamInterpolationType interpolationType, int _height);
        };
        
        
        // =========== ATTRIBUTES ==========
        private :
        LabelledMatrixComponent* parentComponent;
        int selfRow;
        std::shared_ptr<ImageComponent> associatedImageComponent;
        const int height;
        
        /// \brief On doit sauvegarder le dernier type choisi, car on va
        /// changer le texte directement après avoir choisi (sinon la matrice
        /// devient illisible). Après changement, la combo box native Juce ne renverra plus
        /// un choix défini .
        ParamInterpolationType lastActualChoice = ParamInterpolationType::None;
        
        
        
        // =========== Getters and Setters ==========
        public :
        ParamInterpolationType GetSelectedInterpolationType() const {return lastActualChoice;}
        void SetSelectedInterpolationType(ParamInterpolationType);

        
        // =========== METHODS ==========
        // ction and dtion
        public :
        InterpolationCurvesComboBox (const String &componentName, LabelledMatrixComponent* _parentComponent, int _selfRow, std::shared_ptr<ImageComponent> _imageComponent, int _height);
        virtual ~InterpolationCurvesComboBox() {}
        
        /// \brief Self-listener callback, for translation of data, and direct explicit
        /// callback to the parent Labelled Matrix Component
        virtual void comboBoxChanged (ComboBox *comboBoxThatHasChanged) override;
        
        // Internal helpers
        private :
		void setShortNameAndUpdateImage();
        void updateImage();
    };
}


