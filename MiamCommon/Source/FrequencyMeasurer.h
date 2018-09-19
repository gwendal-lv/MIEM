/*
  ==============================================================================

    FrequencyMeasurer.h
    Created: 23 Jan 2017 6:23:09pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef FREQUENCYMEASURER_H_INCLUDED
#define FREQUENCYMEASURER_H_INCLUDED


#include <chrono>
#include <string>


namespace Miam
{
    typedef std::chrono::steady_clock MeasureClock;
    
    class FrequencyMeasurer
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        std::string name;
        const unsigned int digits = 2;
        
        // About last frame only
        MeasureClock::time_point lastFrameTimePt;
        std::chrono::microseconds lastFrameDuration;
        
        // We'll wait a few frames before actually starting measuring
        bool beginStatistics;
        
        
        // min/max values
        std::chrono::microseconds minDuration_us;
        std::chrono::microseconds maxDuration_us;
        
        // average framerate computing
        int averagingFramesCount;
        int lastSequenceFramesCount;
        MeasureClock::time_point lastSequenceTimePt;
        double averageFrequency;
        bool isFreshAverageAvailable;
        
   
        
        // = = = = = = = = = = Getters and Setters = = = = = = = = = =
        public :
        
        double GetAverageFrequency_Hz() const {return averageFrequency;}
        double GetLastFrequency_Hz() const;
        
        double GetLastDuration_ms() const;
        
        long GetElapsedTimeSinceLastNewFrame_us() const;
        
        long GetLastDuration_us() const {return (long)lastFrameDuration.count(); }
        long GetMinDuration_us() const {return (long)minDuration_us.count();}
		long GetMaxDuration_us() const {return (long)maxDuration_us.count();}
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        FrequencyMeasurer(std::string name_, int averagingFramesCount_ = 100, bool delayMeasureStart = true);
        
        /// \brief Must be called at the beginning (or the end) or the measured
        /// pseudo-periodical system
        void OnNewFrame();
        
        /// \brief Returns a formatted string containing average F, Tmin and Tmax
        std::string GetInfo(bool displayMinMax = true);
        /// \brief True if a new frequency average has been computed, but the
        /// GetInfo() method wans't called yet
        bool IsFreshAverageAvailable() const {return isFreshAverageAvailable;}
        
        
        protected :
        void init();
        
    };
}


#endif  // FREQUENCYMEASURER_H_INCLUDED
