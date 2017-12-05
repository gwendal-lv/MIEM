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
    // Background is automatically visible, after startup though
    addChildComponent(mainBackgroundComponent = new MainBackgroundComponent());
    addChildComponent(startupComponent = new StartupComponent());
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
    
    delete startupComponent;
    delete mainBackgroundComponent;
    
    // Sinon assertion lancée.... Pas sérieux de Juce mais bon...
    setLookAndFeel(nullptr);
}




// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    
    mainBackgroundComponent->CompleteInitialization(presenter);
    startupComponent->CompleteInitialization(presenter);
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
        case AppMode::Startup :
            startupComponent->setVisible(true);
            mainBackgroundComponent->setVisible(false);
            break;
            
        case  AppMode::Loading :
            // all component remains unvisible, only background component is visible
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            break;
            
        case AppMode::EditSpeakers :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
            speakersEditionComponent->setVisible(true);
            break;
            
        case AppMode::EditSpatStates :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
            spatStatesEditionComponent->setVisible(true);
            break;
            
        case AppMode::EditSpatScenes :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
            sceneEditionComponent->setVisible(true);
            break;
        
        case AppMode::EditHardwareConfiguration :
            startupComponent->setVisible(false);
            mainBackgroundComponent->setVisible(true);
            
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
void MainContentComponent::DisplayInfo(const String& message, int priority)
{
    mainBackgroundComponent->DisplayInfo(message, priority);
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
    startupComponent->setBounds(getLocalBounds());
    // Other children are only resized if visible
    int yChildrenOffset = 5*8; // 5 cases depuis le haut (pour l'instant)
    for (size_t i=0;i<secondLevelComponents.size();i++)
    {
        //if (secondLevelComponents[i]->isVisible()) { // EVERYTHING ACTUALIZED, VISIBLE OR NOT
            secondLevelComponents[i]->setBounds(0, yChildrenOffset, getWidth(), getHeight()-yChildrenOffset);
        //}
    }
}


