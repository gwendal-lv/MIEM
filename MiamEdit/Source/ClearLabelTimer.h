/*
  ==============================================================================

    ClearLabelTimer.h
    Created: 13 Apr 2016 2:57:58pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef CLEARLABELTIMER_H_INCLUDED
#define CLEARLABELTIMER_H_INCLUDED

#include "JuceHeader.h"



namespace Miam
{
    
    class ClearLabelTimer : public Timer
    {
        
        public :
        void startTimer(Label* _label);
        void timerCallback() override;
        
        private :
        Label* label;
        int intervalInMilliseconds = 5000;
        
    };
    
}



#endif  // CLEARLABELTIMER_H_INCLUDED
