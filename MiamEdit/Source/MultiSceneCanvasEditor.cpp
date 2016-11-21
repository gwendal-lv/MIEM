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

MultiSceneCanvasEditor::MultiSceneCanvasEditor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId)
: MultiSceneCanvasInteractor(_graphicSessionManager, _canvasComponent, _selfId)
{
    
    
    // After canvases are created : scenes creation
    // DEFAULT SCENES, TO BE CHANGED
    std::shared_ptr<EditableScene> newScene1(new EditableScene(this, canvasComponent->GetCanvas()));
    scenes.push_back( newScene1 );
    std::shared_ptr<EditableScene> newScene2(new EditableScene(this, canvasComponent->GetCanvas()));
    scenes.push_back( newScene2 );
    std::shared_ptr<EditableScene> newScene3(new EditableScene(this, canvasComponent->GetCanvas()));
    scenes.push_back( newScene3 );
    
    SelectScene(rand()%3);
    
}



// ------ Setters and Getters ------

std::shared_ptr<IEditableArea> MultiSceneCanvasEditor::GetSelectedArea()
{
    if (selectedScene)
        return selectedScene->GetSelectedArea();
    else throw std::runtime_error("Cannot get the selected area : no scene selected on canvas" + std::to_string(selfId));
}



// ------ Areas managing : Add and Delete ------
void MultiSceneCanvasEditor::DeleteSelectedArea()
{
    if (selectedScene)
        selectedScene->DeleteSelectedArea();
    else throw std::runtime_error("Cannot get the selected area : no scene selected on canvas" + std::to_string(selfId));
    
    canvasComponent->repaint();
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
            editableScene->SendSelectedAreaToBack();
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
            editableScene->SendSelectedAreaBackward();
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
            editableScene->BringSelectedAreaForward();
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
            editableScene->BringSelectedAreaToFront();
        else throw std::runtime_error("Cannot bring an area to front : current selected scene is not a Miam::EditableScene");
    }
    else throw std::runtime_error("Cannot bring an area to front : no scene selected");
}
