/*
  ==============================================================================

    View.cpp
    Created: 11 Mar 2016 5:22:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "View.h"

#include "Presenter.h"

using namespace Miam;


// Default constructor
View::View(DocumentWindow* mainWindow_, MainContentComponent* mainContentComponent_)
:
PlayerView(mainWindow_, mainContentComponent_->GetBackgroundComponent())
{
    mainContentComponent = mainContentComponent_;
    
    mainContentComponent->SetMiamView(this);
}


// Default destructor
View::~View()
{
}

// Called by the Presenter during its own construction, after the View is constructed
// ?? Règle précise pour appeler les "compléments d'initialisation" ??
//
void View::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    
    mainContentComponent->CompleteInitialization(presenter);
}
void View::CompleteInitialization(GraphicSessionPlayer* _graphicSessionManager, MultiCanvasComponent* _multiCanvasComponent)
{
    mainContentComponent->CompleteInitialization(_graphicSessionManager, _multiCanvasComponent);
}


void View::ForceResized()
{
    mainContentComponent->resized();
}



void View::ButtonClicked(const String& /*name*/)
{
    throw std::runtime_error("Unimplemented behavior on button click");
}



