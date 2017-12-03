/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "SceneEditionComponent.h"


// Presenter
#include "Presenter.h"
#include "AppMode.h"

#include "MultiCanvasComponent.h"
#include "editScene.h"
#include "SoundFilesManager.h"
#include "SoundBrowser.h"
//#include "AudioManager.h"
//#include "AudioPlayer.h"

using namespace Miam;

class SoundBrowser;
class SoundFilesManager;
class MultiCanvasComponentAmusing;

namespace Amusing
{
    class Presenter; // cross-inclusion issue
    class View; // cross-inclusion issue
	class AmusingModel;
	class GraphicSessionManager;
	class OptionWindow;
}
using namespace Amusing;
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
    
    // Links to sub-modules
	Amusing::GraphicSessionManager* graphicSessionManager = 0;
    
    // Graphical component
    MultiCanvasComponentAmusing* multiCanvasComponent = 0; // belongs to the presenter
	ScopedPointer<	EditScene> editSceneC;
	//SceneEditionComponent* sceneEditionComponent;
    
	// Audio component
	//AudioPlayer* audioPlayer = 0; // belongs to the model
	AmusingModel* model;
    
    // ============== SETTERS AND GETTERS ==============
    public :
    /// \brief Function called after both Miam::View and Miam::Presenter are contructed
    void CompleteInitialization(Presenter* _presenter);
    /// \brief Function called after both View and Presenter are contructed
    void CompleteInitialization(GraphicSessionManager*, MultiCanvasComponentAmusing*);
	/// \brief Function called after both View, Presenter and Model are constructed
	void CompleteInitialization(AmusingModel* _model);
    
    /// \brief Necessary for the Miam::View to reference itself, because this class is always
    /// constructed by the MainWindow before the View module.
    void SetMiamView(View* _view);
    
    EditScene* GetSceneEditionComponent()
	{
		return editSceneC;
	}
    
	void ShowDeviceOptionsDialog();
	void ShowSoundManagerComponent();
	void CloseOptionWindow();
	void CloseSoundFileManager();
	void OpenSoundBrowser(int idx,Colour concernedColor);
	void CloseSoundBrowser(String m_path);
	void removeDeviceManagerFromOptionWindow();

	void setSamplesColor(const int numSamples, Colour colorCode[]);
	void setDefaultPath(String m_defaultPath);
	//void CreateDeviceSelector(AudioDeviceManager* deviceManager);
    
    // = = = = = = = = = = METHODS (JUCE AND USER-DEFINED) = = = = = = = = = =
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
	//SafePointer<AudioDeviceSelectorComponent> audioSetupComp;
	ScopedPointer<OptionWindow> optionWindow;
	ScopedPointer<SoundFilesManager> soundFilesManager;
	ScopedPointer<SoundBrowser> soundBrowser;
	Colour colorToAssociate;
	int idxToAssociate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
