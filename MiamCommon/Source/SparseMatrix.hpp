/*
  ==============================================================================

    SparseMatrix.hpp
    Created: 25 May 2017 9:42:43pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <cmath> // std::abs : available for ints, floats, complexes, ....

#include "MiamExceptions.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


namespace Miam
{
    
    /// \brief Sparse matrix optimized for computation efficiency, and
    /// not for memory efficiency. Everything is allocated during construction.
    ///
    /// T is a type that std::abs can process
    /// matrix of N rows, and M columns
    ///
    /// ZT10 is the integer power of ten that allow us to compute the actual
    /// zero-threshold
    /// (as float ain't authorized in templates)
    ///
    /// Needs at least 2 * N * M * sizeof(T) bytes
    template<typename T, size_t N, size_t M, int ZT10>
    class SparseMatrix
    {
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // The matrix itself
        T matrix[N*M];
        const T zeroThreshold;
        
        // List of non-zero coefficients
        size_t nonZeroIndexes[N*M];
        const size_t endIndex = N*M;
        // Stores the (maybe inexistant) index of the last useful coeff of the matrix
        int indexOfLastNonZeroIndex = -1;
        size_t iteratorPos = 0;
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // Default constructor
        SparseMatrix() :
        zeroThreshold( (T) std::pow((long double)10.0, (long double)ZT10) )
        {
            reinitMatrix();
        }
        // Copy constructor
        SparseMatrix(SparseMatrix<T,N,M,ZT10> & originalMatrix) :
        zeroThreshold(originalMatrix.zeroThreshold)
        {
            copyConstruct(originalMatrix);
        }
        /// \brief Optimized constructor from a raw dense matrix (stored in an
        /// 1d array of the right size)
        ///
        /// No check is performed on the array size
        SparseMatrix(T* rawDenseMatrix) :
        zeroThreshold( (T) std::pow((long double)10.0, (long double)ZT10) )
        {
            reinitMatrix();
            for (size_t k=0 ; k<endIndex ; k++)
            {
                if ( ! isNegligible(rawDenseMatrix[k]) )
                {
                    matrix[k] = rawDenseMatrix[k];
                    indexOfLastNonZeroIndex++;
                    nonZeroIndexes[indexOfLastNonZeroIndex] = k;
                }
            }
        }
        
        // Auxiliary functions
        private :
        void reinitMatrix()
        {
            for (size_t k=0 ; k<endIndex ; k++)
            {
                matrix[k] = {};
                nonZeroIndexes[k] = endIndex;
            }
        }
        void copyConstruct(SparseMatrix<T,N,M,ZT10> & originalMatrix)
        {
            indexOfLastNonZeroIndex = originalMatrix.indexOfLastNonZeroIndex;
            iteratorPos = originalMatrix.iteratorPos;
            for (size_t k=0 ; k<endIndex ; k++)
            {
                matrix[k] = originalMatrix.matrix[k];
                nonZeroIndexes[k] = originalMatrix.nonZeroIndexes[k];
            }
        }
        inline size_t idx(size_t i, size_t j) const { return i*M+j; }
        inline bool isNegligible(T value) const
        { return ! (std::abs(value) >= (T)zeroThreshold) ; }
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        /// \brief Direct assignation from original data
        void operator= (SparseMatrix<T,N,M,ZT10> & originalMatrix)
        {
            copyConstruct(originalMatrix);
        }
        
        /// \brief Operator "Get" overloaded (! "Set" not overloaded !)
        ///
        /// k is a global matrix index within [0 ; N*M-1]
        /// Not used for now, as a class can access private members of other instances
        //inline T operator() (size_t k) { return matrix[k]; }
        
        /// \brief Operator "Get" overloaded (! "Set" not overloaded !)
        ///
        /// i,j are the row and col indexes
        inline T operator() (size_t i, size_t j) const { return matrix[idx(i,j)]; }
        
        /// \brief Deals with zero/non-zero cases then sets the coefficient
        inline void Set(size_t i, size_t j, T value)
        {
            // Non-zero case : the coeff is added to the list (if it wasn't in)
            if ( ! isNegligible(value) )
            {
                matrix[idx(i,j)] = value;
                bool foundCoeff = false; // was it already there ?
                for (size_t k=0 ; k<endIndex && !foundCoeff ; k++)
                {
                    if (nonZeroIndexes[k] == idx(i,j))
                        foundCoeff = true;
                }
                if (!foundCoeff) // if is wasn't in yet
                {
                    indexOfLastNonZeroIndex++;
                    nonZeroIndexes[indexOfLastNonZeroIndex] = idx(i,j);
                }
            }
            // Else, we might need to remove it from the list
            else
            {
                // If wasn't zero, we must find and remove it
                if ( ! isNegligible(matrix[idx(i,j)]) )
                {
                    size_t k=0;
                    bool foundCoeff = false;
                    // 2-features loop : research, then move of coeffs if necessary
                    while(k != endIndex && nonZeroIndexes[k] != endIndex)
                    {
                        if (! foundCoeff) // si on cherche toujours
                        {
                            // suppression if necessary
                            if (nonZeroIndexes[k] == idx(i,j))
                            {
                                foundCoeff = true;
                                nonZeroIndexes[k] = endIndex;
                                indexOfLastNonZeroIndex--;
                            }
                        }
                        else // si on a trouvé, décalage de tous vers le début
                            nonZeroIndexes[k-1] = nonZeroIndexes[k];
                        // Next iteration
                        k++;
                    }
                }
                // Else, true zero setting (must be unuseful, but...)
                else
                    matrix[idx(i,j)] = {};
            }
        }
        
        
        // = = = = = = = = = = ITERATOR = = = = = = = = = =
        
        /// \brief Makes the internal "iterator system" restart to beginning
        ///
        /// The start might be the end as well...
        inline void ResetIterator() {iteratorPos = 0;}
        
        /// \Returns the current iterator value
        inline size_t GetIterator() const {return iteratorPos;}
        /// \Returns the value pointed at by the current internal iterator.
        inline T GetIteratorValue() const { return matrix[nonZeroIndexes[iteratorPos]]; }
        /// \Returns The 1d matrix coordinate of the value pointed at by the current
        /// internal iterator
        inline size_t GetIterator1dCoord() const { return nonZeroIndexes[iteratorPos]; }
        typedef struct Index2d {
            size_t i;
            size_t j;
        } Coord2d;
        /// \Returns The 2d matrix coordinates of the value pointed at by the current
        /// internal iterator
        inline Index2d GetIterator2dCoord() const
        {
            Index2d returnCoord;
            std::ldiv_t divisionResult = std::ldiv((long)nonZeroIndexes[iteratorPos], (long)M);
            returnCoord.i = (size_t)divisionResult.quot;
            returnCoord.j = (size_t)divisionResult.rem;
            return returnCoord;
        }
        
        /// \brief Increments the iterator
        inline void IncIterator() {iteratorPos++;}
        
        /// \brief Value that is >= 0
        inline size_t GetEndIterator() const {return (size_t)indexOfLastNonZeroIndex+1;}
        
        
        
        // = = = = = = = = Property tree (for XML) import/export = = = = = = = = =
        std::shared_ptr<bptree::ptree> GetTree()
        {
            auto pTree = std::make_shared<bptree::ptree>();
            // This sub-tree does not know its own "master name tag" = <matrix>
            for (ResetIterator() ; GetIterator() != GetEndIterator() ; IncIterator())
            {
                bptree::ptree currentCoeffTree;
                Index2d index2d = GetIterator2dCoord();
                currentCoeffTree.put("<xmlattr>.row", index2d.i);
                currentCoeffTree.put("<xmlattr>.col", index2d.j);
                currentCoeffTree.put("<xmlattr>.value", GetIteratorValue());
                pTree->add_child("coeff", currentCoeffTree);
            }
            return pTree;
        }
        /// \brief Reinits, then loads the matrix from the given property tree
        void SetFromTree(bptree::ptree & matrixTree)
        {
            try {
                // get_child get as 1st parameter the address of the
                // *parents* of the children
                // So we'll get ALL children, and we'll test one-by-one if they
                // actually are <coeff> children
                for(bptree::ptree::value_type &matChildTree : matrixTree.get_child(""))
                {
                    if (matChildTree.first == "coeff")
                    {
                        // Properties reading from tree, and checking
                        auto coeffTree = matChildTree.second;
                        size_t row = coeffTree.get<size_t>("<xmlattr>.row");
                        if (row >= N)
                            throw XmlReadException("matrix.coeff.<xmlattr>.row must be < " + std::to_string(N));
                        size_t col = coeffTree.get<size_t>("<xmlattr>.col");
                        if (col >= M)
                            throw XmlReadException("matrix.coeff<xmlattr>.col must be < " + std::to_string(M));
                        T value = coeffTree.get<T>("<xmlattr>.value");
                        if (value < zeroThreshold) // value too small : not loaded
                            continue;
                        // Actual data insertion in matrix
                        Set(row, col, value);
                    }
                }
            }
            catch (bptree::ptree_bad_data &e) { // Parse error: we actually treat it
                XmlReadException::FromBptree("matrix.coeff, attribute (row|col|value)", e);
            }
            catch (bptree::ptree_bad_path &e) {
                XmlReadException::FromBptree("matrix.coeff, attribute (row|col|value)", e);
            }
        }
    };
    
    
}
