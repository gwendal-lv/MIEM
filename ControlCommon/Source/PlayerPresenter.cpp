/*
  ==============================================================================

    PlayerPresenter.cpp
    Created: 1 Mar 2018 3:01:42pm
    Author:  Gwendal

  ==============================================================================
*/



#include "PlayerPresenter.h"
#include "GraphicSessionPlayer.h"

#include "PlayerView.h"

using namespace Miam;


PlayerPresenter::PlayerPresenter(PlayerView* _view)
:
ControlPresenter(_view),

view(_view),

appMode(PlayerAppMode::Null),

loadFileChooser({App::GetPurpose()}) // purpose théoriquement initialisé avant tout (statique en dehors de toute fonction)
{
    
}

void PlayerPresenter::CompleteInitialisation(PlayerModel* _model)
{
    model = _model;
}







PlayerAppMode PlayerPresenter::appModeChangeRequest(PlayerAppMode newAppMode)
{
    AsyncParamChange paramChange;
    
    // - - - - - Autorisation ou non du changement - - - - -
    bool modeChangeAllowed = true; // vrai par défaut, on voit si c'est bon
    // Si aucune session n'est chargée : on ne peut pas passer aux mode de play.
    // On autorise stop, ce n'est peut-être pas une bonne idée... à voir.
    if (newAppMode == appMode)
        modeChangeAllowed = false;
    else if (lastFilename.empty() && newAppMode == PlayerAppMode::Playing)
    {
        modeChangeAllowed = false;
        view->DisplayInfo("Cannot begin spatialisation: no session loaded.");
    }
    
    // First check : are we running a new mode ?
    // Si on fait un transition directe entre modes qui ne sont pas 'loading' ->
    // on force le passage par loading
    if (modeChangeAllowed)
    {
        // - - - - - Traitements pré-changement - - - - -
        // Passage par le mode loading de View, si nécessaire
        if (appMode != PlayerAppMode::Loading && newAppMode != PlayerAppMode::Loading)
            view->ChangeAppMode(PlayerAppMode::Loading);
        // Sauvegarde du statut de spat (si continue en tâche de fond)
        if (appMode == PlayerAppMode::Stopped || appMode == PlayerAppMode::Playing)
            previousSpatialisationStatus = appMode;
        
        // - - - - - Changement interne - - - - -
        appMode = newAppMode;
        
        // - - - - - Traitements Post-changement - - - - -
        if (modeChangeAllowed)
        {
            switch (appMode)
            {
                case PlayerAppMode::LoadingFile :
                    // On sait qu'on était pas en attente d'un résultat,
                    // car c'est justement le mode LoadingFile qui indique cette attente.
                    //
                    // Et juste au-dessus, on a une protection contre les changements de mode vers le même mode
                    loadFileChooser.launchAsync (FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
                                                 [this] (const FileChooser& chooser)
                                                 { this->OnFileChooserReturn(chooser); });
                    break;
                    
                case PlayerAppMode::Stopped :
                    // on ne renvoie l'ordre au modèle que dans le cas où l'on change de statut de spat
                    // par rapport au statut de spat en tâche de fond.
                    if (previousSpatialisationStatus != appMode)
                    {
                        paramChange.Type = AsyncParamChange::Stop;
                        SendParamChange(paramChange);
                    }
                    break;
                    
                case PlayerAppMode::Playing :
                    // on ne renvoie l'ordre au modèle que dans le cas où l'on change de statut de spat
                    // par rapport au statut de spat en tâche de fond.
                    if (previousSpatialisationStatus != appMode)
                    {
                        paramChange.Type = AsyncParamChange::Play;
                        SendParamChange(paramChange);
                        
                        // Sélection des scènes 0 de chaque canevas -> remise en place des excitateurs
                        GetGraphicSessionPlayer()->OnModelStarted();
                    }
                    break;
                    
                    
                default:
                    break;
            }
        }
        // - - - - - Application graphique - - - - -
        view->ChangeAppMode(appMode);
    }
    
    
    // Actual new mode
    return appMode;
}



void PlayerPresenter::OnFileChooserReturn(const FileChooser& chooser)
{
    if (&chooser == &loadFileChooser)
    {
        File returnedFile = loadFileChooser.getResult();
        // Si l'utilisateur a choisi un fichier, on le charge
        if (returnedFile.exists())
            LoadSession(returnedFile.getFullPathName().toStdString());
        // Sinon, on remet le menu...
        else
            appModeChangeRequest(PlayerAppMode::MainMenu);
    }
}







void PlayerPresenter::OnMainMenuButtonClicked()
{
    // Si l'on était déjà sur le menu : on retourne à l'affichage des canevas de jeu, mais selon l'état
    // précédent de spatialisation.
    if (appMode == PlayerAppMode::MainMenu)
        appModeChangeRequest(previousSpatialisationStatus);
    else
        appModeChangeRequest(PlayerAppMode::MainMenu);
}


void PlayerPresenter::OnNewConnectionStatus(bool isConnectionEstablished, std::shared_ptr<bptree::ptree> connectionParametersTree)
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



