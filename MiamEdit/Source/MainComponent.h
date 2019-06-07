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
#include "StartupComponent.h"
#include "SpeakersEditionComponent.h"
#include "SpatStatesEditionComponent.h"
#include "SceneEditionComponent.h"
#include "HardwareConfigurationComponent.h"

#include "MiamLookAndFeel.h"

// Presenter
#include "Presenter.h"
#include "AppMode.h"


using namespace Miam;
namespace Miam
{
    // forward declarations
    class Presenter;



    
    //==============================================================================
    /*
     This component lives inside our window, and this is where you should put all
     your controls and content.
     */
    
    /// \brief Parent component of all others.
    class MainContentComponent : public Component
    {
    private:
        View* view;
        Presenter* presenter;
        
        std::unique_ptr<TooltipWindow> tooltipWindow;
        
        MainBackgroundComponent* mainBackgroundComponent;
        StartupComponent* startupComponent;
        
        SpatStatesEditionComponent* spatStatesEditionComponent;
        SpeakersEditionComponent* speakersEditionComponent;
        SceneEditionComponent* sceneEditionComponent;
        HardwareConfigurationComponent* hardwareConfigurationComponent;
        std::vector<Component*> secondLevelComponents;
        
        MiamLookAndFeel lookAndFeel;
        
        
    public:
        //==============================================================================
        MainContentComponent();
        ~MainContentComponent();
        
        /// \brief Function called after both Miam::View and Miam::Presenter are contructed
        void CompleteInitialization(Presenter* _presenter);
        
        
        
        void paint (Graphics&) override;
        void resized() override;
        
        // Events might be called directly from children similar events (if unused)
        bool keyPressed(const KeyPress& key) override;
        void modifierKeysChanged (const ModifierKeys& modifiers) override;
        
        
        // SETTERS and GETTERS
        
        /// \brief Necessary for the Miam::View to reference itself, because this class is always
        /// constructed by the MainWindow before the View module.
        void SetMiamView(View* _view);
        /// \brief Pointer to one of MiamEdit's main components.
        SceneEditionComponent* GetSceneEditionComponent()
        {return sceneEditionComponent; }
        SpatStatesEditionComponent* GetSpatStatesEditionComponent()
        {return spatStatesEditionComponent;}
        HardwareConfigurationComponent* GetConfigurationComponent()
        {return hardwareConfigurationComponent;}
        
        // To obey orders transmitted from the Presenter to the View
        /// \brief Dispatches event re-transmitted by Miam::View::ChangeAppMode
        void ChangeAppMode(Miam::AppMode newAppMode);
        /// \brief Dispatches event re-transmitted by Miam::View::DisplayInfo
        void DisplayInfo(const String& message, int priority = 0);
        
        
        
        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    };

    
}    // namespace Miam

#endif  // MAINCOMPONENT_H_INCLUDED
