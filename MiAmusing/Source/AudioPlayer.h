/*
  ==============================================================================

    AudioPlayer.h
    Created: 12 Dec 2016 10:46:37am
    Author:  ayup1

  ==============================================================================
*/

#ifndef AUDIOPLAYER_H_INCLUDED
#define AUDIOPLAYER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SquareSignal.h"

/*enum TransportState
{
	Stopped,
	Starting,
	Playing,
	Stopping
};*/

class AudioPlayer : public AudioAppComponent
{
	// utiliser un vecteur de Source, associer une ID aux formes (creation de AmusingArea...) pour pouvoir retrouver la source assoicee
	// le model doit recevoir l'ID de la forme etb l'action à effectuer pour pouvoir agir
	public :
		AudioPlayer();
		~AudioPlayer();

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
		void releaseResources() override;

		void setFrequency(String sceneName, int ID, double frequency);
		void setAmplitude(String sceneName, int ID, double amplitude);
		void setPath(String sceneName, int ID, String path);
		void setState(String sceneName, int ID, TransportState transportState); // gere l'etat de la source : play, start, stop
		void setVolume(String sceneName, double amplitude);
		void setReverse(String sceneName, int ID, bool enable);


		void addSource(String sceneName, int areaId, int sourceType); // ajoute une nouvelle source et return l'ID de la source pour faire le lien entre la source et la forme

		ToneGeneratorAudioSource* sinus;
};

            


#endif  // AUDIOPLAYER_H_INCLUDED
