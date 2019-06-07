/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "AppPurpose.h"
#include "View.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    // Ajout des composants enfant
    backgroundComponent.reset( new PlayerBackgroundComponent() );
    addAndMakeVisible(backgroundComponent.get());
    
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
    g.fillAll ( juce::Colour::fromRGB(48, 48, 48));
}

void MainContentComponent::resized()
{
    if (view)
        backgroundComponent->setBounds( view->GetSafeBackgroundBounds( getLocalBounds() ) );
    else
        backgroundComponent->setBounds( getLocalBounds() );
    
    backgroundComponent->resized(); //  forcé.... ---> ?????????????????????????
}



// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    assert(view != 0);
    backgroundComponent->CompleteInitialization(presenter, view);
}
void MainContentComponent::CompleteInitialization(MultiCanvasComponent* multiCanvasComponent_)
{
    backgroundComponent->CompleteInitialization(multiCanvasComponent_);
}

void MainContentComponent::SetMiamView(Miam::View* _view)
{
    view = _view;
    
}

