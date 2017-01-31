/*
  ==============================================================================

    AsyncParamChange.h
    Created: 23 Dec 2016 4:31:20pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef ASYNCPARAMCHANGE_H_INCLUDED
#define ASYNCPARAMCHANGE_H_INCLUDED


namespace Miam
{
    /// \brief A simple structure employed to send messages between the
    /// Miam::Presenter and the Miam::Model (in one way or another) via
    /// a Miam::LockFreeQueue
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
        };
        
        ParamType Type = ParamType::None;
        
        int Id1 = 0, Id2 = 0;
        
        double DoubleValue = 0.0;
        int IntegerValue = 0;
        
        
        
    };
    
}


#endif  // ASYNCPARAMCHANGE_H_INCLUDED
