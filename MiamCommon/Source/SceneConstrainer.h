/*
  ==============================================================================

    SceneConstrainer.h
    Created: 13 Mar 2019 10:11:57pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>
#include <map>

#include "JuceHeader.h"

#include "IDrawableArea.h" // bpt typedefs

#include "AreasGroup.h"


namespace Miam
{
    class Exciter;
    class AreasGroup;
    
    
    /// \brief Class which was initially supposed to be used only for scientific
    /// experiments of the MIEM touch interface, but it is finally used to implement
    /// "touch-safety" features (exciters that cannot go the black playground by
    /// accident, etc...)
    class SceneConstrainer
    {
        public :
        enum class ConstraintType
        {
            None = 0,
            
            // Expérience à 4 paramètres
            Params4_Fader1,
            Params4_Fader2,
            Params4_Fader3,
            Params4_Fader4,
            Params4_CentralInterpRectangle,
            
            // MIEM Play, MIEM Spat : exciters contraints dans des zones continues d'aires
            Bypass, ///< No constraint will be actually applied
            RemainInsideAreasGroups, ///< Once an exciter enters a group, it cannot leave it
        };
        
        private :
        class ConstraintParams {
            public :
            ConstraintType Type;
            bpt InitialTouchOffset; ///< Offset from the center of the exciter that was touched
			const Point<float> InitialTouchOffset_float; ///< idem, casted as juce float point
            Point<float> LastValidCenterPosition;
            int AreasGroupIndex = -1;
            // = = = Constructeur par défaut = = =
            ConstraintParams()
            : Type(ConstraintType::None)
            {}
            ConstraintParams(bpt touchInitialPosition, bpt exciterInitialCenter, int _areasGroupIndex)
            : Type(ConstraintType::None),
				// - init of const attributes -
				InitialTouchOffset(touchInitialPosition.get<0>() - exciterInitialCenter.get<0>(),
					touchInitialPosition.get<1>() - exciterInitialCenter.get<1>()),
				InitialTouchOffset_float((float)InitialTouchOffset.get<0>(),
					(float)InitialTouchOffset.get<1>()),
				// - other attributes -
            LastValidCenterPosition((float)exciterInitialCenter.get<0>(),
				(float)exciterInitialCenter.get<1>()),
            AreasGroupIndex(_areasGroupIndex)
            {
            }
			// = = = Constructeur de copie = = =
			// doit être déclaré explicitement sous VS2017... à cause des attributs const ?
			ConstraintParams(const ConstraintParams&) = default;
        };
        
        
        
        // ====================== ATTRIBUTES =====================
        
        private :
        /// \brief To store constrained touch moves (for __MIEM_EXPERIMENTS)
        std::unique_ptr<MouseEvent> constrainedMouseEvent;
        
        /// \brief for __MIEM_EXPERIMENTS only.
        /// Permet de relier une touch source à un numéro de "sous-zone de l'aire de jeu"
        /// à laquelle les mouvements seront restreints. Permet de contraindre
        /// facilement les déplacements à certaines zones pour les expériences MIEM !
        std::map<int, ConstraintParams> touchSourceToExperimentConstraint;
        
        
        
        
        // ====================== Setters and Getters =====================
        public :
        // - - - to be properly implemented by the scene that inherits from this class - - -
        virtual SceneConstrainer::ConstraintType GetExcitersConstraint() = 0;
        virtual std::shared_ptr<AreasGroup>
        GetGroupFromPreComputedImage(int curX, int curY, int curW, int curH)= 0;
        
        virtual Point<float> GetClosestPointOfGroup(const Point<float>& pointOutsideGroup, int areasGroupIndexInScene) = 0;
        
        
        // ====================== METHODS =====================
        protected :
        virtual ~SceneConstrainer() {}
        
        /// \brief Function that behaves as a bypass in "normal" mode, but actually
        /// constrains the movements of the exciters when compiled when
        /// __MIEM_EXPERIMENTS is defined
        const MouseEvent& constrainMouseEvent(const MouseEvent& e, int canvasWidth, int canvasHeight);
        
        void beginTouchConstraint(const MouseEvent& e, int canvasWidth, int canvasHeight,
                                  std::shared_ptr<Exciter>& exciter,
                                  std::string sceneName);
        void endTouchConstraint(const MouseEvent& e);
        
        void clearConstraints()
        { touchSourceToExperimentConstraint.clear(); }
        
    };
    
}
