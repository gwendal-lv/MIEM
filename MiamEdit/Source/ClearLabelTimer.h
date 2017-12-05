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
        
        ClearLabelTimer(int& currentDisplayedInfoPriority_) :
        currentDisplayedInfoPriority(currentDisplayedInfoPriority_)
        {}
        
        void SetLabelToClearAfterTimeout(Label* label_);
        
		/// \brief Starts the timer, and plans the cleaning of given label.
        void StartTimer();
        
        void timerCallback() override;
        
        
        
        
        private :
        Label* label = nullptr;
        int& currentDisplayedInfoPriority; // original info in MainBackgroundComponent
	public :
		/// \brief Amount of time remaining until the specified label is cleared.
        const int IntervalInMilliseconds = 10000; // 15s... parfois msg complexe
        
    };
    
}



#endif  // CLEARLABELTIMER_H_INCLUDED
