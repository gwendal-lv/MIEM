/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "AmusingModel.h"
#include "MultiCanvasComponentAmusing.h"
#include "OptionWindow.h"
using namespace Amusing;
using namespace Miam;


//==============================================================================
MainContentComponent::MainContentComponent()
{
	//addAndMakeVisible(sceneEditionComponent = new SceneEditionComponent());
	editSceneC = new EditScene();
	soundFilesManager = new SoundFilesManager();
	addChildComponent(soundFilesManager);
	addAndMakeVisible(editSceneC);
    setSize (600, 400);
	soundFilesManager->completeInitialisation(this);
}

MainContentComponent::~MainContentComponent()
{
	DBG("MainContentComponent destructor");
	//delete sceneEditionComponent;
	//delete editSceneC;
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	int toolbarWidth = getLocalBounds().getWidth() / 12;
	if (multiCanvasComponent)
	{
		multiCanvasComponent->setBounds(toolbarWidth, 0, getLocalBounds().getWidth()-toolbarWidth, getLocalBounds().getHeight());
	}
	if (editSceneC)
		editSceneC->setBounds(0, 0, toolbarWidth, getLocalBounds().getHeight());
	if (soundFilesManager)
		soundFilesManager->setBounds(toolbarWidth, 0, getLocalBounds().getWidth() - toolbarWidth, getLocalBounds().getHeight());
    //    multiCanvasComponent->setBounds(getLocalBounds());
	//sceneEditionComponent->setBounds(0, 0, 50, getLocalBounds().getHeight());
	//multiCanvasComponent->setBounds(50, 0, 550, getLocalBounds().getHeight());
}




// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
}
void MainContentComponent::CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponentAmusing* _multiCanvasComponent)
{
    graphicSessionManager = _graphicSessionManager;
    
    // Add of the canvas to this component's children directly (for now ?)
    multiCanvasComponent = _multiCanvasComponent;
    addAndMakeVisible((MultiCanvasComponent*)multiCanvasComponent);
}
void MainContentComponent::CompleteInitialization(AmusingModel* _model)
{
	model = _model;

	/*audioSetupComp = new AudioDeviceSelectorComponent(model->getAudioDeviceManager(),
		0, 256, 0, 256, true, true, true, false);*/
	optionWindow = new OptionWindow(this);
	optionWindow->CompleteInitialization(model->sharedAudioDeviceManager);//getAudioDeviceManager());
	//addAndMakeVisible(model->audioPlayer);
}

void MainContentComponent::SetMiamView(Amusing::View* _view)
{
    view = _view;
    
}

void MainContentComponent::ShowDeviceOptionsDialog()
{
	addAndMakeVisible(optionWindow);//addAndMakeVisible(audioSetupComp);
	multiCanvasComponent->setVisible(false);
	optionWindow->setBounds(50, 0, 550, getLocalBounds().getHeight());//audioSetupComp->setBounds(50, 0, 550, getLocalBounds().getHeight());
	optionWindow->setVisible(true);//audioSetupComp->setVisible(true);
	//multiCanvasComponent->ShowDeviceOptionsDialog(deviceManager);
}

void MainContentComponent::ShowSoundManagerComponent()
{
	multiCanvasComponent->setVisible(false);
	optionWindow->setVisible(false);
	soundFilesManager->setVisible(true);
}

void MainContentComponent::CloseOptionWindow()
{
	multiCanvasComponent->setVisible(true);
	optionWindow->setVisible(false);
}

void MainContentComponent::CloseSoundFileManager()
{
	multiCanvasComponent->setVisible(true);
	soundFilesManager->setVisible(false);
}

void MainContentComponent::removeDeviceManagerFromOptionWindow()
{

	optionWindow->removeDeviceManager();
}

//void MainContentComponent::CreateDeviceSelector(AudioDeviceManager* deviceManager)
//{
//	multiCanvasComponent->CreateDeviceSelector(deviceManager);
//}
