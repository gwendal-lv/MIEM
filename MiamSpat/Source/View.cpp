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
    /*
    AppMode answeredAppMode = AppMode::Null;
    if (name == "Speakers text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpeakers);
    else if (name == "Speakers Groups text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpeakersGroups);
    else if (name == "Scenes text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpatScenes);
    else if (name == "Hardware Configuration text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditHardwareConfiguration);
    else if (name == "Start text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::MiamSpatPlaying);
    */
    
    // This is a behavior : defined in presenter then....
    /*if (answeredAppMode != AppMode::Null)
        mainContentComponent->ChangeAppMode(answeredAppMode);*/
}



void View::ChangeAppMode(AppMode newAppMode)
{
    std::cerr << "[VIEW] Changement de mode à implémenter (mode " << (int)(newAppMode) << ")" << std::endl;
    //throw std::runtime_error("Unimplemented behavior on app mode change");
}
void View::DisplayInfo(const String& message)
{
    mainContentComponent->GetBackgroundComponent()->DisplayInfo(message);
}

void View::SetTitle(std::string title)
{
    mainWindow->setName(title);
}


