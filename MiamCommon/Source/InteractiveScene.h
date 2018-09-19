/*
  ==============================================================================

    InteractiveScene.h
    Created: 17 Nov 2016 4:53:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef INTERACTIVESCENE_H_INCLUDED
#define INTERACTIVESCENE_H_INCLUDED



#include <memory>
#include <vector>
#include <string>
#include <map>

#include "JuceHeader.h"

#include "MultiAreaEvent.h"

#include "IEditableArea.h"
#include "Exciter.h"


// Pre-declarations for pointers
class SceneCanvasComponent;

namespace Miam
{
    // Pre-declarations for pointer members
    class Exciter;
    class IDrawableArea;
    class IInteractiveArea;
    class MultiSceneCanvasInteractor;
    
    
    
    /// \brief A graphical scene made of areas and exciters, to be drawn on
    /// SceneCanvasComponent
    ///
    /// This kind of scene can be loaded (from a file for example) and destroyed,
    /// but is not fully editable. See Miam::EditableArea for editing features.
    /// However, the exciters (if enabled) can be moved with mouse/touch/pen events
    class InteractiveScene : public std::enable_shared_from_this<InteractiveScene>
    {
        
        // ...Enums....
        public :
        /// \brief For Miam Spat research purposes
        enum class ExcitersBehaviorType {
            
            NoExciters,
            
            AppearOnTouch,
            
            ManualAddAndDelete,
            
            ManualAddAndDelete_AutoDisappear,
            
        };
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        protected :
        
        // links back to parent modules
        std::weak_ptr<MultiSceneCanvasInteractor> canvasManager;
        /// \brief The associated Juce (mono-scene) canvas component
        ///
        /// A basic pointer that has to be properly (at some point) transmitted to the Juce API
        SceneCanvasComponent* canvasComponent = 0;
        
        
        // name
        std::string name;
        
        /// \brief The areas to be drawn
        std::vector< std::shared_ptr<IInteractiveArea> > areas;
        
        std::vector< std::shared_ptr<Exciter> > initialExciters;
        std::vector< std::shared_ptr<Exciter> > currentExciters;
        
        /// \brief On aura parfois besoin d'un excitateur particulier sélectionné :
        /// - pour savoir lequel supprimer en particulier (quand on supprime par clic
        /// sur un bouton)
        /// - pour savoir lequel faire sauter par double-clic
        /// - pour d'autres trucs...
        ///
        /// dans tous les cas ça peut juste ne pas être utilisé
        std::shared_ptr<Exciter> selectedExciter;
        
        ExcitersBehaviorType excitersBehavior;
        
        
        /// \brief Links (Locks) a multi-touch souce ID to an EditableArea
        ///
        /// L'aire concernée peut être soit une aire générique éditable, soit
        /// un excitateur, soit....
        std::map<int, std::shared_ptr<IEditableArea>> touchSourceToEditableArea;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        // - - - - - Pour interfaçage avec classes de dessin OpenGL - - - - -
        
        /// \brief Aires + excitateurs
        size_t GetDrawableObjectsCount();
        
        /// \brief Les aires sont envoyées en premier, les excitateurs derrière
        ///
        /// Les excitateurs n'ont donc ici pas leur index dans le tableau
        /// d'excitateurs, mais leur index par rapport à toutes les aires dessinables
        /// dans la scène
        std::shared_ptr<IDrawableArea> GetDrawableObject(size_t i);
        
        protected :
        /// \brief Calcule l'index d'un excitateur parmi toutes les aires graphiques
        /// dessinables en openGL.
        virtual size_t getExciterDrawingIndex(size_t exciterVectorIndex) const
        { return areas.size() + exciterVectorIndex; }
        
        public :
        
        
        // - - - - - Vraies aires interactives - - - - -
        
        size_t GetInteractiveAreasCount();
        std::shared_ptr<IInteractiveArea> GetInteractiveArea(size_t i);
        
        public :
        std::shared_ptr<Exciter> GetSelectedExciter() const {return selectedExciter;}
        protected :
        std::shared_ptr<MultiAreaEvent> setSelectedExciter(std::shared_ptr<Exciter> exciterToSelect);
        
        public :
        /* useless normally...
        size_t GetExcitersCount() {return currentExciters.size(); }
        std::shared_ptr<Exciter> GetExciter(size_t i) {return currentExciters[i];}
         */
        
        std::string GetName() {return name;}
        virtual void SetName(std::string _name);
        
        // Modes de basse opacité pour des groupes entiers d'objets graphiques
        void SetExcitersOpacityMode(OpacityMode opacityMode);
        void SetAreasOpacityMode(OpacityMode opacityMode);
    
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction and Destruction (and helpers) - - - - -
        InteractiveScene(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_, SceneCanvasComponent* canvasComponent_, ExcitersBehaviorType excitersBehavior_ = ExcitersBehaviorType::ManualAddAndDelete);
        virtual ~InteractiveScene();
        
        
        
        
        // - - - - - Areas Managing : Add and Delete - - - - -
        
        
        /// \brief Adds an area without creating it before
        virtual std::shared_ptr<AreaEvent> AddArea(std::shared_ptr<IInteractiveArea> newArea);
        
        
        // - - - - - Exciters Managing - - - - -
        
        
        /// \brief Adds a default exciter
        virtual std::shared_ptr<AreaEvent> AddDefaultExciter();
        /// \brief Adds an exciter without creating it before
        virtual std::shared_ptr<AreaEvent> AddExciter(std::shared_ptr<Exciter> newExciter, bool forceSelect = true);
        /// \brief Deletes a current ("active") exciter by reference.
        ///
        /// Modifie les indexes des excitateurs courants plus loin dans le tableau.
        virtual std::shared_ptr<AreaEvent> DeleteCurrentExciterByIndex(size_t excitersVectorIndex);
        /// \brief Supprime l'excitateur actuellement sélectionné (si existe)
        ///
        /// Fonctionne par recherche inverse dans le tableau d'excitateurs... Pas
        /// optimisé.
        std::shared_ptr<AreaEvent> DeleteSelectedExciter();
        
        /// \brief Replace le jeu d'excitateurs dans son état d'origine.
        virtual std::shared_ptr<MultiAreaEvent> ResetCurrentExcitersToInitialExciters();
        /// \brief Pendant l'édition, on pourra sauvegarder les excitateurs.
        ///
        /// En mode de jeu classique, cette fonctionnalité ne servira normalement pas.
        virtual void SaveCurrentExcitersToInitialExciters(bool deleteCurrentExciters = false);
        
        
        // - - - - - Selection events managing (orders from parent manager) - - - - -

		/// \brief Might update exciters interaction, send events about the new state, ....
        ///
        /// Pas encore définitif (va évoluer dans les prochaines versions)
        virtual std::shared_ptr<MultiAreaEvent> OnSelection(bool resetExciters = true);
        /// \ brief Behavior on unselection commanded from parent (area
        /// transformations are stopped, ...). Must be called by classes that
        /// inherit from this.
        ///
        /// \return A list of all the events that just happened
        virtual std::shared_ptr<MultiAreaEvent> OnUnselection(bool shutExcitersDown = true);
        
        
        // - - - - - Canvas (mouse) events managing - - - - -
        // CES FONCTIONS IMPLÉMENTERONT LES DÉPLACEMENTS DES EXCITATEURS
        // Elles pourront être appelées depuis la classe file EditableScene si
        // nécessaire, si les excitateurs sont effectivement utilisés
        //
        // Par défaut elles sont overridées par la classe fille
        
        /// \brief Defines the behavior of the scene when an input occurs. Possibly begins an exciter's movement
        ///
        /// Within the event,
        /// may send an error message, or an explanation if nothing happened
        /// Empty return string means that everything went as expected
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE);
        /// \brief Defines the behavior of the scene when an input occurs. Possibly moves an exciter if enabled
        ///
        /// Does not compute a new spatialization state : a parent will ask the
        /// scene to do so at the right time.
        ///
        /// \returns An event that describes what happened
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE);
        /// \brief Defines the behavior of the scene when an input occurs. Possibly ends an exciter move if enabled
        ///
        /// \returns An event that describes what happened
        virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE);
        
        /// \brief Called when the scene's mode changes (from exciters transformation
        /// to something else...). because : The scene does not know its own mode !
        virtual std::shared_ptr<MultiAreaEvent> StopCurrentTransformations();

        
        // - - - - - Quantification, gestion des interactions - - - - -
        private :
        /// \brief Modification possible des interactions :
        /// recherche obligatoire dans toutes les aires graphiques !
        /// En 2 passes : d'abord on checke toutes les modifs (qui renvoient des events) puis on recalcule
        /// les excitations en fonction du tout (car les excitateurs ont besoin que tout soit à jour)
        std::shared_ptr<MultiAreaEvent> testAreasInteractionsWithExciter(std::shared_ptr<Exciter>& exciter);
        public :
        /// \brief Re-quantifie les interactions entre excitateurs et aires graphiques de manière
        /// forcée, renvoie les évènements associés.
        ///
        /// Utile par exemple quand les aires graphiques ont été modifées dans un éditeur sans renvoyer
        /// proprement tous les évènements associés.
        std::shared_ptr<MultiAreaEvent> RecomputeAreaExciterInteractions();
        
        // - - - - - XML import/export - - - - -
        public :
        /// \brief Exports the current areas of course, and the initial exciters previously saved
        /// (does not export the current active exciters)
        virtual std::shared_ptr<bptree::ptree> GetTree() const;
        // Pas de SetFromTree :
        // les aires graphiques concrètes ne peuvent être instanciées
        // que depuis une classe concrète spécifique de l'application finale.
        
    };
    
    
    
}


#endif  // INTERACTIVESCENE_H_INCLUDED
