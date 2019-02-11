/*
  ==============================================================================

    AsyncParamChange.h
    Created: 23 Dec 2016 4:31:20pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef ASYNCPARAMCHANGE_H_INCLUDED
#define ASYNCPARAMCHANGE_H_INCLUDED

#include <string>
#include "boost/lexical_cast.hpp"

#include "ParametersIds.h"


namespace Miam
{
    /// \brief A simple structure employed to send messages between the
    /// Miam::Presenter and the Miam::Model (in one way or another) via
    /// a boost lock-free queue
    struct AsyncParamChange
    {
        /// \brief The following parameter types are quite generic,
        /// and they can be detailed using the IDs from ParametersIds.h
        ///
        /// The IDs from ParametersIds.h can be stored within the
        /// Id1, Id2 integer atributes.
        ///
        /// Insert new values juste before the fixed ones (100, 200, etc...)
        /// because the int values are also employed for TCP interprocess
        /// communication.
        enum ParamType {
            
            None = 0,
            
            Reinitialize,
            LoadingComplete,
            
            
            
            Play = 100, ///< Play command
                Playing, ///< Play information
            Pause,
            Stop, ///< Stop command
                Stopped, ///< Stop information
            
            
            Activate = 200,
            MasterVolume,
            Volume,
                Volume_CorrelatedInputs,
                Volume_DecorrelatedInputs,
            Frequency,
            Excitement,
            Duration,
            
            
            Position = 300,
            Source,
            
            
            InputsCount = 400,
            OutputsCount,
            InputsAndOutputsCount,
            
            
            
            Update = 500,
            UpdateDisplay,
            
            
            
            UdpPort = 600,
            TcpPort,
            
            
            Scene = 700,
        };
        
        
        
        public :
        
        /// \brief Default constructor with init to null values.
        AsyncParamChange() {Reset();}
        
        /// \brief Constructor with init of the type of parameter only
        AsyncParamChange(ParamType paramType)
        {
            Reset();
            Type = paramType;
        }
        
        
        ParamType Type = ParamType::None;
        
        int Id1, Id2;
        
        double DoubleValue;
        int IntegerValue;
        float FloatValue;
        
        void Reset()
        {
            Type = ParamType::None;
            
            Id1 = 0;
            Id2 = 0;
            
            DoubleValue = 0.0;
            IntegerValue = 0;
            FloatValue = 0.0f;
        }
        
        std::string ToString()
        {
            std::string returnString = "ParamChange : ";
            
            switch(Type)
            {
                case None: returnString = "None"; break;
                case Volume : returnString = "Volume"; break;
                    
                default :
                    returnString = boost::lexical_cast<std::string>((int)Type);
            }
            
            returnString += " Id1=";
            returnString += boost::lexical_cast<std::string>(Id1);
            returnString += " Id2=";
            returnString += boost::lexical_cast<std::string>(Id2);
            returnString += " DoubleValue=";
            returnString += boost::lexical_cast<std::string>(DoubleValue);
            returnString += " IntegerValue=";
            returnString += boost::lexical_cast<std::string>(IntegerValue);
            
            return returnString;
        }
        
        
    };
    
}


#endif  // ASYNCPARAMCHANGE_H_INCLUDED
