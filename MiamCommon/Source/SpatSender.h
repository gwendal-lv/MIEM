/*
  ==============================================================================

    SpatSender.h
    Created: 26 Nov 2016 3:17:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSENDER_H_INCLUDED
#define SPATSENDER_H_INCLUDED


namespace Miam
{
    
    
    
    
    /// \brief An abstract class to be inherited by any class that is able
    /// to send (midi, OSC, network, camel, pony, ...) some Miam Spatialization
    /// data.
    
    class SpatSender
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :

        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
         // to make it abstract... TO REMOVE
        virtual void SendTruc() = 0;

        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
       
        
        // - - - - - Construction / destruction - - - - -
        virtual ~SpatSender() {}
        
    };
    
}



#endif  // SPATSENDER_H_INCLUDED
