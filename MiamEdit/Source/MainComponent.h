/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// View
#include "MainBackgroundComponent.h"
#include "SpeakersEditionComponent.h"
#include "SpeakersGroupsEditionComponent.h"
#include "SceneEditionComponent.h"
#include "HardwareConfigurationComponent.h"

// Presenter
#include "Presenter.h"
#include "AppMode.h"


using namespace Miam;
namespace Miam
{
    class Presenter; // cross-inclusion issue
}



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public Component
{
private:
    View* view;
    Presenter* presenter;
    
    
    MainBackgroundComponent* mainBackgroundComponent;
    
    SpeakersGroupsEditionComponent* speakersGroupsEditionComponent;
    SpeakersEditionComponent* speakersEditionComponent;
    SceneEditionComponent* sceneEditionComponent;
    HardwareConfigurationComponent* hardwareConfigurationComponent;
    std::vector<Component*> secondLevelComponents;
    
    
    
    
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
    
    void CompleteInitialization(Presenter* _presenter);

    

    void paint (Graphics&);
    void resized();
    
    
    
    // SETTERS and GETTERS
    void setMiamView(View* _view);
    SceneEditionComponent* getSceneEditionComponent()
    {return sceneEditionComponent; }
    
    // To obey orders transmitted from the Presenter to the View
    void ChangeAppMode(Miam::AppMode newAppMode);
    void DisplayInfo(const String& message);


    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
