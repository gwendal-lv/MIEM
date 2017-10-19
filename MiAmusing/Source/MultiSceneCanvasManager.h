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

class Cursor;

namespace Amusing {

	class CompletePolygon;
    
    
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
		void SetAllAudioPositions(double position);
		void SetAudioPositions(std::shared_ptr<Cursor> area, double position);
		int getNumberArea();
		void handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE);
		void handleAndSendMultiAreaEventSync(std::shared_ptr<MultiAreaEvent> multiAreaE);


		void deleteUnusedFollowers();
		void deleteAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToDelete);
		void OnCanvasMouseUp(const MouseEvent& mouseE);
		void OnCanvasMouseDown(const MouseEvent& mouseE);

		void OnInteraction(std::shared_ptr<AreaEvent> areaE);

		void SetAllChannels();
		std::shared_ptr<EditableScene> GetSelectedScene();

		void ChangeBaseNote(double newBaseNote);
		void ChangeSpeed(double newSpeed);
		double getSpeed(std::shared_ptr<IEditableArea> area);
		void ChangeVelocity(double newVelocity);
		double getVelocity(std::shared_ptr<IEditableArea> area);
		void SetMode(Miam::CanvasManagerMode newMode);
    };
    
    
}



#endif  // MULTISCENECANVASMANAGER_H_INCLUDED
