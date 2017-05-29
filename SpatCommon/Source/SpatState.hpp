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

#include "SparseMatrix.hpp"
#include "AudioDefines.h"

#include "SpatArea.h"

namespace Miam
{
    // Forward declarations
    class SpatArea;
    
    
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
        // For convenience and optimizations...
        int index;
        
        
        /// \brief List of areas that represent this spatialization state
        ///
        /// Not the most optimal STL container (research is not optimized)
        std::vector< std::weak_ptr<SpatArea> > linkedAreas;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        /// \returns A string containing the Id (counting from 1) and the name
        /// concatenated
        virtual std::string GetName(bool withId = true)
        {
            std::string indexString;
            if (withId)
                indexString += std::to_string(index+1) + ". ";
            return indexString + name;
        }
        virtual void SetName(std::string _name)
        {
            name = _name;
            for (auto it = linkedAreas.begin() ; it!=linkedAreas.end() ; it++)
                (*it).lock()->OnSpatStateNameChanged();
        }
        
        void SetIndex(int newIndex) {index = newIndex;}
        int GetIndex() {return index;}
        
        virtual size_t GetOutputsCount() = 0;
        
        virtual size_t GetLinkedAreasCount() {return linkedAreas.size();}
        virtual std::shared_ptr<SpatArea> GetLinkedArea(size_t i)
        {
            if(auto area = linkedAreas[i].lock())
                return area;
            else
                throw std::runtime_error("Spat state is linked to a non-existant area");
        }
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        SpatState() : index(-1) {}
        
        
        // - - - - - Construction / destruction - - - - -
        virtual ~SpatState()
        {
            // does not need to unlink from area : the link is a weak_ptr
        }

        
        // - - - - - Linking to spat areas - - - - -
        void LinkToArea(std::shared_ptr<SpatArea> spatArea)
        {
            // no check for double addition (but it's weak ptrs...)
            linkedAreas.push_back(spatArea);
        }
        void UnlinkToArea(std::shared_ptr<SpatArea> spatArea)
        {
            for (auto it = linkedAreas.begin() ; it!=linkedAreas.end() ; it++)
            {
                auto curAreaPtr = (*it).lock();
                if (curAreaPtr == spatArea)
                {
                    linkedAreas.erase(it);
                    break;
                }
            }
        }
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        virtual void AddOutput() = 0;
        virtual void DeleteOutput(size_t i) = 0;
        virtual void SwapOutputs(size_t i, size_t j) = 0;
        
    };
    
}


#endif  // SPATSTATE_H_INCLUDED
