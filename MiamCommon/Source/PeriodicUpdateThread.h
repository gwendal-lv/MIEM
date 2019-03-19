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
        std::atomic<bool> threadHasBeenLaunched;
        double updateThreadF_Hz;
        int updateThreadT_us;
        std::atomic<bool> continueUpdate;
        /// \brief Time measuring helper for thread frequency stabilisation
        FrequencyMeasurer updateThreadMeasurer;
        
        
        public :
        PeriodicUpdateThread(std::string name_);
        
        
        
        
        // - - - - - Static functions for threads manipulation - - - - -
        /// \brief If possible, sets the name of the calling thread using available
        /// POSIX interface on current OS.
        static void SetThreadName(std::string name);
        /// \brief If possible, sets the current thread to high priority using
        /// available POSIX interface on current OS.
        static void SetHighThreadPriority();
    };
    
    
    
} // namespace Miam
