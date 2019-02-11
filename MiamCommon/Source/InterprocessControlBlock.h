/*
  ==============================================================================

    InterprocessControlBlock.h
    Created: 11 Feb 2019 10:33:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "AsyncParamChange.h"

#include "JuceHeader.h"


#include "boost/endian/conversion.hpp"
#include "boost/lexical_cast.hpp"

// tailles en octets
#define MIEM_Block_Type_Size        (4)   // int 32
#define MIEM_Block_Int_Size         (4)   // int 32

#define MIEM_Block_Full_Size        (64)  // 512 bits: 16 * 32-bit integers


class InterprocessControlBlock : public juce::MemoryBlock
{
    private :
    
    public :
    // Peut être construit à partir d'un AsyncParamChange
    InterprocessControlBlock(Miam::AsyncParamChange blockParamChange) :
    MemoryBlock(MIEM_Block_Full_Size, true) // fill with zeros
    {
        int32_t nativeIntType = static_cast<int32_t>(blockParamChange.Type);
        int32_t bigEndianIntType = boost::endian::native_to_big<int32_t>(nativeIntType);
        
        int32_t nativeId1 = static_cast<int32_t>(blockParamChange.Id1);
        int32_t bigEndianId1 = boost::endian::native_to_big<int32_t>(nativeId1);
        
        this->copyFrom(&bigEndianIntType, 0, sizeof(int32_t));
        this->copyFrom(&bigEndianId1, sizeof(int32_t), sizeof(int32_t));
    }
    
    
    
    // à partir d'un bloc reçu: on sort juste les paramètres
    static Miam::AsyncParamChange DecodeMemoryBlock(const juce::MemoryBlock & memoryBlock)
    {
        return Miam::AsyncParamChange();
    }
};


