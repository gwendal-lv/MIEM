/*
  ==============================================================================

    MultiSceneCanvasManager.h
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MULTISCENECANVASMANAGER_H_INCLUDED
#define MULTISCENECANVASMANAGER_H_INCLUDED


#include "MultiSceneCanvasInteractor.h"


namespace Miam {
    
    
    // Simple declarations
    
    
    /// \brief
    class MultiSceneCanvasManager : public MultiSceneCanvasInteractor {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        
        
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - - Construction and Destruction - - - - - -
        
        // same args as MultiSceneCanvasEditor
        MultiSceneCanvasManager(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_);
        virtual ~MultiSceneCanvasManager();
        
        // - - - - - - Scenes managing : Add and Delete - - - - - -
        virtual void AddScene(std::string name, bool selectNewScene = false) override;
        
        
        
        // - - - - - - Gestion des évènements des scènes - - - - - -
        protected :

        /// \brief Après avoir retransmis la commande à la classe parente, sélectionnera les évènements
        /// concernant les aires intéressantes,
        /// puis crée et envoie un évènement
        /// directement au GraphicSessionManager concernant les seules aires
        /// dont le degré d'excitation aura changé
        ///
        /// Le graphicsessionmanager traduit et envoie au presenter ce qu'il faut
        virtual void processSingleAreaEventSync(std::shared_ptr<AreaEvent>& areaE) override;
        

    };
    
    
}



#endif  // MULTISCENECANVASMANAGER_H_INCLUDED
