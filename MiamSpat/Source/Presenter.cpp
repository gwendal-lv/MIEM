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
    appMode = AppMode::None;
    
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    //view->GetMainContentComponent()->resized();
    
    
    appModeChangeRequest(AppMode::Loading);
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
    // Récupération des infos du modèle... Par exemple : info "OK je suis prêt
    // à être contrôlé en SINGLE THREAD" lors de re-chargement d'une scène
}


AppMode Presenter::appModeChangeRequest(AppMode newAppMode)
{
    // First check : are we running a new mode ?
    // Si on fait un transition directe entre modes qui ne sont pas 'loading' ->
    // on force le passage par loading
    if (newAppMode != appMode)
    {
        if (appMode != AppMode::Loading && newAppMode != AppMode::Loading)
            view->ChangeAppMode(AppMode::Loading);
        
        // Temps de changement ici : totalement négligeable !! Mais ça le sera peutêtre pas toujours...
        appMode = newAppMode;
        
        view->ChangeAppMode(appMode);
    }
    
    
    // Actual new mode
    return appMode;
}


void Presenter::OnNewConnectionStatus(bool isConnectionEstablished, std::shared_ptr<bptree::ptree> connectionParametersTree)
{
    // On ne doit pas avoir d'exception ici.... Mais bon dans le doute on checke quand même
    int udpPort = -1;
    std::string ipv4 = "";
    try {
        udpPort = connectionParametersTree->get<int>("udp.port");
        ipv4 = connectionParametersTree->get<std::string>("ip");
    }
    catch (bptree::ptree_error&) { }
    
    // - - - écriture de la chaîne à afficher - - -
    std::string displayString;
    // Si pas de connection
    if (!isConnectionEstablished)
    {
        // et si pb de paramètres
        if (udpPort == -1 || ipv4.empty())
        {
            displayString = "[error] Cannot send spatialization data (no valid connection parameters found).";
        }
        // Sinon on affiche les paramètres qui ont échoué
        else
        {
            displayString = "[error] Cannot send OSC to " + ipv4 + " on UDP port " + std::to_string(udpPort) + ".";
        }
    }
    // Si connection OK
    else
    {
        // mais si pb de paramètres
        if (udpPort == -1 || ipv4.empty())
        {
            displayString = "Connected, sending spatialization data (no valid connection parameters found).";
        }
        // Sinon tout est bon pour affichage
        else
        {
            displayString = "Connected, sending OSC to " + ipv4 + " on UDP port " + std::to_string(udpPort) + ".";
        }
    }
    
    view->DisplayInfo(displayString);
    
    
}


// = = = = = XML loading only = = = = =
void Presenter::LoadSession(std::string filename)
{
    // Arrêt des envois de ce module, déjà pour commencer
    appModeChangeRequest(AppMode::Loading);
    
    // Arrêt du modèle
    // Arrêt du modèle
    // Arrêt du modèle
    // Arrêt du modèle
    // Arrêt du modèle
    // Arrêt du modèle
    // Arrêt du modèle
    AsyncParamChange paramChange;
    paramChange.Type = AsyncParamChange::Stop;
    SendParamChange(paramChange);
    // Il faudra un temps d'attente !! Une confirmation que tout s'est bien arrêté...
    // Avant de faire le chargement qui lui sera SINGLE THREAD
    
    // Chargement d'une nouvelle session
    SpatPresenter::LoadSession(filename);
    
    // Ensuite on se change de mode
    appModeChangeRequest(AppMode::Playing);
    
    // Re-démarrage du modèle avec les bonnes infos
    paramChange.Type = AsyncParamChange::Play;
    SendParamChange(paramChange);
    
    // On force la ré-actualisation graphique
    graphicSessionManager.OnModelStarted();
}

void Presenter::SetConfigurationFromTree(bptree::ptree&)
{
    // Rien d'affiché : on attend le retour effectif des infos depuis le Modèle
}


