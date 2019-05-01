/*
  ==============================================================================

    OSCListenerForwarder.cpp
    Created: 12 Feb 2019 10:57:17pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>
#include <thread>

#include "PeriodicUpdateThread.h" // from Miam Common
#include "MiamMath.h" // from Miam Common

#include "MonitoringServer.h"

#include "OSCListenerForwarder.h"

using namespace Miam;

OSCListenerForwarder::OSCListenerForwarder(int udpPort, std::chrono::time_point<MiemClock> _startTimePoint, std::string midiDeviceName)
:
oscReceiver("MIEM real-time OSC receiver thread"),
startTimePt_NetworkThreadCopy(_startTimePoint),
experimentStartTimePoint(_startTimePoint),
startTimePt_MidiThreadCopy(_startTimePoint)
{
    // Even if not recording.... The restart Time Point must be initialized,
    // because it is also the reference time for the MIDI forwarder
    restartTimePoint = MiemClock::now();
    reinitMidiDuplicatesDetection();
    
    isRecording = false;
    wasOscThreadRecording = true;
    bufferedSamples.resize(100000); // ordre de grandeur du Mo
    
    if (lastMiemSamples.is_lock_free())
        std::cout << "Lock-Free queue of samples if actually lock-free" << std::endl;
    else
        assert(false);
    
    // added as real-time listener
    oscReceiver.addListener(this);
    
    if (! oscReceiver.connect(udpPort))
    {
        String errorStr = "Cannot open UDP socket for OSC receiving on port " + String(udpPort) + ". Please check parameters and restart experiment.";
        MonitoringServer::SendLog(errorStr.toStdString()); // won't be graphically displayed... console only
        // we just on quit if an error happens
#ifndef __MIEM_DONT_WAIT_BEFORE_EXCEPTIONS
        // on garde ça pour l'instant, pour ne pas devoir modifié le MIEM EXPERIMENTS
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
#endif
        throw std::runtime_error(errorStr.toStdString());
    }
    else
    {
        MonitoringServer::SendLog("Listening for OSC messages on UDP port "
                                      + boost::lexical_cast<std::string>(udpPort));
    }
    
    
    // Init of the MIDI virtual output device
    virtualMidiOutput.reset( MidiOutput::createNewDevice(midiDeviceName) );
    
    
    if (virtualMidiOutput.get())
    {
        MonitoringServer::SendLog("[OSC to MIDI bridge] Virtual MIDI device created.");
    }
    else
    {
        MonitoringServer::SendLog("[OSC to MIDI bridge] Cannot create virtual MIDI device!");
        assert(false);
    }
    // and associated thread
    continueForwardingMidi = true;
    midiForwardThread = std::thread([this] { RunMidiSamplesForwarding(); } );
}
OSCListenerForwarder::~OSCListenerForwarder()
{
    // attente de fin du thread midi seulement... Le network est géré par Juce
    continueForwardingMidi = false;
    midiPoolConditionVariable.notify_all();
    if (midiForwardThread.joinable())
        midiForwardThread.join();
    else
        MonitoringServer::SendLog("MIDI bridge thread is not joinable");
    
    MonitoringServer::SendLog("OSC-MIDI bridge stopped.");
}



void OSCListenerForwarder::oscMessageReceived (const OSCMessage &message)
{
    auto receptionTime = MiemClock::now();
    
    // Fonction écrite avec des "return" dès qu'un problème est détecté dans le message
    // (pour efficacité max sur le thread network)
    std::string msgAddress;
    MiemExpeSample expeSample;
    MiemMidiSample midiSample;
    
    bool recordingStateChanged = (isRecording == wasOscThreadRecording);
    bool recordingHasJustStarted = false;
    if (recordingStateChanged)
    {
        recordingHasJustStarted = (! wasOscThreadRecording);
        wasOscThreadRecording = ! wasOscThreadRecording;
    }
    // Si on vient juste de démarrer l'enregistrement, on réinit toutes
    // les infos de test des duplicats MIDI (pour déclencher un
    // rafraîchissement forcé)
    if (recordingHasJustStarted)
        reinitMidiDuplicatesDetection();
        
    
    // Vérification la + basique : on doit avoir juste 1 float
    if (message.size() != 1)
        return;
    
    // vérification de l'adresse qui doit être du /miem/N (avec N sur autant que digits qu'on veut...)
    // (avec le min. vital d'allocations dynamiques...)
    // doit contenir /miem pour commencer
    // ---> allocations mémoire à optimiser sérieusement si on veut augmenter les perfs !!!!
    msgAddress = message.getAddressPattern().toString().toStdString();
    
    // Tous les autres messages seront juste oubliés
    if (msgAddress.length() == 7 || msgAddress.length() == 8)
    {
        if (msgAddress[0] == '/' && msgAddress[1] == 'm' && msgAddress[2] == 'i' && msgAddress[3] == 'e' && msgAddress[4] == 'm' && msgAddress[5] == '/')
        {
            // code ultra-basique pour éviter les allocations de substrings...
            // qui fontionne pour des messages à 2 chiffres seulement !
            if (msgAddress.length() == 7) // 1 chiffre de num de preset
            {
                if (('0' <= msgAddress[6]) && (msgAddress[6] <= '9'))
                    expeSample.parameterIndex = (int)( msgAddress[6] - '0' );
                else
                    return;
            }
            else if (msgAddress.length() == 8) // 2 digits
            {
                if (('0' <= msgAddress[6]) && (msgAddress[6] <= '9')
                    && ('0' <= msgAddress[7]) && (msgAddress[7] <= '9'))
                    expeSample.parameterIndex = (int)( msgAddress[6] - '0' ) * 10 + (int)( msgAddress[7] - '0' );
                else
                    return;
            }
            else
                return;
        }
        else
            return;
    }
    else
        return;
    // valeurs limité aux MIDI CC
    if (expeSample.parameterIndex < 0 || expeSample.parameterIndex > (maxMidiParametersCount - 1))
        return;
    // copie de l'info dans le message MIDI
    midiSample.parameterIndex = expeSample.parameterIndex;
    // puis enregistrement de la valeur avant quantification/discrétisation
    double sampleValue;
    if (! message[0].isFloat32())
        return;
    else
        sampleValue = (double) message[0].getFloat32(); // pour calculs ensuite
    
    
    // - - - - - si tout s'est bien passé - - - - -
    // On discrétise/quantifie tout d'abord l'info
    // pour coller avec Analog Lab 3 (qui transforme 0 à 127 en 0.0 à 10) : on prend 127 intervalles
    // ((avant : pour avoir une erreur moyenne de quantification nulle, on prenait l'arrondi...
    // ... mais il faut alors faire attention aux valeurs à 2^N -> à réduire à 2^N - 1))
    sampleValue = std::round( ((double) midiResolution - 1) * sampleValue ); // valeur int , ici
    midiSample.value = Miam::Math::Clamp<int>((int) sampleValue, 0, midiResolution-1); // sécurité... inutile ici
    sampleValue = midiSample.value;
    // renormalisation après discrétisation
    sampleValue = sampleValue / ((double) midiResolution);
    expeSample.value = (float) sampleValue;
    
    
    // - - -  calcul de temps 1 - - -
    // (différente pour OSC de l'expérience et pour MIDI FORWARD)
    //
    // on utilise ici tranquillement la copie du temps initial, réservée pour ce thread
    // Et on récupère le temps correspondant si tout s'est bien passé jusque là
    auto durationSinceExpeStart_ms
    = std::chrono::duration_cast<std::chrono::milliseconds>(receptionTime
                                                            - startTimePt_NetworkThreadCopy);
    
    // APRES DISCRETISATION (et après la mesure de temps...) :
    // on n'envoie un message que s'il n'est pas en double !
    bool wasElapsedTimeLongEnough = true;
    bool isValueChangeSignificant = true;
    // Et que le temps n'est pas supérieur à 200ms par exemple par rapport à la dernière
    // valeur (comme ça on autorise quand même les updates, mais juste pas les double
    // valeurs trop rapprochées, pour ne pas saturer Reaper qui semble parfois avoir du mal...)
    auto durationSinceLastUpdate_ms = durationSinceExpeStart_ms.count();
    // pour éviter des casts non-maîtrisés dans un if avec test direct... on fait l'opération ici
    durationSinceLastUpdate_ms -= lastMidiForwardingTimes_ms[midiSample.parameterIndex];
    durationSinceLastUpdate_ms -= allowedUpdatePeriodForConstantMidiValue_ms;
    if (durationSinceLastUpdate_ms < 0)
        wasElapsedTimeLongEnough = false;
    // on teste aussi l'égalité
    if (midiSample.value == lastMidiValues[midiSample.parameterIndex])
        isValueChangeSignificant = false;
    // On quitte si rien n'est intéressant pour ce sample...
    if ( (! wasElapsedTimeLongEnough) && (! isValueChangeSignificant) )
        return;
    
    
    // Si on est là... c'est que le message n'est pas en double
    // -> on met à jour les infos du "last"
    lastMidiForwardingTimes_ms[midiSample.parameterIndex] = durationSinceExpeStart_ms.count();
    lastMidiValues[midiSample.parameterIndex] = midiSample.value;
    
    // On forwarde le MIDI ; si on est là c'est que ce n'était pas un doublon
    midiSample.time_ms = (int64_t) durationSinceExpeStart_ms.count();
    midiSamplesQueue.push(midiSample);
    // Et on ré-active la condition variable (--> à faire à tous les coups ? oui...)
    midiPoolConditionVariable.notify_one(); // on suppose que ça ne coûte rien en CPU...
    
    
    // - - -  calcul de temps 2 - - -
    // (différente pour OSC de l'expérience et pour MIDI FORWARD)
    //
    // ATTENTION : restart time point ne peut être utilisé que si on enregistre....
    // Alors que la copie du point de début d'expé peut elle être utilisée tout le temps
    auto durationSinceRecordingStart_ms = std::chrono::duration_cast<std::chrono::milliseconds>(receptionTime - restartTimePoint);
    expeSample.time_ms = (int)durationSinceRecordingStart_ms.count();
    // Puis on enregistre effectivement le sample, si on est bien en phase d'enregistrement
    if (! isRecording)
        return;
    lastMiemSamples.push(expeSample);

    // Fin : callback message thread juce ?
    // OU ALORS on laisse le recorder vider la liste quand il faut ???
    // ça va économiser potentiellement pas mal de calcul...
}


void OSCListenerForwarder::RunMidiSamplesForwarding()
{
    // Thread init
    Miam::PeriodicUpdateThread::SetThreadName("MIEM OSC-MIDI bridge Thread");
    Miam::PeriodicUpdateThread::SetHighThreadPriority();
    
    // Attente par 'condition variable': on doit avoir locké le mutex
    // AVANT de faire un wait sur la
    // variable conditionnelle... Ce qui est fait automatiquement à la construction
    // du unique lock
    std::unique_lock<std::mutex> uniqueLock(midiConditionVariableMutex);
    
    // Local sample to store results of the lock-free queue pop ops
    MiemMidiSample midiSample;
    
    // Main thread pooling loop
    bool continueForwarding = true;
    while (continueForwarding)
    {
        // Attente sur cond var
        // pas de timeout, et pas de sécurité contre les spurious wakeups
        // (qui ne sont pas fondamentalement un problème vu le code ensuite)
        // Unique_lock est déverrouillé au wait, re-verrouillé au notify
        midiPoolConditionVariable.wait(uniqueLock);
        
        // Ensuite on vide tous les messages MIDI à retransmettre
        while( midiSamplesQueue.pop(midiSample) )
        {
            // ENVOI MIDI - tout sur Channel 1
            virtualMidiOutput->sendMessageNow(MidiMessage::controllerEvent(
               1, midiSample.parameterIndex, midiSample.value));
            /*
            std::cout << "Midi envoyé CC" << midiSample.parameterIndex << " valeur" << midiSample.value
            << " (OSC quantifié valeur=" << ((double)(midiSample.value))/127.0 << ")"
            << std::endl;
             */
        }
        
        // Vérification de la condition le + tard possible
        continueForwarding = continueForwardingMidi;
    }
}



