/*
  ==============================================================================

    Presenter.cpp
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Presenter.h"

#include "PlayerModel.h"
#include "View.h"

#include "MiamExceptions.h"
#include "TextUtils.h"

#include "JuceHeader.h"


#include "MiemFileChoosers.h"

#include "AppPurpose.h"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -
AppPurpose App::appPurpose = AppPurpose::Spatialisation;

Presenter::Presenter(View* _view) :
    PlayerPresenter(_view),

    view(_view),

    graphicSessionManager(this, _view)
{
    appMode = PlayerAppMode::None;
    previousSpatialisationStatus = PlayerAppMode::None;
    
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    //view->GetMainContentComponent()->resized();
    
    
    appModeChangeRequest(PlayerAppMode::Loading);
}


void Presenter::CompleteInitialisation(PlayerModel* _model)
{
    // - - - Init des attributs privés puis des parents - - -
    model = _model;
    
    PlayerPresenter::CompleteInitialisation(_model);
    ControlPresenter::CompleteInitialisation(&graphicSessionManager, model);
    
    view->ForceResized();
    
    appModeChangeRequest(PlayerAppMode::Stopped); // pour forcer l'état
}

void Presenter::TryLoadFirstSession(std::string commandLine)
{
    // - - - Traitement du nom de fichier - - -
    // Copie du paramètre d'entrée,
    // pour permettre le chargement automatique de la session de test....
    std::string commandLineToParse = commandLine;
#ifdef __MIAM_DEBUG
    //commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Session de débug.miam\" ";
    //commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Test.miam\" ";
#endif
    // Récupération du nom de fichier à charger
    std::string fileName = TextUtils::FindFilenameInCommandLineArguments(commandLineToParse);
    
    // - - - Premier chargement de session OU BIEN menu principal - - -
    bool firstSessionLoaded = false;
    // D'abord on essaie de charger le truc depuis la ligne de commande, si possible
    if (! fileName.empty())
    {
        try {
            LoadSession(fileName);
            firstSessionLoaded = true;
        }
        catch (XmlReadException& ) {} // firstSessionLoaded reste à false
    }
    if (! firstSessionLoaded)
    {
        // Sinon si rien n'a marché, on affiche le menu principal
        appModeChangeRequest(PlayerAppMode::MainMenu);
    }
}




// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =



// = = = = = = = = = = METHODES = = = = = = = = = =



void Presenter::Update()
{
    // Récupération des infos du modèle... Par exemple : info "OK je suis prêt
    // à être contrôlé en SINGLE THREAD" lors de re-chargement d'une scène
}

// = = = = = XML loading only = = = = =
void Presenter::LoadSession(std::string filename)
{
    appModeChangeRequest(PlayerAppMode::Stopped); // aussi : Arrêt du modèle
    // Il faudra un temps d'attente !! Une confirmation que tout s'est bien arrêté...
    // Avant de faire le chargement qui lui sera SINGLE THREAD
    
    
    
    // Arrêt des envois de ce module, déjà pour commencer
    appModeChangeRequest(PlayerAppMode::Loading);
    
    // Chargement d'une nouvelle session
    ControlPresenter::LoadSession(filename);
    
    // Ensuite on se change de mode
    appModeChangeRequest(PlayerAppMode::Playing); // va démarrer le modèle
}

void Presenter::SetConfigurationFromTree(bptree::ptree&)
{
    // Rien d'affiché : on attend le retour effectif des infos depuis le Modèle
}


