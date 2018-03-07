/*
  ==============================================================================

    StatesInterpolator.hpp
    Created: 26 Nov 2016 12:03:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef STATESINTERPOLATOR_HPP_INCLUDED
#define STATESINTERPOLATOR_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <memory>

#include "ControlState.hpp"
#include "MatrixState.hpp"
#include "MatrixBackupState.hpp"
#include "MiamExceptions.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "AudioUtils.hpp"


namespace Miam
{
    class ControlModel;
    
    
    
    /// \brief Describes the current method
    enum class InterpolationType {
        
        None, ///< Interpolator not configured yet
        
        Matrix_LinearInterpolation, ///< états matriciels, avec interpolation linéaire basique, coefficient par coefficient
        Matrix_ConstantVolumeInterpolation, ///< Interpolation à conservation de volume (si transitions entre matrices de même volume total)
        
    };
    
    
    
    /// \brief Manages some Miam::ControlState children, and does the
    /// interpolation between some of these states when asked to do so.
    ///
    template<typename T>
    class StatesInterpolator
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        const InterpolationType interpolationType;
        std::vector< std::shared_ptr<ControlState<T>> > states;
        
        MatrixBackupState<T> currentInterpolatedMatrixState;
        
        /// \brief Le nombre d'états mis à jour durant cette phase de mise à jour du Modèle
        int updatedStatesCount = 0;
        
        
        
        // E/S
        int inputsCount = 0;
        int outputsCount = 0;
        // Pour l'instant, on crée un maximum de noms de canaux... Quitte à ce que beaucoup soient vides
        // (on fait comme les coefficients de la matrice, en fait...)
        InOutChannelsName channelsName;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        int GetInputsCount() {return inputsCount;}
        int GetOutputsCount() {return outputsCount;}
        /// \brief Copie de toutes les données
        InOutChannelsName GetInOutChannelsName() {return channelsName;}
        /// \brief Va copier les nouvelles données en gardant les noms des canaux non-précisés
        void SetInOutChannelsName(InOutChannelsName& channelsName_) {channelsName = channelsName_;}
        
        InterpolationType GetType() {return interpolationType;}
        
        size_t GetStatesCount() {return states.size();}
        std::shared_ptr<ControlState<T>> GetState(size_t i) {return states[i];}
        
        /// \brief Unoptimized access : copy-constructs a whole vector of shared pointers
        std::vector< std::shared_ptr<ControlState<T>> > GetStates() {return states;}
        
        /// \brief Optimized reference access.... OK pour toutes les situations ?
        ///
        /// En tout cas : pas const (quand on envoie un état comme ça, il va être modifié lui-même
        /// à l'intérieur par le Miam::ControlStateSender, et d'autres....)
        ControlState<T>& GetCurrentInterpolatedState()
        {return currentInterpolatedMatrixState;}
        
        /// \brief Inputs and Outputs config (applies changes to all states)
        void SetInputOutputChannelsCount(int _inputsCount, int _outputsCount)
        {
            // At first : update of internal data
            inputsCount = _inputsCount;
            outputsCount = _outputsCount;
            
            // Config transmission to the individual states
            currentInterpolatedMatrixState.SetInputOuputChannelsCount(inputsCount,outputsCount);
            for (size_t i=0 ; i<states.size() ; i++)
            {
                // Dynamic cast of the state to a MatrixState only for now
                if (std::shared_ptr<MatrixState<T>> matrixState = std::dynamic_pointer_cast<MatrixState<T>>(states[i]))
                {
                    matrixState->SetInputOuputChannelsCount(inputsCount,outputsCount);
                }
                // Else : behavior not implemented
                else
                    throw std::runtime_error("in/out channels count modification is not implemented yet for this control state");
            }
        }

        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction and Destruction  - - - - -

        /// \brief The type of interpolation defines the class
        StatesInterpolator(InterpolationType _interpolationType) :
        interpolationType(_interpolationType)
        {
            channelsName.Inputs.resize(Miam_MaxNumInputs);
            for (size_t i=0 ; i<channelsName.Inputs.size() ; i++)
                channelsName.Inputs[i] = "interpolator input channel " + boost::lexical_cast<std::string>(i+1);
            
            channelsName.Outputs.resize(Miam_MaxNumOutputs);
            for (size_t i=0 ; i<channelsName.Outputs.size() ; i++)
                channelsName.Outputs[i] = "interpolator output channel " + boost::lexical_cast<std::string>(i+1);
        }
        
        
        
        // - - - - - States management  - - - - -
        /// \brief Adds a pre-allocated and configured state at the end of
        /// the current state's list.
        void AddState(std::shared_ptr<ControlState<T>> newState)
        {
            newState->SetIndex((int)states.size()); // is automatically the last
            states.push_back(newState);
        }
        /// \brief Adds a default empty zero-matrix state.
        ///
        /// \returns A pointer to the newly created state
        std::shared_ptr<ControlState<T>> AddDefaultState()
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
        void DeleteState(std::shared_ptr<ControlState<T>> stateToDelete)
        {
            auto statesIt = states.begin();
            std::advance(statesIt, stateToDelete->GetIndex());
            // Actual unregistering and erase
            // returns an iterator the the next element (may be .end())
            (*statesIt)->UnregisterFromAreas();
            auto nextAfterErased = states.erase(statesIt);
            // Indexes actualisation for remaining states after the one erased
            for (auto it=nextAfterErased ; it!=states.end() ; it++)
                (*it)->SetIndex((*it)->GetIndex()-1); // minus 1 for everyone
        }
        void SwapStatesByIndex(size_t index1, size_t index2)
        {
            auto backUpPtr = states[index1];
            states[index1] = states[index2];
            states[index1]->SetIndex((int)index1);
            states[index2] = backUpPtr;
            states[index2]->SetIndex((int)index2);
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
            for (auto&& state : states)
                state->ClearExcitements();
            
            /// Le OnUpdateFinished devra être appelé par qq'un par contre, + tard
            updatedStatesCount = 1; // pour forcer une mise à jour... même s'il y en sûrement + que 1 qui a changé
        }
        
        /// \brief Permet de comptabiliser les modifs (ce que l'accès direct aux états ne permet pas)
        void OnNewExcitementAmount(size_t stateIndex, uint64_t excitementSenderUID, double newExcitement)
        {
            states[stateIndex]->OnNewExcitementAmount(excitementSenderUID, newExcitement);
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
                // Pour l'instant : on ne peut ajouter que des matrices ! Pas d'autre état
                for (size_t i=0 ; i<states.size() ; i++)
                {
                    try {
                        MatrixState<T>& matrixState = dynamic_cast<MatrixState<T>&>( *(states[i].get()) );
                        
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
                            std::cout << "***** Valeur d'excitation anormale (=" << matrixState.GetExcitement() << " pour state idx=" << i << ")" << std::endl;
                            std::cout << "- - - - - - - - - - - - - - - - - - -" << std::endl;
                        }
                    }
                    catch (std::bad_cast& e) {
                        throw std::logic_error(std::string("Impossible pour l'instant de traiter autre chose que des états matriciels : ") + e.what());
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

        /// \brief Returns the property tree describing the states' data
        ///
        /// This sub-tree does not know its own "master name tag" = <control>,
        /// but only knows its <states> tag and its <state> children
        std::shared_ptr<bptree::ptree> GetStatesTree()
        {
            bptree::ptree statesInnerTree;
            for (size_t i=0 ; i<states.size() ; i++)
            {
                // Children sub-trees written within a <state> tag
                auto stateTree = states[i]->GetTree();
                stateTree->put("<xmlattr>.index", i);
                statesInnerTree.add_child("state", *stateTree);
            }
            
            if (states.size() == 0)
                std::cout << "***Attention*** : [Interpolator] : ZERO état de contrôle ne sera exporté." << std::endl;
            
            auto statesTree = std::make_shared<bptree::ptree>();
            statesTree->add_child("states", statesInnerTree);
            return statesTree;
        }
        /// \brief Exact inverse process of GetStatesTree()
        ///
        /// If an empty ptree is given : just triggers a reset of the module.
        void SetStatesFromTree(bptree::ptree & statesTree)
        {
            // At first : reinit
            states.clear();
            // If it is an empty tree : it was a command for a reinit...
            if (statesTree.empty())
                return;
            
            // Récupération du noeud qui contiendra tous les états séparément
            bptree::ptree statesNode;
            try {
                statesNode = statesTree.get_child("states");
            }
            catch (bptree::ptree_error& e) {
                throw XmlReadException(std::string("Inside <control> node : ") + e.what());
            }
            
            // State tree est une sorte d'itérateur
            // Attribut ".first" contient le contenu du tag XML concerné
            // Attribut ".second" contient le ptree = les enfants du tag XML concerné
            for(auto &stateTree : statesNode)
            {
                try {
                    // DEVRAIT DÉPENDRE DU TYPE DE CONTROLE
                    // DEVRAIT DÉPENDRE DU TYPE DE CONTROLE
                    // DEVRAIT DÉPENDRE DU TYPE DE CONTROLE
                    // DEVRAIT DÉPENDRE DU TYPE DE CONTROLE
                    // DEVRAIT DÉPENDRE DU TYPE DE CONTROLE
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


#endif  // STATESINTERPOLATOR_HPP_INCLUDED
