/*
  ==============================================================================

    PeriodicUpdateThread.h
    Created: 3 May 2017 5:29:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once


#include <thread>
#include <atomic>


#include "FrequencyMeasurer.h"


namespace Miam
{
    
    class PeriodicUpdateThread
    {
        
        protected :
        /// \brief Model update thread running at an almost constant frequency
        std::thread updateThread;
        double updateThreadF_Hz;
        int updateThreadT_us;
        std::atomic<bool> continueUpdate;
        /// \brief Time measuring helper for thread frequency stabilisation
        FrequencyMeasurer updateThreadMeasurer;
        
        
        public :
        PeriodicUpdateThread(std::string name_);
    };
    
    
    
} // namespace Miam
