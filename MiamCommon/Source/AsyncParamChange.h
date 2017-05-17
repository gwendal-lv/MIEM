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


namespace Miam
{
    /// \brief A simple structure employed to send messages between the
    /// Miam::Presenter and the Miam::Model (in one way or another) via
    /// a boost lock-free queue
    struct AsyncParamChange
    {
        public :
        
        
        
        enum ParamType {
            
            None,

			Play,
			Pause,
			Stop,
            
            Activate,
            Volume,
            Frequency,

			Duration,
			Position,
			Source,
            
            InputsCount,
            OutputsCount,
            InputsAndOutputsCount,
            
            Update,
            UpdateDisplay,
            
            UdpPort,
            TcpPort,
        };
        
        ParamType Type = ParamType::None;
        
        int Id1 = 0, Id2 = 0;
        
        double DoubleValue = 0.0;
        int IntegerValue = 0;
        float FloatValue = 0.0f;
        
        
        std::string ToString()
        {
            std::string returnString = "ParamChange : ";
            
            switch(Type)
            {
                case None: returnString = "None"; break;
                case Volume : returnString = "Volume"; break;
                    
                default :
                    returnString = std::to_string((int)Type);
            }
            
            returnString += " Id1=";
            returnString += std::to_string(Id1);
            returnString += " Id2=";
            returnString += std::to_string(Id2);
            returnString += " DoubleValue=";
            returnString += std::to_string(DoubleValue);
            returnString += " IntegerValue=";
            returnString += std::to_string(IntegerValue);
            
            return returnString;
        }
        
        
    };
    
}


#endif  // ASYNCPARAMCHANGE_H_INCLUDED
