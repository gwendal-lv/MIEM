/*
  ==============================================================================

    PathUtils.h
    Created: 8 Jan 2018 3:29:55pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
