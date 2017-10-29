/*
  ==============================================================================

    SceneCanvasComponent.cpp
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "SceneCanvasComponent.h"

#include "MultiSceneCanvasInteractor.h"

//==============================================================================
SceneCanvasComponent::SceneCanvasComponent() :
    selectedForEditing(false),
    displayFrequencyMeasurer("SceneCanvasComponent display")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    openGlContext.setComponentPaintingEnabled(true); // default behavior, lower perfs
    // OpenGL final initialization will happen in the COmpleteInitialization method
    
}

SceneCanvasComponent::~SceneCanvasComponent()
{
}

void SceneCanvasComponent::CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager)
{
    canvasManager = _canvasManager;
    
    // OpenGL final initialization here
    openGlContext.setRenderer(this);
    openGlContext.attachTo(*this);
    openGlContext.setContinuousRepainting(true);
    isSwapSynced = openGlContext.setSwapInterval(swapInterval);
    if (isSwapSynced)
        DBG("Platform supports synced OpenGL swaps");
    else
        DBG("Platform does not support synced OpenGL swaps");
}







// - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (auto manager = canvasManager.lock())
    {
        // We update ALL areas NOW, to avoid a consequent amount of calculus on
        // scene change (which should happen as fast as possible)
        for (size_t i=0;i<manager->GetScenesCount();i++)
        {
            for (size_t j=0 ; j<manager->GetScene(i)->GetDrawableObjectsCount() ; j++)
                manager->GetScene(i)->GetDrawableObject(j)->CanvasResized(this);
        }
    }
}




// - - - - - - - - OpenGL specific - - - - - - - - -
void SceneCanvasComponent::newOpenGLContextCreated()
{
    DBG("SceneCanvasComponent : init OpenGL");

	// Will init the counter
	//displayFrequencyMeasurer.OnNewFrame();
}
void SceneCanvasComponent::renderOpenGL()
{
	//DBG("render : " + getName());
    auto manager = canvasManager.lock();
    
    const float desktopScale = (float) openGlContext.getRenderingScale();
    std::unique_ptr<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (openGlContext,
                                                                                    roundToInt (desktopScale * getWidth()),
                                                                                    roundToInt (desktopScale * getHeight())));
    Graphics g(*glRenderer);
    
    g.addTransform(AffineTransform::scale(desktopScale));
    
    // Pure black background
    g.fillAll (Colours::black);
    
    // White interior contour 2px line to show when the canvas is active
    if (selectedForEditing)
    {
        g.setColour(Colours::white);
        g.drawRect(1, 1, getWidth()-2, getHeight()-2, 2);
    }
    
    
    // - - - - - THIRD Duplication of the drawable objects for thread-safe rendering, - - - - -
    // Lorsque nécessaire seulement !
    manager->LockAsyncDrawableObjects();
    
    bool areasCountChanged = (manager->GetAsyncDrawableObjects().size() != duplicatedAreas.size());
    // POUR L'INSTANT ALGO BÊTE
    // on resize le vecteur : la construction des shared n'est pas grave, leur bloc de contrôle reste
    // en mémoire finalement (on utilisera reset() )
    if (areasCountChanged)
    {
        canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
        duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
    }
    // Vérification simple de chaque aire 1 par 1
    size_t i = 0;
    for (auto it = manager->GetAsyncDrawableObjects().begin() ;
         it != manager->GetAsyncDrawableObjects().end() ;
         ++it)
    {
        // S'il y a eu un changement : on re-crée un pointeur déjà, puis
        // on fait effectivement la copie d'un nouvel objet
        if (canvasAreasPointersCopies[i] != (*it))
        {
            canvasAreasPointersCopies[i] = (*it);
            duplicatedAreas[i].reset( (*it)->Clone() ); // pas d'allocation d'un nouveau ptr
        }
        // Double compteur
        i++;
    }
    
    manager->UnlockAsyncDrawableObjects();
    
    
    // - - - - - Areas painting (including exciters if existing) - - - - -
    // Sans bloquer, du coup, les autres threads (pour réactivité max...)
    for (size_t i=0;i<duplicatedAreas.size();i++)
        duplicatedAreas[i]->Paint(g);
    
    
    // Call to a general Graphic update on the whole Presenter module
	if ( ! manager->isUpdatePending() )
		manager->triggerAsyncUpdate();
    
    // Time measures just before swap (or the closer that we can get to the swaps)
    displayFrequencyMeasurer.OnNewFrame();
    if (selectedForEditing)
    {
        if (displayFrequencyMeasurer.IsFreshAverageAvailable())
        {
            //DBG(displayFrequencyMeasurer.GetInfo());
        }
    }
    // Forced sleep if drawing is too fast
    double underTime = desiredPeriod_ms - displayFrequencyMeasurer.GetLastDuration_ms();
    if (underTime > 0.0)
    {
        Thread::sleep((int)std::floor(underTime));
    }
}


void SceneCanvasComponent::openGLContextClosing()
{
    
}





// - - - - - - - - Juce events - - - - - - - - -

void SceneCanvasComponent::mouseDown(const juce::MouseEvent& event)
{
    canvasManager.lock()->OnCanvasMouseDown(event);
}

void SceneCanvasComponent::mouseDrag(const juce::MouseEvent& event)
{
    canvasManager.lock()->OnCanvasMouseDrag(event);
}

void SceneCanvasComponent::mouseUp(const juce::MouseEvent& event)
{
    canvasManager.lock()->OnCanvasMouseUp(event);
}



// Getters and Setters

void SceneCanvasComponent::SetIsSelectedForEditing(bool isSelected)
{
    selectedForEditing = isSelected;
}

