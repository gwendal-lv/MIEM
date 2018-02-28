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

namespace Miam
{
    
    class PathUtils
    {
        public :
        
        
        static std::string GetAppToRepositoryRootPath();
        
        static std::string GetAppToRepositorySpatSessionsPath();
        static std::string GetAppToRepositoryControlSessionsPath();
        
        static File GetSessionsFolderDefaultPath();

    };
    
}
