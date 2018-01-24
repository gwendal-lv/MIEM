/*
 ==============================================================================
 
 PathUtils.cpp
 Created: 8 Jan 2018 3:29:55pm
 Author:  Gwendal Le Vaillant
 
 ==============================================================================
 */

#include "JuceHeader.h"

#include "PathUtils.h"

using namespace Miam;



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



std::string PathUtils::GetAppToRepositorySessionsPath()
{
    if (GetAppToRepositoryRootPath() == "")
        throw std::logic_error("Path to repo root is not defined. Cannot continue");
    else
        return GetAppToRepositoryRootPath() + "SpatCommon/Sessions/";
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


