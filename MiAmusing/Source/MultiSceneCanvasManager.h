/*
  ==============================================================================

    MultiSceneCanvasManager.h
    Created: 10 Jan 2017 9:37:54pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MULTISCENECANVASMANAGER_H_INCLUDED
#define MULTISCENECANVASMANAGER_H_INCLUDED


#include "MultiSceneCanvasEditor.h"
#include "IGraphicSessionManager.h"

namespace Amusing {
    
    
    // Simple declarations
    
    
    /// \brief
    class MultiSceneCanvasManager : public Miam::MultiSceneCanvasEditor {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        
		IGraphicSessionManager *graphicSessionManager;
        
        // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
        public :
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // ------ Construction and Destruction ------
        
        // same args as MultiSceneCanvasEditor
        MultiSceneCanvasManager(IGraphicSessionManager* graphicSessionManager_, MultiSceneCanvasComponent* canvasComponent_, SceneCanvasComponent::Id selfId_);
        virtual ~MultiSceneCanvasManager();
        
        // ------ Scenes managing : Add and Delete ------
        virtual void AddScene(std::string name) override;
		void __AddAnimatedTestAreas();
		void AddNedgeArea(uint64_t nextAreaId, int N);
		void AddTrueCircle(uint64_t nextAreaId);
		void AddFollower(uint64_t nextAreaId);
		void AddCompleteArea();
		void OnDelete();
		void OnFollowerTranslation(std::shared_ptr<GraphicEvent> graphicE);
		void OnAudioPosition(double position);
		int getNumberArea();
		void handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE);


		void deleteUnusedFollowers();
		void deleteAsyncDrawableObject(int idInScene, std::shared_ptr<IDrawableArea> originalAreaToDelete);
		void OnCanvasMouseDown(const MouseEvent& mouseE);
    };
    
    
}



#endif  // MULTISCENECANVASMANAGER_H_INCLUDED
