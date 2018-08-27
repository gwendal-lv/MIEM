/*
  ==============================================================================

    MultiCanvasComponent.cpp
    Created: 22 Nov 2016 5:40:30pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "MultiCanvasComponent.h"

#include "IGraphicSessionManager.h"



// = = = = = = = = = = GETTERS and SETTERS = = = = = = = = = =

void MultiCanvasComponent::SetBarXPos(int newXPos)
{
    // SÉCURITÉ À IMPLÉMENTER
    // POUR NE PAS DÉBORDER
    // + sortie BOOLEEN si on refuse le déplacement ?
    // plutôt présentateur, pour désactiver proprement un éventuel multicanvas qui a dégagé...
    
    // OU RETOUR DE L'INDICE DU CANEVAS DÉGAGÉ à désactiver
    normalizedBarXPos = newXPos / (float)(getWidth());
    
    // Actualisations graphiques
    resized();
    repaint();
}



// = = = = = = = = = = METHODS = = = = = = = = = =


MultiCanvasComponent::MultiCanvasComponent(IGraphicSessionManager* _graphicSessionManager)
{
    graphicSessionManager = _graphicSessionManager;
    
    // Initial width and height
    canvasResizeBar.setWidth(10.0f);
    canvasResizeBar.setHeight(10.0f); // to be updated
    Rectangle<float> diamond(10.0f, 10.0f);
    canvasResizeDiamond.addRectangle(diamond);
    canvasResizeDiamondTransform = AffineTransform::rotation( float_Pi / 2.0f );

}

void MultiCanvasComponent::paint(Graphics& g)
{
    // For debug
    //g.fillAll(Colours::chartreuse);
    
    if (multiSceneCanvasComponents.size() == 2)
    {
        g.setColour(Colour (0x33000000)); // comme les boutons dans l'éditeur
        g.fillRoundedRectangle(canvasResizeBar, 3.0f);
        g.fillPath(canvasResizeDiamond, canvasResizeDiamondTransform);
    }
}


void MultiCanvasComponent::resized()
{
    // - - - 1 canvas -> centered with margin - - -
    if (multiSceneCanvasComponents.size() == 1)
    {
        Rectangle<int> rCanvas = getLocalBounds();
        rCanvas.reduce(8, 8);
        multiSceneCanvasComponents[0]->setBounds(rCanvas);
    }
    
    // - - - 2 canvases -> resizable canvases with movable vertical bar - - -
    else if (multiSceneCanvasComponents.size() == 2)
    {
        // D'abord on place la barre (unique pour l'instant)
        canvasResizeBar.setHeight((float)(getHeight())/2.0f);
        canvasResizeBar.setY(canvasResizeBar.getHeight()/2.0f);
        canvasResizeBar.setX(normalizedBarXPos*(float)(getWidth())-canvasResizeBar.getWidth()/2.0f);
        // Puis le diamant (double flèche...) au milieu
        Rectangle<float> diamond(16.0f, 16.0f);
        diamond.setCentre(canvasResizeBar.getCentre());
        Path p; // empty path
        p.addRoundedRectangle(diamond, 3.0);
        canvasResizeDiamond = p;
        canvasResizeDiamondTransform = AffineTransform::rotation(float_Pi / 4.0f, diamond.getCentreX(), diamond.getCentreY());
        
        // Code de placement pour 2 canevas avec barre de déplacement
        for (size_t i=0 ; i<multiSceneCanvasComponents.size() ; i++)
        {
            Rectangle<int> rCanvas = getLocalBounds();
            rCanvas.removeFromTop(8);
            rCanvas.removeFromBottom(8);
            if (i == SceneCanvasComponent::Canvas1)
            {
                rCanvas.removeFromLeft(8);
                rCanvas.removeFromRight((getWidth()-juce::roundToInt(canvasResizeBar.getX())) +8);
                multiSceneCanvasComponents[i]->setBounds(rCanvas);
            }
            else if (i == SceneCanvasComponent::Canvas2)
            {
                // Relative to the first canvas
                rCanvas.removeFromRight(8);
                rCanvas.removeFromLeft(juce::roundToInt(canvasResizeBar.getRight()) +8);
                multiSceneCanvasComponents[i]->setBounds(rCanvas);
            }
        }
    }
    
    else if (multiSceneCanvasComponents.size() >=3 )
        throw std::runtime_error("Cannot currently do anything with more than 2 canvases");
}

void MultiCanvasComponent::CompleteInitialization()
{
    for (size_t i=0 ; i<multiSceneCanvasComponents.size() ; i++)
    {
        // Access to private member vector because of friendship
        multiSceneCanvasComponents[i]->LinkToManager(graphicSessionManager->canvasManagers[i]);
    }
}

void MultiCanvasComponent::ReleaseOpengGLResources()
{
	for (size_t i = 0; i<multiSceneCanvasComponents.size(); i++)
	{
		multiSceneCanvasComponents[i]->ReleaseOpengGLResources();
	}
}

// - - - - - Mouse events re-transmission to Graphic Session Manager - - - - -

void MultiCanvasComponent::mouseDown (const MouseEvent &event)
{
    graphicSessionManager->OnBackgroundMouseDown(event);
}
void MultiCanvasComponent::mouseDrag (const MouseEvent &event)
{
    graphicSessionManager->OnBackgroundMouseDrag(event);
}
void MultiCanvasComponent::mouseUp (const MouseEvent &event)
{
    graphicSessionManager->OnBackgroundMouseUp(event);
}




// - - - - - Canvases & canvas group - - - - -
MultiSceneCanvasComponent* MultiCanvasComponent::AddCanvas()
{
    multiSceneCanvasComponents.push_back(new MultiSceneCanvasComponent());
    addAndMakeVisible(multiSceneCanvasComponents.back());
    return multiSceneCanvasComponents.back();
}



