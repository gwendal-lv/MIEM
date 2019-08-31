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

#include <boost/predef.h> // OS-specific defines

#include "JuceHeader.h"

#include <string>

#include "PathUtils.h"

namespace Miam
{
    

    class LoadFileChooser : public FileChooser
    {
        public :
        LoadFileChooser(std::initializer_list<AppPurpose> appTypeArgs) :
        FileChooser(TRANS("Please select a MIEM session to load: "),
                    PathUtils::GetSessionsFolderDefaultPath(),
                    PathUtils::GenerateAllowedFilePatterns(appTypeArgs),
                    true)
        {
#if defined( __MIAMOBILE )
            /*
             * Only desktop version can at the moment use these generic Pop-up File Choosers
             */
            //assert(0);
            // Pour l'instant : version DEVELOP de Juce qui permet des filechooser ASYNC natifs iOS/Android
#endif
        }
    };
    
    
    class SaveFileChooser : public FileChooser
    {
        public :
        SaveFileChooser(std::initializer_list<AppPurpose> appTypeArgs) :
        FileChooser(TRANS("Save MIEM session as..."),
                    PathUtils::GetSessionsFolderDefaultPath(),
                    PathUtils::GenerateAllowedFilePatterns(appTypeArgs),
                    true)
        {
#if defined( __MIAMOBILE )
            /*
             * Only desktop version can at the moment use these generic Pop-up File Choosers
             */
            //assert(0);
            // Pour l'instant : version DEVELOP de Juce qui permet des filechooser ASYNC natifs iOS/Android
#endif
            
        }
    };
    
    
}
