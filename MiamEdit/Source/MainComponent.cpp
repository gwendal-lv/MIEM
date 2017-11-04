/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "View.h"
using namespace Miam;


//==============================================================================
MainContentComponent::MainContentComponent()
{
    // We add all big components from here (components edited from the introJucer)
    // Background is automatically visible
    addAndMakeVisible(mainBackgroundComponent = new MainBackgroundComponent());
    // Other display modes aren't displayed by default
    addChildComponent(speakersEditionComponent = new SpeakersEditionComponent());
    addChildComponent(spatStatesEditionComponent = new SpatStatesEditionComponent());
    addChildComponent(sceneEditionComponent = new SceneEditionComponent());
    addChildComponent(hardwareConfigurationComponent = new HardwareConfigurationComponent());
    secondLevelComponents.push_back(speakersEditionComponent);
    secondLevelComponents.push_back(spatStatesEditionComponent);
    secondLevelComponents.push_back(sceneEditionComponent);
    secondLevelComponents.push_back(hardwareConfigurationComponent);
        
    /* On doit préciser une taille (pour donner un repère aux enfants)
     *
     * As the "MainBackground" child's size is specified from the IntroJucer,
     * we can use it to specify its "Main Component" parent's size
     */
    setSize(mainBackgroundComponent->getWidth(),mainBackgroundComponent->getHeight());
    
    
    // Changement du look'n'feel, qui sera répercuté sur tous les enfants
    setLookAndFeel(&lookAndFeel);
}

MainContentComponent::~MainContentComponent()
{
    delete hardwareConfigurationComponent;
    delete sceneEditionComponent;
    delete spatStatesEditionComponent;
    delete speakersEditionComponent;
    
    delete mainBackgroundComponent;
    
    // Sinon assertion lancée.... Pas sérieux de Juce mais bon...
    setLookAndFeel(nullptr);
}




// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    
    mainBackgroundComponent->CompleteInitialization(presenter);
}

void MainContentComponent::SetMiamView(Miam::View* _view)
{
    view = _view;
    
    mainBackgroundComponent->setMiamView(view);
}





void MainContentComponent::ChangeAppMode(AppMode newAppMode)
{
    // Firstly, we hide everything
    for (size_t i=0 ; i<secondLevelComponents.size() ; i++)
    {
        secondLevelComponents[i]->setVisible(false);
    }
    
    
    switch (newAppMode)
    {
        case  AppMode::Loading :
            // all component remains unvisible, and the "loading" label is revealed...
            break;
            
        case AppMode::EditSpeakers :
            speakersEditionComponent->setVisible(true);
            break;
            
        case AppMode::EditSpatStates :
            spatStatesEditionComponent->setVisible(true);
            break;
        case AppMode::EditSpatScenes :
            sceneEditionComponent->setVisible(true);
            break;
        
        case AppMode::EditHardwareConfiguration :
            hardwareConfigurationComponent->setVisible(true);
            break;
            
        case AppMode::MiamSpatPlaying :
            DisplayInfo("Can't launch the external Spat player yet!");
            break;
            
        default :
            break;
    }

    // Dans le doute...
    resized();
}
void MainContentComponent::DisplayInfo(const String& message)
{
    mainBackgroundComponent->DisplayInfo(message);
}





void MainContentComponent::paint (Graphics& /*g*/)
{
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    // Background must always be resized
    mainBackgroundComponent->setBounds(getLocalBounds());
    // Other children are only resized if visible
    int yChildrenOffset = 5*8; // 5 cases depuis le haut (pour l'instant)
    for (size_t i=0;i<secondLevelComponents.size();i++)
    {
        //if (secondLevelComponents[i]->isVisible()) { // EVERYTHING ACTUALIZED, VISIBLE OR NOT
            secondLevelComponents[i]->setBounds(0, yChildrenOffset, getWidth(), getHeight()-yChildrenOffset);
        //}
    }
}


