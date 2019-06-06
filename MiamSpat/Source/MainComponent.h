/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// Presenter
#include "Presenter.h"
#include "PlayerAppMode.h"



// View
#include "MiamLookAndFeel.h"
#include "PlayerBackgroundComponent.h"


using namespace Miam;
namespace Miam
{
    class Presenter; // cross-inclusion issue
    class View; // cross-inclusion issue
    class MultiCanvasComponent;
}



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
private:
    // Links back to modules
    View* view = 0;
    Presenter* presenter = 0;
    
    // Graphical component
    std::unique_ptr<PlayerBackgroundComponent> backgroundComponent;
    //MultiCanvasComponent* multiCanvasComponent = 0; // belongs to the presenter
    
    // Display attributes
    MiamLookAndFeel lookAndFeel;
    
    // ============== SETTERS AND GETTERS ==============
    public :
    /// \brief Function called after both Miam::View and Miam::Presenter are contructed
    void CompleteInitialization(Presenter* _presenter);
    /// \brief Function called after both View and Presenter are contructed
    void CompleteInitialization(MultiCanvasComponent*);
    
    /// \brief Necessary for the Miam::View to reference itself, because this class is always
    /// constructed by the MainWindow before the View module.
    void SetMiamView(View* _view);
    
    PlayerBackgroundComponent* GetBackgroundComponent() const {return backgroundComponent.get(); }
    
    
    // = = = = = = = = = = METHODS (JUCE AND USER-DEFINED) = = = = = = = = = =
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
