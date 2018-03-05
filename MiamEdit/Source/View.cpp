/*
  ==============================================================================

    View.cpp
    Created: 11 Mar 2016 5:22:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <string>
#include <regex>

#include "View.h"

#include "Presenter.h"

using namespace Miam;


// Default constructor
View::View(DocumentWindow* mainWindow_, MainContentComponent* _mainContentComponent)
:
ControlView(mainWindow_)
{
    mainContentComponent = _mainContentComponent;
    
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




// ========== Setters and Getters ==========

AppPurpose View::GetSessionPurpose()
{
    return presenter->GetSessionPurpose();
}





// ========== METHODS ==========

void View::ButtonClicked(const String& name)
{    
    AppMode answeredAppMode = AppMode::Null;
    if (name == "Speakers text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpeakers);
    else if (name == "States text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpatStates);
    else if (name == "Scenes text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditSpatScenes);
    else if (name == "Hardware Configuration text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::EditHardwareConfiguration);
    else if (name == "Start text button")
        answeredAppMode = presenter->appModeChangeRequest(AppMode::MiamSpatPlaying);
}



void View::ChangeAppMode(AppMode newAppMode)
{
    mainContentComponent->ChangeAppMode(newAppMode);
}
void View::DisplayInfo(const std::string& message, int priority, bool alsoDisplayInNewWindow)
{
    mainContentComponent->DisplayInfo(message, priority);
#ifndef __MIAMOBILE
    if (alsoDisplayInNewWindow)
        displayInfoInNewWindow(message);
#endif
}

void View::SetTitle(std::string title)
{
    ControlView::SetTitle(title + " [" + App::GetPurposeName(presenter->GetSessionPurpose()) + " Session]");
}

void View::displayInfoInNewWindow(const std::string& message)
{
    // Code pompé à mort depuis la démo Juce
    DialogWindow::LaunchOptions options;
    Label* label = new Label();
    label->setText (message, dontSendNotification);
    label->setColour (Label::textColourId, Colours::white);
    options.content.setOwned (label);
    
    int width = 400;
    int height = 320;
    options.content->setSize (width - 100, height - 50);
    
    // Regex pour retrouver le titre entre crochets
    // parenthèses pour récupérer le contenu uniquement
    // + signifie "au moins 1", * signifie "au moins 0"
    // double \\ parce que \ est aussi un caractère spécial pour le c++
    // ? -> "non-greedy" ou "lazy" -> s'arrêtera dès le premier ] rencontré
    // (on met le ? juste après le + parce que le caractère "lazy" s'applique au
    // contenu matché : on est le plus feignant possible sur la sortie)
    std::regex betweenBracketsRegex("\\[(.*?)\\]");
    std::smatch matchResults;
    std::string windowTitle = "Information";
    // If something is found
    if (std::regex_search(message, matchResults, betweenBracketsRegex))
    {
        windowTitle += ": ";
        // We get only the second match results (matchResults is an iterator)
        // First result seem to be the entire string concerned about the regex
        // (not the inner [] selected text)
        windowTitle += *( ++ (matchResults.begin() ) );
    }
    
    options.dialogTitle                   = windowTitle.c_str();
    options.dialogBackgroundColour        = Colour (0xff4a4a4a);
    options.escapeKeyTriggersCloseButton  = true;
    options.useNativeTitleBar             = true;
    options.resizable                     = true;
    
    auto dialogWindow = options.launchAsync();
    
    if (dialogWindow != nullptr)
        dialogWindow->centreWithSize (width, height);
}