void OSCListenerForwarder::reinitMidiDuplicatesDetection()
{
    lastMidiValues.clear();
    lastMidiValues.resize(maxMidiParametersCount, -1);
    lastMidiForwardingTimes_ms.clear();
    // Negatives times = times before the launch of the program
    lastMidiForwardingTimes_ms.resize(maxMidiParametersCount,
                                      -allowedUpdatePeriodForConstantMidiValue_ms - 1);
}


void OSCListenerForwarder::StartRecording()
{
    bufferedSamples.clear();
    
    MiemExpeSample sample;
    // vidange forcée du ring buffer d'enregistrement seulement
    while( lastMiemSamples.pop(sample) ) {}
    // pas de vidange du ring buffer du bridge MIDI
    // (le serveur/bridge continue !)
    
    // garantie d'absence d'accès concurrents, tant que
    // le thread network ne touche pas au time point lorsqu'il
    // n'est pas en train d'enregistrer.
    // (et tant qu'on ne fait pas de double start... évidemment)
    restartTimePoint = MiemClock::now();
    isRecording = true;
}
void OSCListenerForwarder::StopRecording()
{
    isRecording = false;
    // pas de clear du buffer : on autorise la lecture du buffer après
}



const std::vector<MiemExpeSample>& OSCListenerForwarder::GetBufferedSamples()
{
    // clear() should leave the capacity of the vector unchanged
    // https://stackoverflow.com/questions/37027674/how-to-clear-a-vector-but-keeping-its-capacity/37028452
    bufferedSamples.clear();
    
    // on espère que le push_back n'entraînera pas de ré-allocation !
    // Sinon en vrai on s'en fout... pour l'instant les débits OSC sont
    // très faibles, il faudra peut-être re-vérifier
    // si le CPU consommé est trop grand pour des expériences à
    // 20 ou 100 paramètres....
    MiemExpeSample sample;
    while( lastMiemSamples.pop(sample) )
    {
        bufferedSamples.push_back(sample);
    }
    
    return bufferedSamples;
}
