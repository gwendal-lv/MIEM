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

void DrawableArea::init()
{
    centerCircleRadius = 5;
    displayCenter = true;
    
    fillOpacity = 1.0;
    enableLowOpacityMode = false;
    
    contourColour = Colours::white;
    contourWidth = 2.0f;
    centerContourWidth = contourWidth*1.5f;
    
    isNameVisible = true; // par défaut
    
    keepRatio = false;
}




void DrawableArea::Paint(Graphics& g)
{
    // Dessin du centre (couleur du contour) si nécessaire
    if (displayCenter)
    {
        Colour actualContourColour = (enableLowOpacityMode) ?
        Colour(contourColour.getRed(), contourColour.getGreen(), contourColour.getBlue(),
               getLowFillOpacity() )
        : contourColour;
        g.setColour(actualContourColour);
        g.drawEllipse((float)centerInPixels.get<0>()-centerCircleRadius,
                      (float)centerInPixels.get<1>()-centerCircleRadius,
                      centerCircleRadius*2.0f, centerCircleRadius*2.0f, centerContourWidth);
    }
    
    // Dessin du texte :
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    if (isNameVisible && false)
    {
        g.setColour(Colours::black); // black shadow
        g.drawSingleLineText(name,
                             (int)centerInPixels.get<0>()+1,
                             (int)(centerInPixels.get<1>()-centerCircleRadius*2+1));
        g.setColour(Colours::white); // white text
        g.drawSingleLineText(name,
                             (int)centerInPixels.get<0>(),
                             (int)(centerInPixels.get<1>() -centerCircleRadius*2));
    }
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
    // DEBUG pb MT getGlyphPosition
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

void DrawableArea::SetFillColour(Colour newColour)
{
    fillColour = newColour;
}
void DrawableArea::SetName(String newName)
{
    name = newName;
}

void DrawableArea::SetAlpha(float newAlpha)
{
	fillOpacity = newAlpha;
}

void DrawableArea::EnableLowOpacityMode(bool enable)
{
    enableLowOpacityMode = enable;
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



