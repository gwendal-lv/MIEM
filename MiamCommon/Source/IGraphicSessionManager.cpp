/*
  ==============================================================================

    IGraphicSessionManager.cpp
    Created: 21 Nov 2016 12:10:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "IGraphicSessionManager.h"

#include <cmath>

using namespace Miam;



IGraphicSessionManager::IGraphicSessionManager() :
    multiCanvasComponent(new MultiCanvasComponent(this)) // first init of shared_ptr
{
    
}


IGraphicSessionManager::~IGraphicSessionManager()
{
    delete multiCanvasComponent;
    
    for(int i=0 ; i<canvasManagers.size(); i++)
        delete canvasManagers[i];
}




// - - - - - Mouse Events - - - - -
void IGraphicSessionManager::OnBackgroundMouseDown(const MouseEvent &event)
{
    if (event.source.getIndex() == 0) // mouse or first touch only
    {
        if ( abs(event.getPosition().getX() - multiCanvasComponent->GetBarXPos()) < 10 )
        {// 10 px de marge des 2 côtés pour chopper la barre
            mouseResizingCanvas = true;
        }
    }
}
void IGraphicSessionManager::OnBackgroundMouseDrag(const MouseEvent &event)
{
    if (mouseResizingCanvas)
    {
        if (event.source.getIndex() == 0) // mouse or first touch only
        {
            // FOR NOW, A CANVAS HAS 100PX MINIMUM SIZE
            const int xPos = event.getPosition().getX();
            if ( xPos > 116
                && xPos < (multiCanvasComponent->getWidth()-116))
            multiCanvasComponent->SetBarXPos(event.getPosition().getX());
        }
    }
}
void IGraphicSessionManager::OnBackgroundMouseUp(const MouseEvent& /* event */)
{
    mouseResizingCanvas = false;
}
