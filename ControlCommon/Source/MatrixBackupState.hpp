/*
  ==============================================================================

    MatrixBackupState.h
    Created: 31 Oct 2017 2:29:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "MatrixState.hpp"

#include "AudioUtils.hpp"
#include "InterpolationTypes.h"

namespace Miam
{
    /// \brief Routing matrix with a backup, to help
    /// compute the non-negligible difference between the backup matrix
    /// and the actual current internal matrix.
    template<typename T>
    class MatrixBackupState : public MatrixState<T>
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // La base : la seconde matrice creuse cachée à l'intérieur de l'état
        SparseMatrix<T, Miam_MaxNumInputs, Miam_MaxNumOutputs, Miam_MinVolume_PowOf10> backupMatrix;
        
        // Liste des indexes des coeffs de la matrice qui ont suffisamment changé.
        //
        // On va réserver la bonne taille pour le vecteur dès la construction, car :
        // C++2003 23.2.4.2/5 "It is guaranteed that no reallocation takes place during insertions
        // that happen after a call to reserve() until the time when an insertion would make the
        // size of the vector greater than the size specified in the most recent call to reserve()."
        std::vector< size_t > significantChangesIndexes;
        
        /// \brief The minimal distance between two T values, to consider them different
        /// DEVRA DEVENIR UN TABLEAU, ON AURA UNE DIFF MINIMALE SELON CHAQUE COEFFICIENT DE LA MATRICE
        /// DEVRA DEVENIR UN TABLEAU, ON AURA UNE DIFF MINIMALE SELON CHAQUE COEFFICIENT DE LA MATRICE
        /// DEVRA DEVENIR UN TABLEAU, ON AURA UNE DIFF MINIMALE SELON CHAQUE COEFFICIENT DE LA MATRICE
        /// DEVRA DEVENIR UN TABLEAU, ON AURA UNE DIFF MINIMALE SELON CHAQUE COEFFICIENT DE LA MATRICE
        /// DEVRA DEVENIR UN TABLEAU, ON AURA UNE DIFF MINIMALE SELON CHAQUE COEFFICIENT DE LA MATRICE
        T linearSignificantDifference;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        std::vector< size_t >& GetSignificantChangesIndexes()
        { return significantChangesIndexes; }

        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        
        MatrixBackupState()
        :
        MatrixState<T>()
        {
            significantChangesIndexes.reserve(Miam_MaxNumInputs * Miam_MaxNumOutputs);
            
            // précision max = celle des float pour l'OSC entre 0 et 1...
            linearSignificantDifference = (T) 0.000001;
            
            // Sécurité : on vérifie que la différence minimale ne soit pas zéro
            assert(linearSignificantDifference != ((T)0));
        }
        virtual ~MatrixBackupState() {}
        
        
        // - - - - - Recherche et transmission des données modifiées - - - - -
        public :
        
        /// \returns Les indexes 1d des cases de la matrice qui ont changé de manière significative
        /// par rapport à la matrice sauvegardée en interne.
        std::vector< size_t >& FindSignificantChanges(InterpolationType changeDetectionType)
        {
            // Vérification du type de changement de détection
            assert(InterpolationTypes::IsActualInterpolationType(changeDetectionType));
            
            // 0 : on n'efface pas les dernières modifs, tant qu'elles n'ont pas été utilisées....
            
            // 1 : pour tout coeff ACTUEL non-nul, on regarde si la différence
            // est négligeable avec le coeff SAUVEGARDÉ correspondant
            for (this->matrix.ResetIterator() ;
                 this->matrix.GetIterator() < this->matrix.GetEndIterator() ;
                 this->matrix.IncIterator())
            {
                T currentValue = this->matrix.GetIteratorValue();
                T backupValue = backupMatrix[this->matrix.GetIterator1dCoord()];
                bool isDifferenceSignificant = false;
                if (changeDetectionType == InterpolationType::Matrix_Linear)
                {
                    isDifferenceSignificant = (std::abs(currentValue - backupValue) > linearSignificantDifference);
                }
                else // currently : audio volume cases
                {
                /*(changeDetectionType == InterpolationType::Matrix_ConstantPower
                    || changeDetectionType == InterpolationType::Matrix_ConstantAmplitude)*/
                    isDifferenceSignificant = AudioUtils<T>::IsVolumeDifferenceSignificant(currentValue, backupValue);
                }
                
                if (isDifferenceSignificant)
                significantChangesIndexes.push_back(this->matrix.GetIterator1dCoord() );
            }
            
            // 2 : on cherche les zéros qui sont arrivés dans cette matrice actuelle, c-à-d :
            // pour coeff SAUVEGARDÉ non-nul, on regarde si le coeff ACTUEL n'est pas nul.
            // !!! pas besoin de re-checker la différence entre les deux, c'est déjà fait !
            // Seulement la nullité ou non des coeffs ACTUELS
            for (backupMatrix.ResetIterator() ;
                 backupMatrix.GetIterator() < backupMatrix.GetEndIterator() ;
                 backupMatrix.IncIterator())
            {
                // Si dans la matrice courant le coeff est négligeable (en gros : est zéro) alors
                // on doit notifier cet apparition d'un nouveau zéro
                if (this->matrix.IsNegligible( this->matrix[ backupMatrix.GetIterator1dCoord() ] ) )
                    significantChangesIndexes.push_back( backupMatrix.GetIterator1dCoord() );
            }
            
            return significantChangesIndexes;
        }
        
        void ApplyAndClearChangesList()
        {
            // Application des changements prévus
            for (size_t i=0 ; i<significantChangesIndexes.size() ; i++)
                backupMatrix.Set( significantChangesIndexes[i], this->matrix[significantChangesIndexes[i]] );
            
            // nettoyage
            significantChangesIndexes.clear();
        }
        
    };
}
