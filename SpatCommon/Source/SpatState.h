/*
  ==============================================================================

    SpatState.h
    Created: 26 Nov 2016 12:00:26pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSTATE_H_INCLUDED
#define SPATSTATE_H_INCLUDED


#include <string>
#include <vector>


namespace Miam
{
    
    
    
    
    /// \brief An abstract class that represents a state of the
    /// speakers' system, that represents a spatialisation state
    /// (or spatialisation effect)
    ///
    /// T is a floating-point type. It might be an int/fixed-point
    /// type some day in the future....
    ///
    /// The number of channels might not be exactly the same as
    /// the parent Miam::SpatInterpolator (and this has to be
    /// properly managed)
    template<typename T>
    class SpatState
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        std::string name;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        virtual std::string GetName() {return name;}
        virtual void SetName(std::string _name) {name = _name;}
        virtual T GetVolume(size_t i) = 0;
        virtual void SetVolume(size_t i, T volume) = 0;
        
        virtual size_t GetOutputsCount() = 0;
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        virtual ~SpatState() {}
        
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        virtual void AddOutput() = 0;
        virtual void DeleteOutput(size_t i) = 0;
        virtual void SwapOutputs(size_t i, size_t j) = 0;
        
    };
    
}


#include "SpatState.tpp.h"

#endif  // SPATSTATE_H_INCLUDED
