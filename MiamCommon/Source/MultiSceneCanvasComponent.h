/*
  ==============================================================================

    MultiSceneCanvasComponent.h
    Created: 16 Nov 2016 5:35:10pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef MULTISCENECANVASCOMPONENT_H_INCLUDED
#define MULTISCENECANVASCOMPONENT_H_INCLUDED


#include "JuceHeader.h"

#include "SceneCanvasComponent.h"
#include "MultiSceneCanvasInteractor.h"

using namespace Miam;




//==============================================================================
/*
*/
class MultiSceneCanvasComponent    : public Component
{

    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    
    protected :
    // links back to parent modules
    MultiSceneCanvasInteractor* canvasManager = 0;
    
    // Children canvas
    SceneCanvasComponent* childrenCanvas;
    
    // Button objects
    std::vector<ScopedPointer<TextButton>> sceneChoiceTextButtons;
    
    
    
    
    // = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =
    public :
    // - - - - - General - - - - -
    SceneCanvasComponent* GetCanvas() {return childrenCanvas;}
    
    
    // - - - - - From Presenter - - - - -
    void SetIsSelectedForEditing(bool isSelected);
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
public:
    MultiSceneCanvasComponent();
    ~MultiSceneCanvasComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    // - - - - - Buttons management - - - - -
    // Default behavior : updates all scenes (when sceneThatHasChanged = -1)
    void UpdateSceneButtons(std::vector<std::string> Scenes, int sceneThatHasChanged = -1);
    
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiSceneCanvasComponent)
};


#endif  // MULTISCENECANVASCOMPONENT_H_INCLUDED
