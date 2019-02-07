/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Expérience 1 : écoute OSC
    oscRecorderComponent.reset(new OSCRecorderComponent());
    addAndMakeVisible(oscRecorderComponent.get());
    
    
    // Size after children are init
    setSize (800, 600);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
}

void MainComponent::resized()
{
    oscRecorderComponent->setBounds(this->getBounds());
}
