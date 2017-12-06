/*
  ==============================================================================

    InteractionParameters.h
    Created: 5 Dec 2017 9:55:51pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once


namespace Miam
{
    /// \brief Paramètre de l'IHM. Dans une classe séparée, pour expérimentations.
    ///
    /// Voir fichier .cpp pour les valeurs des paramètres, et plus d'informations
    /// sur les paramètres eux-mêmes.
    /// Pas d'infos ici pour ne pas devoir tout recompiler
    /// à chaque tentative de paramétrage.... Ou à chaque export d'une version paramétrée...
    class InteractionParameters
    {
        public :
        
        static const double InfluenceOfDistanceFromCenter;
        
        static const double LowVolumePrecisionFactor;
    };
}



