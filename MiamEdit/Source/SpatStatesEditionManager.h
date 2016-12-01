/*
  ==============================================================================

    SpatStatesEditionManager.h
    Created: 26 Nov 2016 6:47:49pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSTATESEDITIONMANAGER_H_INCLUDED
#define SPATSTATESEDITIONMANAGER_H_INCLUDED

#include "SpatInterpolator.h"

// pre-declarations for components' pointers
class SpatStatesEditionComponent;

namespace Miam
{
    // pre-declarations for pointers
    class View;
    /*class SpatInterpolator<double>;*/ // from Model
    
    
    /// \brief
    class SpatStatesEditionManager
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // Links to other modules
        View* view;
        SpatStatesEditionComponent* editionComponent;
        SpatInterpolator<double>* spatInterpolator; // from Model
        
        // Selected spat state
        std::shared_ptr<SpatState<double>> selectedSpatState = nullptr;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        SpatType GetSpatType() {return spatInterpolator->GetSpatType();}
        void SelectSpatState(std::shared_ptr<SpatState<double>> _spatState);
        
        // 1 dimension speakers' volumes, faders edition
        double GetFadersCount();
        std::string GetFaderName(size_t _i) {return spatInterpolator->GetOutputName(_i);}
        
    
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        SpatStatesEditionManager(View* _view);
        void CompleteInitialisation(SpatInterpolator<double>* _spatInterpolator);
        
    };
    
}


#endif  // SPATSTATESEDITIONMANAGER_H_INCLUDED
