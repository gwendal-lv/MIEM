/*
  ==============================================================================

    InteractionParameters.cpp
    Created: 6 Dec 2017 10:37:09pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InteractionParameters.h"

using namespace Miam;

/* Plus cette valeur est grande, plus l'influence d'une aire augmente rapidement
 * à mesure que l'on s'approche du centre.
 * Cette valeur agit indépendemment du calcul de poids d'interaction par projection 2D
 * dans un sous-triangle d'un polygone.
 *
 * Avec de grandes valeur, on tend à donner + de précision aux faibles poids d'interaction
 * (on va "vite" dans le domaine des forts poids)
 *
 * Par rapport au script Matlab : c'est le facteur d'influence numéro 2
 * (le 1 donne de l'importance aux faibles poids et casse la dynamique des transitions)
 */

const double InteractionParameters::InfluenceOfDistanceFromCenter = 0.75;



/* Plus cette valeur est grande, plus les volumes sont déformés
 * de sorte qu'on ait des + de précision pour les faibles volumes. Avec de grandes valeurs,
 * la déformation fait qu'on "reste" + longtemps dans les faibles volumes, c'est très
 * important pour faire des entrées de HPs "douces".
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
 * Valeur neutre = ???
 * Valeur à partir de laquelle on sent qu'on plus vraiment de marge de manoeuvre pour les
 * déplacements = 2.5 (avec facteur d'influence de distance = 0)
 * Valeur beaucoup trop élevée = ???
 */
const double InteractionParameters::LowVolumePrecisionFactor = 0.5;

