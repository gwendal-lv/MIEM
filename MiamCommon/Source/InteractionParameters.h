/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
        
        static const int DistorsionSplineClass;
        static const double CenterToSegmentsWeightRatio;
        
        
        static const double LowVolumePrecisionFactor;
    };
}



