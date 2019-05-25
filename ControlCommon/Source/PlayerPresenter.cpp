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
#include "PlayerBackgroundComponent.h"

#include "PlayerModel.h"

#include "TextUtils.h"

using namespace Miam;



// = = = = = = = = = = Construction, destruction, initialisation = = = = = = = = = =

PlayerPresenter::PlayerPresenter(PlayerView* _view)
:
ControlPresenter(_view),

view(_view),

appMode(PlayerAppMode::Null),
previousPlayerStatus(PlayerAppMode::Null),

loadFileChooser({App::GetPurpose()}), // purpose théoriquement initialisé avant tout (statique en dehors de toute fonction)

remoteControlServer(this)
{
    appMode = PlayerAppMode::None;
    previousPlayerStatus = PlayerAppMode::None;
}


void PlayerPresenter::CompleteInitialisation(PlayerModel* _model)
{
    // - - - Init des attributs privés puis des parents - - -
    model = _model;
    
    ControlPresenter::CompleteInitialisation(GetGraphicSessionPlayer(), model);
    
    view->ForceResized();
    
    appModeChangeRequest(PlayerAppMode::Stopped); // pour forcer l'état
}


void PlayerPresenter::TryLoadFirstSession(std::string commandLine)
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




// = = = = = = = = = = GENERAL MANAGEMENT = = = = = = = = = =

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
    // Si on est en train de charger un mode de Play (info de View... pas très propre...)
    // On le laisse finir ! Sinon problème de ressources VBO/textures OpenGL
    else if (appMode == PlayerAppMode::Playing // même si en attente : visible directement
             && isViewPreparingToPlay)
    {
        modeChangeAllowed = false; // on refuse juste la demande
        DBG("[Player Presenter] Mode change request refused: View is currently in transition to the Playing mode");
    }
        
    
    // First check : are we running a new mode ?
    // Si on fait un transition directe entre modes qui ne sont pas 'loading' ->
    // on force le passage par loading
    if (modeChangeAllowed)
    {
        // - - - - - Traitements pré-changement - - - - -
        // Le + important : libération ressources VBO/textures/shaders si nécessaire
        // Attention : on doit s'être assuré avant de ne pas libérer avant d'avoir affiché le canevas...
        if (appMode == PlayerAppMode::Playing)
            view->TriggerGLResourcesRelease();
        
        // Passage par le mode loading de View, si nécessaire
        if (appMode != PlayerAppMode::Loading && newAppMode != PlayerAppMode::Loading)
            view->ChangeAppMode(PlayerAppMode::Loading);
        // Sauvegarde du statut de spat (si continue en tâche de fond)
        if (appMode == PlayerAppMode::Stopped || appMode == PlayerAppMode::Playing)
            previousPlayerStatus = appMode;
        
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
                    if (previousPlayerStatus != appMode)
                    {
                        paramChange.Type = AsyncParamChange::Stop;
                        SendParamChange(paramChange);
                    }
                    break;
                    
                case PlayerAppMode::Playing :
                    appHasBeenPlayingOnce = true;
                    // on ne renvoie l'ordre au modèle que dans le cas où l'on change de statut de spat
                    // par rapport au statut de spat en tâche de fond.
                    if (previousPlayerStatus != appMode)
                    {
                        paramChange.Type = AsyncParamChange::Play;
                        SendParamChange(paramChange);
                        
                        // Sélection des scènes 0 de chaque canevas -> remise en place des excitateurs
                        GetGraphicSessionPlayer()->OnModelStarted();
                    }
                    break;
                    
                case PlayerAppMode::MainMenu :
                    // We auto-hide the Help Contents if the app has already been playing
                    // (if the user played something, he knows how the app works...)
                    view->ShowHelpContents(! appHasBeenPlayingOnce);
                    break;
                    
                    
                default:
                    break;
            }
            // General treatment : TIMER ON/OFF
            // reminder : this timer must be activated when the main OpenGL component is not visible
            // and does not send update calls anymore
            if (appMode == PlayerAppMode::Playing)
                stopTimer();
            else if (appMode != PlayerAppMode::None
                     && appMode != PlayerAppMode::Null
                     && appMode != PlayerAppMode::Loading)
                startTimerHz(10);
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
        std::cout << "[Presenter] Loading session file '" << returnedFile.getFullPathName().toStdString() << "'... ";
        // Si l'utilisateur a choisi un fichier (accès direct ou URL), on le charge
		// En gérant l'exception s'il y en a eu une
		if (returnedFile.exists() || (! loadFileChooser.getURLResult().isEmpty()))
		{
            // Lecture de la ressource
            if (returnedFile.exists()) // accès direct
            {
                std::cout << "Accessible directly on the device." << std::endl;
            }
            else if (! loadFileChooser.getURLResult().isEmpty()) // Cloud, etc., sur plateforme mobile
            {
                std::cout << "Accessible via URL at '" << loadFileChooser.getURLResult().toString(true) << "'" << std::endl;
                // code issu de https://forum.juce.com/t/cant-see-userdocumentsdirectory-in-ios-file-chooser/25672/5
                StringPairArray responseHeaders;
                int statusCode = 0;
                ScopedPointer<InputStream> p_stream( loadFileChooser.getURLResult().createInputStream(
                    false, nullptr, nullptr, String(), 10000, &responseHeaders, &statusCode ) );
                // Ce code enclenche peut-être des l'attribution des "security bookmarks" dont parle la doc Juce ??
                // voir : https://docs.juce.com/master/classFileChooser.html#a5964a831e9d12cd53de3606240dfd4c9
                
            }
            
            // Chargement dans fonction dédiée
            // = = = = = = = ATTENTION LECTURE DIRECTE DU FICHIER MEME POUR FICHIER CLOUD via URL = = = = = = = =
            // a l'air de fonctionne sour iOS si on pré-charge ci-dessus le stream... même si on n'utilise
            // pas le stream lui-même.
            // !!!!! à tester sous Android !!!!
			try {
				LoadSession(returnedFile.getFullPathName().toStdString());
			}
			catch (XmlReadException& e) {
				view->DisplayInfo(e.what(), true); // important display
				appModeChangeRequest(PlayerAppMode::MainMenu);
			}
		}
        // Sinon si le fichier n'existe plus, on remet le menu...
        else
        {
            view->DisplayInfo(TRANS("File '").toStdString() + returnedFile.getFullPathName().toStdString() + TRANS("' does not exist...").toStdString(), true); // important display
            appModeChangeRequest(PlayerAppMode::MainMenu);
        }
    }
}

