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





void PeriodicUpdateThread::SetThreadName(std::string name)
{
#if defined(JUCE_ANDROID)
    // Android alone seems to support the POSIX classical interface
    pthread_setname_np(pthread_self(), name.c_str());
#elif defined(JUCE_MAC) || defined(JUCE_IOS)
    pthread_setname_np(/*pthread_self(), */ name.c_str()); // pas de TID pour FreeBSD 2003... (doc macOS)
#elif defined(JUCE_WINDOWS)
    // rien pour Windows.... pour l'instant
#endif
}


void PeriodicUpdateThread::SetHighThreadPriority()
{
    // Si possible : mise à un niveau de priorité pour le thread de mise à jour
    // -> voire même un autre ordonnancement sous certains OS ?
    
#if defined(JUCE_MAC) || defined(JUCE_IOS)
    // On n'utilisera pas les fonctions d'accès direct à l'ordonnanceur MACH, trop complexe...
    // - - Identification des politiques d'ordonnancement POSIX - -
    std::map<int, std::string> policyToString;
    policyToString[SCHED_FIFO] = "FIFO"; // 4
    policyToString[SCHED_RR] = "Round-Robin"; // 2
    policyToString[SCHED_OTHER] = "Other"; // 1
    int requestedPolicy = SCHED_FIFO;
    // - - Choix de la priorité - -
    // Le 2 (c-à-d Round-Robin) est bien précisé dans la doc via terminal macOS...
    int minPriority = sched_get_priority_min(requestedPolicy);
    int maxPriority = sched_get_priority_max(requestedPolicy);
    int requestedPriority = maxPriority;
    if (minPriority == maxPriority)
        std::cout << "[Model] The OS does not provide thread priorities." << std::endl;
    // - - Application de la priorité - -
    struct sched_param initialSchedParam;
    int initialPolicy = 0;
    pthread_getschedparam(pthread_self(), &initialPolicy, &initialSchedParam);
    struct sched_param actualSchedParam;
    actualSchedParam.sched_priority = requestedPriority;
    pthread_setschedparam(pthread_self(), requestedPolicy, &actualSchedParam);
    int actualPolicy = 0;
    pthread_getschedparam(pthread_self(), &actualPolicy, &actualSchedParam);
    // Infos graphiques
    std::cout << "[Model Thread] Scheduling was changed from "
    << policyToString[initialPolicy] << "@prio=" << initialSchedParam.sched_priority
    << " to " << policyToString[actualPolicy] << "@prio=" << actualSchedParam.sched_priority
    << std::endl
    << "(" << policyToString[actualPolicy] << " scheduling: minPriority=" << minPriority
    << ", maxPriority=" << maxPriority << ")"
    << std::endl;
    
#else
    /* - - - - - Pour les autres OS - - - - -
     * à développer...
     */
	Logger::outputDebugString("[Thread Model] No priority change implemented for this OS.");
#endif
}
