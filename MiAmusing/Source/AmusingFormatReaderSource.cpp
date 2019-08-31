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

#include "AmusingFormatReaderSource.h"

AmusingFormatReaderSource::AmusingFormatReaderSource(AudioFormatReader* const r,
	const bool deleteReaderWhenThisIsDeleted)
	: reader(r, deleteReaderWhenThisIsDeleted),
	nextPlayPos(0),
	looping(false),
	reverse(false)
{
	jassert(reader != nullptr);
}

AmusingFormatReaderSource::~AmusingFormatReaderSource() {}

int64 AmusingFormatReaderSource::getTotalLength() const 
{
	//return reverse ? -1 : reader->lengthInSamples;
	return reader->lengthInSamples;
}
void AmusingFormatReaderSource::setNextReadPosition(int64 newPosition) { nextPlayPos = newPosition; }
void AmusingFormatReaderSource::setLooping(bool shouldLoop) { looping = shouldLoop; }

void AmusingFormatReaderSource::setReverse(bool m_reverse) 
{
	reverse = m_reverse;
}

int64 AmusingFormatReaderSource::getNextReadPosition() const
{
	/*int64 pos = looping ? nextPlayPos % reader->lengthInSamples
		: nextPlayPos;
	return reverse ? -pos : pos;*/
	return looping ? nextPlayPos % reader->lengthInSamples
		: nextPlayPos;
}

void AmusingFormatReaderSource::prepareToPlay(int /*samplesPerBlockExpected*/, double /*sampleRate*/) {}
void AmusingFormatReaderSource::releaseResources() {}

void AmusingFormatReaderSource::getNextAudioBlock(const AudioSourceChannelInfo& info)
{
	if (info.numSamples > 0)
	{
		if (reverse)
			nextPlayPos -= info.numSamples;
		if (nextPlayPos < 0)
			nextPlayPos += reader->lengthInSamples;
		const int64 start = nextPlayPos;

		if (looping)
		{
			const int64 newStart = start % reader->lengthInSamples;
			const int64 newEnd = (start + info.numSamples) % reader->lengthInSamples;

			if (newEnd > newStart)
			{
				reader->read(info.buffer, info.startSample,
					(int)(newEnd - newStart), newStart, true, true);
			}
			else
			{
				
					const int endSamps = (int)(reader->lengthInSamples - newStart);

					reader->read(info.buffer, info.startSample,
						endSamps, newStart, true, true);

					reader->read(info.buffer, info.startSample + endSamps,
						(int)newEnd, 0, true, true);
					if (reverse)
						info.buffer->reverse(info.startSample,info.numSamples);
			}

			if (reverse)
				nextPlayPos = newStart;
			else
				nextPlayPos = newEnd;
		}
		else
		{
			reader->read(info.buffer, info.startSample,
				info.numSamples, start, true, true);

			if (reverse)
				info.buffer->reverse(info.startSample, info.numSamples);			
			else
				nextPlayPos += info.numSamples;

		}
	}
}
