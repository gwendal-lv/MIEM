/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "AppPurpose.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    // Ajout des composants enfant
    addAndMakeVisible(backgroundComponent = new PlayerBackgroundComponent());
    
    setSize (600, 400);
    
    setLookAndFeel(&lookAndFeel);
}

MainContentComponent::~MainContentComponent()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainContentComponent::resized()
{
    backgroundComponent->setBounds(getLocalBounds());
    backgroundComponent->resized(); //  forcé.... ---> ?????????????????????????
}



// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    backgroundComponent->CompleteInitialization(presenter);
}
void MainContentComponent::CompleteInitialization(MultiCanvasComponent* multiCanvasComponent_)
{
    backgroundComponent->CompleteInitialization(multiCanvasComponent_);
}

void MainContentComponent::SetMiamView(Miam::View* _view)
{
    view = _view;
    
}

