/*
  ==============================================================================

    ExperimentsSceneConstrainer.h
    Created: 13 Mar 2019 10:11:57pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>
#include <map>

#include "JuceHeader.h"

#include "IDrawableArea.h" // bpt typedefs

namespace Miam
{
    
    class ExperimentsSceneConstrainer
    {
        enum class ConstraintType
        {
            None = 0,
            
            // Expérience à 4 paramètres
            Params4_Fader1,
            Params4_Fader2,
            Params4_Fader3,
            Params4_Fader4,
            Params4_CentralInterpRectangle,
        };
        
        
        class ConstraintParams {
            public :
            ConstraintType Type;
            bpt InitialTouchOffset; ///< Offset from the center of the exciter that was touched
            
            ConstraintParams()
            : Type(ConstraintType::None)
            {}
            ConstraintParams(bpt touchInitialPosition, bpt exciterInitialCenter)
            : Type(ConstraintType::None)
            {
                InitialTouchOffset.set<0>(touchInitialPosition.get<0>() - exciterInitialCenter.get<0>());
                InitialTouchOffset.set<1>(touchInitialPosition.get<1>() - exciterInitialCenter.get<1>());
            }
            ConstraintParams(const ConstraintParams&) = default;
        };
        typedef struct ConstraintParams ConstraintParams;
        
        
        
        // ====================== ATTRIBUTES =====================
        
        private :
        /// \brief To store constrained touch moves (for __MIEM_EXPERIMENTS)
        std::unique_ptr<MouseEvent> constrainedMouseEvent;
        
        /// \brief for __MIEM_EXPERIMENTS only.
        /// Permet de relier une touch source à un numéro de "sous-zone de l'aire de jeu"
        /// à laquelle les mouvements seront restreints. Permet de contraindre
        /// facilement les déplacements à certaines zones pour les expériences MIEM !
        std::map<int, ConstraintParams> touchSourceToExperimentConstraint;
        
        
        
        
        // ====================== METHODS =====================
        protected :
        
        /// \brief Function that behaves as a bypass in "normal" mode, but actually
        /// constrains the movements of the exciters when compiled when
        /// __MIEM_EXPERIMENTS is defined
        MouseEvent& constrainMouseEvent(const MouseEvent& e, int canvasWidth, int canvasHeight);
        
        void beginTouchConstraint(const MouseEvent& e, int canvasWidth, int canvasHeight,
                                  bpt exciterCenter,
                                  std::string sceneName);
        void endTouchConstraint(const MouseEvent& e);
        
        void clearConstraints()
        { touchSourceToExperimentConstraint.clear(); }
        
    };
    
}
