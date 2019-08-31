/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
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


