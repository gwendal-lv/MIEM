/*
  ==============================================================================

    SpatSender.hpp
    Created: 26 Nov 2016 3:17:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSENDER_H_INCLUDED
#define SPATSENDER_H_INCLUDED

#include <memory>
#include <string>
#include "SpatState.hpp"


#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


namespace Miam
{
    
    
    
    
    /// \brief An abstract class to be inherited by any class that is able
    /// to send (midi, OSC, network, camel, pony, ...) some Miam Spatialization
    /// data.
    template<typename T>
    class SpatSender
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :

        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual std::string GetTypeAsString() const = 0;


        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
       
        
        // - - - - - Construction / destruction - - - - -
        virtual ~SpatSender() {}
        
        
        // - - - - - Communication - - - - -
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        // BESOIN DE CONNAITRE LE SPAT INTERPOLATOR (LE NB DE CANAUX) en weak_ptr
        virtual void SendState(SpatState<double>& spatState) = 0;
        
        
        // - - - - - XML import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetConfigurationTree()
        {
            auto configurationTree = std::make_shared<bptree::ptree>();
            configurationTree->put("<xmlattr>.type", GetTypeAsString());
            return configurationTree;
        }
        virtual void SetConfigurationFromTree(bptree::ptree& /*tree*/)
        {}
    };
    
}



#endif  // SPATSENDER_H_INCLUDED
