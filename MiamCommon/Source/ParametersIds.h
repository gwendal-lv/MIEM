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


namespace Miam
{
    // - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - -
    
    // Async Param Changes types specification common to all Miam-related projects
    
    struct ActivateId
    {
        enum : int
        {
            PresenterToModelParametersTransmission = 0,
            ModelToPresenterParametersTransmission,
        };
    };
    struct DurationId
    {
        enum : int
        {
            AttackTime = 0,
        };
    };
    struct UpdateDisplayId
    {
        enum : int
        {
            GeneralParameters = -1,
        };
    };
    // - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - -
}
