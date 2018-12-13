/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include <string>

//#include "MultiCanvasComponent.h"

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
	backgroundComponent = nullptr;

    setLookAndFeel(nullptr);
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText (std::string(ProjectInfo::projectName) + " " + std::to_string(ProjectInfo::versionNumber),
                getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    backgroundComponent->setBounds(getLocalBounds());
    backgroundComponent->resized(); //  forcé....
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

