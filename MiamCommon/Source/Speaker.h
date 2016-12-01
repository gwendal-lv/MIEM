/*
  ==============================================================================

    Speaker.h
    Created: 30 Nov 2016 3:13:47pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPEAKER_H_INCLUDED
#define SPEAKER_H_INCLUDED



#include <string>



namespace Miam
{
    
    
    
    
    /// \brief
    ///
    template<typename T>
    class Speaker
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        std::string name;
        double posX = 0.0, posY = 0.0;
        
        // useless for now
        T volume;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        std::string GetName() {return name;}
        void SetName(std::string _name) {name = _name;}
        
        void GetX() {return posX;}
        void SetX(double _X) {posX = _X;}
        void GetY() {return posY;}
        void SetY(double _Y) {posY = _Y;}
        
        
        /*
        virtual T GetVolume() = 0;
        virtual void SetVolume(T volume) = 0;
        */
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        Speaker();
        
        
        // - - - - - Construction / destruction - - - - -
        
        
        
    };
    
}



#endif  // SPEAKER_H_INCLUDED
