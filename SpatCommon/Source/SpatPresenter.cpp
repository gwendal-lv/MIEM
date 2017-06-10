/*
  ==============================================================================

    SpatPresenter.cpp
    Created: 6 Jun 2017 10:49:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatPresenter.h"


#include "SpatModel.h"


#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


void SpatPresenter::CompleteInitialisation(SpatModel* _model)
{
    model = _model;
}


void SpatPresenter::LoadSession(std::string filename)
{
    bptree::ptree xmlTree;
    try {
        bptree::read_xml(filename, xmlTree);
    }
    catch(bptree::ptree_error &e) {
        throw XmlReadException("Cannot load session: ", e);
    }
    // Séparation des grandes parties du fichier
    auto miamTree = xmlTree.get_child("miam");
    auto spatTree = miamTree.get_child("spat");
    // Envoi de chaque grande partie à la sous-partie de spat concernée
    model->GetSpatInterpolator()->SetSpatStatesFromTree(spatTree);
}
