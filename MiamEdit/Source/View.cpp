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
    
    mainContentComponent->setMiamView(this);
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
    if (name == "textButtonSpeakers")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpeakers);
    else if (name == "textButtonSpeakersGroups")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpeakersGroups);
    else if (name == "textButtonScenes")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpatScenes);
    else if (name == "textButtonHardware")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditHardwareConfiguration);
    
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
