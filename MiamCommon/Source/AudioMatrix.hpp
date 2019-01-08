/*
  ==============================================================================

    AudioMatrix.hpp
    Created: 8 Jan 2019 11:57:49am
    Author:  Gwendal

  ==============================================================================
*/

#pragma once

#include "SparseMatrix.hpp"

namespace Miam
{
    
    enum class CorrelationLevel {
        Low, ///< Represents a low inter-correlation value between signals, e.g. two mono recordings of two different instruments playing the same song.
        
        High ///< Represents a high inter-correlation value between signals, e.g. two tracks L/R of a standard stereo recording with very few stereo spatialisation effects
    };
    
    
    template<typename T, size_t N, size_t M, int ZT10>
    class AudioMatrix :
    public SparseMatrix<T, N, M, ZT10>
    {
        
        // = = = = = = = = = = METHODS = = = = = = = = = =

        // - - - - - - contructors of SparseMatrix have to be
        // redeclared and called from here - - - - - - - - - - - -
        public :
        AudioMatrix() :
        SparseMatrix<T, N, M, ZT10>()
        { }
        // Copy constructor
        AudioMatrix(AudioMatrix<T,N,M,ZT10> & originalMatrix) :
        SparseMatrix<T, N, M, ZT10>(originalMatrix)
        { }
        /// \brief Optimized constructor from a raw dense matrix (stored in an
        /// 1d array of the right size)
        ///
        /// No check is performed on the array size
        AudioMatrix(T* rawDenseMatrix) :
        SparseMatrix<T, N, M, ZT10>(rawDenseMatrix)
        { }
        
        
        
        
        // - - - Audio Computations on the matrix - - -
        public :
        
        /// \brief Computes the total volume of the matrix, considering that the
        /// inputs and outputs are audio signals (inputs and outputs can be
        /// considered correlated or not)
        ///
        /// Parameters of type "size_t" define the indexes of the first row, first col,
        /// height and width of the sub-matrix within which the total volume is computed.
        T ComputeTotalVolume(CorrelationLevel inputCorrelationLevel, CorrelationLevel outputCorrelationLevel,
                                  size_t subMatrixFirstRow = 0, size_t subMatrixFirstCol = 0,
                                  size_t subMatrixRowsCount = N, size_t subMatrixColsCount = M)
        {
            const size_t subMatrixLastRowIndex = subMatrixFirstRow + subMatrixRowsCount;
            const size_t subMatrixLastColIndex = subMatrixFirstCol + subMatrixColsCount;
            
            /// - - - - general presentation of the computation algorithm - - - -
            // For correlated case, simple sum of amplitudes.
            // For the de-correlated case : we sum the squared amplitudes (~ the powers...)
            // and we will take the square root at the end of the computation
            ///
            /// We proceed by summing all the rows of each col, depending the coherence level
            /// indicated for the inputs.
            /// Then we sum the columns' sums following the rule indicated by the coherence level
            /// of outputs.
            /// - - - - - - - - - - - - - - - - - - - - - - - - -
            
            // Temporary and result volumes (init to zero)
            T colVolumes[M];
            for (size_t i=0 ; i<M ; i++)
            {
                colVolumes[i] = T();
            }
            T totalVolume = T();
            
            // 1st step: computation of the total volume on each output
            for (SparseMatrix<T,N,M,ZT10>::ResetIterator() ;
                 SparseMatrix<T,N,M,ZT10>::GetIterator() < SparseMatrix<T,N,M,ZT10>::GetEndIterator() ;
                 SparseMatrix<T,N,M,ZT10>::IncIterator() )
            {
                Coord2d coeff2dCoord = SparseMatrix<T,N,M,ZT10>::GetIterator2dCoord();
                if (subMatrixFirstRow <= coeff2dCoord.i && coeff2dCoord.i <= subMatrixLastRowIndex
                    && subMatrixFirstCol <= coeff2dCoord.j && coeff2dCoord.j <= subMatrixLastColIndex)
                {
                    const T value = SparseMatrix<T,N,M,ZT10>::GetIteratorValue();
                    if (inputCorrelationLevel == CorrelationLevel::High)
                        colVolumes[coeff2dCoord.j] += value;
                    else if (inputCorrelationLevel == CorrelationLevel::Low)
                        colVolumes[coeff2dCoord.j] += (value * value);
                }
            }
            // Step 1 bis for Low correlated signals (law of summing of power, not of amplitude):
            // application of the square root to the whole sum
            if (inputCorrelationLevel == CorrelationLevel::Low)
                for (size_t j=subMatrixFirstCol ; j <= subMatrixLastColIndex ; j++)
                    colVolumes[j] = std::sqrt(colVolumes[j]);
            
            // 2nd step: sum of columns total volumes
            for (size_t j=subMatrixFirstCol ; j <= subMatrixLastColIndex ; j++)
            {
                if (outputCorrelationLevel == CorrelationLevel::High)
                    totalVolume += colVolumes[j];
                else if (outputCorrelationLevel == CorrelationLevel::Low)
                    totalVolume += (colVolumes[j] * colVolumes[j]);
            }
            // 2nd step bis for Low correlated outputs: square root of the sum
            if (outputCorrelationLevel == CorrelationLevel::Low)
                totalVolume = sqrt(totalVolume);
            
            return totalVolume;
        }
        
        
    };
    
}
