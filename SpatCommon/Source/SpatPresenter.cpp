/*
  ==============================================================================

    SpatPresenter.cpp
    Created: 6 Jun 2017 10:49:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatPresenter.h"


#include "SpatModel.h"


SpatPresenter::SpatPresenter()
{
    lastSpatStatesTree = std::make_shared<bptree::ptree>();
    lastSpatScenesTree = std::make_shared<bptree::ptree>();
    //lastFilename = "/Users/Gwendal/test.miam";
}

void SpatPresenter::CompleteInitialisation(IGraphicSessionManager* _graphicSessionManager, SpatModel* _model)
{
    graphicSessionManager = _graphicSessionManager;
    model = _model;
}


// = = = = = XML import/export = = = = =

void SpatPresenter::LoadSession(std::string filename)
{
    bptree::ptree xmlTree, miamTree, spatTree, graphicSessionTree, settingsTree;
    try {
        // Lecture
        bptree::read_xml(filename, xmlTree);
        // puis Séparation des grandes parties du fichier
        miamTree = xmlTree.get_child("miam");
        spatTree = miamTree.get_child("spat");
        graphicSessionTree = miamTree.get_child("graphicsession");
        settingsTree = miamTree.get_child("settings");
    }
    catch(bptree::ptree_error &e) {
        throw XmlReadException("Cannot load session: ", e);
    }
    // Envoi de chaque grande partie à la sous-partie de spat concernée
    model->GetSpatInterpolator()->SetSpatStatesFromTree(spatTree);
    graphicSessionManager->SetFromTree(graphicSessionTree);
    model->SetConfigurationFromTree(settingsTree.get_child("model"));
    this->SetConfigurationFromTree(settingsTree);
}
void SpatPresenter::SaveSession(std::string _filename)
{
    // Checks about the filename
    if (! _filename.empty() )
        lastFilename = _filename;
    else if (lastFilename.empty())
        throw XmlWriteException("No filename currently in use : please use 'Save As' before calling any direct 'Save'");
    
    // Whole properties tree reconstruction
    bptree::ptree miamChildrenTree;
    // Les settings sont ajoutés catégorie par catégorie
    bptree::ptree settingsTree;
    settingsTree.add_child("model", *(model->GetConfigurationTree()) );
    settingsTree.add_child("presenter", *(this->GetConfigurationTree()) );
    miamChildrenTree.add_child("settings", settingsTree);
    // Et c'est seulement ensuite que l'on sauvegarde effectivement les données
    miamChildrenTree.add_child("spat", *lastSpatStatesTree);
    miamChildrenTree.add_child("graphicsession", *lastSpatScenesTree);
    auto wholeMiamTree = std::make_shared<bptree::ptree>();
    wholeMiamTree->add_child("miam", miamChildrenTree);
    
    // Actual XML data writing into file
    bptree::xml_writer_settings<std::string> xmlSettings(' ', 4);
    try {
        bptree::write_xml(lastFilename, *wholeMiamTree, std::locale(), xmlSettings);
    }
    catch (bptree::xml_parser::xml_parser_error& e) {
        throw XmlWriteException(e.what());
    }
}
void SpatPresenter::updateSpatStatesTree(std::shared_ptr<bptree::ptree> newTree)
{
    lastSpatStatesTree = newTree;
    SaveSession();
}
void SpatPresenter::updateSpatScenesTree(std::shared_ptr<bptree::ptree> newTree)
{
    lastSpatScenesTree = newTree;
    SaveSession();
}
