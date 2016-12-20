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

		// override mouse callback
		std::string OnCanvasMouseDown(Point<int> &clicLocation) override;
		bool OnCanvasMouseDrag(Point<int> &clicLocation) override;
		bool OnCanvasMouseUp() override;
		
		// private attribut
		
		//Model *model;
	};


}



#endif  // AMUSINGSCENE_H_INCLUDED
