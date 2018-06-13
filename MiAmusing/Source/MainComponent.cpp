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

	addAndMakeVisible(editSceneC);
    setSize (600, 400);
	
	soundBrowser = new SoundBrowser();
	soundBrowser->completeInitialisation(this);
	addChildComponent(soundBrowser);
}

MainContentComponent::~MainContentComponent()
{
	DBG("MainContentComponent destructor");
	//delete sceneEditionComponent;
	//delete editSceneC;
	soundFilesManager->release();
	soundBrowser->release();
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
   // g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
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
	if(soundBrowser)
		soundBrowser->setBounds(toolbarWidth, 0, getLocalBounds().getWidth() - toolbarWidth, getLocalBounds().getHeight());
	if (optionWindow)
		optionWindow->setBounds(toolbarWidth, 0, getLocalBounds().getWidth() - toolbarWidth, getLocalBounds().getHeight());
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

std::shared_ptr<bptree::ptree> MainContentComponent::GetSoundTree()
{
	return soundFilesManager->GetSoundTree();
}

void MainContentComponent::setSoundSettings(bptree::ptree tree)
{
	soundFilesManager->SetSoundTree(tree);
}

void MainContentComponent::ShowDeviceOptionsDialog()
{
	optionWindow->saveAudioDeviceCurrentState();
	addAndMakeVisible(optionWindow);//addAndMakeVisible(audioSetupComp);
	multiCanvasComponent->setVisible(false);
	optionWindow->setBounds(getLocalBounds().getWidth() / 12, 0, getLocalBounds().getWidth() - (getLocalBounds().getWidth() / 12), getLocalBounds().getHeight());
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

void MainContentComponent::OpenSoundBrowser(int idx, Colour concernedColour)//Colour concernedColor)
{
	colorToAssociate = concernedColour;// concernedColor;
	idxToAssociate = idx;
	soundFilesManager->setVisible(false);
	soundBrowser->setVisible(true);
}

void MainContentComponent::CloseSoundBrowser(String m_path)
{
	if (m_path != "")
	{
		soundFilesManager->setCurrentSoundFilePath(m_path);
		presenter->setColorPath(idxToAssociate, colorToAssociate, m_path);
	}
	soundFilesManager->setVisible(true);
	soundBrowser->setVisible(false);
}

void MainContentComponent::addColourPath(int idx, Colour colour, String path)
{
	presenter->setColorPath(idx, colour, path);
	multiCanvasComponent->addColourSample(idx, colour);
}

void MainContentComponent::removeDeviceManagerFromOptionWindow()
{

	optionWindow->removeDeviceManager();
}

void MainContentComponent::setSamplesColor(const int numSamples, Colour colorCode[])
{
	soundFilesManager = new SoundFilesManager(numSamples,colorCode);
	addChildComponent(soundFilesManager);
	soundFilesManager->completeInitialisation(this);
	resized();
}

void MainContentComponent::setDefaultPath(String m_defaultPath)
{
	soundFilesManager->setDefaultPath(m_defaultPath);
}

//void MainContentComponent::CreateDeviceSelector(AudioDeviceManager* deviceManager)
//{
//	multiCanvasComponent->CreateDeviceSelector(deviceManager);
//}
