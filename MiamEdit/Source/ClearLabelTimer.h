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
    
	/// \brief Timer dedicated to the task of clearing a juce::Label after a specified amount of time.
    class ClearLabelTimer : public Timer
    {
        
        public :
		/// \brief Starts the timer, and plans the cleaning of given label.
        void StartTimer(Label* _label);
        void timerCallback() override;
        
        private :
        Label* label;
	public :
		/// \brief Amount of time remaining until the specified label is cleared.
        const int IntervalInMilliseconds = 5000;
        
    };
    
}



#endif  // CLEARLABELTIMER_H_INCLUDED
