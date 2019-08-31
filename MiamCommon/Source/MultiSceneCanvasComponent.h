/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
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

#ifndef MULTISCENECANVASCOMPONENT_H_INCLUDED
#define MULTISCENECANVASCOMPONENT_H_INCLUDED


#include "JuceHeader.h"

#include "SceneCanvasComponent.h"

#include "InteractiveScene.h"

using namespace Miam;


namespace Miam
{
    // forward declarations
    class MultiSceneCanvasInteractor;
}

//==============================================================================
/*
*/
class MultiSceneCanvasComponent    : public Component,
                                     public Button::Listener
{
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    
    protected :
    // links back to parent modules
    std::shared_ptr<MultiSceneCanvasInteractor> canvasManager;
    
    // Children canvas
    SceneCanvasComponent* childrenCanvas = 0;
    
    // Button objects
    std::vector<std::unique_ptr<TextButton>> sceneChoiceTextButtons;
    public :
    static const int SceneButtonsHeight = 36;
    protected :
    
    
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
    
    void LinkToManager(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_);
	void ReleaseOpengGLResources();
    
    // - - - - - Painting and Resizing - - - - -
    
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
