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
    lastFilename = "/Users/Gwendal/test.miam";
}

void SpatPresenter::CompleteInitialisation(IGraphicSessionManager* _graphicSessionManager, SpatModel* _model)
{
    graphicSessionManager = _graphicSessionManager;
    model = _model;
}


// = = = = = XML import/export = = = = =

void SpatPresenter::LoadSession(std::string filename)
{
    bptree::ptree xmlTree, miamTree, spatTree, graphicSessionTree;
    try {
        // Lecture
        bptree::read_xml(filename, xmlTree);
        // puis Séparation des grandes parties du fichier
        miamTree = xmlTree.get_child("miam");
        spatTree = miamTree.get_child("spat");
        graphicSessionTree = miamTree.get_child("graphicsession");
    }
    catch(bptree::ptree_error &e) {
        throw XmlReadException("Cannot load session: ", e);
    }
    // Envoi de chaque grande partie à la sous-partie de spat concernée
    model->GetSpatInterpolator()->SetSpatStatesFromTree(spatTree);
    graphicSessionManager->SetFromTree(graphicSessionTree);
}
void SpatPresenter::SaveSession(std::string _filename)
{
    // Whole properties tree reconstruction
    bptree::ptree miamChildrenTree;
    miamChildrenTree.add_child("spat", *lastSpatStatesTree);
    miamChildrenTree.add_child("graphicsession", *lastSpatScenesTree);
    auto wholeMiamTree = std::make_shared<bptree::ptree>();
    wholeMiamTree->add_child("miam", miamChildrenTree);
    
    // Actual XML data writing into file
    if (! _filename.empty())
        lastFilename = _filename;
    bptree::xml_writer_settings<std::string> xmlSettings(' ', 4);
    bptree::write_xml(lastFilename, *wholeMiamTree, std::locale(), xmlSettings);
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
