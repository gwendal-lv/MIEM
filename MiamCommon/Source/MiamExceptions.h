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

#include <iostream>


#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


namespace Miam
{
    class BadIdException : public std::runtime_error
    {
        public :
        // Juste une runtime castée...
        BadIdException() : std::runtime_error(std::string("MIEM bad ID Exception"))
        {}
        BadIdException(std::string errorString) : std::runtime_error(errorString)
        {}
    };
    
    class OscException : public std::runtime_error
    {
        public :
        // Juste une runtime castée...
        OscException() : std::runtime_error(std::string("MIEM OSC Exception"))
        {}
        OscException(std::string errorString) : std::runtime_error(errorString)
        {}
    };
    
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
