/*
  ==============================================================================

    SceneConstrainer.cpp
    Created: 13 Mar 2019 10:11:57pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>

#include "boost/lexical_cast.hpp"

#include "SceneConstrainer.h"


#include "MiamMath.h"

// The full Exciter class is now required (for groups, etc...)
#include "Exciter.h"


using namespace Miam;



void SceneConstrainer::beginTouchConstraint(const MouseEvent& e,
                                                       int canvasWidth, int canvasHeight,
                                                       std::shared_ptr<Exciter>& exciter,
                                                       std::string sceneName)
{
    bpt mouseEventBpt = bpt(e.position.getX(), e.position.getY());
    
    // BUG, POURQUOI ICI ON RECOIT BACKGROUND PENDANT LE CALCUL ???
    // BUG, POURQUOI ICI ON RECOIT BACKGROUND PENDANT LE CALCUL ???
    // BUG, POURQUOI ICI ON RECOIT BACKGROUND PENDANT LE CALCUL ???
    // BUG, POURQUOI ICI ON RECOIT BACKGROUND PENDANT LE CALCUL ???
    // BUG, POURQUOI ICI ON RECOIT BACKGROUND PENDANT LE CALCUL ???
    // BUG, POURQUOI ICI ON RECOIT BACKGROUND PENDANT LE CALCUL ???
    // BUG, POURQUOI ICI ON RECOIT BACKGROUND PENDANT LE CALCUL ???
    ConstraintParams newConstraint(mouseEventBpt,
                                   exciter->GetCenterInPixels(),
                                   exciter->FindAreasGroupIndex());
    
#ifndef __MIEM_EXPERIMENTS // - - - - - MIEM Constraints (Play and Spat) - - - - -
    boost::ignore_unused(sceneName);
	boost::ignore_unused(canvasWidth);
	boost::ignore_unused(canvasHeight);
	
    newConstraint.Type = GetExcitersConstraint();
    
    //std::cout << "[SceneConstrainer] Begin. Type = " << (int) newConstraint.Type << ((newConstraint.Type == ConstraintType::RemainInsideAreasGroups) ? (std::string(" group = ") + boost::lexical_cast<std::string>(newConstraint.AreasGroupIndex) ) : "" ) << std::endl;
    
    touchSourceToExperimentConstraint[e.source.getIndex()] = newConstraint;
    
    return;

    
    
    
#else // - - - - - MIEM Experiments - - - - -
    int sceneId = -1;
    try {
        sceneId = boost::lexical_cast<int>(sceneName);
    }
    catch (boost::bad_lexical_cast& ) {
        assert(false); // scene names must be integers !
    }
    
    // Expérience à 4 paramètres
#ifdef __MIEM_EXPERIMENTS_4_PARAMETERS
    // Scène à base de faders : 1, 5, 9, 13, 17, 21, 25
    if ((sceneId % 2) == 1)
    {
        // On divise bêtement l'écran en 4 selon X
        // Ça ne posera pas de problème tant que l'écran est assez large, et que les
        // excitateurs de chaque fader ne sont pas superposés
        if ( ((int)mouseEventBpt.get<0>()) < ((canvasWidth * 1) / 4) )
            newConstraint.Type = ConstraintType::Params4_Fader1;
        else if ( ((int)mouseEventBpt.get<0>()) < ((canvasWidth * 2) / 4) )
            newConstraint.Type = ConstraintType::Params4_Fader2;
        else if ( ((int)mouseEventBpt.get<0>()) < ((canvasWidth * 3) / 4) )
            newConstraint.Type = ConstraintType::Params4_Fader3;
        else //if ( ((int)mouseEventBpt.get<0>()) < ((canvasWidth * 4) / 4) )
            newConstraint.Type = ConstraintType::Params4_Fader4;
    }
    else // interpolation
    {
        // validé par défaut : on suppose bien que l'évènement se passe dans le carré central !
        newConstraint.Type = ConstraintType::Params4_CentralInterpRectangle;
    }
#endif
    
    // Si on a trouvé une contrainte, alors
    if (newConstraint.Type != ConstraintType::None)
    {
        // insertion par tentative d'accès (temps de recherche totalement négligeable...)
        touchSourceToExperimentConstraint[e.source.getIndex()] = newConstraint;
        std::cout << "     ****>>>> Nouvelle contrainte, #" << (int)newConstraint.Type << std::endl;
    }
    else
    {
        std::cout << "     ****>>>> [Warning] [EXPERIMENTS] Unconstrained touch drag begins." << std::endl;
        assert(false); // all events must be constrained...
    }
    
#endif // __MIEM_EXPERIMENTS
}


void SceneConstrainer::endTouchConstraint(const MouseEvent& e)
{
    int touchIndex = e.source.getIndex();
    auto mapIt = touchSourceToExperimentConstraint.find(touchIndex);
    // If not found in map : we don't do anything (constraint did not start)
    if (mapIt != touchSourceToExperimentConstraint.end())
        touchSourceToExperimentConstraint.erase(mapIt);
}






const MouseEvent& SceneConstrainer::constrainMouseEvent(const MouseEvent& e,
                                                             int canvasWidth, int canvasHeight)
{
    // [[[ Generic start code ]]]
    
    Point<float> constrainedPosition = e.position;
    
    int touchIndex = e.source.getIndex();
    auto mapIt = touchSourceToExperimentConstraint.find(touchIndex);
    // If the touch is related to an area being moved
    if ((mapIt != touchSourceToExperimentConstraint.end()) && true)
    {
    
    
    
        
         // - - - - - MIEM Constraints (Play and Spat) - - - - -
        
        // If NOT in experiment mode : we apply the constraint rule linked to the touch event
#ifndef __MIEM_EXPERIMENTS
        
        auto& constraint = mapIt->second;
        
        if (constraint.Type == ConstraintType::RemainInsideAreasGroups)
        {
            // application de l'offset (pour se retrouver comme au centre de l'excitateur)
            constrainedPosition -= Point<float>(constraint.InitialTouchOffset.get<0>(),
                                                constraint.InitialTouchOffset.get<1>());
            
            // Vérification que le nouveau point est bien dans la même zone
            // -> recherche optimisée via l'image enregistrée.... (remise à l'échelle)
            // pour éviter un 2ème calcul de collision avec toutes les zones....
            auto curPosAreaGroup =
            GetGroupFromPreComputedImage((int) std::round(constrainedPosition.getX()),
                                         (int) std::round(constrainedPosition.getY()),
                                         canvasWidth, canvasHeight);
            const int curGroupIdx = curPosAreaGroup->GetIndexInScene();
            
            // Est-ce qu'on doit bloquer l'excitateur ?
            // Si idx différent ou si blocking,  OUI....
            if ( (curGroupIdx != constraint.AreasGroupIndex)
                || (curGroupIdx == (int)AreasGroup::SpecialIds::Blocking)
                || (curGroupIdx == (int)AreasGroup::SpecialIds::BlockUntilComputationFinished) )
            {
                // .... OUI, SAUF SI   on quitte le "blocking until"
                // Si on quite "block until", la nouvelle valeur, si elle est positive ou BACK,
                // devient le groupe auquel va s'attacher l'excitateur. Si c'est le groupe back,
                // l'excitateur pourra en sortir + tard
                if ( (constraint.AreasGroupIndex
                      == (int)AreasGroup::SpecialIds::BlockUntilComputationFinished)
                    && ( (curGroupIdx >= (int)AreasGroup::SpecialIds::FirstActualGroup)
                        || (curGroupIdx == (int)AreasGroup::SpecialIds::Background)) )
                {
                    constraint.AreasGroupIndex = curGroupIdx;
                }
                // OU SAUF Si on est dans le background : on a le droit de bouger vers une nouvelle zone.
                // Ensuite on ne pourra plus en sortir.
                else if ( (constraint.AreasGroupIndex == (int)AreasGroup::SpecialIds::Background)
                         && (curGroupIdx >= (int)AreasGroup::SpecialIds::FirstActualGroup) )
                {
                    constraint.AreasGroupIndex = curGroupIdx;
                }
                // Sinon OUI, on reste juste bloqué
                else
                    constrainedPosition = constraint.LastValidCenterPosition;
            }
            // Sinon, on garde cet évènement comme la dernier valide
            else
                constraint.LastValidCenterPosition = constrainedPosition;
            
            // suppression de l'offset (retour au point touch réel), pour finir
            constrainedPosition += Point<float>(constraint.InitialTouchOffset.get<0>(),
                                                constraint.InitialTouchOffset.get<1>());
        }
        else
        {} // no constraint : we do nothing at the moment... not even a check...
        
        
        
        
        
        // - - - - - MIEM Experiments - - - - -
        
#else // defined __MIEM_EXPERIMENTS
        auto& constraint = mapIt->second;
        
        // application de l'offset (pour se retrouver comme au centre de l'excitateur)
        constrainedPosition -= Point<float>(constraint.InitialTouchOffset.get<0>(),
                                            constraint.InitialTouchOffset.get<1>());

#ifdef __MIEM_EXPERIMENTS_4_PARAMETERS
        int minY, maxY, minX, maxX;
        // Central rectangle
        if (mapIt->second.Type == ConstraintType::Params4_CentralInterpRectangle)
        {
            minY = (int) std::round((0.02 + 0.04) * (double)canvasHeight);
            maxY = (int) std::round((0.98 - 0.04) * (double)canvasHeight);
            minX = (int) std::round((0.15 + 0.03) * (double)canvasWidth);
            maxX = (int) std::round((0.85 - 0.03) * (double)canvasWidth);
        }
        // Any of the 4 faders
        else if ( static_cast<int>(constraint.Type) >= static_cast<int>(ConstraintType::Params4_Fader1)
               && static_cast<int>(constraint.Type) <= static_cast<int>(ConstraintType::Params4_Fader4))
        {
            // Y range is the same for all faders
            minY = (int) std::round((0.04 + 0.02) * (double)canvasHeight);
            maxY = (int) std::round((0.96 - 0.02) * (double)canvasHeight);
            // but X depends
            switch(constraint.Type)
            {
                case ConstraintType::Params4_Fader1:
                    minX = (int) std::round( 0.14 * (double)canvasWidth );
                    break;
                case ConstraintType::Params4_Fader2:
                    minX = (int) std::round( 0.38 * (double)canvasWidth );
                    break;
                case ConstraintType::Params4_Fader3:
                    minX = (int) std::round( 0.62 * (double)canvasWidth );
                    break;
                case ConstraintType::Params4_Fader4:
                    minX = (int) std::round( 0.86 * (double)canvasWidth );
                    break;
                default :
                    assert(false); // all constraints should be managed in previous cases
                    break;
            }
            // fixed X for faders
            maxX = minX;
        }
        else
            // all possible contraints must be managed
            assert(false); // check beginConstraint for consistency with this function
        // Application of rectangular constraint (with float/int types mixing... but ok...)
        constrainedPosition = Point<float>(Math::Clamp<float>(constrainedPosition.getX(),
                                                              minX, maxX),
                                           Math::Clamp<float>(constrainedPosition.getY(),
                                                              minY, maxY));
#endif // end __MIEM_EXPERIMENTS_4_PARAMETERS
        
        // suppression de l'offset (retour au point réel), pour finir
        constrainedPosition += Point<float>(constraint.InitialTouchOffset.get<0>(),
                                            constraint.InitialTouchOffset.get<1>());
#endif // defined __MIEM_EXPERIMENTS
    
        
        
    
    
    // [[[ Generic end code ]]]
    }
    
    // then, we copy everything but the position
    constrainedMouseEvent = std::make_unique<MouseEvent>(e.source, constrainedPosition, e.mods,
                                                         e.pressure, e.orientation, e.rotation,
                                                         e.tiltX, e.tiltY,
                                                         e.eventComponent, e.originalComponent,
                                                         e.eventTime,
                                                         e.mouseDownPosition, e.mouseDownTime,
                                                         e.getNumberOfClicks(),
                                                         e.mouseWasDraggedSinceMouseDown());
    // De-referencement of internal pointer (to cast to a reference)
    return *(constrainedMouseEvent.get());
}

