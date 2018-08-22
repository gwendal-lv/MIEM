/*
  ==============================================================================

    OutputVolumesState.h
    Created: 26 Nov 2016 12:09:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef OUTPUTVOLUMESSTATE_H_INCLUDED
#define OUTPUTVOLUMESSTATE_H_INCLUDED



#include "SpatState.hpp"


namespace Miam
{
    
    
    
    /// \brief
    template<typename T>
    class OutputVolumesState : public SpatState<T>
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        std::string name;
        
        std::vector<T> outputVolumes;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        virtual T GetVolume(size_t i);
        virtual void SetVolume(size_t i, T volume);
        
        virtual size_t GetOutputsCount() override;
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        // - - - - - Construction and Destruction - - - - -
        OutputVolumesState(size_t outputsCount);
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        public :
        virtual void AddOutput() override;
        virtual void DeleteOutput(size_t i) override;
        virtual void SwapOutputs(size_t i, size_t j) override;
        
        
        
        
    };
    
}


#include "OutputVolumesState.tpp.h"

#endif  // OUTPUTVOLUMESSTATE_H_INCLUDED
