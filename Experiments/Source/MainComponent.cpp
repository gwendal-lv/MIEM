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
    backLabel.reset(new Label("back label", TRANS("Initialisation... Please check console for errors if program is not responding.")));
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
    bigGuiComponents.push_back(firstUserQuestionsComponent.get());
    finalUserQuestionsComponent.reset(new UserFinalQuestions());
    addChildComponent(finalUserQuestionsComponent.get());
    bigGuiComponents.push_back(finalUserQuestionsComponent.get());
    
    // Expérience 1 : écoute OSC
    oscRecorderIntroComponent.reset(new OSCRecorderIntroComponent());
    addChildComponent(oscRecorderIntroComponent.get());
    bigGuiComponents.push_back(oscRecorderIntroComponent.get());
    
    oscRecorderComponent.reset(new OSCRecorderComponent());
    addChildComponent(oscRecorderComponent.get());
    // self-managed !
    //bigGuiComponents.push_back(oscRecorderComponent.get());
    
    
    // Size after children are init
    setSize (1250, 830);
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
    oscRecorderIntroComponent->setBounds(this->getBounds());
    firstUserQuestionsComponent->setBounds(this->getBounds());
    finalUserQuestionsComponent->setBounds(this->getBounds());
    
    
}


void MainComponent::SetOneGuiComponentVisible(Component* component)
{
    for (size_t i=0 ; i<bigGuiComponents.size() ; i++)
    {
        if (bigGuiComponents[i] == component)
            bigGuiComponents[i]->setVisible(true);
        else
            bigGuiComponents[i]->setVisible(false);
    }
}


