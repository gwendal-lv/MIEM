/*
  ==============================================================================

    AmusingFormatReaderSource.cpp
    Created: 28 Dec 2016 4:28:15pm
    Author:  ayup1

  ==============================================================================
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
