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


#include "SpatFileChoosers.h"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    SpatPresenter(_view),

    view(_view),
    appMode(AppMode::Null),

    graphicSessionManager(this, _view)
{
    appMode = AppMode::None;
    previousSpatialisationStatus = AppMode::None;
    
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
    
    appModeChangeRequest(AppMode::Stopped); // pour forcer l'état
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
        appModeChangeRequest(AppMode::MainMenu);
    }
}






AppMode Presenter::appModeChangeRequest(AppMode newAppMode)
{
    AsyncParamChange paramChange;

    // - - - - - Autorisation ou non du changement - - - - -
    bool modeChangeAllowed = true; // vrai par défaut, on voit si c'est bon
    // Si aucune session n'est chargée : on ne peut pas passer aux mode de play.
    // On autorise stop, ce n'est peut-être pas une bonne idée... à voir.
    if (newAppMode == appMode)
        modeChangeAllowed = false;
    else if (lastFilename.empty() && newAppMode == AppMode::Playing)
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
        if (appMode != AppMode::Loading && newAppMode != AppMode::Loading)
            view->ChangeAppMode(AppMode::Loading);
        // Sauvegarde du statut de spat (si continue en tâche de fond)
        if (appMode == AppMode::Stopped || appMode == AppMode::Playing)
            previousSpatialisationStatus = appMode;
        
        // - - - - - Changement interne - - - - -
        appMode = newAppMode;
        
        // - - - - - Traitements Post-changement - - - - -
        if (modeChangeAllowed)
        {
            switch (appMode)
            {
                case AppMode::LoadingFile :
                    // On sait qu'on était pas en attente d'un résultat,
                    // car c'est justement le mode LoadingFile qui indique cette attente.
                    //
                    // Et juste au-dessus, on a une protection contre les changements de mode vers le même mode
                    loadFileChooser.launchAsync (FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
                                                 [this] (const FileChooser& chooser)
                                                 { this->OnFileChooserReturn(chooser); });
                    break;
                    
                case AppMode::Stopped :
                    // on ne renvoie l'ordre au modèle que dans le cas où l'on change de statut de spat
                    // par rapport au statut de spat en tâche de fond.
                    if (previousSpatialisationStatus != appMode)
                    {
                        paramChange.Type = AsyncParamChange::Stop;
                        SendParamChange(paramChange);
                    }
                    break;
                    
                case AppMode::Playing :
                    // on ne renvoie l'ordre au modèle que dans le cas où l'on change de statut de spat
                    // par rapport au statut de spat en tâche de fond.
                    if (previousSpatialisationStatus != appMode)
                    {
                        paramChange.Type = AsyncParamChange::Play;
                        SendParamChange(paramChange);
                        
                        // Sélection des scènes 0 de chaque canevas -> remise en place des excitateurs
                        graphicSessionManager.OnModelStarted();
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

void Presenter::OnFileChooserReturn(const FileChooser& chooser)
{
    if (&chooser == &loadFileChooser)
    {
        File returnedFile = loadFileChooser.getResult();
        // Si l'utilisateur a choisi un fichier, on le charge
        if (returnedFile.exists())
            LoadSession(returnedFile.getFullPathName().toStdString());
        // Sinon, on remet le menu...
        else
            appModeChangeRequest(AppMode::MainMenu);
    }
}



void Presenter::Update()
{
    // Récupération des infos du modèle... Par exemple : info "OK je suis prêt
    // à être contrôlé en SINGLE THREAD" lors de re-chargement d'une scène
}



void Presenter::OnMainMenuButtonClicked()
{
    // Si l'on était déjà sur le menu : on retourne à l'affichage des canevas de jeu, mais selon l'état
    // précédent de spatialisation.
    if (appMode == AppMode::MainMenu)
        appModeChangeRequest(previousSpatialisationStatus);
    else
        appModeChangeRequest(AppMode::MainMenu);
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
    appModeChangeRequest(AppMode::Stopped); // aussi : Arrêt du modèle
    // Il faudra un temps d'attente !! Une confirmation que tout s'est bien arrêté...
    // Avant de faire le chargement qui lui sera SINGLE THREAD
    
    
    
    // Arrêt des envois de ce module, déjà pour commencer
    appModeChangeRequest(AppMode::Loading);
    
    // Chargement d'une nouvelle session
    SpatPresenter::LoadSession(filename);
    
    // Ensuite on se change de mode
    appModeChangeRequest(AppMode::Playing); // va démarrer le modèle
}

void Presenter::SetConfigurationFromTree(bptree::ptree&)
{
    // Rien d'affiché : on attend le retour effectif des infos depuis le Modèle
}


