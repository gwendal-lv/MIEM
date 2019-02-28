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

#define MIEM_MAGIC_MESSAGE_HEADER_NUMBER        (0x19982018) // lol

// tailles en octets
#define MIEM_Block_Type_Size        (4)   // int 32
#define MIEM_Block_Int_Size         (4)   // int 32

// Actual JUCE memory block in TCP packet will be +8 (here: 72)
#define MIEM_Block_Full_Size        (64)  // 512 bits: 16 * 32-bit integers

namespace Miam
{
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
        
        
        
        /// \brief à partir d'un bloc reçu: on sort juste les paramètres
        ///
        /// Return a param with a "None' type if the block was not valid
        static Miam::AsyncParamChange DecodeMemoryBlock(const juce::MemoryBlock & memoryBlock)
        {
            Miam::AsyncParamChange returnParam;
            
            // not valid if wrong size
            if (memoryBlock.getSize() != MIEM_Block_Full_Size)
                return Miam::AsyncParamChange(Miam::AsyncParamChange::None);
            
            // trying to get and cast the type
            int32_t bigEndianType = 0;
            memoryBlock.copyTo(&bigEndianType, 0, sizeof(int32_t));
            auto nativeType = boost::endian::big_to_native<int32_t>(bigEndianType);
            if (! IsConnectionIntTypeValid(nativeType))
                return Miam::AsyncParamChange(Miam::AsyncParamChange::None);
            returnParam.Type = static_cast<Miam::AsyncParamChange::ParamType>(nativeType);
            
            // value
            int32_t bigEndianId1 = 0;
            memoryBlock.copyTo(&bigEndianId1, sizeof(int32_t), sizeof(int32_t));
            auto nativeId1 = boost::endian::big_to_native<int32_t>(bigEndianId1);
            returnParam.Id1 = nativeId1;
            
            return returnParam;
        }
        
        /// \brief Checks whether the param type sent as an int32_t from a TCP (or other ?)
        /// interprocess connection is valid.
        static bool IsConnectionIntTypeValid(int intParamType)
        {
            // At first, we check for general rules (including all params)
            if (Miam::AsyncParamChange::IsIntValidParamType(intParamType))
            {
                Miam::AsyncParamChange::ParamType castedParamType
                = static_cast<Miam::AsyncParamChange::ParamType>(intParamType);
                // Then we limit the params to the ones that can be controlled through TCP
                switch (castedParamType)
                {
                    case Miam::AsyncParamChange::Scene :
                        return true;
                        
                    default :
                        break;
                }
            }
            
            // If we couldn't go to a return true... then it is false
            return false;
        }
    };
}


