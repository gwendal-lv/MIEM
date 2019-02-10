/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // - - - - Instanciation des contenus graphiques - - - - -
    
    // label d'info à l'arrache
    backLabel.reset(new Label("back label", "Initialisation..."));
    addAndMakeVisible(backLabel.get());
    backLabel->setColour(Label::ColourIds::textColourId, Colours::white);
    expeLabel.reset(new Label("expe label 1", ""));
    addAndMakeVisible(expeLabel.get());
    expeLabel->setColour(Label::ColourIds::textColourId, Colours::grey);
    expeLabel2.reset(new Label("expe label 2", ""));
    addAndMakeVisible(expeLabel2.get());
    expeLabel2->setColour(Label::ColourIds::textColourId, Colours::grey);
    
    // components génériques
    firstUserQuestionsComponent.reset(new UserQuestions());
    addChildComponent(firstUserQuestionsComponent.get());
    finalUserQuestionsComponent.reset(new UserFinalQuestions());
    addChildComponent(finalUserQuestionsComponent.get());
    
    // Expérience 1 : écoute OSC
    oscRecorderComponent.reset(new OSCRecorderComponent());
    addChildComponent(oscRecorderComponent.get());
    
    
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
    backLabel->setBounds(16, 16, getWidth()-32, 24);
    
    expeLabel->setBounds(16, getHeight() - 24 - 16 - 24, getWidth()-32, 24);
    expeLabel2->setBounds(16, getHeight() - 24 - 16, getWidth()-32, 24);
    
    oscRecorderComponent->setBounds(this->getBounds());
    firstUserQuestionsComponent->setBounds(this->getBounds());
    finalUserQuestionsComponent->setBounds(this->getBounds());
}
