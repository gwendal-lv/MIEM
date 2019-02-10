/*
  ==============================================================================

    MiemExpePreset.h
    Created: 9 Feb 2019 8:44:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <iostream>

class MiemExpePreset {
    
    // =============== ATTRIBUTES ================
    private :
    
    const int UID; ///< Universal ID, désigne aussi la track dans Reaper qui joue le preset
    
    int index; ///< Index in the vector of presets. Negative values for trial presets
    
    
    
    /// \brief Did the experiment encoutered issues during the recording of this preset ?
    bool isValid = true;
    

    
    // =============== Setters and Getters ================
    public :
    void SetIsValid(bool _isValid) {isValid = _isValid;}
    
    
    
    // =============== METHODS ================
    public :
    MiemExpePreset(int _UID, int _index);
    
};
