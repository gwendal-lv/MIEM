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
View::View(MainContentComponent* _mainContentComponent)
{
    mainContentComponent = _mainContentComponent;
    
    mainContentComponent->SetMiamView(this);
}


// Default destructor
View::~View()
{
}

// called after View construction
void View::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    
    mainContentComponent->CompleteInitialization(presenter);
}



void View::ButtonClicked(const String& name)
{    
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
    
    // This is a behavior : defined in presenter then....
    /*if (answeredAppMode != AppMode::Null)
        mainContentComponent->ChangeAppMode(answeredAppMode);*/
}



void View::ChangeAppMode(AppMode newAppMode)
{
    mainContentComponent->ChangeAppMode(newAppMode);
}
void View::DisplayInfo(const String& message)
{
    mainContentComponent->DisplayInfo(message);
}
