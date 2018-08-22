/*
  ==============================================================================

    SpatType.h
    Created: 28 Nov 2016 10:47:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATTYPE_H_INCLUDED
#define SPATTYPE_H_INCLUDED


namespace Miam
{
    
    /// \brief Describes a current method to actually render spatialisation
    enum class SpatType {
        
        None, ///< Spatialisation method not chosen yet
        
        Volumes1d, ///< Classical acousmatic spat, like if we had faders on an analog mixing console
        RoutingMatrix, ///< Available with the MIAM ROUTER daw plugin, or on hardware such as RME cards with TotalMix
        
    };
    
}


#endif  // SPATTYPE_H_INCLUDED
