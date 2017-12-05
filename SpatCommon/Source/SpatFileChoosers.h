/*
  ==============================================================================

    SpatFileChoosers.h
    Created: 18 Nov 2017 5:06:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <boost/predef.h> // OS-specific defines

#include "JuceHeader.h"

#include <string>

#define Miam_SessionFileExtension       "mspat"

namespace Miam
{
    
    class PathUtils
    {
        public :
        
        static std::string GetAppToRepositoryRootPath()
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
        
        static std::string GetAppToRepositorySessionsPath()
        {
            if (GetAppToRepositoryRootPath() == "")
                throw std::logic_error("Path to repo root is not defined. Cannot continue");
            else
                return GetAppToRepositoryRootPath() + "SpatCommon/Sessions/";
        }
        
        static File GetSessionsFolderDefaultPath()
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
    };
    
    
    class LoadFileChooser : public FileChooser
    {
        public :
        LoadFileChooser() :
        FileChooser("Please select a MIAM session to load: ",
                    PathUtils::GetSessionsFolderDefaultPath(),
                    std::string("*.") + Miam_SessionFileExtension,
                    true)
        {}
    };
    
    
    class SaveFileChooser : public FileChooser
    {
        public :
        SaveFileChooser() :
        FileChooser("Save MIAM session as...",
                    PathUtils::GetSessionsFolderDefaultPath(),
                    std::string("*.") + Miam_SessionFileExtension,
                    true)
        {}
    };
    
    
}
