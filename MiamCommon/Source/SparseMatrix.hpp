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
    
    
    typedef struct Index2d {
        size_t i;
        size_t j;
    } Coord2d;
    
    
    
    /// \brief Sparse matrix optimized for computation efficiency, and
    /// not for memory efficiency. Everything is allocated during construction.
    ///
    /// On garde quand même un accès direct aux coefficients ! Puisque tout est alloué d'avance.
    ///
    /// T is a type that std::abs can process
    /// matrix of N rows, and M columns
    ///
    /// ZT10 is the integer power of ten that allow us to compute the actual
    /// zero-threshold
    /// (as float is not authorized in templates)
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
        int indexOfLastNonZeroIndex;
        size_t iteratorPos;
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // Default constructor
        SparseMatrix() :
        zeroThreshold( (T) std::pow((long double)10.0, (long double)ZT10) )
        {
            Clear();
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
            Clear();
            for (size_t k=0 ; k<endIndex ; k++)
            {
                if ( ! IsNegligible(rawDenseMatrix[k]) )
                {
                    matrix[k] = rawDenseMatrix[k];
                    indexOfLastNonZeroIndex++;
                    nonZeroIndexes[indexOfLastNonZeroIndex] = k;
                }
            }
        }
        
        // Auxiliary functions
        public :
        void Clear()
        {
            indexOfLastNonZeroIndex = -1;
            for (size_t k=0 ; k<endIndex ; k++)
            {
                matrix[k] = {};
                nonZeroIndexes[k] = endIndex;
            }
            
            iteratorPos = 0;
        }
        private :
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
        public :
        inline bool IsNegligible(T value) const
        { return ( std::abs(value) <= (T)zeroThreshold ) ; }
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        inline Index2d GetIndex2dFromIndex(size_t index) const
        {
            Index2d returnCoord;
            std::ldiv_t divisionResult = std::ldiv((long)index, (long)M);
            returnCoord.i = (size_t)divisionResult.quot;
            returnCoord.j = (size_t)divisionResult.rem;
            return returnCoord;
        }
        
        
        
        /// \brief Direct assignation from original data
        void operator= (SparseMatrix<T,N,M,ZT10> & originalMatrix)
        {
            copyConstruct(originalMatrix);
        }
        
        /// \brief Operator "Get" overloaded (! "Set" not overloaded !)
        ///
        /// k is a global matrix index within [0 ; N*M-1]
        inline T operator[] (size_t k) const { return matrix[k]; }
        
        /// \brief Operator "Get" overloaded (! "Set" not overloaded !)
        ///
        /// i,j are the row and col indexes
        inline T operator() (size_t i, size_t j) const { return matrix[idx(i,j)]; }
        
        /// \brief Deals with zero/non-zero cases then sets the coefficient
        inline void Set(size_t i, size_t j, T value) { Set(idx(i,j), value); }
        inline void Set(size_t coeffIndex, T value)
        {
            // Non-zero case : the coeff is added to the list (if it wasn't in)
            if ( ! IsNegligible(value) )
            {
                // Avant d'écrire effectivement le coeff : s'il était négligeable,
                // on ne risque pas de le trouver...
                bool coeffMustBeFound = ! IsNegligible(matrix[coeffIndex]); // was it already non-zero ?
                
                // Ensuite on fait l'affectation, et on update les états comme il faut
                matrix[coeffIndex] = value;
                
                // Tant qu'on a pas trouvé le coeff on le cherche
                // MAIS seulement si on a une chance de le trouver...
                bool foundCoeff = false; // was it already there ?
                if (coeffMustBeFound)
                {
                    for (size_t k=0 ; k<endIndex && !foundCoeff ; k++)
                    {
                        if (nonZeroIndexes[k] == coeffIndex)
                            foundCoeff = true;
                    }
                }
                // if is wasn't in yet : là on ajoute effectivement, à la fin
                if (!foundCoeff)
                {
                    indexOfLastNonZeroIndex++;
                    nonZeroIndexes[indexOfLastNonZeroIndex] = coeffIndex;
                }
#ifdef __MIAM_DEBUG
                // Erreur pour le débug.... Si on est dans ce cas là, c'est qu'il y a un truc
                // qui a chié qqpart (mais çe ne devrait plus arriver)
                if (coeffMustBeFound && !foundCoeff)
                    throw std::logic_error("Problème logique dans une matrice creuse ! Coefficient non-nul qui devrait être trouvé dans la liste, mais qui n'est plus là en pratique...");
#endif
            }
            // Else, we might need to remove it from the list
            else
            {
                // If wasn't zero (before set),
                // we must find and remove it
                if ( ! IsNegligible(matrix[coeffIndex]) )
                {
                    size_t k=0;
                    bool foundCoeff = false;
                    // 2-features loop : research, then move of coeffs if necessary
                    while(k != endIndex && nonZeroIndexes[k] != endIndex)
                    {
                        if (! foundCoeff) // si on cherche toujours
                        {
                            // suppression if necessary
                            if (nonZeroIndexes[k] == coeffIndex)
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
                
                // In any case : ACTUAL true zero setting (must be unuseful, but...)
                matrix[coeffIndex] = {};
            }
        }
        
        
        // = = = = = = = = = = ITERATOR = = = = = = = = = =
        
        /// \brief Makes the internal "iterator system" restart to beginning
        ///
        /// The start might be the end as well...
        inline void ResetIterator() {iteratorPos = 0;}
        
        /// \Returns The current iterator value, pointing to a non-null coefficient.
        inline size_t GetIterator() const {return iteratorPos;}
        /// \Returns The non-zero value pointed at by the current internal iterator.
        inline T GetIteratorValue() const { return matrix[nonZeroIndexes[iteratorPos]]; }
        /// \Returns The 1d matrix coordinate of the value pointed at by the current
        /// internal iterator
        inline size_t GetIterator1dCoord() const { return nonZeroIndexes[iteratorPos]; }
        /// \Returns The 2d matrix coordinates of the value pointed at by the current
        /// internal iterator
        inline Index2d GetIterator2dCoord() const { return GetIndex2dFromIndex(nonZeroIndexes[iteratorPos]); }
        
        /// \brief Increments the iterator. The iterator will then point to the next
        /// non-zero coefficient of the matrix.
        inline void IncIterator() {iteratorPos++;}
        
        /// \brief Value that is >= 0
        inline size_t GetEndIterator() const {return (size_t)indexOfLastNonZeroIndex+1;}
        /// \brief pour l'instant c'est la même chose que l'itérateur (= un size_t) de fin (similaire .end)
        inline size_t GetNonZeroCoeffsCount() const {return (size_t)indexOfLastNonZeroIndex+1;}
        
        
        // = = = = = = = = = = ARITHMETIC OPERATIONS = = = = = = = = = =
        public :
        
        /// \brief La seule manière d'additionner... L'opérateur avec 2 arguments doit renvoyer
        /// un nouvel objet (ce qui fait une grosse allocation mémoire ou de grosses copies de données)
        ///
        /// Peut prendre beaucoup de temps pour des matrices quasi-pleines, évidemment.
        /// Si S non-nuls et T non-nuls, complexité max S*T
        void operator+=(SparseMatrix<T,N,M,ZT10>& matrixToAdd)
        {
            // D'abord on parcourt tous les termes non-nuls de la matrice en argument
            for (matrixToAdd.ResetIterator() ;
                 matrixToAdd.GetIterator() < matrixToAdd.GetEndIterator() ;
                 matrixToAdd.IncIterator() )
            {
                Coord2d coeff2dCoord = matrixToAdd.GetIterator2dCoord();
                // On fait juste un "set" et on laisse cette méthode faire toutes les recherches
                // nécessaires...
                // En tout cas les temps d'accès ici sont négligeables.
                Set(coeff2dCoord.i, coeff2dCoord.j,
                    matrixToAdd.GetIteratorValue() + matrix[matrixToAdd.GetIterator1dCoord()]);
            }
        }
        
        /// \brief Fonction qui fait "multiplier l'argument par un scalaire
        /// puis l'ajouter à this" de
        /// manière optimisée.
        ///
        /// L'optimisation serait plus difficile via surcharge d'opérateurs. Code très proche de la
        /// surcharge de +=
        void MultiplyAndAccumulate(SparseMatrix<T,N,M,ZT10>& matrixToMultAndAdd, T factor)
        {
            // D'abord on parcourt tous les termes non-nuls de la matrice en argument
            for (matrixToMultAndAdd.ResetIterator() ;
                 matrixToMultAndAdd.GetIterator() < matrixToMultAndAdd.GetEndIterator() ;
                 matrixToMultAndAdd.IncIterator() )
            {
                Coord2d coeff2dCoord = matrixToMultAndAdd.GetIterator2dCoord();
                // On fait juste un "set" et on laisse cette méthode faire le nécessaire
                Set(coeff2dCoord.i, coeff2dCoord.j,
                    matrixToMultAndAdd.GetIteratorValue() * factor
                    + matrix[matrixToMultAndAdd.GetIterator1dCoord()]);
            }
        }
        
        
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
        
        
        
        // = = = = = = = = Display functionnalities = = = = = = = = =
        void DisplayInStdCout()
        {
            // D'abord on parcourt tous les termes non-nuls de la matrice en argument
            for (ResetIterator() ;
                 GetIterator() < GetEndIterator() ;
                 IncIterator() )
            {
                Coord2d coeff2dCoord = GetIterator2dCoord();
                std::cout << " [" << coeff2dCoord.i << "," << coeff2dCoord.j << "]=" << GetIteratorValue() << " ";
            }
        }

    };
    
    
}
