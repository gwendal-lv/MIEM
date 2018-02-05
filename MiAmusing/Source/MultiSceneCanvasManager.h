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
        virtual void AddScene(std::string name,bool selectNewScene) override;
		void __AddAnimatedTestAreas();
		void AddNedgeArea(uint64_t nextAreaId, int N);
		void AddTrueCircle(uint64_t nextAreaId);
		void AddCompleteArea();
		void AddAreaToScene(size_t sceneIndex, std::shared_ptr<IInteractiveArea> area_) override;
		void hideAddPolygon();
		void showAddPolygon();
		void OnDelete();
		void OnFollowerTranslation(std::shared_ptr<GraphicEvent> graphicE);
		void OnAudioPosition(double position);
		void SetAllAudioPositions(double position);
		void SetAudioPositions(std::shared_ptr<Cursor> area, double position);
		void lookForAreasConcerned(Colour colourConcerned);
		int getNumberArea();
		void handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE);
		void handleAndSendMultiAreaEventSync(std::shared_ptr<MultiAreaEvent> multiAreaE);

		virtual std::shared_ptr<bptree::ptree> GetTree() override;
		
		void deleteAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToDelete);
		void OnCanvasMouseUp(const MouseEvent& mouseE);
		void OnCanvasMouseDown(const MouseEvent& mouseE);
		void OnCanvasMouseDoubleClick(const MouseEvent& mouseE);

		void OnInteraction(std::shared_ptr<AreaEvent> areaE);

		void SetAllChannels();
		std::shared_ptr<EditableScene> GetSelectedScene();

		void ChangeBaseNote(int newBaseNote);
		void ChangeSpeed(double newSpeed);
		double getSpeed(std::shared_ptr<IEditableArea> area);
		void ChangeVelocity(double newVelocity);
		void ChangeColour(Colour newColour, int colourIdx);
		double getVelocity(std::shared_ptr<IEditableArea> area);
		double getOctave(std::shared_ptr<IEditableArea> area);
		int getCurrentColor(std::shared_ptr<IEditableArea> area);
		void SetMode(Miam::CanvasManagerMode newMode);

		void UnselectScene();
		
    };
    
    
}



#endif  // MULTISCENECANVASMANAGER_H_INCLUDED
