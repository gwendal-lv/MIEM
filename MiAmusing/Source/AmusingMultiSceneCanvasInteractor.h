/*
  ==============================================================================

    AmusingMultiSceneCanvasInteractor.h
    Created: 9 Dec 2016 11:38:09am
    Author:  ayup1

  ==============================================================================
*/

#ifndef AMUSINGMULTISCENECANVASINTERACTOR_H_INCLUDED
#define AMUSINGMULTISCENECANVASINTERACTOR_H_INCLUDED

#include "MultiSceneCanvasInteractor.h"
#include "MultiSceneCanvasEditor.h"



using namespace Miam;

namespace Miam
{
    class AmusingMultiSceneCanvasInteractor : public MultiSceneCanvasEditor//MultiSceneCanvasInteractor
    {
	public :
		// Constructor
		AmusingMultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId);
		~AmusingMultiSceneCanvasInteractor();

		// Add AmusingScene
		void AddScene(std::string name) override;
	
		// Example
		void __AddTestAreas();

		// callback when one surface of a scene is changed
		void OnSurfaceChanged(double newSurface) override;

	};
};


#endif  // AMUSINGMULTISCENECANVASINTERACTOR_H_INCLUDED
