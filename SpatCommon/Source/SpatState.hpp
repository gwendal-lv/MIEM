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
#include <memory>

#include "IInteractiveArea.h"

#include "SparseMatrix.hpp"
#include "AudioDefines.h"


namespace Miam
{
    // Forward declarations
    class IInteractiveArea;
    
    
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
    ///
    /// !!! Attention aux fonctions membres virtuelles qui ne peuvent alors
    /// pas être template !!! -> encore vrai sur tous les compilateurs ?
    template<typename T>
    class SpatState
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        // Own attributes
        std::string name;
        
        
        
        /// \brief List of areas that represent this spatialization state
        ///
        /// Not the most optimal STL container (research is not optimized)
        std::vector< std::shared_ptr<IInteractiveArea> > linkedAreas;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        virtual std::string GetName() {return name;}
        virtual void SetName(std::string _name) {name = _name;}
        
        virtual size_t GetOutputsCount() = 0;
        
        virtual size_t GetLinkedAreasCount() {return linkedAreas.size();}
        virtual std::shared_ptr<IInteractiveArea> GetLinkedArea(size_t i) {return linkedAreas[i];}
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        // TODO
        // MUST UNLINK FROM AREA (if remaining...)
        // TODO
        // MUST UNLINK FROM AREA (if remaining...)
        // TODO
        // MUST UNLINK FROM AREA (if remaining...)
        // TODO
        // MUST UNLINK FROM AREA (if remaining...)
        virtual ~SpatState()
        {}
        
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        virtual void AddOutput() = 0;
        virtual void DeleteOutput(size_t i) = 0;
        virtual void SwapOutputs(size_t i, size_t j) = 0;
        
    };
    
}


#endif  // SPATSTATE_H_INCLUDED
