/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (600, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    if (multiCanvasComponent)
        multiCanvasComponent->setBounds(getLocalBounds());
}




// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
}
void MainContentComponent::CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponent* _multiCanvasComponent)
{
    graphicSessionManager = _graphicSessionManager;
    
    // Add of the canvas to this component's children directly (for now ?)
    multiCanvasComponent = _multiCanvasComponent;
    addAndMakeVisible(multiCanvasComponent);
}

void MainContentComponent::SetMiamView(Miam::View* _view)
{
    view = _view;
    
}

