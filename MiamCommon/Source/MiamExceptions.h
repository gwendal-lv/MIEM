/*
  ==============================================================================

    MiamExceptions.h
    Created: 8 Jun 2017 8:51:43pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>


#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


namespace Miam
{
    class ForceQuitException : public std::runtime_error
    {
        public :
        // Juste une runtime castée...
        ForceQuitException() : std::runtime_error(std::string("ForceQuitException"))
        {}
        ForceQuitException(std::string errorString) : std::runtime_error(errorString)
        {}
    };
    
    class ParseException : public std::runtime_error
    {
        public :
        // Juste une runtime castée...
        ParseException() : std::runtime_error(std::string("Parse Error"))
        {}
        ParseException(std::string errorString) : std::runtime_error(errorString)
        {}
    };
    
    class XmlReadException : public std::runtime_error
    {
        public :
        
        XmlReadException()
        :
        std::runtime_error(std::string("XML read error"))
        {}
        
        XmlReadException(std::string errorString)
        :
        std::runtime_error(errorString)
        {}
        
        /// \brief Contructor thats creates a new exception, which "what" message
        /// is the addition of a given text and the text of an already existing
        /// exception
        XmlReadException(std::string textErrorToAddFirst, std::runtime_error& originalError)
        :
        std::runtime_error(textErrorToAddFirst + originalError.what())
        {}
        
        ~XmlReadException() {}
        
        
        
        static XmlReadException FromBptree(std::string nodeName, std::runtime_error& bptreeException)
        {
            if (auto badPath = dynamic_cast<bptree::ptree_bad_path*>(&bptreeException))
                return XmlReadException("Path error in node <" + nodeName + ">: ", bptreeException);
            else if (auto badData = dynamic_cast<bptree::ptree_bad_data*>(&bptreeException))
                return XmlReadException("Data translation error in node <" + nodeName + ">: ", bptreeException);
            else
                return XmlReadException();
        }
        
    };
    
    
    class XmlWriteException : public std::runtime_error
    {
        public :
        
        XmlWriteException()
        :
        std::runtime_error(std::string("XML write error"))
        {}
        
        XmlWriteException(std::string errorString)
        :
        std::runtime_error(errorString)
        {}
        
        /// \brief Contructor thats creates a new exception, which "what" message
        /// is the addition of a given text and the text of an already existing
        /// exception
        XmlWriteException(std::string textErrorToAddFirst, std::runtime_error& originalError)
        :
        std::runtime_error(textErrorToAddFirst + originalError.what())
        {}
    };
    
}
