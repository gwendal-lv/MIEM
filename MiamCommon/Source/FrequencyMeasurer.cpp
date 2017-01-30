/*
  ==============================================================================

    FrequencyMeasurer.cpp
    Created: 23 Jan 2017 6:23:09pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include <iostream>

#include <iomanip> // setprecision
#include <sstream> // stringstream


#include "FrequencyMeasurer.h"

using namespace Miam;


FrequencyMeasurer::FrequencyMeasurer(std::string name_, int averagingFramesCount_, bool delayMeasureStart)
:
name(name_),
beginStatistics(!delayMeasureStart), // we do start right there if there is a delay
minDuration_us(std::chrono::microseconds(1<<30)), // us : signed integer on at least 55 bits, doc says !
maxDuration_us(std::chrono::microseconds(0)),
averagingFramesCount(averagingFramesCount_),
lastSequenceFramesCount(0),
isFreshAverageAvailable(false)
{
	init();
}


void FrequencyMeasurer::init()
{
    lastFrameTimePt = MeasureClock::now();
    lastSequenceTimePt = lastFrameTimePt;
    lastSequenceFramesCount = 0;
}


// = = = = = = = = = = Getters and Setters = = = = = = = = = =

double FrequencyMeasurer::GetLastFrequency_Hz()
{
    return 1000.0/GetLastDuration_ms();
}

double FrequencyMeasurer::GetLastDuration_ms()
{
    return (double)(lastFrameDuration.count())/1000.0;
}



void FrequencyMeasurer::OnNewFrame()
{
    // 1-frame values in any case
    auto currentTimePt = MeasureClock::now();
    lastFrameDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePt - lastFrameTimePt);
    lastFrameTimePt = currentTimePt;

	// Initialization/Launching phase
	// (not counting Average/Min/Max yet)
	if (!beginStatistics)
	{
		if (lastSequenceFramesCount>50)
			beginStatistics = true;
	}
	// Normal case, computing statistic values
	else
	{
		// Comparison of min/max values on 1 frame
		if (lastFrameDuration < minDuration_us)
			minDuration_us = lastFrameDuration;
		else if (lastFrameDuration > maxDuration_us)
			maxDuration_us = lastFrameDuration;
		// Average computing when necessary
		if (lastSequenceFramesCount >= averagingFramesCount)
		{
			auto totalDuration_us = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePt - lastSequenceTimePt);
			averageFrequency = (double)lastSequenceFramesCount * 1000000.0f / (double)(totalDuration_us.count());
			isFreshAverageAvailable = true;
            
			// reinit of sequence counter
			lastSequenceTimePt = currentTimePt;
			lastSequenceFramesCount = -1;
		}
    }
	// In any case : increment of the sequence counter
	lastSequenceFramesCount++;
}


std::string FrequencyMeasurer::GetInfo(bool displayMinMax)
{
    std::string result = name + " F=";
    {
        std::stringstream numberStream;
        numberStream << std::fixed << std::setprecision(digits) << averageFrequency;
        result += numberStream.str();
        result += "Hz ";
    }
    {
        result += "Tmin=";
        std::stringstream numberStream;
        numberStream << std::fixed << std::setprecision(digits) << ((double)(minDuration_us.count())/1000.0);
        result += numberStream.str();
        result += "ms ";
    }
    {
        result += "Tmax=";
        std::stringstream numberStream;
        numberStream << std::fixed << std::setprecision(digits) << ((double)(maxDuration_us.count())/1000.0);
        result += numberStream.str();
        result += "ms";
    }
    
    isFreshAverageAvailable = false;
    
    return result;
}


