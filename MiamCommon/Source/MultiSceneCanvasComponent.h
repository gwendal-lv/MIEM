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

#include "InteractiveScene.h"

using namespace Miam;




//==============================================================================
/*
*/
class MultiSceneCanvasComponent    : public Component,
                                     public ButtonListener
{

    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    
    protected :
    // links back to parent modules
    MultiSceneCanvasInteractor* canvasManager = 0;
    
    // Children canvas
    SceneCanvasComponent* childrenCanvas;
    
    // Button objects
    std::vector<ScopedPointer<TextButton>> sceneChoiceTextButtons;
    
    
    // - - - - - Graphical CONST options - - - - -
    
    // Buttons on the top (to avoid bottom false manipulations.... good idea ?)
    // Same size as in the menus, but spaces are 4 PIXELS
    // height 24px + 4px under
    // BUTTONS SIZE SHOULD BE DIFFERENT ON COMPUTERS AND ON TABLETS/SMARTPHONES
    const int space = 4;
    
    
    
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
    
    void LinkToManager(MultiSceneCanvasInteractor* _canvasManager);

    void paint (Graphics&) override;
    void resized() override;
    
    // - - - - - Buttons management - - - - -
    public :
    // Default behavior : updates all scenes (when sceneThatHasChanged = -1)
    void UpdateSceneButtons(std::vector< std::shared_ptr<InteractiveScene> > scenes, int sceneThatHasChanged = -1);
    
    protected : // internal helpers
    void updateSceneButtonsBounds();
    void addButton(std::string buttonName);
    
    public :
    // the listener
    void buttonClicked(Button* buttonThatWasClicked) override;
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiSceneCanvasComponent)
};


#endif  // MULTISCENECANVASCOMPONENT_H_INCLUDED