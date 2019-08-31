/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
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
