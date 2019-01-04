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
        // L'aire ne notifie pas aux excitateur liés, lorsque qu'elle est détruite/modifiée/etc...
        // Ce sont les excitateur qui gèreront leurs weak_ptr vers this
        //
        // On ne pas utiliser facilement de map
        // car weak_ptr ne permet pas la comparaison sans .lock() ...
        // Et  vu la taille max du conteneur (10aine d'excitateurs max par aire...) on prend juste
        // un vecteur (accès direct random) plutôt qu'une liste (accès par itérateur)
        std::vector< std::weak_ptr<Exciter> > excitersInteractingWithThis;
        std::vector< double > excitersWeights;
        std::vector< Excitement > excitementAmounts;
        
        
        /// \brief Maximum distance in pixels between a displayed element (point,
        /// edge, surface...), and a user event that could move this element
        float elementInteractionRadius;

        
        
    protected:
        double surface;
        
        
        
        
        
        
        // - - - - - Construction/destruction + clonage polymorphique - - - - -
        public :
        virtual ~InteractiveArea();
        protected :
        // Suppression des liens avec les excitateurs
        virtual void onCloned() override;
        public :
        
        
        // - - - - - Fonctions de mise à jour - - - - -
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

        
        // - - - - - Interaction avec Excitateurs - - - - -
        
        /// \brief Internally updates the interaction weight with a given exciter.
        ///
        /// The interaction weight is however different from the total excitement from
        /// several exciters.
        ///
        /// \param forceDisableInteraction If true, even if the exciter intersects
        /// the shape, its effects will be null
        ///
        /// \returns Wether the interaction between this
        /// area and an exciter has changed, or not.
        std::shared_ptr<AreaEvent> UpdateInteraction(std::shared_ptr<Exciter>& exciter, bool forceDisableInteraction = false) override;
        /// \brief Appelé par un excitateur qui est supprimé (mais on ne sais lequel).
        /// On doit forcer la suppression, en effet
        /// sinon le test de collision de "UpdateInteraction" resterait
        /// vrai, et donc l'excitateur resterait enregistré
        ///
        /// Pas d'argument : l'excitateur est en train d'être détruit... Pas de shared_ptr donc !!
        /// On va juste re-rechecker
        virtual void OnExciterDestruction() override;
        private :
        /// \brief Supprime toutes les références internes à l'excitateur décrit par son index interne.
        ///
        /// Pas de vérification de validité de l'index
        void deleteLinksToExciter(size_t exciterLocalIndex);
        public :
        
        /// \brief Retourne la somme (sans saturation) de l'excitation cumulée
        /// de tous les excitateurs interagissant actuellement avec la forme.
        virtual double GetTotalLinearExcitement() const override;
        virtual double GetTotalAudioExcitement() const override;
        virtual double GetTotalInteractionWeight() const override;
        
        /// \brief Appelé par un excitateur lié à cette aire. On sauvegarde cette donnée en interne.
        virtual void OnNewExcitementAmount(const std::shared_ptr<Exciter>& sender, Excitement excitementAmount) override;
        
        private :
        /// \brief Renvoie l'index de l'excitateur dans le tableau interne de
        /// weak_ptr (ou size() si n'est pas trouvé)
        size_t getExciterLocalIndex(const std::shared_ptr<Exciter>& exciter);

        
        
    };
    
    
}



#endif  // INTERACTIVEAREA_H_INCLUDED
