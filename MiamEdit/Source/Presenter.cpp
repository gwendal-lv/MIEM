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

#include "TextUtils.h"
#include "PathUtils.h"

#include "JuceHeader.h"

#include <sstream>

// Aucune de ces bilbiothèques ne convient... Retour aux regex c++11...
//#include "boost/program_options.hpp"
// #include "cxxopts.hpp" // https://github.com/jarro2783/cxxopts // de la vraie saloperie aussi !
#include <regex>

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -
AppPurpose App::appPurpose = AppPurpose::Multi;

Presenter::Presenter(View* _view) :
    ControlPresenter(_view),

    view(_view),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(_view, this),
    spatStatesEditionManager(_view),
    settingsManager(_view, this)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    
}

void Presenter::CompleteInitialisation(Model* _model)
{
    // Self init
    model = _model;
    ControlPresenter::CompleteInitialisation(&graphicSessionManager, _model);
    // Sub-modules (graphic session manager : init from SpatPresenter)
    spatStatesEditionManager.CompleteInitialisation(model->GetInterpolator());
    settingsManager.CompleteInitialisation(model);
    
    // Après initialisation : on ne montre RIEN
    // On genère une requête interne puis on notifie View
    // appModeChangeRequest(AppMode::Loading);
    // view->ChangeAppMode(AppMode::Loading);
}

void Presenter::ManageInitialSession(std::string commandLine)
{
    // Copie du paramètre d'entrée,
    // pour permettre le chargement automatique de la session de test....
    std::string commandLineToParse = commandLine;
#ifdef __MIAM_DEBUG
    //commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Session de débug.mspat\" ";
    //commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Mini-golf bureau.mspat\" ";
#endif
    
    // Récupération du nom de fichier à charger
    std::string commandLineFileName = TextUtils::FindFilenameInCommandLineArguments(commandLineToParse);
    
    // Affichage de l'écran classique de démarrage si on a pas donné de nom de scène
    if (commandLineFileName.empty())
    {
        appModeChangeRequest(AppMode::Startup);
    }
    // Ou bien chargement si on avait un paramètre dans la ligne de commande
    else
        LoadSession(commandLineFileName); // fait les changements de modes (avant/après) automatiquement
    
    // À ce moment : on ne fait rien !
    // On est peut-être toujours en train d'attendre un choix de l'utilisateur
}

void Presenter::OnShutdownRequest()
{
    if (appMode != AppMode::Startup)
        SaveSession("", true); // current filename, forced data refresh
}



AppMode Presenter::appModeChangeRequest(AppMode newAppMode)
{
    // First check : are we running a new mode ?
    if (newAppMode != appMode)
    {
        view->ChangeAppMode(AppMode::Loading);
        
        // - - - - - PRE-CHANGE PROCESSING - - - - -
        // Si on passe à "Loading", on ne fait pas de traitement...
        // Ça sera fait au prochain changement de mode de toute manière !
        // Sinon -> gros crashes avec les sauvegardes de données dans des modules
        // pas encore existants (ou pas finis)
        if (newAppMode != AppMode::Loading)
        {
            std::shared_ptr<bptree::ptree> dataTree;
            switch(appMode)
            {
                case AppMode::EditSpatStates :
                    // If leaving the matrices editing : data save before changing mode
                    dataTree = spatStatesEditionManager.OnLeaveSpatStatesEdition();
                    // va déclencher sauvegarde dans fichier xml
                    updateSpatStatesTree(dataTree, true);
                    break;
                    
                case AppMode::EditSpatScenes :
                    // If leaving the matrices editing : data save before changing mode
                    dataTree = graphicSessionManager.OnLeaveSpatScenesEdition();
                    // va déclencher sauvegarde dans fichier xml
                    updateSpatScenesTree(dataTree, true);
                    break;
                    
                default :
                    break;
            }
        }
        
        // Internal+graphical update
        appMode = newAppMode;
        view->ChangeAppMode(appMode);
        
        // - - - - - POST-CHANGE PROCESSING - - - - -
        switch(appMode)
        {
            case AppMode::EditSpatStates :
                // At leats : reloading of maybe changed data from other mode
                spatStatesEditionManager.OnEnterSpatStatesEdition();
                break;
                
            case AppMode::EditSpatScenes :
                // At leats : reloading of maybe changed data from other mode
                graphicSessionManager.OnEnterSpatScenesEdition();
                break;
                
            default :
                break;
        }
    }
    
    
    // Actual new mode
    return appMode;
}



