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

using namespace Miam;

namespace Miam
{
	class MultiSceneCanvasInteractor;

	class AmusingScene : public EditableScene
	{
	public :
		AmusingScene(MultiSceneCanvasInteractor* _canvasManager, SceneCanvasComponent* _canvasComponent);
		virtual ~AmusingScene();
		// override mouse callback
		
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
