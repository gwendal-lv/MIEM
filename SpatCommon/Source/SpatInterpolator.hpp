/*
  ==============================================================================

    SpatInterpolator.hpp
    Created: 26 Nov 2016 12:03:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATINTERPOLATOR_HPP_INCLUDED
#define SPATINTERPOLATOR_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <memory>

#include "SpatType.h"

#include "SpatState.hpp"


#include "OutputVolumesState.hpp"
#include "MatrixState.hpp"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

namespace Miam
{
    class SpatModel;
    
    
    /// \brief Manages some Miam::SpatState children, and does the
    /// interpolation between some of these states when asked to do so.
    ///
    template<typename T>
    class SpatInterpolator
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        const SpatType spatType;
        std::vector< std::shared_ptr<SpatState<T>> > spatStates;
        
        // may not be used... The balanced should be done on the DAW, or
        // on totalmix (or any software like that, that focuses on hardware
        // control)
        //OutputVolumesState speakersBalanceVolumes;
        
        
        //
        int inputsCount = 0;
        int outputsCount = 0;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        int GetInputsCount() {return inputsCount;}
        int GetOutputsCount() {return outputsCount;}
        
        SpatType GetSpatType() {return spatType;}
        
        size_t GetSpatStatesCount() {return spatStates.size();}
        std::shared_ptr<SpatState<T>> GetSpatState(size_t i) {return spatStates[i];}
        
        /// \brief Unoptimized access : copy-constructs a whole vector of shared pointers
        std::vector< std::shared_ptr<SpatState<T>> > GetSpatStates() {return spatStates;}
        
        
        /// \brief Inputs and Outputs config (applies changes to all states)
        void SetInputOuputChannelsCount(int _inputsCount, int _outputsCount)
        {
            // At first : update of internal data
            inputsCount = _inputsCount;
            outputsCount = _outputsCount;
            
            // Config transmission to the individual states
            for (size_t i=0 ; i<spatStates.size() ; i++)
            {
                // Dynamic cast of the state to a MatrixState only for now
                if (std::shared_ptr<MatrixState<T>> matrixState = std::dynamic_pointer_cast<MatrixState<T>>(spatStates[i]))
                {
                    matrixState->SetInputOuputChannelsCount(inputsCount,outputsCount);
                }
                // Else : behavior not implemented
                else
                    throw std::runtime_error("in/out channels count modification is not implemented yet for this spatialization state");
            }
        }

        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction and Destruction  - - - - -

        /// \brief The type of spatialization defines the class
        SpatInterpolator(SpatType _spatType) :
        spatType(_spatType)
        {}
        
        
        
        // - - - - - States management  - - - - -
        /// \brief Adds a pre-allocated and configured state at the end of
        /// the current state's list.
        void AddState(std::shared_ptr<SpatState<T>> newState)
        {
            newState->SetIndex(spatStates.size()); // is automatically the last
            spatStates.push_back(newState);
        }
        /// \brief Adds a default empty zero-matrix state.
        ///
        /// \returns A pointer to the newly created state
        std::shared_ptr<SpatState<T>> AddDefaultState()
        {
            auto emptyState = std::make_shared<MatrixState<T>>(); // default ctor
            AddState(emptyState);
            return emptyState;
        }
        /// \brief For debug/testing purposes
        void __AddDefaultStates()
        {
            for (size_t i = 0 ; i<3 ; i++)
            {
                std::shared_ptr<MatrixState<T>> newState = std::make_shared<MatrixState<T>>();
                newState->SetName("Matrice " + std::to_string(i+1));
                AddState(newState);
            }
        }
        /// \brief Searches for the selected state (by index) then delete it.
        ///
        /// Triggers all necessary updates.
        void DeleteState(std::shared_ptr<SpatState<T>> stateToDelete)
        {
            auto statesIt = spatStates.begin();
            std::advance(statesIt, stateToDelete->GetIndex());
            // returns an iterator the the next element (may be .end())
            auto nextAfterErased = spatStates.erase(statesIt);
            // Indexes actualisation for remaining states after the one erased
            for (auto it=nextAfterErased ; it!=spatStates.end() ; it++)
                (*it)->SetIndex((*it)->GetIndex()-1); // minus 1 for everyone
        }
        void SwapStatesByIndex(size_t index1, size_t index2)
        {
            auto backUpPtr = spatStates[index1];
            spatStates[index1] = spatStates[index2];
            spatStates[index1]->SetIndex(index1);
            spatStates[index2] = backUpPtr;
            spatStates[index2]->SetIndex(index2);
        }
        
        // - - - - - Property tree (for XML) import/export - - - - -
        std::shared_ptr<bptree::ptree> GetSpatStatesTree()
        {
            auto ptree = std::make_shared<bptree::ptree>();
            //ptree.
            return ptree;
        }
    };
    

}


#endif  // SPATINTERPOLATOR_HPP_INCLUDED
