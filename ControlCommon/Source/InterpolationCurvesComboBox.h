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
        void updateImage();
    };
}