void PlayerPresenter::timerCallback()
{
    // Used when the MultiSceneCanvasInteractor is not shown !
    Update();
}
/*
void PlayerPresenter::processParamChangeFromModel(AsyncParamChange const & paramChange)
{
    switch(paramChange.Type)
    {
            // All untreated events : directed to parent class
        default :
            ControlPresenter::processParamChangeFromModel(paramChange);
            break;
    }
}
 */


// = = = = = = = = = = EVENTS FROM VIEW = = = = = = = = = =

void PlayerPresenter::OnMainMenuButtonClicked()
{
    // Si l'on était déjà sur le menu : on retourne à l'affichage des canevas de jeu, mais selon l'état
    // précédent de spatialisation.
    if (appMode == PlayerAppMode::MainMenu)
        appModeChangeRequest(previousPlayerStatus);
    else
        appModeChangeRequest(PlayerAppMode::MainMenu);
}
void PlayerPresenter::OnViewIsPreparingToPlay(bool _isPreparingToPlay)
{
    isViewPreparingToPlay = _isPreparingToPlay;
}
void PlayerPresenter::OnHelpButtonClicked(bool isHelpCurrentlyDisplayed)
{
    view->ShowHelpContents(! isHelpCurrentlyDisplayed);
}
bool PlayerPresenter::OnFullscreenButtonClicked()
{
#ifdef __MIAMOBILE
    assert(false); // fullscreen button should not be available for click on mobile platforms !
    return true;
#else
    // On inverse juste l'état de fullscreen, sans faire de check
    // (on aura peut-être des problèmes OpenGL à gérer, par exemple.... ?)
    view->SetFullscreenState(! view->GetFullscreenState());
    return view->GetFullscreenState();
#endif
}


// = = = = = = = = = = EVENTS FROM MODEL = = = = = = = = = =


