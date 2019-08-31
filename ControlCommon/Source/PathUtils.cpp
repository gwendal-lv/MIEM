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

#include "JuceHeader.h"

#include "PathUtils.h"

using namespace Miam;



std::string PathUtils::GetSessionFileExtension(AppPurpose appType)
{
    switch (appType) {
            
        case AppPurpose::Spatialisation:
            return std::string("mspat"); // Miem Spatialisation
            break;
            
        case AppPurpose::GenericController:
            return std::string("mcs"); // Miem Controller Session
            break;
            
        default:
            // on ne devrait pas demander l'extension d'un type pas clairement précisé...
            assert(false);
            return std::string("*");
            break;
    }
}


bool PathUtils::CheckForExtensionAndPurposeCoherence(std::string filename, AppPurpose sessionPurpose)
{
    auto lastDotPos = filename.find_last_of(".");
    std::string extension = filename.substr(lastDotPos + 1); // sans le point
    
    return (GetSessionFileExtension(sessionPurpose) == extension);
}


std::string PathUtils::GenerateAllowedFilePatterns(std::initializer_list<AppPurpose> appTypeArgs,
                                                   bool includeStarInPattern,
                                                   std::string separator)
{
    std::string returnString;
    
    bool firstElementWritten = false;
    bool multiPurposeApp = false;
    for (auto appPurpose : appTypeArgs)
    {
        // Cas spéciaux : multi et/ou none
        if (appPurpose == AppPurpose::None)
            continue;
        else if (appPurpose == AppPurpose::Multi)
        {
            multiPurposeApp = true;
            break;
        }
        else
        {
            // On ajoute un séparateur seulement après le premier élément
            if (firstElementWritten)
                returnString += separator;
            // Ajout de l'étoile si demandé seulement
            if (includeStarInPattern)
                returnString = returnString + "*";
            // Ajout de l'extension demandée
            returnString = returnString + "." + GetSessionFileExtension(appPurpose);
            // élément suivant...
            firstElementWritten = true;
        }
    }
    
    if (!multiPurposeApp)
        return returnString;
    else // précision ici du type multi !!!
        return GenerateAllowedFilePatterns({AppPurpose::Spatialisation, AppPurpose::GenericController});
}




std::string PathUtils::GetAppToRepositoryRootPath()
{
    // Chemin par rapport aux exécutables : dépend de l'OS
#if BOOST_OS_WINDOWS // Car chemin de la forme : MiamXyz/Builds/VisualStudio2015/x64/Debug/App
    return "../../../../../../";
#elif BOOST_OS_MACOS // Car chemin de la forme : MiamXyz/Builds/MacOSX/build/Debug/
    return "../../../../../";
#else
    return "";
#endif
}



std::string PathUtils::GetAppToRepositorySpatSessionsPath()
{
    if (GetAppToRepositoryRootPath() == "")
        throw std::logic_error("Path to repo root is not defined. Cannot continue");
    else
        return GetAppToRepositoryRootPath() + "SpatCommon/Sessions/";
}


std::string PathUtils::GetAppToRepositoryControlSessionsPath()
{
    if (GetAppToRepositoryRootPath() == "")
        throw std::logic_error("Path to repo root is not defined. Cannot continue");
    else
        return GetAppToRepositoryRootPath() + "GenericController/Sessions/";
}



File PathUtils::GetSessionsFolderDefaultPath()
{
#ifdef __MIAM_DEBUG // Debug : sessions
#if defined(BOOST_OS_WINDOWS) || defined(BOOST_OS_MACOS) // Debug DESKTOP : on cherche les sessions dans le repo
    // Chemin du dossier, sans le '/' final
    File appDirectory = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory();
    String sessionDirectoryPath = appDirectory.getFullPathName() + GetAppToRepositorySessionsPath();
    return File(sessionDirectoryPath);
#else // Debug MOBILE : on balance le dossier musique par défaut
    return File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory);
#endif
#else // Release : dossier musique, par défaut
    return File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory);
#endif
}



