/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "Model.h"
#include "View.h"


#include "JuceHeader.h"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    view(_view),
    appMode(AppMode::Null),

    graphicSessionManager(this, _view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    view->GetMainContentComponent()->resized();
    
    
    appModeChangeRequest(AppMode::None);
}


void Presenter::CompleteInitialisation(Model* _model)
{
    // Self init
    model = _model;
    
}


void Presenter::Update()
{
    
}


AppMode Presenter::appModeChangeRequest(AppMode newAppMode)
{
    // First check : are we running a new mode ?
    if (newAppMode != appMode)
    {
        view->ChangeAppMode(AppMode::Loading);
        
        appMode = newAppMode;
        view->ChangeAppMode(appMode);
    }
    
    
    // Actual new mode
    return appMode;
}


// = = = = = XML loading only = = = = =
void Presenter::SetConfigurationFromTree(bptree::ptree&)
{
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    DBG("Aucune info de configuration affichée pour le moment !");
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
    // TODO : récupérer info OSC, nombre de canaux, etc... ET LES AFFICHER
}


