/*
  ==============================================================================

    GraphicSpatSessionManager.cpp
    Created: 13 Jun 2017 9:11:20pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "GraphicSpatSessionManager.h"
#include "SpatPolygon.h"

#include "XmlUtils.h"

GraphicSpatSessionManager::GraphicSpatSessionManager(SpatPresenter* _presenter)
:
IGraphicSessionManager(_presenter),
presenter(_presenter)
{}

void GraphicSpatSessionManager::CompleteInitialisation(std::shared_ptr<SpatInterpolator<double>> _spatInterpolator)
{
    spatInterpolator = _spatInterpolator;
}


// = = = = = = = = = = XML import/export = = = = = = = = = =

void GraphicSpatSessionManager::SetFromTree(bptree::ptree& graphicSessionTree)
{
    bool forceReinit = false;
    if (graphicSessionTree.empty())
        forceReinit = true;
    
    // Peut lancer une exception (et donc il faudra lancer une réinitialisation du module entier)
    std::vector< std::shared_ptr<bptree::ptree> > canvasTrees;
    if ( ! forceReinit )
    {
        try {
            canvasTrees = IGraphicSessionManager::ExtractCanvasesSubTrees(graphicSessionTree);
        }
        // S'il y a une erreur : on quitte la fonction directement
        catch (bptree::ptree_error& e) {
            throw XmlReadException("<graphicsession> : error extracting <canvas> nodes : ", e);
        }
    }
    
    // On fait des actions pour tous les canevas même si une erreur était apparue
    for (size_t i=0 ; i<canvasManagers.size() ; i++)
    {
        // D'abord on crée toutes les scènes
        // Mais quand même, s'il y a eu une erreur, on réinitialise (en envoyant un arbre vide)
        if (forceReinit)
        {
            bptree::ptree emptyTree = bptree::ptree(); // pour le passer en référence
            canvasManagers[i]->SetScenesFromTree<EditableScene>( emptyTree );
        }
        // Si pas de réinit
        else
        {
            auto sceneTrees = canvasManagers[i]->SetScenesFromTree<EditableScene>(*(canvasTrees[i]));
            
            // Puis on parcourt toutes les scènes du canevas en cours, pour
            // n'ajouter que les aires qui correspondent à notre application
            // (seulement les SpatPolygon pour l'instant...)
            for (size_t j=0 ; j<sceneTrees.size() ; j++)
            {
                std::string canvasAndSceneString = "Canvas " + std::to_string(i) + ", Scene " + std::to_string(j) + ": ";
                
                // Première passe de lecture
                size_t areasCount;
                try {
                    areasCount = XmlUtils::CheckIndexes(*(sceneTrees[j]), "scene.areas", "area");
                }
                catch (XmlReadException &e) {
                    throw XmlReadException(canvasAndSceneString, e);
                }
                // Pré-chargement des aires
                std::vector<std::shared_ptr<SpatArea>> areas;
                std::vector< std::shared_ptr<bptree::ptree> > spatStateTrees;
                areas.resize(areasCount);
                spatStateTrees.resize(areasCount);
                for (auto& area : sceneTrees[j]->get_child("scene.areas"))
                {
                    try {
                        auto index = area.second.get<size_t>("<xmlattr>.index");
                        auto type = area.second.get<std::string>("<xmlattr>.type");
                        if (type == "SpatPolygon")
                        {
                            try {
                                areas[index] = std::make_shared<SpatPolygon>(area.second);
                                try {
                                    spatStateTrees[index] = std::make_shared<bptree::ptree>(area.second.get_child("spatstate"));
                                }
                                catch (bptree::ptree_bad_path) {/* we do nothing */}
                            }
                            catch (XmlReadException &e) {
                                throw XmlReadException(canvasAndSceneString + e.what());
                            }
                        }
                        else
                            throw XmlReadException(canvasAndSceneString + "only SpatPolygon objects can be loaded at the moment.");
                    }
                    catch (bptree::ptree_error &e) {
                        throw XmlReadException(canvasAndSceneString, e);
                    }
                }
                // Finalement, ajout effectif des aires dans le bon ordre
                for (size_t k=0 ; k<areas.size() ; k++)
                {
                    canvasManagers[i]->AddAreaToScene(j, areas[k]);
                    LoadSpatAreaLinks(areas[k], spatStateTrees[k]);
                }
            }
        }
    }
}
void GraphicSpatSessionManager::LoadSpatAreaLinks(std::shared_ptr<SpatArea> area, std::shared_ptr<bptree::ptree> spatStateTree)
{
    if (spatStateTree == nullptr)
        return;
    
    auto spatStateIndex = spatStateTree->get<int64_t>("<xmlattr>.index", -1);
    if (spatStateIndex >= 0)
        area->LinkToSpatState(spatInterpolator->GetSpatState(spatStateIndex));
}

