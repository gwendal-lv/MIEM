/*
  ==============================================================================

    MultiSceneCanvasEditor.cpp
    Created: 20 Nov 2016 11:06:18am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MultiSceneCanvasEditor.h"
#include "MultiSceneCanvasComponent.h"

#include "EditableScene.h"

using namespace std;



MultiSceneCanvasEditor::MultiSceneCanvasEditor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId)
: MultiSceneCanvasInteractor(_graphicSessionManager, _canvasComponent, _selfId)
{
    
}

MultiSceneCanvasEditor::~MultiSceneCanvasEditor()
{    
}



// ------ Setters and Getters ------

std::shared_ptr<IEditableArea> MultiSceneCanvasEditor::GetSelectedArea()
{
    if (selectedScene)
        return selectedScene->GetSelectedArea();
    else throw std::runtime_error("Cannot get the selected area : no scene selected on canvas" + juce::String(selfId).toStdString());
}

void MultiSceneCanvasEditor::SetSelectedSceneName(std::string _name)
{
    selectedScene->SetName(_name);
    canvasComponent->UpdateSceneButtons(GetInteractiveScenes(), GetSelectedSceneId());
}

std::vector<std::shared_ptr<EditableScene>>::iterator MultiSceneCanvasEditor::GetSelectedSceneIt()
{
    auto it = std::find(scenes.begin(), scenes.end(), selectedScene);
    if (it == scenes.end())
        throw std::runtime_error("No selected scene : cannot return an iterator to it");
    else
        return it;
}



// ------ Scenes managing ------

bool MultiSceneCanvasEditor::MoveSelectedSceneTowardsFirst()
{
    // We cannot move the first scene
    if (selectedScene == scenes.front())
        return false;
    else
    {
        // Iterators swap (this one and the one before)
        auto selectedSceneIt = GetSelectedSceneIt();
        std::iter_swap(selectedSceneIt, selectedSceneIt-1);
        
        // Updates and return
        canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
        return true;
    }
}
bool MultiSceneCanvasEditor::MoveSelectedSceneTowardsLast()
{
    // We cannot move the last scene
    if (selectedScene == scenes.back())
        return false;
    else
    {
        // Iterators swap (this one and the one before)
        auto selectedSceneIt = GetSelectedSceneIt();
        std::iter_swap(selectedSceneIt, selectedSceneIt+1);
        
        // Updates and return
        canvasComponent->UpdateSceneButtons(GetInteractiveScenes());
        return true;
    }
}



// ------ Areas managing : Add and Delete ------
void MultiSceneCanvasEditor::AddArea(std::shared_ptr<IEditableArea> newArea)
{
    handleAndSendAreaEventSync(selectedScene->AddArea(newArea));
}
void MultiSceneCanvasEditor::AddDefaultArea(uint64_t nextAreaId)
{
    handleAndSendAreaEventSync(selectedScene->AddDefaultArea(nextAreaId));
}
void MultiSceneCanvasEditor::SetSelectedArea(std::shared_ptr<IEditableArea> newSelectedArea)
{
    selectedScene->SetSelectedArea(newSelectedArea);
    // we don't look to what's been selected... total refresh instead
    // (optimization to be made here !!)
    recreateAllAsyncDrawableObjects();
}
void MultiSceneCanvasEditor::DeleteSelectedArea()
{
    if (selectedScene)
        handleAndSendAreaEventSync(selectedScene->DeleteSelectedArea());
    else throw std::runtime_error("Cannot get the selected area : no scene selected on canvas" + juce::String(selfId).toStdString());
    
    //canvasComponent->repaint(); // useless with OpenGL
}



// ---------- Events from a canvas component (from View) ----------

void MultiSceneCanvasEditor::OnNewColour(Colour colour)
{
    if (selectedScene)
    {
        auto editableScene = std::dynamic_pointer_cast<EditableScene>(selectedScene);
        if (editableScene)
            editableScene->ChangeSelectedAreaColour(colour);
        else throw std::runtime_error("Cannot change colour : current selected scene is not a Miam::EditableScene");
    }
    else throw std::runtime_error("The given colour cannot be applied : no scene is selected");
}

void MultiSceneCanvasEditor::OnSendToBack()
{
    if (selectedScene)
    {
        auto editableScene = std::dynamic_pointer_cast<EditableScene>(selectedScene);
        if (editableScene)
        {
            editableScene->SendSelectedAreaToBack();
            recreateAllAsyncDrawableObjects();
        }
        else throw std::runtime_error("Cannot send an area to back : current selected scene is not a Miam::EditableScene");
    }
    else throw std::runtime_error("Cannot send an area to back : no scene selected");
}

void MultiSceneCanvasEditor::OnSendBackward()
{
    if (selectedScene)
    {
        auto editableScene = std::dynamic_pointer_cast<EditableScene>(selectedScene);
        if (editableScene)
        {
            editableScene->SendSelectedAreaBackward();
            recreateAllAsyncDrawableObjects();
        }
        else throw std::runtime_error("Cannot send an area backward : current selected scene is not a Miam::EditableScene");
    }
    else throw std::runtime_error("Cannot send an area backward : no scene selected");
}
void MultiSceneCanvasEditor::OnBringForward()
{
    if (selectedScene)
    {
        auto editableScene = std::dynamic_pointer_cast<EditableScene>(selectedScene);
        if (editableScene)
        {
            editableScene->BringSelectedAreaForward();
            recreateAllAsyncDrawableObjects();
        }
        else throw std::runtime_error("Cannot bring an area forward : current selected scene is not a Miam::EditableScene");
    }
    else throw std::runtime_error("Cannot bring an area forward : no scene selected");
}
void MultiSceneCanvasEditor::OnBringToFront()
{
    if (selectedScene)
    {
        auto editableScene = std::dynamic_pointer_cast<EditableScene>(selectedScene);
        if (editableScene)
        {
            editableScene->BringSelectedAreaToFront();
            recreateAllAsyncDrawableObjects();
        }
        else throw std::runtime_error("Cannot bring an area to front : current selected scene is not a Miam::EditableScene");
    }
    else throw std::runtime_error("Cannot bring an area to front : no scene selected");
}

