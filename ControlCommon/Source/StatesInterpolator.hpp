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
#include <atomic>

#include "InterpolationTypes.h"

#include "ControlState.hpp"
#include "MatrixState.hpp"
#include "MatrixBackupState.hpp"
#include "MiamExceptions.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "boost/lexical_cast.hpp"

#include "AudioUtils.hpp"
#include "TextUtils.h"


namespace Miam
{
    class ControlModel;
    
    
    /// \brief Manages some Miam::ControlState children, and does the
    /// interpolation between some of these states when asked to do so.
    ///
    template<typename T>
    class StatesInterpolator
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        InterpolationType interpolationType;
        std::atomic<int> lockFreeInterpolationType;
        
        std::vector< std::shared_ptr<ControlState<T>> > states;
        
        std::unique_ptr<MatrixBackupState<T>> currentInterpolatedMatrixState;
        
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
        void SetInOutChannelsName(InOutChannelsName& channelsName_) { channelsName = channelsName_; }
        
        InterpolationType GetType() {return interpolationType;}
        InterpolationType GetType_Atomic() {
            int typeCopyAsInt = (int)lockFreeInterpolationType;
            InterpolationType typeCopy = (InterpolationType)typeCopyAsInt;
            return typeCopy;
        }
        
        size_t GetStatesCount() {return states.size();}
        std::shared_ptr<ControlState<T>> GetState(size_t i) {return states[i];}
        
        /// \brief Unoptimized access : copy-constructs a whole vector of shared pointers
        std::vector< std::shared_ptr<ControlState<T>> > GetStates() {return states;}
        
        /// \brief Optimized reference access.... OK pour toutes les situations ?
        ///
        /// En tout cas : pas const (quand on envoie un état comme ça, il va être modifié lui-même
        /// à l'intérieur par le Miam::ControlStateSender, et d'autres....)
        ControlState<T>& GetCurrentInterpolatedState()
        {return *(currentInterpolatedMatrixState.get());}
        
