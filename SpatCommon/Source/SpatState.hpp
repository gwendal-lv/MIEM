/*
  ==============================================================================

    SpatState.h
    Created: 26 Nov 2016 12:00:26pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSTATE_H_INCLUDED
#define SPATSTATE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <numeric> // std::accumulate

#include "SparseMatrix.hpp"
#include "AudioUtils.hpp"

#include "SpatArea.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

namespace Miam
{
    // Forward declarations
    class SpatArea;
    
    
    /// \brief An abstract class that represents a state of the
    /// speakers' system, that represents a spatialisation state
    /// (or spatialisation effect)
    ///
    /// T is a floating-point type. It might be an int/fixed-point
    /// type some day in the future....
    ///
    /// The number of channels might not be exactly the same as
    /// the parent Miam::SpatInterpolator (and this has to be
    /// properly managed)
    ///
    /// !!! Attention aux fonctions membres virtuelles qui ne peuvent alors
    /// pas être template !!! -> encore vrai sur tous les compilateurs ?
    template<typename T>
    class SpatState
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        /// \brief Pour éviter des boucles de suppression (les aires re-notifient quand on se délie d'elles)
        bool unlinkingFromAreas = false;
        
        
        protected :
        
        // Own attributes
        std::string name;
        
        // For convenience and optimizations...
        int index;
        
        
        /// \brief List of areas that represent this spatialization state
        ///
        /// Not the most optimal STL container (research is not optimized)
        ///
        /// \remark Pour accès synchrone aux aires graphiques ! Donc pour édition offline seulement...
        /// On mode de jeu, on devra travailler avec les UIDs qui viennt des paquets lock-free
        std::vector< std::weak_ptr<SpatArea> > linkedAreas;
        
        /// \brief Excitations en cours, issues d'objets graphiques quelconques référencés par leur UID
        std::map< uint64_t, T > areaToExcitement;
        
        /// Copie Thread-safe d'une donnée qu'on pourrait retrouver depuis les linkedAreas
        T excitement;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        /// \returns A string containing the Id (counting from 1) and the name
        /// concatenated
        virtual std::string GetName(bool withId = true) const
        {
            std::string indexString;
            if (withId)
                indexString += boost::lexical_cast<std::string>(index+1) + ". ";
            return indexString + name;
        }
        virtual void SetName(std::string _name)
        {
            name = _name;
            for (auto it = linkedAreas.begin() ; it!=linkedAreas.end() ; it++)
                (*it).lock()->OnSpatStateNameChanged();
        }
        
        void SetIndex(int newIndex) {index = newIndex;}
        int GetIndex() const {return index;}
        
        virtual size_t GetOutputsCount() = 0;
        
        virtual size_t GetLinkedAreasCount() {return linkedAreas.size();}
        virtual std::shared_ptr<SpatArea> GetLinkedArea(size_t i)
        {
            if(auto area = linkedAreas[i].lock())
                return area;
            else
                throw std::runtime_error("Spat state is linked to a non-existant area");
        }
        
        T GetExcitement() const {return excitement;}
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        SpatState() : index(-1)
        {
            // pas possible de le mettre dans la liste d'initialisation
            // car le double ne prend pas (sous XCode) un zéro de type 'void'
            excitement = {};
        }
        
        
        // - - - - - Construction / destruction - - - - -
        virtual ~SpatState()
        {
            // Called when this spat state is unregistered from all areas that
            // kept a shared_ptr to this
        }

        
        // - - - - - Internal linking to spat areas - - - - -
        /// \brief Ordre reçu d'une aire
        void LinkToArea(std::shared_ptr<SpatArea> spatArea)
        {
            // no check for double addition (but it's weak ptrs...)
            linkedAreas.push_back(spatArea);
        }
        /// \brief Ordre reçu d'une aire
        void UnlinkToArea(std::shared_ptr<SpatArea> spatArea)
        {
            if (! unlinkingFromAreas)
            {
                for (auto it = linkedAreas.begin() ; it!=linkedAreas.end() ; it++)
                {
                    auto curAreaPtr = (*it).lock();
                    if (curAreaPtr == spatArea)
                    {
                        linkedAreas.erase(it);
                        break;
                    }
                }
            }
        }
        
        // - - - - - External linking to spat areas - - - - -
        void UnregisterFromAreas()
        {
            unlinkingFromAreas = true;
            
            // does not need to internally unlink from linked areas : the link is a weak_ptr
            // But still needs to unregister from linked areas
            for (size_t i=0 ; i < linkedAreas.size() ; i++)
            {
                auto areaPtr = linkedAreas[i].lock();
                if (areaPtr)
                    areaPtr->LinkToSpatState(nullptr);
            }
            
            unlinkingFromAreas = false;
        }
        
        
        // - - - - - Interpolation computation - - - - -
        
        /// \brief Sauvegarde de l'excitation qui vient d'arriver, en utilisant/actualisation
        /// la map qui permet d'associer des UIDs à des excitations
        virtual void OnNewExcitementAmount(uint64_t senderUID, T newExcitement)
        {
            // Cas 1 : élement existe
            try {
                double& concernedExcitement = areaToExcitement.at(senderUID); // may throw except
                // Cas 1.a : suppression d'un elmt existant, si excitation trop faible
                if (AudioUtils<T>::IsVolumeNegligible(newExcitement))
                    areaToExcitement.erase(senderUID);
                // Cas 1.b : simpe mise à jour d'un elmt existant
                else
                    concernedExcitement = newExcitement;
            }
            // Cas 2 : insertion d'un nouvel elmt dans la map
            catch (std::out_of_range & /*e*/) { // si on a pas trouvé la clé dans la map
                 // on teste quand même si pas trop faible, avant d'insérer
                if ( ! AudioUtils<T>::IsVolumeNegligible(newExcitement) )
                    areaToExcitement[senderUID] = newExcitement; // insertion automatique via []
            }
            
            // à la fin : update direct de la somme (accumulate ne fonctionne pas sur des elts de type map)
            excitement = {};
            for ( auto&& mapPair : areaToExcitement)
                excitement += mapPair.second;
        }
        
        
        
        // - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -
        virtual void AddOutput() = 0;
        virtual void DeleteOutput(size_t i) = 0;
        virtual void SwapOutputs(size_t i, size_t j) = 0;
        
        // - - - - - Property tree (for XML) import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree()
        {
            auto pTree = std::make_shared<bptree::ptree>();
            // This sub-tree does not know its own "master name tag" = <state>
            pTree->put("name", GetName(false));
            return pTree;
        }
        /// \brief Receives all the children nodes and contents inside a <state> tag,
        /// but not the <state> and </state> tags themselves.
        virtual void SetFromTree(bptree::ptree & stateTree)
        {
            // Common attributes = name only, for now
            // With default value to avoid try/catch block
            // And automatic data type identifying (= default value type)
            SetName( stateTree.get("name", std::string("unnamed")) );
        }
        
    };
    
}


#endif  // SPATSTATE_H_INCLUDED