void PlayerPresenter::OnNewConnectionStatus(bool isConnectionEstablished, std::shared_ptr<bptree::ptree> connectionParametersTree)
{
    // On ne doit pas avoir d'exception ici.... Mais bon dans le doute on checke quand même
    udpPort = -1; // not a local anymore ; saved for future re-use
    std::string ipv4 = "";
    try {
        udpPort = connectionParametersTree->get<int>("udp.port");
        ipv4 = connectionParametersTree->get<std::string>("ip");
    }
    catch (bptree::ptree_error&) { }
    
    // might not exist (then, unvalid -1 udp port)
    auto udpPort2 = connectionParametersTree->get<int>("udp.additional_port_1", -1);
	auto udpPort3 = connectionParametersTree->get<int>("udp.additional_port_2", -1);
    
    // - - - écriture de la chaîne à afficher - - -
    std::string displayString;
    // Si pas de connection
    if (!isConnectionEstablished)
    {
        // et si pb de paramètres
        if (udpPort == -1 || ipv4.empty())
        {
            displayString = "[error] Cannot send spatialisation data (no valid connection parameters found).";
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
            displayString = "Sending spatialisation data (unknown connection parameters).";
        }
        // Sinon tout est bon pour affichage
        else
        {
            displayString = "Sending OSC to " + ipv4 + " on UDP port " + std::to_string(udpPort);
			if (udpPort2 >= 0)
			{
				displayString += " (and ";
				displayString += boost::lexical_cast<std::string>(udpPort2);
				if (udpPort3 >= 0)
					displayString += ", " + boost::lexical_cast<std::string>(udpPort3);
				displayString += ")";
			}
        }
    }
    
    view->DisplayInfo(displayString);
    
    
}




// = = = = = = = = = = Remote control = = = = = = = = = = 
void PlayerPresenter::ReinitRemoteControlServer()
{
    bool serverRunning = false;
    int tcpPort = -1;
    if (udpPort > 0) // might be still -1.... ?
        tcpPort = udpPort + tcpServerPortOffset;
    
    // Stop and re-wait
    remoteControlServer.stop();
    serverRunning = remoteControlServer.beginWaitingForSocket(tcpPort);
    
#ifdef __MIEM_EXPERIMENTS
    String tcpServerMessage;
    if (serverRunning)
        tcpServerMessage = TRANS("TCP port ") + String(tcpPort) + TRANS(" ready for connection.");
    else
        tcpServerMessage = TRANS("Remote control server is not running.");
    #ifdef __MIEM_EXPERIMENTS_FULL_LOCK
    tcpServerMessage += " " + TRANS("Scene changes are locked (controlled by MIEM Experiments).");
    #endif
    // Display
    view->GetBackgroundComponent()->GetMainMenuComponent()->SetInfoLabelText(tcpServerMessage);
#else
    view->GetBackgroundComponent()->GetMainMenuComponent()->SetInfoLabelText("");
#endif
}



// = = = = = XML loading only = = = = =

void PlayerPresenter::LoadSession(std::string filename)
{
    appModeChangeRequest(PlayerAppMode::Stopped); // aussi : Arrêt du modèle
    
    // Il faut un temps d'attente !! Une confirmation que tout s'est bien arrêté...
    // Avant de faire le chargement qui lui sera single thread
    // Sauf que là si on bloquait en attentant un paquet lock-free, alors on bloquerait
    // aussi la fonction qui va chercher ces paquets lock-free.
    // Donc : attente active dégueu sur un bon vieux booléen atomique des familles
    while (! hasModelActuallyStopped)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // can be improved.... !!!!
    }
    
    // Arrêt des envois de ce module, déjà pour commencer
    appModeChangeRequest(PlayerAppMode::Loading);
    
    // Chargement d'une nouvelle session
    ControlPresenter::LoadSession(filename);
    
    // Various forced updates after loading
    view->GetBackgroundComponent()->SetMainSliderEnabled(model->GetIsMasterGainEnabled());
    
    // Server for remote control: to be re-configured
    ReinitRemoteControlServer();
    
    // Ensuite on se change de mode
    appModeChangeRequest(PlayerAppMode::Playing); // va démarrer le modèle
}

void PlayerPresenter::SetConfigurationFromTree(bptree::ptree&)
{
    // Rien d'affiché : on attend le retour effectif des infos depuis le Modèle
}








