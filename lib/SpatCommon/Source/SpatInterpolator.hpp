/*
  ==============================================================================

    SpatInterpolator.hpp
    Created: 26 Nov 2016 12:03:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATINTERPOLATOR_HPP_INCLUDED
#define SPATINTERPOLATOR_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <memory>

#include "SpatType.h"

#include "SpatState.hpp"
#include "MatrixState.hpp"
#include "MatrixBackupState.hpp"
#include "MiamExceptions.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

namespace Miam
{
    class SpatModel;
    
    
    /// \brief Manages some Miam::SpatState children, and does the
    /// interpolation between some of these states when asked to do so.
    ///
    template<typename T>
    class SpatInterpolator
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        const SpatType spatType;
        std::vector< std::shared_ptr<SpatState<T>> > spatStates;
        
        MatrixBackupState<T> currentInterpolatedMatrixState;
        
        /// \brief Le nombre d'états mis à jour durant cette phase de mise à jour du Modèle
        int updatedStatesCount = 0;
        
        // may not be used... The balanced should be done on the DAW, or
        // on totalmix (or any software like that, that focuses on hardware
        // control)
        //OutputVolumesState speakersBalanceVolumes;
        
        
        //
        int inputsCount = 0;
        int outputsCount = 0;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        int GetInputsCount() {return inputsCount;}
        int GetOutputsCount() {return outputsCount;}
        
        SpatType GetSpatType() {return spatType;}
        
        size_t GetSpatStatesCount() {return spatStates.size();}
        std::shared_ptr<SpatState<T>> GetSpatState(size_t i) {return spatStates[i];}
        
        /// \brief Unoptimized access : copy-constructs a whole vector of shared pointers
        std::vector< std::shared_ptr<SpatState<T>> > GetSpatStates() {return spatStates;}
        
        /// \brief Optimized reference access.... OK pour toutes les situations ?
        ///
        /// En tout cas : pas const (quand on envoie un état comme ça, il va être modifié lui-même
        /// à l'intérieur par le SpatSender, et d'autres....)
        SpatState<T>& GetCurrentInterpolatedState()
        {return currentInterpolatedMatrixState;}
        
        /// \brief Inputs and Outputs config (applies changes to all states)
        void SetInputOutputChannelsCount(int _inputsCount, int _outputsCount)
        {
            // At first : update of internal data
            inputsCount = _inputsCount;
            outputsCount = _outputsCount;
            
            // Config transmission to the individual states
            currentInterpolatedMatrixState.SetInputOuputChannelsCount(inputsCount,outputsCount);
            for (size_t i=0 ; i<spatStates.size() ; i++)
            {
                // Dynamic cast of the state to a MatrixState only for now
                if (std::shared_ptr<MatrixState<T>> matrixState = std::dynamic_pointer_cast<MatrixState<T>>(spatStates[i]))
                {
                    matrixState->SetInputOuputChannelsCount(inputsCount,outputsCount);
                }
                // Else : behavior not implemented
                else
                    throw std::runtime_error("in/out channels count modification is not implemented yet for this spatialization state");
            }
        }

        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction and Destruction  - - - - -

        /// \brief The type of spatialization defines the class
        SpatInterpolator(SpatType _spatType) :
        spatType(_spatType)
        {}
        
        
        
        // - - - - - States management  - - - - -
        /// \brief Adds a pre-allocated and configured state at the end of
        /// the current state's list.
        void AddState(std::shared_ptr<SpatState<T>> newState)
        {
            newState->SetIndex((int)spatStates.size()); // is automatically the last
            spatStates.push_back(newState);
        }
        /// \brief Adds a default empty zero-matrix state.
        ///
        /// \returns A pointer to the newly created state
        std::shared_ptr<SpatState<T>> AddDefaultState()
        {
            auto emptyState = std::make_shared<MatrixState<T>>(); // default ctor
            AddState(emptyState);
            return emptyState;
        }
        /// \brief For debug/testing purposes
        void __AddDefaultStates()
        {
            for (size_t i = 0 ; i<3 ; i++)
            {
                std::shared_ptr<MatrixState<T>> newState = std::make_shared<MatrixState<T>>();
                newState->SetName("Matrice " + boost::lexical_cast<std::string>(i+1));
                AddState(newState);
            }
        }
        /// \brief Searches for the selected state (by index) then delete it.
        ///
        /// Triggers all necessary updates.
        void DeleteState(std::shared_ptr<SpatState<T>> stateToDelete)
        {
            auto statesIt = spatStates.begin();
            std::advance(statesIt, stateToDelete->GetIndex());
            // Actual unregistering and erase
            // returns an iterator the the next element (may be .end())
            (*statesIt)->UnregisterFromAreas();
            auto nextAfterErased = spatStates.erase(statesIt);
            // Indexes actualisation for remaining states after the one erased
            for (auto it=nextAfterErased ; it!=spatStates.end() ; it++)
                (*it)->SetIndex((*it)->GetIndex()-1); // minus 1 for everyone
        }
        void SwapStatesByIndex(size_t index1, size_t index2)
        {
            auto backUpPtr = spatStates[index1];
            spatStates[index1] = spatStates[index2];
            spatStates[index1]->SetIndex((int)index1);
            spatStates[index2] = backUpPtr;
            spatStates[index2]->SetIndex((int)index2);
        }
        
        
        
        // - - - - - Interpolation  - - - - -
        
        /// \brief Lorsque le Modèle commence à jouer
        void OnPlay()
        {
        }
        
        /// \brief Lorsque le Modèle reçoit un ordre de stop
        void OnStop()
        {
            /// On éteint tous les excitateurs de manière brutale
            for (auto&& spatState : spatStates)
                spatState->ClearExcitements();
            
            /// Le OnUpdateFinished devra être appelé par qq'un par contre, + tard
            updatedStatesCount = 1; // pour forcer une mise à jour... même s'il y en sûrement + que 1 qui a changé
        }
        
        /// \brief Permet de comptabiliser les modifs (ce que l'accès direct aux spatStates ne permet pas)
        void OnNewExcitementAmount(size_t spatStateIndex, uint64_t excitementSenderUID, double newExcitement)
        {
            spatStates[spatStateIndex]->OnNewExcitementAmount(excitementSenderUID, newExcitement);
            updatedStatesCount++;
        }
        /// \brief Fonction appelée lorsqu'on bien vidé les lock-free queues : on est prêt
        /// à calculer des trucs (si nécessaire)
        ///
        /// On calcule la nouvelle matrice résultante de l'interpolation, par exemple !
        ///
        /// \returns Si on a mis quelque chose à jour, ou pas du tout
        bool OnDataUpdateFinished()
        {
            if (updatedStatesCount >= 1)
            {
                // Addition des matrices creuses : on recalcule tout plutôt que d'essayer d'optimiser...
                currentInterpolatedMatrixState.ClearMatrix();
                // Pour l'instant : on ne peut ajouter que des matrices ! Pas d'autre état de spat
                for (size_t i=0 ; i<spatStates.size() ; i++)
                {
                    try {
                        MatrixState<T>& matrixState = dynamic_cast<MatrixState<T>&>( *(spatStates[i].get()) );
                        
                        // On applique bêtement les volumes de excitateurs : leur excitation totale
                        // répartie est de 1, et on suppose que chaque HP à 0dB donne la même valeur
                        // de pression acoustique au niveau de l'auditeur (on néglige les pbs de placement
                        // imparfaits, couleurs de HPs, pbs de phase, ...)
                        //
                        // On suppose en fait que la balance et le placement dans la salle sont bien
                        // pensés.
                        currentInterpolatedMatrixState.MultiplyAndAccumulate(matrixState, matrixState.GetExcitement());
                        
                        
                        if (matrixState.GetExcitement() < 0.0 || matrixState.GetExcitement() > 6.0)
                        {
                            std::cout << "- - - - - - - - - - - - - - - - - - -" << std::endl;
                            std::cout << "***** Valeur d'excitation anormale (=" << matrixState.GetExcitement() << " pour spat state idx=" << i << ")" << std::endl;
                            std::cout << "- - - - - - - - - - - - - - - - - - -" << std::endl;
                        }
                    }
                    catch (std::bad_cast& e) {
                        throw std::logic_error(std::string("Impossible pour l'instant de traiter autre chose que des états de spat matriciels : ") + e.what());
                    }
                }
                
                // On va chercher les différences dès maintenant
                currentInterpolatedMatrixState.FindSignificantChanges();
            }

            // On lance une nouvelle frame
            bool somethingWasUpdated = (updatedStatesCount > 0);
            updatedStatesCount = 0;
            return somethingWasUpdated;
        }
        
        
        
        // = = = = = = = = Property tree (for XML) import/export = = = = = = = =
        /// \brief Formats configuration data managed by the interpolator
        /// in a property tree.
        ///
        /// May also someday return the name of ins/outs, ....
        std::shared_ptr<bptree::ptree> GetConfigurationTree()
        {
            auto confTree = std::make_shared<bptree::ptree>();
            confTree->put("inputs.<xmlattr>.count", inputsCount);
            confTree->put("outputs.<xmlattr>.count", outputsCount);
            return confTree;
        }
        void SetConfigurationFromTree(bptree::ptree& tree)
        {
            inputsCount = tree.get<int>("inputs.<xmlattr>.count");
            outputsCount = tree.get<int>("outputs.<xmlattr>.count");
        }

        /// \brief Returns the property tree describing the spat states' data
        ///
        /// This sub-tree does not know its own "master name tag" = <spat>,
        /// but only knows its <states> tag and its <state> children
        std::shared_ptr<bptree::ptree> GetSpatStatesTree()
        {
            bptree::ptree statesInnerTree;
            for (size_t i=0 ; i<spatStates.size() ; i++)
            {
                // Children sub-trees written within a <state> tag
                auto stateTree = spatStates[i]->GetTree();
                stateTree->put("<xmlattr>.index", i);
                statesInnerTree.add_child("state", *stateTree);
            }
            
            if (spatStates.size() == 0)
                std::cout << "***Attention*** : [Spat Interpolator] : ZERO état de spatialisation ne sera exporté." << std::endl;
            
            auto statesTree = std::make_shared<bptree::ptree>();
            statesTree->add_child("states", statesInnerTree);
            return statesTree;
        }
        /// \brief Exact inverse process of GetSpatStatesTree()
        ///
        /// If an empty ptree is given : just triggers a reset of the module.
        void SetSpatStatesFromTree(bptree::ptree & spatStatesTree)
        {
            // At first : reinit
            spatStates.clear();
            // If it is an empty tree : it was a command for a reinit...
            if (spatStatesTree.empty())
                return;
            
            // Récupération du noeud qui contiendra tous les états séparément
            bptree::ptree statesNode;
            try {
                statesNode = spatStatesTree.get_child("states");
            }
            catch (bptree::ptree_error& e) {
                throw XmlReadException(std::string("Inside <spat> node : ") + e.what());
            }
            
            // State tree est une sorte d'itérateur
            // Attribut ".first" contient le contenu du tag XML concerné
            // Attribut ".second" contient le ptree = les enfants du tag XML concerné
            for(auto &stateTree : statesNode)
            {
                try {
                     // DEVRAIT DÉPENDRE DU TYPE DE SPAT
                     // DEVRAIT DÉPENDRE DU TYPE DE SPAT
                     // DEVRAIT DÉPENDRE DU TYPE DE SPAT
                     // DEVRAIT DÉPENDRE DU TYPE DE SPAT
                     // DEVRAIT DÉPENDRE DU TYPE DE SPAT
                    auto lastMatrix = std::make_shared<MatrixState<T>>();
                    /// À optimiser (refait l'initialisation de la construction
                    /// de la SparseMatrix interne...)
                    lastMatrix->SetFromTree(stateTree.second);
                    // At the very end (no xml loading exceptions thrown)
                    AddState(lastMatrix);
                }
                catch (XmlReadException& e) {
                    throw XmlReadException("node <states>: ", e);
                }
            }
            
            // Actualisation forcée
            SetInputOutputChannelsCount(inputsCount, outputsCount);
        }
    };
    

}


#endif  // SPATINTERPOLATOR_HPP_INCLUDED
