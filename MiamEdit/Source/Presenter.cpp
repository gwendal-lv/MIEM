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

#include "JuceHeader.h"

#include <sstream>

// Aucune de ces bilbiothèques ne convient... Retour aux regex c++11...
//#include "boost/program_options.hpp"
// #include "cxxopts.hpp" // https://github.com/jarro2783/cxxopts // de la vraie saloperie aussi !
#include <regex>

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -

Presenter::Presenter(View* _view) :
    view(_view),
    appMode(AppMode::Loading), // app is loading while the Model hasn't fully loaded yet

    graphicSessionManager(_view, this),
    spatStatesEditionManager(_view),
    settingsManager(_view, this)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    
}

void Presenter::CompleteInitialisation(Model* _model, std::string& commandLine)
{
    // Self init
    model = _model;
    SpatPresenter::CompleteInitialisation(&graphicSessionManager, _model);
    // Sub-modules (graphic session manager : init from SpatPresenter)
    spatStatesEditionManager.CompleteInitialisation(model->GetSpatInterpolator());
    settingsManager.CompleteInitialisation(model);
    
    // Après initialisation : on montre des objets graphiques
    // On genère une requête interne puis on notifie View
    appModeChangeRequest(AppMode::EditSpatScenes);
    view->ChangeAppMode(AppMode::EditSpatScenes);
    
    // Copie du paramètre d'entrée,
    // pour permettre le chargement automatique de la session de test....
    std::string commandLineToParse = commandLine;
//#ifdef __MIAM_DEBUG
    //commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Session de débug.miam\" ";
    commandLineToParse += " -session \"/Users/Gwendal/Music/Spat sessions/Test.miam\" ";
//#endif
    
    // Récupération du nom de fichier à charger
    std::string commandLineFileName = TextUtils::FindFilenameInCommandLineArguments(commandLineToParse);
    
    // Chargement, ou sauvegarde forcée selon le paramètre passé !
    if (commandLineFileName.empty())
        SaveSession(); // Va demander un chemin de fichier pour la sauvegarde automatique ensuite
    else
        LoadSession(commandLineFileName);
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
                    updateSpatStatesTree(dataTree);
                    break;
                    
                case AppMode::EditSpatScenes :
                    // If leaving the matrices editing : data save before changing mode
                    dataTree = graphicSessionManager.OnLeaveSpatScenesEdition();
                    updateSpatScenesTree(dataTree);
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



// = = = = = = = = = = XML import/export  = = = = = = = = = =
void Presenter::LoadSession(std::string filename)
{
    // Pas tant pour l'affichage graphique....
    // Que pour que le changement de mode vers les spat scenes à la fin
    // déclenche de vraies actualisations
    appModeChangeRequest(AppMode::Loading);
    
    try {
        SpatPresenter::LoadSession(filename);
    }
    catch (XmlReadException& e)
    {
        // Also display in a new window
        view->DisplayInfo(std::string("[Reading XML] ") + e.what(), true);
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
        auto dataTree = spatStatesEditionManager.OnLeaveSpatStatesEdition();
        updateSpatStatesTree(dataTree);
        dataTree = graphicSessionManager.OnLeaveSpatScenesEdition();
        updateSpatScenesTree(dataTree);
    }
    
    // Puis sauvegarde effective vers XML
    try { SpatPresenter::SaveSession(filename, forceDataRefresh); }
    catch (XmlWriteException& e) {
        view->DisplayInfo(e.what());
    }
}

std::shared_ptr<bptree::ptree> Presenter::GetConfigurationTree()
{
    return settingsManager.GetTree();
}
void Presenter::SetConfigurationFromTree(bptree::ptree& tree)
{
    settingsManager.SetFromTree(tree);
}






