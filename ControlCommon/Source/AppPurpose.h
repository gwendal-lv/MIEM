/*
  ==============================================================================

    AppType.h
    Created: 28 Feb 2018 3:48:03pm
    Author:  Gwendal

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "MiamExceptions.h"

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

				case AppPurpose::Multi:
					return translate("Multi-purpose session").toStdString();
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
        
        private :
        /// \brief Retourne la version minimale des sessions (fichiers mspat ou mcs) que cette version
        /// de l'application peut traiter.
        static const int MinimumCompatibleSessionVersionNumber = 0x200;
        public :
        
        /// \brief Checks if the version number (from the main <miam> node) is big enough.
        /// Throws an XmlReadException if any issue is identified
        static void CheckSessionVersionNumber(bptree::ptree& miemTree)
        {
            int versionNumber = 0x00;
            try {
                versionNumber = miemTree.get<int>("<xmlattr>.appVersion");
            }
            catch (bptree::ptree_error& e) {
                throw XmlReadException(std::string("Cannot retrieve the XML appVersion attribute from the main <miam> tag: ") + e.what());
            }
            
            if (versionNumber < MinimumCompatibleSessionVersionNumber)
                throw XmlReadException("Cannot open this session, which was created by a too old version of a MIEM application.");
        }
    };
    
}
