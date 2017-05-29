/*
  ==============================================================================

    SpatArea.cpp
    Created: 28 May 2017 4:51:42pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatArea.h"

using namespace Miam;



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

int SpatArea::GetSpatStateIndex()
{
    if (linkedSpatState)
        return linkedSpatState->GetIndex();
    else
        return -1;
}


// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction/Destruction (no polymorphic cloning) - - - - -
void SpatArea::OnVirtualBaseClassesContructed()
{
    setDefaultSpatName();
}

// - - - - - Spat attributes - - - - -
void SpatArea::setDefaultSpatName()
{
    SetName(String("[[[unlinked ") + String(GetId()) + String("]]]"));
}
void SpatArea::LinkToSpatState(std::shared_ptr< SpatState<double> > spatState)
{
    if (linkedSpatState != spatState)
    {
        // De-registering if necessary
        if (linkedSpatState)
            linkedSpatState->UnlinkToArea(shared_from_this());
        
        // Internal updates
        linkedSpatState = spatState;
        if (linkedSpatState)
        {
            SetName(linkedSpatState->GetName(false));
        
            // Registering if necessary
            linkedSpatState->LinkToArea(shared_from_this());
        }
        else
            setDefaultSpatName();
    }
}
void SpatArea::OnSpatStateNameChanged()
{
    SetName(linkedSpatState->GetName(false));
}
