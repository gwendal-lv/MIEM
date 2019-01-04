/*
  ==============================================================================

    PeriodicUpdateThread.cpp
    Created: 3 May 2017 5:29:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "PeriodicUpdateThread.h"

#include "JuceHeader.h"

using namespace Miam;


PeriodicUpdateThread::PeriodicUpdateThread(std::string name_)
:
updateThreadMeasurer(name_, 5000) // refresh toutes les 5000 frames
{
    threadHasBeenLaunched = false;
   
    // Test of lockfreeness
    if (continueUpdate.is_lock_free())
        DBG("Atomic bool is lock free on this platform");
    else
        throw std::runtime_error("Atomic bool is not lock free on this platform");
}

