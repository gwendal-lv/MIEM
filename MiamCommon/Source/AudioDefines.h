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

#define Miam_MinVolume_dB               (-60.0)
#define Miam_MinVolume                  (0.001)
#define Miam_MinVolume_PowOf10          (-3)
#define Miam_MaxVolume_dB               (6.0)
#define Miam_MaxVolume                  (1.9952623149688)

#define Miam_SignificantVolumeDifference_dB (0.03)

// Overall maxima (particular applications, VSTs for example, might not
// tolerate such numbers of inputs/outputs)
#define Miam_MaxNumInputs               (64)
#define Miam_MaxNumOutputs              (64)



