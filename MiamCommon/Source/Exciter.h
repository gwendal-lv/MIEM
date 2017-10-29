/*
  ==============================================================================

    Exciter.h
    Created: 11 Jan 2017 2:36:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef EXCITER_H_INCLUDED
#define EXCITER_H_INCLUDED


#include <vector>
#include <string>
#include <chrono>

#include "EditablePolygon.h"
#include "EditableEllipse.h"


namespace Miam
{
    class MultiAreaEvent;
    
    /// \brief 
    class Exciter : public EditableEllipse
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        double volume;
        
        /// \brief Ids of the Mouse/touch inputs manipulating this area
        //std::vector<int> touchInputSources;
        
        //std::shared_ptr<EditableEllipse> volumeControlArea;
        
        // Display attributes
        
        // Clignotement
        typedef std::chrono::steady_clock clock;
        std::chrono::time_point<clock> startTimePt;
        std::chrono::time_point<clock> commonStartTimePt;
        double const omega = 2.0 * M_PI * 1.0; // 1 Hz
        double const deltaBrightnessAmplitude = 0.3;
        double const deltaBrightnessOffset = 0.7;
        
        // Lien avec des aires graphiques
        // Le + optimisé serait une liste, mais vu le nombre d'éléments stockés (10aine au grand max)
        // on s'en fout complètement...
        std::vector< std::weak_ptr<IInteractiveArea> > areasInteractingWith;
		/// \brief Le poids d'interaction d'une aire vis-à-vis de cet excitateur en particulier
		std::vector< double > areaInteractionWeights;
        /// \brief Les excitations pour chaque aire (dont la somme vaut 1.0)
        std::vector< double > areaExcitationAmounts;
        
        // Pour synchronisation du clignotement de tous les excitateurs ensemble.
        private :
        bool isAnimationSynchronized;
        
        // BESOIN D'UN TIMER QUI DÉCLENCHE DES ÉVÈNEMENTS SUR LE THREAD JUCE UI
        // BESOIN D'UN TIMER QUI DÉCLENCHE DES ÉVÈNEMENTS SUR LE THREAD JUCE UI
        // BESOIN D'UN TIMER QUI DÉCLENCHE DES ÉVÈNEMENTS SUR LE THREAD JUCE UI
        // BESOIN D'UN TIMER QUI DÉCLENCHE DES ÉVÈNEMENTS SUR LE THREAD JUCE UI
        // BESOIN D'UN TIMER QUI DÉCLENCHE DES ÉVÈNEMENTS SUR LE THREAD JUCE UI
        // pour gérer les fade in / out proprement non ?
        // Fade in/out peuvent aussi être commandés par l'air graphique... C'est elle qui voit
        // si elle n'est plus excitée que par 1 seul excitateur
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual std::string GetTypeAsString() const override { return "Exciter"; }
        
        // Pour calcul d'interaction externe
        bpt GetCenterInPixels() const {return centerInPixels;}

		double GetVolume() { return volume; }
        
        /// \brief Sets whether this exciter is animated the same as the others exciters that
        /// have this option activated.
        ///
        /// The synchronized clock must have been initialized before
        void SetIsAnimationSynchronized(bool isSynchronized);
        
        private :
        std::shared_ptr<Exciter> getCastedSharedFromThis()
        { return std::static_pointer_cast<Exciter>(shared_from_this()); }
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        Exciter(bptree::ptree & areaTree, std::chrono::time_point<clock> commonStartTimePoint_);
        /// \brief Will automatically define the shape of the exciter
        Exciter(uint64_t uniqueId, std::chrono::time_point<clock> commonStartTimePoint_);
        virtual ~Exciter();
        virtual IDrawableArea* Clone() const override {return new Exciter(*this);}
        
        // - - - - - Ction helpers - - - - -
        private :
        void init();
        
        // - - - - - Display - - - - -
        public :
        /// \brief Paints specific elements over the elements painted
        /// by the EditableEllipse::Paint() method
        virtual void Paint(Graphics& g) override;

        
        
        // - - - - - Interactions - - - - -
        /// \brief Peut être appelé par une aire excitée par cette instance.
		///
		/// Peut être appelé plusieurs (voire de nombreuses) fois par la même aire
        ///
        /// \returns L'excitation résultante du poids d'interaction calculé entre l'aire et cet
        /// excitateur (sachant que l'excitateur répartit son excitation sur les aires qu'il excite).
        void OnAreaExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis, double interactionWeight);
        /// \brief Peut être appelé par une aire qui n'est plus excitée par cette instance.
		///
		/// Normalement appelée une seule fois par aire enregistrée auprès de cet excitateur
		/// (plusieurs notifications tolérées).
        void OnAreaNotExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis);
        
        private :
        /// \brief Recherche d'un shared_ptr parmi un tableau de weak_ptr.
        /// L'itérateur retourné peut pointer sur .end()
        std::vector< std::weak_ptr<IInteractiveArea> >::iterator findAreaInteracting(std::shared_ptr<IInteractiveArea> areaToFind);
        /// \brief Recherche d'un shared_ptr parmi le tableau de weak_ptr. L'indice retourné
        /// peut valoir size() des vector concernés, si rien n'a été trouvé.
        size_t findAreaInteractingIndex(std::shared_ptr<IInteractiveArea> areaToFind);
        /// \brief To be called when any interaction weight has changed. Updates the excitation of all
        /// excited areas.
        ///
        /// Fait une normalisation de sorte que la quantité totale d'excitation soit 1. Les tableaux de
        /// quantité doivent déjà faire la bonne taille.
        void updateExcitationAmounts();
        
        public :
        /// \brief Informs the areas of their new excitement amount (different from their interaction
        /// weight).
        ///
        void NotifyNewExcitationToAreas();
        
        // - - - - - XML import/export - - - - -
        public :
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
    };
    
    
    
}




#endif  // EXCITER_H_INCLUDED
