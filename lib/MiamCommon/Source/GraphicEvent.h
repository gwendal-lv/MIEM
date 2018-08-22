/*
  ==============================================================================

    GraphicEvent.h
    Created: 31 Dec 2016 10:16:44am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef GRAPHICEVENT_H_INCLUDED
#define GRAPHICEVENT_H_INCLUDED


#include <string>


namespace Miam
{
	
    
    class GraphicEvent {

		

        protected :
        /// \brief Message that can contain anything...
        std::string message;
		
        
        public :
        virtual void SetMessage(std::string message_) {message = message_;}
        virtual std::string GetMessage() {return message;}
        
		GraphicEvent(){}
        virtual ~GraphicEvent() {}
        
    };
    
}



#endif  // GRAPHICEVENT_H_INCLUDED
