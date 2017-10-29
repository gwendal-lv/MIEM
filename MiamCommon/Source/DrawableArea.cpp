/*
  ==============================================================================

    DrawableArea.cpp
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "DrawableArea.h"

#include "SceneCanvasComponent.h"

using namespace Miam;


DrawableArea::DrawableArea(bptree::ptree& areaTree)
{
    init();
    
    // Données obligatoires
    Id = areaTree.get<int64_t>("<xmlattr>.id");
    center.set<0>( areaTree.get<double>("center.<xmlattr>.x") );
    center.set<1>( areaTree.get<double>("center.<xmlattr>.y") );
    std::string colourString = areaTree.get<std::string>("colour", std::string("ffaaaaaa"));
    fillColour = Colour::fromString( colourString.c_str() );
    
    // Données facultatives
    try {
        keepRatio = areaTree.get<bool>("keep_ratio");
    }
    catch (bptree::ptree_error&) {}
}

DrawableArea::DrawableArea(int64_t _Id, bpt _center, Colour _fillColour)
{
	Id = _Id;
	center = _center;
    fillColour = _fillColour;
    
    init();
}

DrawableArea::~DrawableArea()
{
}

void DrawableArea::init()
{
    // On suppose un écran classique pour commencer...
    renderingScale = 1.0;
    
    centerCircleRadius = 5;
    displayCenter = true;
    
    fillOpacity = 1.0f;
    opacityMode = OpacityMode::Independent;
    
    contourColour = Colours::white;
    contourWidth = 2.0f;
    centerContourWidth = contourWidth*1.5f;
    
    isNameVisible = true; // par défaut
    
    keepRatio = false;
    
    resetImage();
}


void DrawableArea::resetImage()
{
    // Construction de l'image (fond transparent à l'avenir)
    // On en recréée un 2è, qu'on assigne à la 1ière...
    // Pour éviter de nombreux problèmes (écrire un constructeur de copie
    // de DrawableArea par exemple....)
    
    int scaledWidth = (int) std::round((double)nameWidth * baseRenderingScale);
    int scaledHeight = (int) std::round((double)nameHeight * baseRenderingScale);
    
    Image nameImageToAssign = Image(Image::ARGB, scaledWidth, scaledHeight, true);
    nameImage = nameImageToAssign;
    nameImage.duplicateIfShared();
    // Pas d'assignation de texte par défaut...
}

void DrawableArea::renderCachedNameImage()
{
    // Contexte graphique permettant de faire du dessin dans l'image
    // après réinit de l'image (nécessaire ici aussi)
    resetImage();
    Graphics g(nameImage);
    
    // Bounds de base, qu'on va décaler de qqs pixels
    auto textBounds = nameImage.getBounds();
    int shadowOffsetXY = 1 * (int) std::round(baseRenderingScale); // pixels
    // black shadow
    g.setColour(Colours::black);
    textBounds.setPosition(shadowOffsetXY, shadowOffsetXY);
    textBounds.removeFromBottom(shadowOffsetXY);
    textBounds.removeFromRight(shadowOffsetXY);
    g.drawText(name, textBounds, Justification::topLeft);
    // white text
    g.setColour(Colours::white);
    textBounds.setPosition(0, 0);
    g.drawText(name, textBounds, Justification::topLeft);
}



void DrawableArea::Paint(Graphics& g)
{
    // Dessin du centre (couleur du contour) si nécessaire
    if (displayCenter)
    {
        Colour actualContourColour = Colour(contourColour.getRed(), contourColour.getGreen(), contourColour.getBlue(), GetAlpha() );        
        g.setColour(actualContourColour);
        g.drawEllipse((float)centerInPixels.get<0>()-centerCircleRadius,
                      (float)centerInPixels.get<1>()-centerCircleRadius,
                      centerCircleRadius*2.0f, centerCircleRadius*2.0f, centerContourWidth);
    }
    
    // Dessin du texte :
    if (isNameVisible)
    {
        g.drawImageAt(nameImage,
                      (int)centerInPixels.get<0>() + centerCircleRadius + 2,
                      (int)centerInPixels.get<1>() + 2,
                      false); // don't fill alpha channel with current brush
    }
}

void DrawableArea::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    parentCanvas = _parentCanvas;
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(parentCanvas->getWidth(), parentCanvas->getHeight());
	boost::geometry::transform(center, centerInPixels, scale);
}

void DrawableArea::KeepRatio(bool _keepRatio)
{
	keepRatio = _keepRatio;
}
void DrawableArea::SetRenderingScale(double renderingScale_)
{
    // On ne fait rien... Un resize aura seulement peut-être lieu au rendu.
    renderingScale = renderingScale_;
}

void DrawableArea::SetFillColour(Colour newColour)
{
    fillColour = newColour;
}
void DrawableArea::SetName(String newName)
{
    if (name != newName)
        renderCachedNameImage(); // au ratio de base (précisé en attribut constant dans la classe)
    
    name = newName;
}

void DrawableArea::SetAlpha(float newAlpha)
{
	fillOpacity = newAlpha;
}
float DrawableArea::GetAlpha() const
{
    switch (opacityMode)
    {
        case OpacityMode::Low :
            return getLowFillOpacity();
            break;
            
        case OpacityMode::Mid :
            return 0.5f;
            break;
            
        case OpacityMode::High :
            return 0.8f;
            break;
            
        case OpacityMode::Independent :
        case OpacityMode::DependingOnExcitement :
            return fillOpacity;
            break;
            
        default :
            return 1.0;
            break;
    }
}

void DrawableArea::SetOpacityMode(OpacityMode opacityMode_)
{
    opacityMode = opacityMode_;
}

// = = = = = = = = = = XML import/export = = = = = = = = = =
std::shared_ptr<bptree::ptree> DrawableArea::GetTree()
{
    auto drawableAreaTree = std::make_shared<bptree::ptree>();
    drawableAreaTree->put("<xmlattr>.id", Id);
    drawableAreaTree->put("<xmlattr>.type", GetTypeAsString());
    drawableAreaTree->put("center.<xmlattr>.x", center.get<0>());
    drawableAreaTree->put("center.<xmlattr>.y", center.get<1>());
    drawableAreaTree->put("colour", fillColour.toString());
    drawableAreaTree->put("keep_ratio", keepRatio);
    return drawableAreaTree;
}