void Presenter::Update()
{
    // Check of Model messages !!!!!!
}




// = = = = = = = = = = Setters and Getters  = = = = = = = = = =

AppPurpose Presenter::GetSessionPurpose() const
{
    return model->GetSessionPurpose();
}






// = = = = = = = = = = XML import/export  = = = = = = = = = =


void Presenter::LoadSession(std::string filename)
{
    // Pas tant pour l'affichage graphique....
    // Que pour que le changement de mode vers les spat scenes à la fin
    // déclenche de vraies actualisations
    appModeChangeRequest(AppMode::Loading);
    
    
    try {
        // Chargement des données dépendant du type de session
        if (PathUtils::CheckForExtensionAndPurposeCoherence(filename, AppPurpose::Spatialisation))
            model->setSessionPurpose(AppPurpose::Spatialisation);
        else if (PathUtils::CheckForExtensionAndPurposeCoherence(filename, AppPurpose::GenericController))
            model->setSessionPurpose(AppPurpose::GenericController);
        else
            throw XmlReadException("Session type is unknown. Should be either 'Spatialisation' or 'Generic Controller'");
        
        
        // Chargement des données communes à toute session de contrôle
        ControlPresenter::LoadSession(filename);
    }
    catch (XmlReadException& e)
    {
        // Also display in a new window
        view->DisplayInfo(std::string("[Reading XML] ") + e.what(),
                          50, true);
    }
    // Updates (graphical mostly) just after
    spatStatesEditionManager.UpdateView();
    
    // Actual mode change here
    // App mode changer to Scenes Edition by default (should be stored within the file ?)
    appModeChangeRequest(AppMode::EditSpatScenes);
}
void Presenter::SaveSession(std::string filename, bool forceDataRefresh)
{
    // Mise à jour d'abord (par des moyens spécifiques au miam edit)
    if (forceDataRefresh)
    {
        // Sans faire la sauvegarde automatique cette fois !
        updateSpatStatesTree(spatStatesEditionManager.GetTree(), false);
        updateSpatScenesTree(graphicSessionManager.GetCanvasesTree(), false);
    }
    
    // Puis sauvegarde effective vers XML
    try { ControlPresenter::SaveSession(filename, forceDataRefresh); }
    catch (XmlWriteException& e) {
        view->DisplayInfo(e.what(), 50, true); // haute priorité, et dans nouvelle fenêtre
    }
}
void Presenter::CreateSession(std::string filename, AppPurpose sessionPurpose, bool isEmpty)
{
    if (!isEmpty)
        throw std::logic_error("Not implemented. On peut ne peut créer que des session vides pour l'instant.");
    
    // ATTENTION : IL FAUDRAIT TOUT SUPPRIMER AVANT DE RE-CRÉER UNE SESSION
    // ATTENTION : IL FAUDRAIT TOUT SUPPRIMER AVANT DE RE-CRÉER UNE SESSION
    // ATTENTION : IL FAUDRAIT TOUT SUPPRIMER AVANT DE RE-CRÉER UNE SESSION
    // ATTENTION : IL FAUDRAIT TOUT SUPPRIMER AVANT DE RE-CRÉER UNE SESSION
    
    // Initialisation du type de session dans le modèle
    assert( PathUtils::CheckForExtensionAndPurposeCoherence(filename, sessionPurpose) );
    model->setSessionPurpose(sessionPurpose);
    
    // On force l'update (de la session forcément vide, car c'est mal fout pour l'instant...)
    // Pour l'instant on ne peut faire ça que depuis l'écran de démarrage
    SaveSession(filename, true);
    
    // Mode : repasse aux scène graphiques par défaut
    appModeChangeRequest(AppMode::EditSpatScenes);
}


std::shared_ptr<bptree::ptree> Presenter::GetConfigurationTree()
{
    return settingsManager.GetTree();
}

void Presenter::SetConfigurationFromTree(bptree::ptree& tree)
{
    settingsManager.SetFromTree(tree);
}






