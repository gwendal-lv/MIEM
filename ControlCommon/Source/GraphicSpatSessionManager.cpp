/*
  ==============================================================================

    GraphicSpatSessionManager.cpp
    Created: 13 Jun 2017 9:11:20pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "GraphicSpatSessionManager.h"

#include "SpatPresenter.h"
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
    // Variables qui servent tout au long de cet algorithme de chargement
    bool forceReinit = false;
    int64_t biggestAreaUid = -1;
    
    // Si la commande de réinit était clairement passée à cette fonction :
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
                std::string canvasAndSceneString = "Canvas " + boost::lexical_cast<std::string>(i) + ", Scene " + boost::lexical_cast<std::string>(j) + ": ";
                
                // Première passe de lecture
                size_t areasCount;
                try {
                    areasCount = XmlUtils::CheckIndexes(*(sceneTrees[j]), "scene.areas", "area");
                }
                catch (XmlReadException &e) {
                    throw XmlReadException(canvasAndSceneString, e);
                }
                // Pré-chargement des aires
                std::vector<std::shared_ptr<InteractiveArea>> areas; // y compris les excitateurs
                std::vector< std::shared_ptr<bptree::ptree> > spatStateTrees;
                areas.resize(areasCount);
                spatStateTrees.resize(areasCount);
                for (auto& area : sceneTrees[j]->get_child("scene.areas"))
                {
                    try {
                        auto index = area.second.get<size_t>("<xmlattr>.index");
                        auto type = area.second.get<std::string>("<xmlattr>.type");
                        // Spat Polygones
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
                        // Excitateurs
                        else if (type == "Exciter")
                        {
                            try {
                                areas[index] = std::make_shared<Exciter>(area.second, GetCommonTimePoint(), Exciter::AdditionnalGrabRadius::Medium);
                            }
                            catch (XmlReadException &e) {
                                throw XmlReadException(canvasAndSceneString + e.what());
                            }
                        }
                        // Le reste : on ne prend pas...
                        else
                            throw XmlReadException(canvasAndSceneString + "only SpatPolygon and Exciters objects can be loaded at the moment.");
                        
                        // Recherche de l'UID le plus grand utilisé jusqu'ici
                        if ( areas[index]->GetId() > biggestAreaUid )
                            biggestAreaUid = areas[index]->GetId();
                    }
                    catch (bptree::ptree_error &e) {
                        throw XmlReadException(canvasAndSceneString, e);
                    }
                }
                // Finalement, ajout effectif des aires dans le bon ordre
                for (size_t k=0 ; k<areas.size() ; k++)
                {
                    // Fonction commune à toutes les aires
                    canvasManagers[i]->AddAreaToScene(j, areas[k]);
                    
                    // Spat areas seulement
                    if (auto spatArea = std::dynamic_pointer_cast<SpatArea>(areas[k]))
                        LoadSpatAreaLinks(spatArea, spatStateTrees[k]);
                }
                
                // Actualisations finales pour la scène en cours (après la 2nde passe)
                // ... néant ... on accède le moins possible à des scènes sans passer
                // par le canevas maître
            }
            
        } // fin de condition : si pas de réinitialisation forcée
    } // fin de la boucle : pour chaque canevas
    
    // Bonnes valeurs pour les compteurs internes
    nextAreaId = biggestAreaUid + 1; // On passe à l'UID suivant
    
    // Actualisations pour chaque canevas
    for (auto& canvas : canvasManagers)
        canvas->OnXmlLoadFinished();
}
void GraphicSpatSessionManager::LoadSpatAreaLinks(std::shared_ptr<SpatArea> area, std::shared_ptr<bptree::ptree> spatStateTree)
{
    if (spatStateTree == nullptr)
        return;
    
    auto spatStateIndex = spatStateTree->get<int64_t>("<xmlattr>.index", -1);
    if (spatStateIndex >= 0)
        area->LinkToSpatState(spatInterpolator->GetSpatState(spatStateIndex));
}

