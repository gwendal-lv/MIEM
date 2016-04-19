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

	/// \brief Function called after both Miam::View and Miam::Presenter are contructed
    void CompleteInitialization(Presenter* _presenter);

    

    void paint (Graphics&) override;
    void resized() override;
    
    
    
    // SETTERS and GETTERS
	/// \brief Necessary for the Miam::View to reference itself, because this class is always
	/// constructed by the MainWindow before the View module.
    void SetMiamView(View* _view);
	/// \brief Pointer to one of MiamEdit's main components.
    SceneEditionComponent* GetSceneEditionComponent()
    {return sceneEditionComponent; }
    
    // To obey orders transmitted from the Presenter to the View
	/// \brief Dispatches event re-transmitted by Miam::View::ChangeAppMode
    void ChangeAppMode(Miam::AppMode newAppMode);
	/// \brief Dispatches event re-transmitted by Miam::View::DisplayInfo
    void DisplayInfo(const String& message);


    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
