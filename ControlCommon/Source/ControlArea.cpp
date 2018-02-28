/*
  ==============================================================================

    ControlArea.cpp
    Created: 28 May 2017 4:51:42pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ControlArea.h"

#include "Exciter.h"

#include "ControlState.hpp" // ici pour éviter include croisé, pb sous AS 3.0

using namespace Miam;


// = = = = = = Construction/Destruction (no polymorphic cloning) = = = = = =
ControlArea::ControlArea()
{
}



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

int ControlArea::GetStateIndex()
{
    if (linkedState)
        return linkedState->GetIndex();
    else
        return -1;
}


// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction/Destruction (no polymorphic cloning) - - - - -
void ControlArea::OnVirtualBaseClassesContructed()
{
    setDefaultName();
    
    // Forcer la mise à zéro graphique seulement
    OnNewExcitementAmount(nullptr, Excitement()); // excitation nulle
}

// - - - - - Linked State attributes - - - - -
void ControlArea::setDefaultName()
{
    SetName(String("[[[unlinked ") + String(GetId()) + String("]]]"));
}
void ControlArea::LinkToState(std::shared_ptr< ControlState<double> > state)
{
    if ( ( (linkedState) || (state) ) // Pour ne pas comparer 2 ptrs sur NULL
        && linkedState != state)
    {
        // De-registering if necessary
        if (linkedState)
            linkedState->UnlinkToArea(getCastedSharedFromThis());
        
        // Internal updates
        linkedState = state;
        if (linkedState)
        {
            SetName(linkedState->GetName(false));
        
            // Registering if necessary
            linkedState->LinkToArea(getCastedSharedFromThis());
        }
        else
            setDefaultName();
    }
}
void ControlArea::OnStateNameChanged()
{
    SetName(linkedState->GetName(false));
}


// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> ControlArea::getStateTree()
{
    auto stateTree = std::make_shared<bptree::ptree>();
    if (linkedState)
        stateTree->put("<xmlattr>.index", linkedState->GetIndex());
    else
        stateTree->put("<xmlattr>.index", -1);
    return stateTree;
}



