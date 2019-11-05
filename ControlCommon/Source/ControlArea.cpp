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
    SetName(String("[unlinked ") + String(GetId()) + String("]"));
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
            // Registering
            linkedState->LinkToArea(getCastedSharedFromThis());
            
            // auto-update from data of the state
            SetName(linkedState->GetName(false));
            SetFillColour(linkedState->GetColour());
        }
        else
            setDefaultName();
    }
}
void ControlArea::OnStateNameChanged()
{
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // BUG  À TRACKER : fonction appelée ici à un moment, avec une aire NULLPTR
    // (juste après avoir cliqué sur entré, après avoir changé le nom d'une aire)
    //
    // -> rechercher dans la call hierarchy qui pourrait être le coupable...
    //
    // 06/11/2019 : bug rencontré après copie/suppresion d'aires, déclenché via ControlState::SetName
    SetName(linkedState->GetName(false));
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
    // ================ BUG À RETROUVER ================
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



