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
#include "BasicInterpolationCurve.hpp"

namespace Miam
{
    /// \brief Routing matrix with a backup, to help
    /// compute the non-negligible difference between the backup matrix
    /// and the actual current internal matrix.
    /// This class also manages the interpolation curves applied independantly
    /// on parameters.
    ///
    /// The interpolation curves are managed here for convenience, but also because
    /// they are needed to accurately compute whether a difference is negligible or not.
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
        T linearSignificantDifference;
        /// -----> ou alors la différence sera calculée par chaque courbe d'interpolation ? plus
        ///        logique en réalité....
        
        /// \brief Interpolation curves applied to the inputs of the matrix. Won't work properly
        /// with the SPAT version of the Player App.
        std::vector<BasicInterpolationCurve<T>> interpolationCurves;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        /// \brief Renvoi d'une copie des courbes d'interpolation
        std::shared_ptr<std::vector<BasicInterpolationCurve<T>>> GetInterpolationCurves()
        {
            return
            std::make_shared<std::vector<BasicInterpolationCurve<T>>>( interpolationCurves );
        }
        /// \brief Copie les courbes d'interpolation transmises, dans les attributs internes.
        void SetInterpolationCurves(std::shared_ptr<std::vector<BasicInterpolationCurve<T>>> curves)
        {
            // constructeur de copie du vecteur, copie tous les éléments par constructeur de copie
            // par défaut
            interpolationCurves = *(curves.get());
        }
        
        std::vector< size_t >& GetSignificantChangesIndexes()
        { return significantChangesIndexes; }
        
        T GetCurveInterpolatedMatrixCoeff(size_t row, size_t col)
        {
            // on doit re-préciser de quelle classe mère venait l'attribut
            const T normalizedValue = MatrixState<T>::matrix(row, col);
            const T interpolatedValue = interpolationCurves[row].InterpolateValue( normalizedValue );
            return interpolatedValue;
        }

        
        
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
            // TEMP sinon Reaper lâche complètement !
#ifdef __MIEM_EXPERIMENTS
            linearSignificantDifference = (T) 0.02;
#endif
            
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
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
                if (changeDetectionType == InterpolationType::Matrix_Linear)
                {
                    isDifferenceSignificant = (std::abs(currentValue - backupValue) > linearSignificantDifference);
                }
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
                // ---- DETECTION A FAIRE SELON INTERPOLATION -----
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
        
        
        
        
        
        
        
        
        // - - - - - XML - Boost Property Trees import/export - - - - -
        std::shared_ptr<bptree::ptree> GetCurvesTree()
        {
            auto curvesInnerTree = std::make_shared<bptree::ptree>();
            
            for (size_t i=0; i < interpolationCurves.size() ; i++)
            {
                // on écrit uniquement les courbes non-défaut
                if (! interpolationCurves[i].IsDefault())
                {
                    auto curveGeneratedTree = interpolationCurves[i].GetTree(); // does not know its index
                    curveGeneratedTree->put("<xmlattr>.index", i);
                    curvesInnerTree->add_child("curve", *curveGeneratedTree);
                }
            }
            
            return curvesInnerTree;
        }
        
        void SetCurvesFromTree(bptree::ptree& curvesTree)
        {
            // Passe 1 = D'abord on init toutes les curves à la valeur par défaut...
            interpolationCurves.clear();
            for (size_t i = 0 ; i<Miam_MaxNumInputs ; i++)
                interpolationCurves.push_back(BasicInterpolationCurve<T>::GetDefault());
            
            // Passe 2 = Lecture de tous les noeuds enfant, pour trouver les valeurs non-défaut
            // Pas de try car si on a lu le compte, le noeud inputs existe forcément (et a le droit d'etre vide)
            for (auto it = curvesTree.begin() ; it != curvesTree.end() ; it++)
            {
                if (it->first == "curve")
                {
                    try {
                        size_t curveIndex = it->second.get<size_t>("<xmlattr>.index");
                        if (curveIndex >= Miam_MaxNumInputs)
                            throw bptree::ptree_error("given index (" + boost::lexical_cast<std::string>(curveIndex) + ") is bigger than the maximum value (" + boost::lexical_cast<std::string>(Miam_MaxNumInputs) + ")");
                        
                        // Si tout est bon, on continue de lire les attributs (la basic curve de débrouille avec)
                        else
                            interpolationCurves[curveIndex].SetFromTree(it->second);
                    }
                    catch (bptree::ptree_error& e) {
                        throw XmlReadException(std::string("All <curve> tags inside <curves> must have a valid index XML attribute: ") + e.what());
                    }
                }
                else
                {
                    throw XmlReadException(std::string("All children inside <interpolation>.<curves> must be named <curve>"));
                }
            }
        }
        
        
        
    };
}
