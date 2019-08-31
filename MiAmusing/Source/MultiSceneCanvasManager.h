/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MULTISCENECANVASMANAGER_H_INCLUDED
#define MULTISCENECANVASMANAGER_H_INCLUDED

#include "AreaOptionsComponent.h"
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
		void AddNedgeArea(uint64_t nextAreaId, int N);
		std::shared_ptr<IDrawableArea> AddAndSelectNedgeArea(uint64_t nextAreaId, int N, int height);
		void AddTrueCircle(uint64_t nextAreaId);
		void AddCompleteArea();
		void AddAreaToScene(size_t sceneIndex, std::shared_ptr<IInteractiveArea> area_) override;
		void AddAllIntersectionsToScene(int sceneIndex);
		void hideAddPolygon();
		void showAddPolygon();
		void OnDelete();
		void OnDeleteExciter();
		void OnFollowerTranslation(std::shared_ptr<GraphicEvent> graphicE);
		void OnAudioPosition(double position);
		void SetAllAudioPositions(double position);
		void SetAudioPositions(std::shared_ptr<Cursor> area, double position);
		void lookForAreasConcerned(Colour colourConcerned);
		int getNumberArea();
		void handleAndSendAreaEventSync(std::shared_ptr<AreaEvent> areaE);
		void handleAndSendMultiAreaEventSync(std::shared_ptr<MultiAreaEvent> multiAreaE);

		void resetAreaPosition();

		virtual std::shared_ptr<bptree::ptree> GetTree() override;
		
		void deleteAsyncDrawableObject(std::shared_ptr<IDrawableArea> originalAreaToDelete);
		void OnCanvasMouseUp(const MouseEvent& mouseE);
		void OnCanvasMouseDrag(const MouseEvent & mouseE);
		void OnCanvasMouseDown(const MouseEvent& mouseE);
		void OnCanvasMouseDoubleClick(const MouseEvent& mouseE);

		void OnInteraction(std::shared_ptr<AreaEvent> areaE);

		void SetAllChannels();
		std::shared_ptr<EditableScene> GetSelectedScene();

		void ChangeBaseNote(int newBaseNote);
		void ChangeSpeed(double newSpeed);
		double getSpeed(std::shared_ptr<IEditableArea> area);
		void ChangeVelocity(double newVelocity);
		void SetColorCode(int Nsamples, Colour colorCode[]);
		void ChangeColour(Colour newColour, int colourIdx);
		double getVelocity(std::shared_ptr<IEditableArea> area);
		double getOctave(std::shared_ptr<IEditableArea> area);
		int getCurrentColor(std::shared_ptr<IEditableArea> area);
		void SetMode(Miam::CanvasManagerMode newMode);
		void muteOtherAreas(int shouldMuteOtherAreas);
		void SetEditingMode(OptionButtonClicked optionClicked);

		void UnselectScene();
		OptionButtonClicked currentOptionClicked;

		double currentCursorSize;

		std::vector<Colour> colorCode;

		double speedTab[7] = { 0.25, 1.0 / 3.0, 0.5, 1.0, 2.0, 3.0, 4.0 };
    };
    
    
}



#endif  // MULTISCENECANVASMANAGER_H_INCLUDED
