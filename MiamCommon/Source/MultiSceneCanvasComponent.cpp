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


#include "MultiSceneCanvasComponent.h"

#include "MultiSceneCanvasInteractor.h"




// = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =

void MultiSceneCanvasComponent::SetIsSelectedForEditing(bool isSelected)
{
    childrenCanvas->SetIsSelectedForEditing(isSelected);
}






//==============================================================================
MultiSceneCanvasComponent::MultiSceneCanvasComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Children canvas at first
    childrenCanvas = new SceneCanvasComponent();
    addAndMakeVisible(childrenCanvas);
    
    // Buttons then
    // A CHANGER
    // AMUSING SEULEMENT ???????????????????????????????????????????????????????
    /*
    sceneChoiceTextButtons.push_back(new TextButton("Default unique scene"));
	sceneChoiceTextButtons.back()->setLookAndFeel(&customLF);
    addAndMakeVisible(sceneChoiceTextButtons.back());
     */
}

MultiSceneCanvasComponent::~MultiSceneCanvasComponent()
{
    delete childrenCanvas;
}


void MultiSceneCanvasComponent::LinkToManager(std::shared_ptr<MultiSceneCanvasInteractor> canvasManager_)
{
    canvasManager = canvasManager_;
    
    childrenCanvas->CompleteInitialization(canvasManager);
}

void MultiSceneCanvasComponent::ReleaseOpengGLResources()
{
	childrenCanvas->TriggerOpengGLResourcesRelease();
	childrenCanvas->waitForOpenGLResourcesReleased();
}



// - - - - - - - - - - - - - - Painting and Resizing - - - - - - - - - - - - - -

void MultiSceneCanvasComponent::paint (Graphics& /*g*/) // unused Graphics context
{
    //g.fillAll (Colours::mediumpurple);   // PURPLE background for debug
}

void MultiSceneCanvasComponent::resized()
{
    // Notif to parent (to maybe trigger pre-computations)
    canvasManager->OnComponentResized(getWidth(), getHeight(), this);
    
    
#ifndef __MIEM_EXPERIMENTS_LATENCY
    // Children display canvas on the bottom in general use,
    childrenCanvas->setSize(getWidth(), getHeight() - SceneButtonsHeight -space); // appelera l'update du canvasinteractor (le manager)
    childrenCanvas->setTopLeftPosition(0, SceneButtonsHeight +space);
#else
    // on the top for touch drag latency experiments (we need areas on the very top of the screen)
    childrenCanvas->setSize(getWidth(), getHeight() - SceneButtonsHeight -space); // appelera l'update du canvasinteractor (le manager)
    childrenCanvas->setTopLeftPosition(0, 0);
#endif
    // Buttons positionning
    updateSceneButtonsBounds();
    
    // Sizes display in MOBILE versions
#if defined(__MIAMOBILE) || defined(__MIEM_DISPLAY_CANVAS_RESIZE)
    std::cout << "Taille MultiSceneCanvasComponent : " << getWidth() << "px x " << getHeight() << "px, Ratio=" << ((double)(getWidth()) / (double)(getHeight())) << std::endl;
#endif
}



// - - - - - - - - - - - - - - - Buttons management - - - - - - - - - - - - - - -
void MultiSceneCanvasComponent::UpdateSceneButtons(std::vector< std::shared_ptr<InteractiveScene> > scenes, int sceneThatHasChanged)
{
    // Update of all buttons
    if (sceneThatHasChanged == -1)
    {
        int selectedSceneId = canvasManager->GetSelectedSceneId();
        
        // At first, we remove all buttons from the parent
        for (size_t i = 0; i<sceneChoiceTextButtons.size() ; i++)
            removeChildComponent(sceneChoiceTextButtons[i].get());
        sceneChoiceTextButtons.resize(0); // actual buttons deletion (scoped ptrs)
        
        // Then we create the new ones
        for (size_t i = 0; i<scenes.size() ; i++)
        {
			// Actual addition
            String sceneName;
#ifndef __MIEM_EXPERIMENTS_FULL_LOCK
            sceneName = scenes[i]->GetName();
#else
            sceneName = CharPointer_UTF8 ("\xc2\xb0"); // °
#endif
            addButton(sceneName.toStdString());
			// Special graphic attributes for the newly selected one
            if (i == (size_t)(selectedSceneId))
            {
                sceneChoiceTextButtons[i]->setColour(TextButton::buttonColourId, Colours::black);
                sceneChoiceTextButtons[i]->setColour(TextButton::textColourOffId, Colours::white);
            }
            else
            {
                sceneChoiceTextButtons[i]->setColour(TextButton::buttonColourId, Colour(0xFF303030));
                sceneChoiceTextButtons[i]->setColour(TextButton::textColourOffId, Colours::lightgrey);
            }
        }
        
        // Graphical updates (minimal)
        updateSceneButtonsBounds();
    }
    // Update of one precise button
    else
    {
        String sceneName;
#ifndef __MIEM_EXPERIMENTS_FULL_LOCK
        sceneName = scenes[sceneThatHasChanged]->GetName();
#else
        sceneName = CharPointer_UTF8 ("\xc2\xb0"); // °
#endif
        sceneChoiceTextButtons[sceneThatHasChanged]->setButtonText(sceneName.toStdString());
    }
}

void MultiSceneCanvasComponent::updateSceneButtonsBounds()
{
    int buttonWidth = int(((float)(getWidth())-(float)(space))/(float)(sceneChoiceTextButtons.size()))-space;
    for (size_t i=0 ; i<sceneChoiceTextButtons.size() ; i++)
    {
        int topPosition;
#ifndef __MIEM_EXPERIMENTS_LATENCY
        // On the top for general use
        topPosition = 0;
#else
        // On the bottom for latency experiments
        topPosition = childrenCanvas->getHeight() + space;
#endif
        sceneChoiceTextButtons[i]->setBounds(space+(int)(i)*(buttonWidth+space), topPosition, buttonWidth, SceneButtonsHeight);
    }
}


void MultiSceneCanvasComponent::addButton(std::string buttonName)
{
    auto uniqueButton = std::unique_ptr<TextButton>(new TextButton(buttonName));
    sceneChoiceTextButtons.push_back(std::move(uniqueButton));
    addAndMakeVisible(sceneChoiceTextButtons.back().get());
    sceneChoiceTextButtons.back()->addListener(this);

	// Solves an issue with touch : some touch events are interpreted (win, mac)
	// as "right clicks" and would not trigger the button...
	sceneChoiceTextButtons.back()->setTriggeredOnMouseDown(true); 
}

void MultiSceneCanvasComponent::buttonClicked(Button* buttonThatWasClicked)
{
    bool buttonFound = false; // else, the buttons could keep selecting themselves ??
    for (size_t i=0 ; i<sceneChoiceTextButtons.size() && !buttonFound ; i++)
    {
        if (buttonThatWasClicked == sceneChoiceTextButtons[i].get())
        {
            buttonFound = true;
#ifndef __MIEM_EXPERIMENTS_FULL_LOCK
            canvasManager->SelectScene((int)i);
#else
            DBG("Clicks on scene buttons are deactivated in __MIEM_EXPERIMENTS mode (TCP control only).");
#endif
        }
    }
    
    if (buttonFound == false)
        // the clicked button could not be found... Where is the event coming from ???
        assert(false);
}

    
