/*
  ==============================================================================

    SpatArea.cpp
    Created: 28 May 2017 4:51:42pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "SpatArea.h"

#include "Exciter.h"

using namespace Miam;


// = = = = = = Construction/Destruction (no polymorphic cloning) = = = = = =
SpatArea::SpatArea()
{
}



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
    if ( ( (linkedSpatState) || (spatState) ) // Pour ne pas comparer 2 ptrs sur NULL
        && linkedSpatState != spatState)
    {
        // De-registering if necessary
        if (linkedSpatState)
            linkedSpatState->UnlinkToArea(getCastedSharedFromThis());
        
        // Internal updates
        linkedSpatState = spatState;
        if (linkedSpatState)
        {
            SetName(linkedSpatState->GetName(false));
        
            // Registering if necessary
            linkedSpatState->LinkToArea(getCastedSharedFromThis());
        }
        else
            setDefaultSpatName();
    }
}
void SpatArea::OnSpatStateNameChanged()
{
    SetName(linkedSpatState->GetName(false));
}


// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> SpatArea::getSpatStateTree()
{
    auto spatTree = std::make_shared<bptree::ptree>();
    if (linkedSpatState)
        spatTree->put("<xmlattr>.index", linkedSpatState->GetIndex());
    else
        spatTree->put("<xmlattr>.index", -1);
    return spatTree;
}



