/*
  ==============================================================================

    SpatInterpolator.h
    Created: 26 Nov 2016 12:03:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATINTERPOLATOR_H_INCLUDED
#define SPATINTERPOLATOR_H_INCLUDED

#include <iostream>
#include <vector>
#include <memory>

#include "SpatType.h"

#include "SpatState.h"
#include "OutputVolumesState.h"



namespace Miam
{
    class IModel;
    
    
    /// \brief Manages some Miam::SpatState children, and does the
    /// interpolation between some of these states when asked to do so.
    ///
    template<typename T>
    class SpatInterpolator
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // Links to parents
        IModel* model;
        
        std::vector< std::shared_ptr<SpatState<T>> > spatStates;
        
        // may not be used... The balanced should be done on the DAW, or
        // on totalmix (or any software like that, that focuses on hardware
        // control)
        //OutputVolumesState speakersBalanceVolumes;
        
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        SpatType GetSpatType();
        
        size_t GetSpatStatesCount() {return spatStates.size();}
        std::shared_ptr<SpatState<T>> GetSpatState(size_t i) {return spatStates[i];}
        
        std::string GetOutputName(size_t _i);
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        SpatInterpolator(IModel* _model);
        
        void __AddDefaultStates();
        
        
        // - - - - - Speakers management - - - - -
        void AddSpeaker();
        void RemoveSpeaker(size_t _id);
        
    };
    

}


#include "SpatInterpolator.tpp.h"


#endif  // SPATINTERPOLATOR_H_INCLUDED
