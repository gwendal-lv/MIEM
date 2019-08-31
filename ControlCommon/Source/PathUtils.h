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

#include "JuceHeader.h"

#include <string>

#include <initializer_list>

#include "AppPurpose.h"


namespace Miam
{
    
    
    class PathUtils
    {
        public :
        
        
        /// \brief Retourne l'extension associée au type demandé, sans le '.' avant l'extension.
        static std::string GetSessionFileExtension(AppPurpose appType);
        
        /// \brief Retourne si le fichier passé en paramètre a une extension qui correspond
        /// bien au type de session précisé en paramètre, ou non.
        static bool CheckForExtensionAndPurposeCoherence(std::string filename, AppPurpose sessionPurpose);
        
        /// \brief Génère une chaîne de caractère formatée pour les juce::FileChooser
        ///
        /// Les séparateurs sont des ';', et les extensions demandées sont précisées avec le '.'
        /// avant l'extension.
        static std::string GenerateAllowedFilePatterns(std::initializer_list<AppPurpose> appTypeArgs,
                                                       bool includeStarInPattern = true,
                                                       std::string separator = ";");
        
        
        static std::string GetAppToRepositoryRootPath();
        
        static std::string GetAppToRepositorySpatSessionsPath();
        static std::string GetAppToRepositoryControlSessionsPath();
        
        static File GetSessionsFolderDefaultPath();
        
    };
    
}
