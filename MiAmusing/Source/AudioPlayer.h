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

enum TransportState
{
	Stopped,
	Starting,
	Playing,
	Stopping
};

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

		void setFrequency(int ID, double frequency);
		void setAmplitude(int ID, double amplitude);
		void setPath(int ID, String path);
		void setState(int ID, TransportState transportState); // gere l'etat de la source : play, start, stop
		void setVolume(double amplitude);
		void setReverse(bool enable);


		int addSource(); // ajoute une nouvelle source et return l'ID de la source pour faire le lien entre la source et la forme

		ToneGeneratorAudioSource* sinus;
};

            


#endif  // AUDIOPLAYER_H_INCLUDED
