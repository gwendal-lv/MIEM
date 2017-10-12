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

#include "MiamExceptions.h"
#include "TextUtils.h"

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
    //view->GetMainContentComponent()->resized();
    
    
    appModeChangeRequest(AppMode::None);
}


void Presenter::CompleteInitialisation(Model* _model)
{
    // - - - Init des attributs privés puis du parent - - -
    model = _model;
    SpatPresenter::CompleteInitialisation(&graphicSessionManager, model);
    
    view->GetMainContentComponent()->resized();
}

void Presenter::LoadFirstSession(std::string commandLine)
{
    // - - - Traitement du nom de fichier - - -
    // Copie du paramètre d'entrée,
    // pour permettre le chargement automatique de la session de test....
    std::string commandLineToParse = commandLine;
#ifdef __MIAM_DEBUG
    //commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Session de débug.miam\" ";
    commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Test.miam\" ";
#endif
    // Récupération du nom de fichier à charger
    std::string fileName = TextUtils::FindFilenameInCommandLineArguments(commandLineToParse);
    
    // - - - Premier chargement de session - - -
    bool firstSessionLoaded = false;
    // D'abord on essaie de charger le truc depuis la ligne de commande, si possible
    if ( ! fileName.empty() )
    {
        try {
            LoadSession(fileName);
            firstSessionLoaded = true;
        }
        catch (XmlReadException& ) {} // firstSessionLoaded reste à false
    }
    // Ensuite, tant qu'on a rien, on re-demande à l'utilisateur
    // (sauf s'il annule, alors on quitte le programme via une exception)
    while( !firstSessionLoaded )
    {
        // Récupération d'un nouveau nom de fichier
        FileChooser fileChooser("Chargement d'un fichier",
                                File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory),
                                "*.miam",
                                true);
        // Si l'utilisation a bien choisi un truc, on y va
        if ( fileChooser.browseForFileToOpen() )
        {
            fileName = fileChooser.getResult().getFullPathName().toStdString();
            // Et tentative de chargement
            try {
                LoadSession(fileName);
                firstSessionLoaded = true;
            }
            catch (XmlReadException& ) {} // firstSessionLoaded reste à false
        }
        // Sinon, user récalcitrant => programme récalcitrant !
        else
            throw ForceQuitException("User refuses to choose a session .miam file to load.");
    }
    
    // À la fin, on change le titre si tout s'est bien passé...
    view->SetTitle(fileName + " - MIAM Spat");
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


