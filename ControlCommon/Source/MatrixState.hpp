/*
  ==============================================================================

    MatrixState.h
    Created: 22 May 2017 5:57:31pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <memory>

#include "ControlState.hpp"
#include "AudioDefines.h"

#include "ControlMatrix.hpp"

#include "MiamExceptions.h"

namespace Miam
{
    
    /// \brief Low-level interpolable Routing matrix
    template<typename T>
    class MatrixState : public ControlState<T>
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        // La base : la matrice creuse cachée à l'intérieur de l'état
        ControlMatrix matrix;
        
        // Attribut dupliqués depuis l'interpolateur
        int inputsCount = 0;
        int outputsCount = 0;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        
        size_t GetInputsCount()
        {return (size_t) inputsCount;}
        size_t GetOutputsCount() override
        {return (size_t) outputsCount;}

        /// \brief Accès aux cases de la matrice par opérateur [] à simple entrée
        inline T operator[] (size_t k) const { return matrix[k]; }

        /// \brief Accès aux cases de la matrice par opérateur () à double entrée
        ///
        /// Opérateur [] n'accepte pas la surcharge à 2 paramètres
        inline T operator() (size_t i, size_t j) const { return matrix(i,j); }

        inline Index2d GetIndex2dFromIndex(size_t index1d)
        { return matrix.GetIndex2dFromIndex(index1d); }
        inline size_t GetIndexFromIndex2d(Index2d index2d)
        { return matrix.GetIndexFromIndex2d(index2d); }
        
#ifdef __MIAM_DEBUG
        inline size_t GetNonZeroCoeffsCount() {return matrix.GetNonZeroCoeffsCount();}
        void DisplayMatrixInStdCout()
        {
            std::cout << GetNonZeroCoeffsCount() << " non-nuls : ";
            matrix.DisplayInStdCout();
            std::cout << std::endl;
        }
#endif
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        
        /// \brief Builds a state based on a zero Miam::SparseMatrix
        MatrixState()
        {
            this->SetName("Matrix-based generic control state");
        }
        virtual ~MatrixState() {}
        
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        
        void AddOutput() override
        {throw std::logic_error("MIEM not implemented");}
        void DeleteOutput(size_t /*i*/) override
        {throw std::logic_error("MIEM not implemented");}
        void SwapOutputs(size_t /*i*/, size_t /*j*/) override
        {throw std::logic_error("MIEM not implemented");}
        
        // - - - - - Input channels : add, delete, swap, ... - - - - -

        /// \brief Does nothing.... Static matrix for now (keeps all existing coeffs)
        void SetInputOuputChannelsCount(int inputsCount_, int outputsCount_)
        {
            inputsCount = inputsCount_;
            outputsCount = outputsCount_;
        }
        bool IsIndexWithinActualInputOutputBounds(size_t index1d)
        {
            return (GetIndex2dFromIndex(index1d).i < inputsCount) && (GetIndex2dFromIndex(index1d).j < outputsCount);
        }

        // - - - - - Matrix management - - - - -
        public :
        
        void ClearMatrix() { matrix.Clear(); }
        
        /// \brief Copy-constructs a duplicate of the internal matrix
        ///
        /// Dynamically allocates memory !
        std::shared_ptr< ControlMatrix > GetMatrixCopy()
        { return std::make_shared< ControlMatrix >(matrix); }
        
        /// \brief Internally sets the matrix from a shared_ptr of another
        void SetMatrix(std::shared_ptr< ControlMatrix > newMatrix)
        {
            matrix = *(newMatrix.get());
        }
        
        /// \brief Surcharge sans 'const' car la matrice a des attributs modifiés
        /// quand on la lit (conteneur optimisé...)
        void operator+=(MatrixState<T>& matrixState)
        {
            matrix += matrixState.matrix;
        }
        
        /// \brief Voir Miam::SparseMatrix<T>::MultiplyAndAccumulate
        void MultiplyAndAccumulate(MatrixState<T>& matrixToMultAndAdd, T factor)
        {
            matrix.MultiplyAndAccumulate(matrixToMultAndAdd.matrix, factor);
        }
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree() override
        {
            auto pTree = this->ControlState<T>::GetTree();
            pTree->put_child("matrix", *(matrix.GetTree()));
            return pTree;
        }
        
        virtual void SetFromTree(bptree::ptree & stateTree) override
        {
            // At first, common properties for all states
            this->ControlState<T>::SetFromTree(stateTree);
            
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

