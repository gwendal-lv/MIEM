/*
  ==============================================================================

    Exciter.cpp
    Created: 11 Jan 2017 2:36:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "Exciter.h"

#include "MultiAreaEvent.h"

#include <cmath>
#include <algorithm>
#include <numeric> // std::accumulate

#include "SceneCanvasComponent.h" // pour récupérer width/height

using namespace Miam;

// = = = = = = = = = = Construction/Destruction + polymorphic cloning = = = = = = = = = =

Exciter::Exciter(bptree::ptree & areaTree, std::chrono::time_point<clock> commonStartTimePoint_, int additionnalTouchGrabRadius_)
:
EditableEllipse(areaTree),

additionnalTouchGrabRadius(additionnalTouchGrabRadius_),
commonStartTimePt(commonStartTimePoint_)
{
    init();
    
    // Paramètres facultatifs
    try {
        isAnimationSynchronized = areaTree.get<bool>("is_animation_sync");
    }
    catch (bptree::ptree_error&) { }
    
}

Exciter::Exciter(uint64_t uniqueId, std::chrono::time_point<clock> commonStartTimePoint_, int additionnalTouchGrabRadius_)
:
// Taille = 5% du canevas (de la + petite taille). Ratio inconnu, 1 par défaut...
EditableEllipse(uniqueId, bpt(0.5, 0.5), 0.05, Colours::lightgrey, 1.0f),

additionnalTouchGrabRadius(additionnalTouchGrabRadius_),
commonStartTimePt(commonStartTimePoint_)
{
    init();
}

Exciter::~Exciter()
{
    // Dé-exitation des aires qui existent encore
    for (auto& areaInteractingData : areasInteractingWith)
    {
        // ici : pas de test sur le lock... de toute manière la classe est détruite juste après
        if (auto area = areaInteractingData.Area.lock())
            area->OnExciterDestruction();
    }
}
void Exciter::onCloned()
{
    // D'abord on supprime tous les index locaux
    while (areasInteractingWith.size() > 0)
        deleteLinksToArea(areasInteractingWith.size() - 1); // suppresion du dernier (car vecteurs)
    // Puis on appelle les parents qui devraient faire le même genre de chose
    EditableEllipse::onCloned();
}



void Exciter::init()
{
    // Centre (voir DrawableArea)
    displayCenter = false;
    
    SetNameVisible(false);
    
    SetActive(false);
    SetOpacityMode(OpacityMode::Mid);
    SetEnableTranslationOnly(true);
    
    volume = 0.0;
    startTimePt = clock::now();
    
    // Par défaut : volume de 1
    SetVolume(1.0);
    isAnimationSynchronized = true;
}
double Exciter::computeXScale(float _canvasRatio)
{
	return 0.5f + 0.5f / _canvasRatio;
}
double Exciter::computeYScale(float _canvasRatio)
{
	return 0.5f + 0.5f * _canvasRatio;
}



void Exciter::SetVolume(double volume_)
{
    volume = volume_;
    
    // Pour l'instant rien, mais à l'avenir ça devra afficher graphiquement le résultat
}

// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
void Exciter::SetIsAnimationSynchronized(bool isSynchronized_)
{
    // On suppose que le point de départ commun a bien été initialisé auparavant...
    
    // Dans tous les cas on sauvegarde l'état
    isAnimationSynchronized = isSynchronized_;
}


// = = = = = = = = = = Display = = = = = = = = = =
void Exciter::Paint(Graphics& g)
{
    std::chrono::duration<double> duration; // par défaut, en secondes
    if (isAnimationSynchronized)
        duration = clock::now() - commonStartTimePt;
    else
        duration = clock::now() - startTimePt;
    
    // Création d'une couleur opaque (alpha=1.0f)
    // à partir de la luminosité seulement (sat et teinte = 0.0f)
    double brightness = deltaBrightnessOffset + deltaBrightnessAmplitude
                        * std::cos(omega * duration.count());
    contourColour = Colour(0.0f, 0.0f, (float)brightness, 1.0f);
    
    // Parent painting
    EditableEllipse::Paint(g);
}

// = = = = = = = = = = Interactions = = = = = = = = = =
bool Exciter::HitTest(bpt T) const
{
    if (additionnalTouchGrabRadius == 0)
        return EditableEllipse::HitTest(T);
    else
    {
        // Pas besoin d'appliquer le facteur d'échelle ? Il faudrait fouiller le code
        // de Guillaume (avec les xscale et yscale) pour en être sûr
        // ATTENTION a et b semblent être les longeurs des axes (pas les 1/2 longueurs)
        double aInPx = (a/2.0) * (double)parentCanvas->getWidth() + additionnalTouchGrabRadius;
        double bInPx = (b/2.0) * (double)parentCanvas->getHeight() + additionnalTouchGrabRadius;
        // Point à tester, dans le repère du centre de l'ellipse
        bpt relativeT = T; // copie car soustraction boost se fait par référence
        boost::geometry::subtract_point(relativeT, centerInPixels);
        // On applique juste l'inéquation paramétrique de l'ellipse
        if ( (std::pow(relativeT.get<0>() / aInPx, 2)
              + std::pow(relativeT.get<1>() / bInPx, 2) )
            < 1.0 )
            return true;
        else
            return false;
    }
}

void Exciter::OnAreaExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis, double interactionWeight)
{
    size_t areaIndex = findAreaInteractingIndex(areaExcitedByThis);
    // Si l'aire n'existait pas, on l'ajoute
	if (areaIndex == areasInteractingWith.size())
	{
		areasInteractingWith.push_back(AreaInteractingData(areaExcitedByThis));
	}
    // Mise à jour dans tous les cas
    areasInteractingWith[areaIndex].InteractionWeight = interactionWeight;
    
    // Pas de mise à jour des excitations : cela dépend des modifs apportées à TOUTES les aires,
    // on attend l'ordre de notification (par la scène mère, ou autre...)
}
void Exciter::OnAreaNotExcitedByThis(std::shared_ptr<IInteractiveArea> areaExcitedByThis)
{
    // On fait la recherche, et on vérifie le résultat par sécurité...
    size_t areaIndex = findAreaInteractingIndex(areaExcitedByThis);
    if ( areaIndex < areasInteractingWith.size() )
    {
        // D'abord on impose une mise à jour côté aire graphique (excitation nulle forcée)
        // Pas de check du lock : on vient de retrouver l'index de l'aire graphique (via shared_ptr....)
        areasInteractingWith[areaIndex].Area.lock()->OnNewExcitementAmount(getCastedSharedFromThis(), Excitement());
        
        // Puis : Suppression effective
        deleteLinksToArea(areaIndex);
    }
    else
        DBG("Double notification de fin d'excitation d'une aire par cet excitateur");
}
size_t Exciter::findAreaInteractingIndex(std::shared_ptr<IInteractiveArea> areaToFind)
{
    // naive research
    size_t i = 0;
    for (i = 0 ; i < areasInteractingWith.size() ; i++ )
    {
        auto lockedAreaInteractingWith = tryLockAreaOrDelete(i);
        if ( lockedAreaInteractingWith == areaToFind )
            return i;
    }
    // if nothing found :
    return i;
}
std::shared_ptr<IInteractiveArea> Exciter::tryLockAreaOrDelete(size_t areaLocalIndex)
{
    auto lockedAreaInteractingWith = areasInteractingWith[areaLocalIndex].Area.lock();
    if (lockedAreaInteractingWith) // area is valid
        return lockedAreaInteractingWith;
    else // area has been destructed
    {
        deleteLinksToArea(areaLocalIndex);
        return nullptr;
    }
}
void Exciter::deleteLinksToArea(size_t areaLocalIndex)
{
    auto areaIt = areasInteractingWith.begin();
    std::advance(areaIt, areaLocalIndex);
    areasInteractingWith.erase(areaIt);
}
void Exciter::updateExcitationAmounts()
{
    // On appliquera le volume le + tard possible (on fait tout par rapport à 1, puis on diminue ensuite...)
    
    // On calcule d'abord le total des poids d'interaction, et on en profite pour dégager toutes les aires
    // qui ne peuvent pas être lockées
    double totalInteractionWeight = 0.0;
    for (size_t i=0 ; i < areasInteractingWith.size() ; i++)
    {
        // Si n'existe plus : on fait i-- car on aura un i++ au moment de passer l'itération suivante
        if (! tryLockAreaOrDelete(i))
            i--;
        // Sinon, OK, on ajoute bien le poids concerné
        else
            totalInteractionWeight += areasInteractingWith[i].InteractionWeight;
    }
    
    // - - - - - Calcul des excitations, tel que la somme des excitations vaut 1 - - - - -
    
    // en vérifiant les cas bizarres limites...
    // Pour être sûr même si des erreurs
    // numériques sont là...
    
    // Si on a un poids total non-valide : répartition uniforme sur toutes les aires
    if (totalInteractionWeight <= 0.0)
    {
        double uniformWeight = 1.0 / (double)(areasInteractingWith.size());
        for (auto &areaData : areasInteractingWith)
            areaData.ExcitementAmount.Linear = uniformWeight;
    }
    // Si le poids total est OK : on normalise simplement les poids pour calculer les excitations
    else
    {
        for (auto &areaData : areasInteractingWith)
            areaData.ExcitementAmount.Linear = areaData.InteractionWeight / totalInteractionWeight;
    }
    
    // - - - - - Distorsion logarithmique, pour donner la précision aux faibles volumes - - - - -
    // Voir article JIM 2015 pour + d'explications (même si dans l'article la disto était appliquée
    // directement sur les poids d'interaction)
    // on prépare déjà la normalisation qui suit
    double totalAudioExcitement = 0.0;
    for (auto &areaData : areasInteractingWith)
    {
        areaData.ExcitementAmount.Audio = AudioUtils<double>::ApplyLowVolumePrecisionDistorsion(areaData.ExcitementAmount.Linear);
        totalAudioExcitement += areaData.ExcitementAmount.Audio;
    }
    
    // - - - - - Dernière normalisation pour les coeffs Audio - - - - -
    for (auto &areaData : areasInteractingWith)
        areaData.ExcitementAmount.Audio = areaData.ExcitementAmount.Audio / totalAudioExcitement;
    
    // - - - - - Application du volume, de manière uniforme, à la toute fin - - - - -
    for (auto &areaData : areasInteractingWith)
    {
        areaData.ExcitementAmount.Linear *= volume;
        areaData.ExcitementAmount.Audio *= volume;
    }
}

void Exciter::NotifyNewExcitationToAreas()
{
    // Préparatifs internes
    updateExcitationAmounts();
    //auto multiAreaE = std::make_shared<MultiAreaEvent>(); // le premier event va rester Nothing....
    
    // Notifications à toutes les aires
    for (size_t i = 0; i<areasInteractingWith.size() ; i++)
    {
        auto lockedAreaInteractingWith = areasInteractingWith[i].Area.lock();
        // Ici on ne doit plus rater de lock()... Car les weak_ptr 'morts' auraient dû être supprimés
        // au update juste au-dessus
#ifdef __MIAM_DEBUG
        if ( ! (lockedAreaInteractingWith) )
            throw std::logic_error("weak_ptr sur Aire référencée dans un excitateur n'est plus valide...");
#endif
        lockedAreaInteractingWith->OnNewExcitementAmount(getCastedSharedFromThis(),
                                                         areasInteractingWith[i].ExcitementAmount);
    }
}



// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> Exciter::GetTree()
{
    auto inheritedTree = EditableEllipse::GetTree();
    
    inheritedTree->put("is_animation_sync", isAnimationSynchronized);
    
    return inheritedTree;
}


