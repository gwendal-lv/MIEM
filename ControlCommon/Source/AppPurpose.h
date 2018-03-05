/*
  ==============================================================================

    AppType.h
    Created: 28 Feb 2018 3:48:03pm
    Author:  Gwendal

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


namespace Miam
{
    
    enum class AppPurpose
    {
        
        None = 1,
        
        
        Multi = 2,
        
        
        Spatialisation = 4,
        
        GenericController = 8,
    };
    
    
    class App
    {
        private :
        // Si problème de link (symbole non-défini), il ne faut pas oublier de déclarer et d'initialiser cet
        // attribut qqpart dans le code !! Dans le Presenter final par exemple
        static AppPurpose appPurpose;
        
        public :
        static AppPurpose GetPurpose()
        {
            return appPurpose;
        }
        
        static std::string GetPurposeName(AppPurpose appPurpose_)
        {
            switch (appPurpose_)
            {
                case AppPurpose::Spatialisation :
                    return translate("Spatialization").toStdString();
                    break;
                    
                case AppPurpose::GenericController :
                    return translate("Generic controller").toStdString();
                    break;
                    
                default :
                    return translate("Undefined purpose").toStdString();
                    break;
            }
        }
        
        static std::string GetNameWithVersion()
        {
            return std::string(ProjectInfo::projectName) + " " + ProjectInfo::versionString;
        }
    };
    
}
