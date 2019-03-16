/*
  ==============================================================================

    OSCRealtimeListener.cpp
    Created: 12 Feb 2019 10:57:17pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <iostream>
#include <thread>

#include "MonitorCommunication.h"

#include "OSCRealtimeListener.h"

#include "OSCRecorder.h"


OSCRealtimeListener::OSCRealtimeListener(int udpPort, std::chrono::time_point<MiemClock> _startTimePoint)
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
        MonitorCommunication::SendLog("errorStr"); // won't be graphically displayed... console only
        // we just on quit if an error happens
        std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
        throw std::runtime_error(errorStr.toStdString());
    }
    else
    {
        MonitorCommunication::SendLog("Listening for OSC message on UDP port "
                                      + boost::lexical_cast<std::string>(udpPort));
    }
}



void OSCRealtimeListener::oscMessageReceived (const OSCMessage &message)
{
    if (! isRecording)
        return;
    
    // Fonction écrite avec des "return" dès qu'un problème est détecté dans le message
    // (pour efficacité max sur le thread network)
    std::string msgAddress;
    MiemSample sample;
    
    // Vérification la + basique : on doit avoir juste 1 float
    if (message.size() != 1)
        return;
    
    // vérification de l'adresse qui doit être du /miem/N (avec N sur autant que digits qu'on veut...)
    // (avec le min. vital d'allocations dynamiques...)
    // doit contenir /miem pour commencer
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
    
    // puis enregistrement de la valeur
    if (! message[0].isFloat32())
        return;
    else
        sample.value = message[0].getFloat32();
    
    // et du temps si tout s'est bien passé
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(MiemClock::now() - restartTimePoint);
    sample.time_ms = (int)duration_ms.count();
    
    //std::cout << "Adresse : " << msgAddress << " param " << sample.parameterIndex << " valeur " << sample.value << " à t=" << (float)(sample.time_ms)/1000.0f << std::endl;
    
    lastMiemSamples.push(sample);
    
    // Fin : callback message thread juce ?
    // OU ALORS on laisse le recorder vider la liste quand il faut ???
    // ça va économiser potentiellement pas mal de calcul...
}



void OSCRealtimeListener::StartRecording()
{
    bufferedSamples.clear();
    
    MiemSample sample;
    while( lastMiemSamples.pop(sample) )
    {
        // vidange forcée du ring buffer
    }
    
    restartTimePoint = MiemClock::now();
    isRecording = true;
}
void OSCRealtimeListener::StopRecording()
{
    isRecording = false;
    // pas de clear du buffer : on autorise la lecture du buffer après
}



const std::vector<MiemSample>& OSCRealtimeListener::GetBufferedSamples()
{
    // clear() should leave the capacity of the vector unchanged
    // https://stackoverflow.com/questions/37027674/how-to-clear-a-vector-but-keeping-its-capacity/37028452
    bufferedSamples.clear();
    
    // on espère que le push_back n'entraînera pas de ré-allocation !
    // Sinon en vrai on s'en fout... pour l'instant les débits OSC sont
    // très faibles, il faudra peut-être re-vérifier
    // si le CPU consommé est trop grand pour des expériences à
    /// 20 ou 100 paramètres....
    MiemSample sample;
    while( lastMiemSamples.pop(sample) )
    {
        bufferedSamples.push_back(sample);
    }
    
    return bufferedSamples;
}
