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
