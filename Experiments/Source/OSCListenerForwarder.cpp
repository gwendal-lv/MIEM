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

#include "OSCRecorder.h"


OSCListenerForwarder::OSCListenerForwarder(int udpPort, std::chrono::time_point<MiemClock> _startTimePoint)
:
oscReceiver("MIEM real-time OSC receiver thread"),
experimentStartTimePoint(_startTimePoint)
{
    isRecording = false;
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
        MonitoringServer::SendLog("errorStr"); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
    else
    {
        MonitoringServer::SendLog("Listening for OSC message on UDP port "
                                      + boost::lexical_cast<std::string>(udpPort));
    }
    
    
    // Init of the MIDI virtual output device
    virtualMidiOutput.reset( MidiOutput::createNewDevice("MIEM_experiments_OSC_bridge") );
    
    
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



void OSCListenerForwarder::oscMessageReceived (const OSCMessage &message)
{
    // Fonction écrite avec des "return" dès qu'un problème est détecté dans le message
    // (pour efficacité max sur le thread network)
    std::string msgAddress;
    MiemSample<float> sample;
    MiemSample<int> midiSample;
    
    // Peut-être inutile (si message non-conforme)...
    // Mais info récupérée dès maintenant, dans le doute.
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(MiemClock::now() - restartTimePoint);
    
    
    
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
                    sample.parameterIndex = (int)( msgAddress[6] - '0' );
                else
                    return;
            }
            else if (msgAddress.length() == 8) // 2 digits
            {
                if (('0' <= msgAddress[6]) && (msgAddress[6] <= '9')
                    && ('0' <= msgAddress[7]) && (msgAddress[7] <= '9'))
                    sample.parameterIndex = (int)( msgAddress[6] - '0' ) * 10 + (int)( msgAddress[7] - '0' );
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
    // copie de l'info dans le message MIDI
    midiSample.parameterIndex = sample.parameterIndex;
    // puis enregistrement de la valeur avant quantification/discrétisation
    double sampleValue;
    if (! message[0].isFloat32())
        return;
    else
        sampleValue = (double) message[0].getFloat32(); // pour calculs ensuite
    
    // Et on récupère le temps correspondant si tout s'est bien passé jusque là
    sample.time_ms = (int)duration_ms.count();
    midiSample.time_ms = (int)duration_ms.count();
    
    
    // - - - - - si tout s'est bien passé - - - - -
    // On discrétise/quantifie tout d'abord l'info
    // pour avoir une erreur moyenne de quantification nulle, on prend l'arrondi...
    // ... mais il faut alors faire attention aux valeurs à 2^N -> à réduire à 2^N - 1
    sampleValue = std::round( ((double) midiResolution) * sampleValue ); // valeur int , ici
    midiSample.value = Miam::Math::Clamp<int>((int) sampleValue, 0, midiResolution-1);
    sampleValue = midiSample.value;
    
    sampleValue = sampleValue / ((double) midiResolution); // renormalisation après discrétisation
    sample.value = (float) sampleValue;
    
    // APRES DISCRETISATION : on n'envoie un message que s'il n'est pas en double !
    // Et que le temps n'est pas supérieur à 200ms par exemple par rapport à la dernière
    // valeur (comme ça on autorise quand même les updates, mais juste pas les double
    // valeurs trop rapprochés, pour ne pas saturer Reaper qui semble parfois avoir du mal...)
    
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    // ================= TEST VALEURS IDENTIQUES : À ÉCRIRE ===================
    
    // Si on est là... c'est que le message n'est pas en double
    // -> on met à jour la liste des derniers
    
    // On forwarde le MIDI dans tous les cas
    midiSamplesQueue.push(midiSample);
    // Et on ré-active la condition variable (--> à faire à tous les coups ? oui...)
    midiPoolConditionVariable.notify_one(); // on suppose que ça ne coûte rien en CPU...
    
    // Puis on enregistre effectivement le sample, si on est bien en phase d'enregistrement
    if (! isRecording)
        return;
    lastMiemSamples.push(sample);

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
    MiemSample<int> midiSample;
    
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
            std::cout << "midi envoyé CC" << midiSample.parameterIndex << " valeur " << midiSample.value
            << std::endl;
        }
        
        // Vérification de la condition le + tard possible
        continueForwarding = continueForwardingMidi;
    }
}



void OSCListenerForwarder::StartRecording()
{
    bufferedSamples.clear();
    
    MiemSample<float> sample;
    // vidange forcée du ring buffer d'enregistrement seulement
    while( lastMiemSamples.pop(sample) ) {}
    // pas de vidange du ring buffer du bridge MIDI
    // (le serveur/bridge continue !)
    
    restartTimePoint = MiemClock::now();
    isRecording = true;
}
void OSCListenerForwarder::StopRecording()
{
    isRecording = false;
    // pas de clear du buffer : on autorise la lecture du buffer après
}



const std::vector<MiemSample<float>>& OSCListenerForwarder::GetBufferedSamples()
{
    // clear() should leave the capacity of the vector unchanged
    // https://stackoverflow.com/questions/37027674/how-to-clear-a-vector-but-keeping-its-capacity/37028452
    bufferedSamples.clear();
    
    // on espère que le push_back n'entraînera pas de ré-allocation !
    // Sinon en vrai on s'en fout... pour l'instant les débits OSC sont
    // très faibles, il faudra peut-être re-vérifier
    // si le CPU consommé est trop grand pour des expériences à
    // 20 ou 100 paramètres....
    MiemSample<float> sample;
    while( lastMiemSamples.pop(sample) )
    {
        bufferedSamples.push_back(sample);
    }
    
    return bufferedSamples;
}