        /// \brief Inputs and Outputs config (applies changes to all states)
        void SetInputOutputChannelsCount(int _inputsCount, int _outputsCount)
        {
            // At first : update of internal data
            inputsCount = _inputsCount;
            outputsCount = _outputsCount;
            
            // Config transmission to the individual states
            currentInterpolatedMatrixState->SetInputOuputChannelsCount(inputsCount,outputsCount);
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
        StatesInterpolator(InterpolationType _interpolationType)
        {
            reinitConfiguration();
            ReinitInterpolation(_interpolationType);
        }
        
        void ReinitInterpolation(InterpolationType newType)
        {
            interpolationType = newType;
            lockFreeInterpolationType = (int) interpolationType;

            currentInterpolatedMatrixState.reset(new MatrixBackupState<T>());
        }
        private :
        void reinitConfiguration()
        {
            channelsName.Inputs.clear();
            channelsName.Outputs.clear();
            channelsName.Inputs.resize(Miam_MaxNumInputs);
            channelsName.Outputs.resize(Miam_MaxNumOutputs);
        }
        public :
        
        
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
                currentInterpolatedMatrixState->ClearMatrix();
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
                        currentInterpolatedMatrixState->MultiplyAndAccumulate(matrixState, matrixState.GetExcitement());
                    }
                    catch (std::bad_cast& e) {
                        throw std::logic_error(std::string("Impossible pour l'instant de traiter autre chose que des états matriciels : ") + e.what());
                    }
                }
                
                // On va chercher les différences dès maintenant
                currentInterpolatedMatrixState->FindSignificantChanges();
            }

            // On lance une nouvelle frame
            bool somethingWasUpdated = (updatedStatesCount > 0);
            updatedStatesCount = 0;
            return somethingWasUpdated;
        }
        
        
        
        
        
        
        
        
        
        
        
        
        // = = = = = = = = = = = = = = = Property tree (for XML) import/export = = = = = = = = = = = = = = =
        
        
        /// \brief Formats configuration data managed by the interpolator
        /// in a property tree: number and names of in/outs, ...
        std::shared_ptr<bptree::ptree> GetConfigurationTree()
        {
            auto confTree = std::make_shared<bptree::ptree>();
            // Interpolator type
            confTree->put("interpolation.<xmlattr>.type", InterpolationTypes::GetInterpolationName(interpolationType, false, true).toStdString());
            // Inputs (counts and names)
            bptree::ptree inputsInnerTree;
            inputsInnerTree.put("<xmlattr>.activeCount", inputsCount);
            inputsInnerTree.put("<xmlattr>.maxCount", Miam_MaxNumInputs);
            for (size_t i=0 ; i<channelsName.Inputs.size() ; i++)
                if (! channelsName.Inputs[i].empty())
                {
                    bptree::ptree nameInnerTree;
                    nameInnerTree.put("<xmlattr>.id", i);
                    nameInnerTree.put_value(channelsName.Inputs[i]);
                    inputsInnerTree.add_child("name", nameInnerTree);
                }
            confTree->add_child("inputs", inputsInnerTree);
            // Outputs (counts and names)
            bptree::ptree outputsInnerTree;
            outputsInnerTree.put("<xmlattr>.activeCount", outputsCount);
            outputsInnerTree.put("<xmlattr>.maxCount", Miam_MaxNumOutputs);
            for (size_t i=0 ; i<channelsName.Outputs.size() ; i++)
                if (! channelsName.Outputs[i].empty())
                {
                    bptree::ptree nameInnerTree;
                    nameInnerTree.put("<xmlattr>.id", i);
                    nameInnerTree.put_value(channelsName.Outputs[i]);
                    outputsInnerTree.add_child("name", nameInnerTree);
                }
            confTree->add_child("outputs", outputsInnerTree);
            
            return confTree;
        }
        /// \brief Sets the configuration from XML tree
        void SetConfigurationFromTree(bptree::ptree& tree)
        {
            reinitConfiguration();
            
            // - - - Interpolation type - - -
            try {
                std::string typeString = tree.get<std::string>("interpolation.<xmlattr>.type");
                InterpolationType parsedInterpolationType = InterpolationTypes::ParseName(typeString);
                ReinitInterpolation(parsedInterpolationType);
            }
            catch (bptree::ptree_error& e) {
                throw XmlReadException(std::string("Cannot read the 'type' attribute from the <interpolation> tag: ") + e.what());
            }
            catch (ParseException& e) {
                throw XmlReadException(std::string("Cannot parse the 'type' attribute from the <interpolation> tag: ") + e.what());
            }
            // - - - Inputs - - -
            try {
                inputsCount = tree.get<int>("inputs.<xmlattr>.activeCount");
            }
            catch (bptree::ptree_error &e) {
                throw XmlReadException(std::string("<inputs> node : cannot read the 'activeCount' XML attribute: ") + e.what());
            }
            // Lecture de tous les noeuds enfant, on ne prendra que les noms.
            // Pas de try car si on a lu le compte, le noeud inputs existe forcément (et a le droit d'etre vide)
            for (auto& inputChild : tree.get_child("inputs"))
            {
                if (inputChild.first == "name")
                {
                    try {
                        size_t nameId = inputChild.second.get<size_t>("<xmlattr>.id");
                        if (nameId >= Miam_MaxNumInputs)
                            throw bptree::ptree_error("given id (" + boost::lexical_cast<std::string>(nameId) + ") is bigger than the maximum value (" + boost::lexical_cast<std::string>(Miam_MaxNumInputs) + ")");
                        else
                            channelsName.Inputs[nameId] = inputChild.second.get_value<std::string>();
                    }
                    catch (bptree::ptree_error& e) {
#if defined(__MIAM_DEBUG)
                        throw XmlReadException(std::string("All <names> inside <inputs> must have a valid id XML attribute: ") + e.what());
#endif
                    }
                }
            }
            // - - - Ouputs - - -
            try {
                outputsCount = tree.get<int>("outputs.<xmlattr>.activeCount");
            }
            catch (bptree::ptree_error &e) {
                throw XmlReadException(std::string("<outputs> node : cannot read the 'activeCount' XML attribute") + e.what());
            }
            // Lecture de tous les noeuds enfant, on ne prendra que les noms.
            // Pas de try car si on a lu le compte, le noeud inputs existe forcément (et a le droit d'etre vide)
            for (auto& outputChild : tree.get_child("outputs"))
            {
                if (outputChild.first == "name")
                {
                    try {
                        size_t nameId = outputChild.second.get<size_t>("<xmlattr>.id");
                        if (nameId >= Miam_MaxNumOutputs)
                            throw bptree::ptree_error("given id (" + boost::lexical_cast<std::string>(nameId) + ") is bigger than the maximum value (" + boost::lexical_cast<std::string>(Miam_MaxNumOutputs) + ")");
                        else
                            channelsName.Outputs[nameId] = outputChild.second.get_value<std::string>();
                    }
                    catch (bptree::ptree_error& e) {
#if defined(__MIAM_DEBUG)
                        throw XmlReadException(std::string("All <names> inside <outputs> must have a valid id XML attribute: ") + e.what());
#endif
                    }
                }
            }
        }

        /// \brief Returns the property tree describing the states' data
        ///
        /// This sub-tree does not know its own "master name tag" = <control>,
        /// but only knows its <states> tag and its <state> children
        std::shared_ptr<bptree::ptree> GetStatesTree()
        {
#ifdef __MIAM_DEBUG
            if (states.size() == 0)
            {
                std::cout << "----------DEBUG----------- Plus aucun état enregistré !! ----------DEBUG----------- " << std::endl;
                throw std::logic_error("Plus aucun état enregistré !!");
            }
#endif
            
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
