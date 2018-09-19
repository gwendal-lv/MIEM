/*
  ==============================================================================

    ParametersIds.h
    Created: 1 Jun 2017 2:18:30pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
