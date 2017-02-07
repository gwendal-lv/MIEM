/*
  ==============================================================================

    AmusingScene.h
    Created: 8 Dec 2016 2:56:07pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef AMUSINGSCENE_H_INCLUDED
#define AMUSINGSCENE_H_INCLUDED

#include "EditableScene.h"
#include "AnimatedPolygon.h"

using namespace Miam;

namespace Amusing
{
	class AnimatedPolygon;
}

namespace Miam
{
	
	class MultiSceneCanvasInteractor;

	class AmusingScene : public EditableScene,
		public std::enable_shared_from_this<AmusingScene> 
	{
	public :
		AmusingScene(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager, SceneCanvasComponent* _canvasComponent);
		virtual ~AmusingScene();

		void AddAnimatedArea(uint64_t nextAreaId);
		std::shared_ptr<AreaEvent> AddNedgeArea(uint64_t nextAreaId, int N);
		// override mouse callback
		
		std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
		std::shared_ptr<AreaEvent> AddTrueCircle(uint64_t nextAreaId);
		std::shared_ptr<AreaEvent> AddFollower(uint64_t nextAreaId);

		std::shared_ptr<Amusing::AnimatedPolygon> getFirstArea();

		/*
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDown(const MouseEvent& mouseE) override;
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseDrag(const MouseEvent& mouseE) override;
		virtual std::shared_ptr<GraphicEvent> OnCanvasMouseUp(const MouseEvent& mouseE) override;
		*/
		// private attribut
		
		//Model *model;
	};


}



#endif  // AMUSINGSCENE_H_INCLUDED
