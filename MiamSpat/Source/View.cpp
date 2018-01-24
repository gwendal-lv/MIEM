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
SpatView(mainWindow_)
{
    mainWindow = mainWindow_;
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
void View::CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponent* _multiCanvasComponent)
{
    mainContentComponent->CompleteInitialization(_graphicSessionManager, _multiCanvasComponent);
}



void View::ButtonClicked(const String& /*name*/)
{
    throw std::runtime_error("Unimplemented behavior on button click");
}



void View::ChangeAppMode(AppMode newAppMode)
{
    // Pas de traitement global ! On laisse toujours le fond visible.
    // Retransmission simple au composant le + concerné....
    if (mainContentComponent)
        if (mainContentComponent->GetBackgroundComponent())
            mainContentComponent->GetBackgroundComponent()->ChangeAppMode(newAppMode);
}
void View::DisplayInfo(const String& message)
{
    mainContentComponent->GetBackgroundComponent()->DisplayInfo(message);
}



