/*
  ==============================================================================

    SceneCanvasComponent.cpp
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SceneCanvasComponent.h"

#include "MultiSceneCanvasInteractor.h"

//==============================================================================
SceneCanvasComponent::SceneCanvasComponent() :
    selectedForEditing(false),
    lastFrameTimePt(std::chrono::steady_clock::now()),
    lastSequenceFramesCount(0),
    lastSequenceTimePt(std::chrono::steady_clock::now())
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
    bool platformSupportsSyncedSwap = openGlContext.setSwapInterval(10);
    if (platformSupportsSyncedSwap)
        std::cout << "Platform supports synced OpenGL swaps" << std::endl;
    else
        std::cout << "Platform does not support synced OpenGL swaps" << std::endl;
}







// - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (canvasManager != 0)
    {
        // We update ALL areas NOW, to avoid a consequent amount of calculus on
        // scene change (which should happen as fast as possible)
        for (size_t i=0;i<canvasManager->GetScenesCount();i++)
        {
            for (size_t j=0 ; j<canvasManager->GetScene(i)->GetDrawableObjectsCount() ; j++)
                canvasManager->GetScene(i)->GetDrawableObject(j)->CanvasResized(this);
        }
    }
}




// - - - - - - - - OpenGL specific - - - - - - - - -
void SceneCanvasComponent::newOpenGLContextCreated()
{
    std::cout << "SceneCanvasComponent" << " : init OpenGL" << std::endl;
}
void SceneCanvasComponent::renderOpenGL()
{
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
    
    // Duplication of the drawable objects for thread-safe rendering
    canvasManager->LockAsyncDrawableObjects();
    
    // Not a resize but just a pre-allocation (to avoid empty shared ptrs constructing)
    std::vector<std::shared_ptr<IDrawableArea>> duplicatedAreas;
    duplicatedAreas.reserve(canvasManager->GetAsyncDrawableObjects().size());
    for (size_t i = 0 ; i<canvasManager->GetAsyncDrawableObjects().size() ; i++)
        duplicatedAreas.push_back(
            std::shared_ptr<IDrawableArea>(
                canvasManager->GetAsyncDrawableObjects()[i]->Clone() ) );
    
    canvasManager->UnlockAsyncDrawableObjects();
    
    // Areas painting (including exciters if existing)
    for (size_t i=0;i<duplicatedAreas.size();i++)
        duplicatedAreas[i]->Paint(g);
    
    
    
    // Time measures just before swap (or the closer that we can get to the swaps)
    auto currentTimePt = std::chrono::steady_clock::now();
    if (selectedForEditing)
    {
        if (lastSequenceFramesCount >= 100)
        {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePt - lastSequenceTimePt);
            float displayFrequency = (float)lastSequenceFramesCount * 1000000.0f / (float)(duration.count());
            std::cout << "Display frequency = " << displayFrequency << "Hz" << std::endl;
            
            lastSequenceTimePt = currentTimePt;
            lastSequenceFramesCount = -1;
        }
    }
    lastSequenceFramesCount++;
}


void SceneCanvasComponent::openGLContextClosing()
{
    
}





// - - - - - - - - Juce events - - - - - - - - -

void SceneCanvasComponent::mouseDown(const juce::MouseEvent& event)
{
    canvasManager->OnCanvasMouseDown(event);
}

void SceneCanvasComponent::mouseDrag(const juce::MouseEvent& event)
{
    canvasManager->OnCanvasMouseDrag(event);
}

void SceneCanvasComponent::mouseUp(const juce::MouseEvent& event)
{
    canvasManager->OnCanvasMouseUp(event);
}



// Getters and Setters

void SceneCanvasComponent::SetIsSelectedForEditing(bool isSelected)
{
    selectedForEditing = isSelected;
}

