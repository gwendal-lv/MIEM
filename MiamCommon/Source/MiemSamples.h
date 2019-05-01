/*
  ==============================================================================

    MiemSamples.h
    Created: 1 May 2019 3:50:40pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once


template<typename ValueType, typename TimeDurationType>
class MiemSample {
    public :
    TimeDurationType time_ms; ///< Time of reception of the sample, in milliseconds since the beginning of experiment.
    int parameterIndex; ///< Index of the parameter concerned by this sample.
    ValueType value; ///< Actual value of the sample.
};
typedef MiemSample<float, int> MiemExpeSample; ///< To store samples recorder during an experiment
typedef MiemSample<int, int64_t> MiemMidiSample; ///< To store samples that are being MIDI-forwarded.


