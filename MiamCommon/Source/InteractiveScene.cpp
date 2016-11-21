/*
  ==============================================================================

    InteractiveScene.cpp
    Created: 17 Nov 2016 4:53:59pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractiveScene.h"

#include "DrawableArea.h"

#include "MultiSceneCanvasInteractor.h"
#include "InteractiveArea.h"
#include "Exciter.h"


using namespace Miam;


// - - - - - Construction and Destruction (and helpers) - - - - -

InteractiveScene::InteractiveScene(MultiSceneCanvasInteractor* _canvasManager, SceneCanvasComponent* _canvasComponent)
{
    canvasManager = _canvasManager;
    canvasComponent = _canvasComponent;
    
    name = "Default Scene";
    
    
}

InteractiveScene::~InteractiveScene()
{
    
}






// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

size_t InteractiveScene::GetAreasCount()
{
    return areas.size();
}
std::shared_ptr<IDrawableArea> InteractiveScene::GetDrawableArea(size_t i)
{
    return areas[i];
}
std::shared_ptr<IInteractiveArea> InteractiveScene::GetInteractiveArea(size_t i)
{
    return areas[i];
}





// = = = = = = = = = = METHODS = = = = = = = = = =




// - - - - - Areas Managing : Add and Delete - - - - -


void InteractiveScene::AddArea(std::shared_ptr<IInteractiveArea> newArea)
{
    areas.push_back(newArea);
    
    // Forced graphical updates
    newArea->CanvasResized(canvasComponent);
}





