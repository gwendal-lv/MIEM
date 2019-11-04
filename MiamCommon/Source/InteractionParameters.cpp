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

#include "InteractionParameters.h"

using namespace Miam;



/* Plus cette valeur est grande, plus les volumes sont déformés
 * de sorte qu'on ait des + de précision pour les faibles volumes. Avec de grandes valeurs positives,
 * la déformation fait qu'on "reste" + longtemps dans les faibles volumes, c'est très
 * important pour faire des entrées de HPs "douces".
 * Avec de grandes valeurs négatives, on arrive très vite aux forts volumes.
 *
 * S'il devient trop grand, les transitions deviennent trop brusques : le passage de
 * 1 - l'état est quasi-inaudible
 * vers 2 - l'état est largement audible
 * se fait trop vite...
 *
 * L'application de ce facteur n'a pas d'influence sur le retour visuel à l'écran !
 * Donc il serait intéressant de cherche la valeur OPTIMALE de ce facteur, de sorte
 * que les utilisateurs considèrent que le retour à l'écran est le + naturel.
 *
 * Valeur neutre = 0
 * Valeur à partir de laquelle on sent qu'on plus vraiment de marge de manoeuvre pour les
 * déplacements = 2.5 (avec facteur d'influence de distance = 0)
 * Valeur beaucoup trop élevée = ???
 */
const double InteractionParameters::LowVolumePrecisionFactor = 0.5;

