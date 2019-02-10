/*
  ==============================================================================

    MiemExpePreset.cpp
    Created: 9 Feb 2019 8:44:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MiemExpePreset.h"


/// ======== Liste de presets, selon leur UID réel ======
/// Tout ça sera à bien respecter....
///
/// Presets d'essais : index -2 avec faders, index -1 le même en
/// mode interpolation MIEM
///
/// Ensuite, presets classiques avec les mesures :
///
/// De 0 à 9 on aura les presets avec faders
///
/// De 10 à 19 on aura les mêmes presets (valeurs finales idem)
/// mais retrouvés avec l'interpolation graphique MIEM

MiemExpePreset::MiemExpePreset(int _UID, int _index) :
UID(_UID),
index(_index)
{
    
}
