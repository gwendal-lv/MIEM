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
