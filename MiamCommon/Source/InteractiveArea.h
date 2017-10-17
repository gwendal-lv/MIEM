/*
  ==============================================================================

    InteractiveArea.h
    Created: 13 Apr 2016 3:56:27pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef INTERACTIVEAREA_H_INCLUDED
#define INTERACTIVEAREA_H_INCLUDED

#include "JuceHeader.h"

#include "IInteractiveArea.h"



namespace Miam
{
    
    /// \brief Abstract class that defines an interface for all areas that can have an interaction
    /// with a user (via mouse, touch, MIDI events...)
    class InteractiveArea : public virtual IInteractiveArea,
                            public std::enable_shared_from_this<InteractiveArea>
    {
        
        
        
        
        // Common attributes
        protected :
        
        // Pour des questions d'optimisation : on garde tous les excitateurs actuellement en interaction
        // avec cette aire.
        // Comme ça, quand un excitateur interagit avec l'aire, on peut récupérer vite fait
        // les poids d'interaction avec les autres aires.
        //
        // Lien dans le sens inverse pas nécessaire : quand un excitateur bouge,
        // il re-checke son interaction avec toutes les aires graphiques.... (sinon il faudrait
        // découper le canevas en sous-parties pour ne pas tout re-tester, etc...)
        //
        // On ne pas utiliser facilement de map
        // car weak_ptr ne permet pas la comparaison sans .lock() ...
        // Et  vu la taille max du conteneur (10aine d'excitateurs max par aire...) on prend juste
        // un vecteur (accès direct random) plutôt qu'une liste (accès par itérateur)
        std::vector< std::weak_ptr<Exciter> > excitersInteractingWithThis;
        std::vector< double > excitersWeights;
        
        
        
    protected:
        double surface;
        
        
        
        
        
        
        // - - - - - Construction/destruction - - - - -
        public :
        virtual ~InteractiveArea();
        
        
        // - - - - - Interaction avec Excitateurs - - - - -
        
        /// \brief Internally updates the interaction weight with a given exciter.
        ///
        /// \returns Event containing at least if the interaction between this
        /// area and an exciter has changed, or not.
        std::shared_ptr<AreaEvent> UpdateInteraction(std::shared_ptr<Exciter>& exciter) override;
        
        /// \brief Retourne la somme (sans saturation) de l'excitation cumulée
        /// de tous les excitateurs interagissant actuellement avec la forme.
        double GetTotalExcitementAmount() const override;
        
        protected :
        void updateOpacityFromExcitement();
        
        
    };
    
    
}



#endif  // INTERACTIVEAREA_H_INCLUDED
