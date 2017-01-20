/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "AmusingModel.h"

using namespace Amusing;
using namespace Miam;


//==============================================================================
MainContentComponent::MainContentComponent()
{
	//addAndMakeVisible(sceneEditionComponent = new SceneEditionComponent());
	editSceneC = new editScene();
	addAndMakeVisible(editSceneC);
    setSize (600, 400);
}

MainContentComponent::~MainContentComponent()
{
	//delete sceneEditionComponent;
	delete editSceneC;
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

    if (multiCanvasComponent)
		multiCanvasComponent->setBounds(50, 0, 550, getLocalBounds().getHeight());
	if (editSceneC)
		editSceneC->setBounds(0, 0, 50, getLocalBounds().getHeight());
    //    multiCanvasComponent->setBounds(getLocalBounds());
	//sceneEditionComponent->setBounds(0, 0, 50, getLocalBounds().getHeight());
	//multiCanvasComponent->setBounds(50, 0, 550, getLocalBounds().getHeight());
}




// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
}
void MainContentComponent::CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponent* _multiCanvasComponent)
{
    graphicSessionManager = _graphicSessionManager;
    
    // Add of the canvas to this component's children directly (for now ?)
    multiCanvasComponent = _multiCanvasComponent;
    addAndMakeVisible(multiCanvasComponent);
}
void MainContentComponent::CompleteInitialization(AmusingModel* _model)
{
	model = _model;
	//addAndMakeVisible(model->audioPlayer);
}

void MainContentComponent::SetMiamView(Amusing::View* _view)
{
    view = _view;
    
}

