/*
  ==============================================================================

    MatrixState.h
    Created: 22 May 2017 5:57:31pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <memory>

#include "SpatState.hpp"
#include "AudioDefines.h"

#include "MiamExceptions.h"

namespace Miam
{
    
    /// \brief Low-level interpolable Routing matrix
    template<typename T>
    class MatrixState : public SpatState<T>
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        
        SparseMatrix<T, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10> matrix;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        size_t GetOutputsCount() override
        {throw std::logic_error("not implemented");}
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        
        /// \brief Builds a state based on a zero Miam::SparseMatrix
        MatrixState()
        {
            this->SetName("Matrix-based spatialization state");
        }
        virtual ~MatrixState() {}
        
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        
        void AddOutput() override
        {throw std::logic_error("not implemented");}
        void DeleteOutput(size_t /*i*/) override
        {throw std::logic_error("not implemented");}
        void SwapOutputs(size_t /*i*/, size_t /*j*/) override
        {throw std::logic_error("not implemented");}
        
        // - - - - - Input channels : add, delete, swap, ... - - - - -

        /// \brief Does nothing.... Static matrix for now (keeps all existing coeffs)
        void SetInputOuputChannelsCount(int _inputsCount, int _outputsCount)
        {
        }

        // - - - - - Matrix management - - - - -
        public :
        
        /// \brief Copy-constructs a duplicate of the internal matrix
        ///
        /// Dynamically allocates memory !
        std::shared_ptr< SparseMatrix<T, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10> > GetMatrix()
        { return std::make_shared< SparseMatrix<T, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10> >(matrix); }
        
        /// \brief Internally sets the matrix from a shared_ptr of another
        void SetMatrix(std::shared_ptr< SparseMatrix<T, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10> > newMatrix)
        {
            matrix = *(newMatrix.get());
        }
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree() override
        {
            auto pTree = this->SpatState<T>::GetTree();
            pTree->put_child("matrix", *(matrix.GetTree()));
            return pTree;
        }
        
        virtual void SetFromTree(bptree::ptree & stateTree) override
        {
            // At first, common properties for all spat states
            this->SpatState<T>::SetFromTree(stateTree);
            
            // Then, matrix attributes. SparseMatrix will
            // reinit at first (which may not be optimal when called just
            // after construction... But the compiler could see it)
            try {
                auto matrixTree = stateTree.get_child("matrix");
                matrix.SetFromTree(matrixTree);
            }
            catch (bptree::ptree_bad_path& e) {
                throw XmlReadException::FromBptree("state", e);
            }
        }
        
    };
    
}

