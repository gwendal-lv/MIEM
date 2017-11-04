/*
  ==============================================================================

    Model.cpp
    Created: 24 Jan 2017 9:09:12am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Model.h"

#include "Presenter.h"


using namespace Miam;


Model::Model(Presenter* presenter_)
:
SpatModel(presenter_, 500.0),
presenter(presenter_) // own private downcasted pointer
{
    // Pas très propre... Mais pour l'instant c'est la seule option
    miamOscSender = std::dynamic_pointer_cast<MiamOscSender<double>>(spatSenders[0]);
    if (! miamOscSender )
        throw std::runtime_error("First and only SpatSender must be a MiamOscSender at the moment");
    
    // Fin d'Initialisation des autres modules
    presenter->CompleteInitialisation(this);
    
    // Launch of thread, at the specified frequency
    continueUpdate = true;
    // Using a c++11 lambda function for class member calling
    updateThread = std::thread( [this] {this->update();} );
}

Model::~Model()
{
    // On attend le join propre du thread avant la fermeture
    continueUpdate = false;
    updateThread.join();
}


void Model::update()
{
    while(continueUpdate)
    {
        updateThreadMeasurer.OnNewFrame();
        
        // Infos de performance -> à passer dans un fichier texte pour ne pas perturber la mesure...
        // (écriture en asynchrone dans un thread séparé)
#ifdef __MIAM_DEBUG
        if (updateThreadMeasurer.IsFreshAverageAvailable())
            DBG(updateThreadMeasurer.GetInfo());
#endif
      
        // Récupération de toutes les données les + à jour
        AsyncParamChange lastParamChange;
        while (presenter->TryGetAsyncParamChange(lastParamChange))
        {
            switch (lastParamChange.Type)
            {
                case AsyncParamChange::Excitement :
                    spatInterpolator->OnNewExcitementAmount(lastParamChange.Id1, lastParamChange.DoubleValue);
                    break;
                    
                case AsyncParamChange::Play :
                    std::cout << "[Modèle] PLAY" << std::endl;
                    spatInterpolator->OnPlay();
                    break;
                    
                case AsyncParamChange::Stop :
                    std::cout << "[Modèle] STOP (non-implémenté)" << std::endl;
                    break;
                    
                default :
                    break;
            }
        }
        
        // Envoi de la nouvelle matrice, si nécessaire
        bool somethingWasUpdated = spatInterpolator->OnDataUpdateFinished();
        if (somethingWasUpdated)
        {
            miamOscSender->SendStateModifications(spatInterpolator->GetCurrentInterpolatedState());
        }
        
        
        // Envoi d'un coefficient pour avoir toujours une mise à jour qui tourne derrière....
        // Envoi d'un coefficient pour avoir toujours une mise à jour qui tourne derrière....
        // Envoi d'un coefficient pour avoir toujours une mise à jour qui tourne derrière....
        // Envoi d'un coefficient pour avoir toujours une mise à jour qui tourne derrière....
        // Envoi d'un coefficient pour avoir toujours une mise à jour qui tourne derrière....
        // Envoi d'un coefficient pour avoir toujours une mise à jour qui tourne derrière....
        // TO DO
        
        // Sleep forcé uniquement si on est assez loin de la période souhaitée
        // On prend 1 ms de marge pour la réaction de l'OS en sortie de sleep (TOTALEMENT ARBITRAIRE !)
        if (updateThreadMeasurer.GetElapsedTimeSinceLastNewFrame_us() < updateThreadT_us - 1000)
            std::this_thread::sleep_for(std::chrono::microseconds(updateThreadT_us
                                                                  - updateThreadMeasurer.GetElapsedTimeSinceLastNewFrame_us() ) );
    }
}

void Model::SetConfigurationFromTree(bptree::ptree& tree)
{
    // D'abord on appelle le parent qui fait juste du stockage de données
    SpatModel::SetConfigurationFromTree(tree);
    
    // Ensuite on essaie de mettre en route les fonctionnalités, puis
    // on tient le Presenter au courant
    presenter->OnNewConnectionStatus( getMainSpatSender()->TryConnect(),
                                     getMainSpatSender()->GetConfigurationTree() );
}

