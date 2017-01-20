/*
  ==============================================================================

    AmusingFormatReaderSource.h
    Created: 28 Dec 2016 4:28:15pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef AMUSINGFORMATREADERSOURCE_H_INCLUDED
#define AMUSINGFORMATREADERSOURCE_H_INCLUDED

#include "JuceHeader.h"

class AmusingFormatReaderSource : public PositionableAudioSource
{
public:
	AmusingFormatReaderSource(AudioFormatReader* sourceReader,
		bool deleteReaderWhenThisIsDeleted);

	/** Destructor. */
	~AmusingFormatReaderSource();

	//==============================================================================
	/** Toggles loop-mode.

	If set to true, it will continuously loop the input source. If false,
	it will just emit silence after the source has finished.

	@see isLooping
	*/
	void setLooping(bool shouldLoop) override;

	/** Returns whether loop-mode is turned on or not. */
	bool isLooping() const override { return looping; }

	/** Returns the reader that's being used. */
	AudioFormatReader* getAudioFormatReader() const noexcept { return reader; }

	//==============================================================================
	/** Implementation of the AudioSource method. */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/** Implementation of the AudioSource method. */
	void releaseResources() override;

	/** Implementation of the AudioSource method. */
	void getNextAudioBlock(const AudioSourceChannelInfo&) override;

	//==============================================================================
	/** Implements the PositionableAudioSource method. */
	void setNextReadPosition(int64 newPosition) override;

	/** Implements the PositionableAudioSource method. */
	int64 getNextReadPosition() const override;

	/** Implements the PositionableAudioSource method. */
	int64 getTotalLength() const override;

	void setReverse(bool m_reverse);


private:
	OptionalScopedPointer<AudioFormatReader> reader;

	int64 volatile nextPlayPos;
	bool volatile looping;
	bool volatile reverse;

};





#endif  // AMUSINGFORMATREADERSOURCE_H_INCLUDED
