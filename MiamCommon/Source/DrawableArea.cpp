/*
  ==============================================================================

    DrawableArea.cpp
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <MiamMath.h>

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
    
    resetImages();

	// taille des buffers
	opaque_vertex_buffer.resize(opaque_vertex_buffer_size);
	opaque_index_buffer.resize(opaque_index_buffer_size);
	opaque_color_buffer.resize(opaque_color_buffer_size);	

	int numPoints = numPointsRing;
	ComputeRing(numPoints);

	for (int i = 0; i < opaque_color_buffer_size/4; ++i)
	{
		opaque_color_buffer[4 * i + 0] = contourColour.getRed() / 255.0f;
		opaque_color_buffer[4 * i + 1] = contourColour.getGreen() / 255.0f;
		opaque_color_buffer[4 * i + 2] = contourColour.getBlue() / 255.0f;
		opaque_color_buffer[4 * i + 3] = contourColour.getAlpha() / 255.0f;
	}
	//for (int i = 4 * numPoints; i < opaque_color_buffer_size; ++i)
	//	opaque_color_buffer[i] = 0;
}

void DrawableArea::ComputeRing(int numPoints)
{
	// calcul d'un anneau de centre 0, de rayon 5 pixels et avec une épaisseur de 2 pixels
	float radius = 5.0f;
	float width = 3.0f;

	float ri = radius - width / 2.0f;
	float re = radius + width / 2.0f;

	double currentAngle = 0.0;
	double incAngle = 2 * M_PI / (double)numPoints;
	for (int i = 0; i < numPoints; ++i)
	{
		g_vertex_ring[i * 3] = ri * (float)cos(currentAngle);
		g_vertex_ring[i * 3 + 1] = ri * (float)sin(currentAngle);
		g_vertex_ring[i * 3 + 2] = 0.0f;
		g_vertex_ring[numPoints * 3 + i * 3] = re * (float)cos(currentAngle);
		g_vertex_ring[numPoints * 3 + i * 3 + 1] = re * (float)sin(currentAngle);
		g_vertex_ring[numPoints * 3 + i * 3 + 2] = 0.0;
		currentAngle += incAngle;
	}
	for (int i = 0; i < numPoints; ++i)
	{
		ringIndices[i * 6] = i;
		ringIndices[i * 6 + 1] = numPoints + i;
		ringIndices[i * 6 + 2] = numPoints + i + 1 >= 2 * numPoints ? numPoints : numPoints + i + 1;
		ringIndices[i * 6 + 3] = numPoints + i + 1 >= 2 * numPoints ? numPoints : numPoints + i + 1;
		ringIndices[i * 6 + 4] = i;
		ringIndices[i * 6 + 5] = i + 1 >= numPoints ? 0 : i + 1;
	}
}


void DrawableArea::resetImages()
{
    // Construction de l'image (fond transparent à l'avenir)
    // On en recréée un 2è, qu'on assigne à la 1ière pour chaque taille...
    // Pour éviter de nombreux problèmes (écrire un constructeur de copie
    // de DrawableArea par exemple....)
 
    for (int integerScale = 1 ; integerScale<=2 ; integerScale++)
    {
        int scaledWidth = (int) nameWidth * integerScale;
        int scaledHeight = (int) nameHeight * integerScale;
        
        Image nameImageToAssign = Image(Image::ARGB, scaledWidth, scaledHeight, true);
        switch (integerScale)
        {
            case 1 :
                nameImage = nameImageToAssign;
                nameImage.duplicateIfShared();
                break;
            case 2 :
                nameImage2 = nameImageToAssign;
                nameImage2.duplicateIfShared();
                break;
            default :
                throw std::logic_error("Échelle entière de " + boost::lexical_cast<std::string>(integerScale) + "pas prise en compte directement. Rescale dynamique au rendu.");
        }
        // Pas d'assignation de texte par défaut...
    }
}

void DrawableArea::renderCachedNameImages()
{
    resetImages();
    for (int integerScale = 1 ; integerScale<=2 ; integerScale++)
    {
        Image* imagePtr;
        switch (integerScale)
        {
            case 1 :
                imagePtr = &nameImage;
                break;
            case 2 :
                imagePtr = &nameImage2;
                break;
            default :
                throw std::logic_error("Échelle entière de " + boost::lexical_cast<std::string>(integerScale) + "pas prise en compte directement. Rescale dynamique au rendu.");
        }
        // Contexte graphique permettant de faire du dessin dans l'image
        // après réinit de l'image
        Graphics g(*imagePtr);
        // Bounds de base, qu'on va décaler de qqs pixels
        auto textBounds = imagePtr->getBounds();
        int shadowOffsetXY = 1 * integerScale; // pixels
        // black shadow
        g.setFont((float)(imagePtr->getHeight() - shadowOffsetXY));
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
    // à cet instant, le composant qui rend du OpenGL a déjà appliqué une transformation affine.
    // On sauvegarde l'état graphique, on inverse la transfo affine, puis on rend les images
    // comme il faut -> NON l'état ne prend pas en compte les tranformations affines....
    if (isNameVisible)
    {
        // Dans tous les cas, on inverse l'échelle appliquée avant,
        g.addTransform(AffineTransform::scale(1.0f/(float)renderingScale,
                                              1.0f/(float)renderingScale));
        
        // Si on est proche à 10% d'une échelle de base (1 ou 2) : on ne fait rien
        // Par contre, si on est loin des 2 échelles de base, on doit faire du redimensionnement
        Image* scaledNameImage = nullptr;
        Image juceCreatedScaledNameImage;
        if ( std::abs(renderingScale-1.0) < 0.1 ) // échelle proche de 1
            scaledNameImage = &nameImage;
        else if ( std::abs(renderingScale-2.0) < 0.1 ) // échelle proche de 2
            scaledNameImage = &nameImage2;
        else // sinon, redimensionnement coûteux en CPU (très coûteux....)
        {
            // Tout ça sans utiliser de code de dessin de texte ! Car c'est ça qui ne passe pas
            // en multi-threadé....
            int newWidth = (int) std::round((double)nameWidth*renderingScale);
            int newHeight = (int) std::round((double)nameHeight*renderingScale);
            // On rescale alors la + grande et non la + petite (pour un meilleur rendu final...)
            juceCreatedScaledNameImage = nameImage2.rescaled(newWidth, newHeight);
            scaledNameImage = &juceCreatedScaledNameImage;
        }
        
        // Ensuite, le placement doit se faire en mettant soi-même à l'échelle...
        g.saveState();
        float alpha = GetAlpha();
        alpha = alpha + (1.0f - alpha)*0.3f; // légèrement plus opaque
        Colour actualContourColour = Colour(contourColour.getRed(), contourColour.getGreen(), contourColour.getBlue(), alpha );
        g.setColour(actualContourColour);
        // Coordonnées du dessin : dépend de si oui ou non on est proche du bord droit ou du bord bas
        double deltaX = (double)centerCircleRadius + 2.0;
        double deltaY = 2.0;
        // Si on est trop proche de la droite, alors on centre (ça ira comme l'image a le texte à gauche)
        // test avec le parent qui n'est pas mis à l'échelle rétina (pixels 'classiques')
        if ( ((int)std::round(centerInPixels.get<0>()) + nameImage.getWidth())
            > parentCanvas->getWidth() )
        {
            deltaX = -deltaX - name.length()*4; // le 4 est totalement arbitraire...
            deltaY += (double)centerCircleRadius;
        }
        // Si on est trop proche du bas, alors on met le texte en haut de centre de la forme
        if ( ((int)std::round(centerInPixels.get<1>()) + deltaY + nameImage.getHeight() + 8)
            > parentCanvas->getHeight() )
        {
            deltaY = -deltaY - nameImage.getHeight();
        }
        // Code effectif de dessin
        g.drawImageAt(*scaledNameImage,
                      (int)((centerInPixels.get<0>() + deltaX)*renderingScale),
                      (int)((centerInPixels.get<1>() + deltaY)*renderingScale),
                      false); // don't fill alpha channel with current brush
        g.restoreState();
        
        // Et on la remet après.... c'est dégueu mais avec Juce on n'a pas mieux.
        g.addTransform(AffineTransform::scale((float)renderingScale,
                                              (float)renderingScale));
    }
}

void DrawableArea::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
    parentCanvas = _parentCanvas;
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(parentCanvas->getWidth(), parentCanvas->getHeight());
	boost::geometry::transform(center, centerInPixels, scale);

}

void DrawableArea::fillOpenGLBuffers()
{
	int decalage = 0;
	for (int j = 0; j < 3 * numVerticesRing; j += 3)
	{
		if (displayCenter)
		{
			opaque_vertex_buffer[3 * decalage + j] = 1.0f* float(centerInPixels.get<0>() + g_vertex_ring[j]);
			opaque_vertex_buffer[3 * decalage + j + 1] = 1.0f*float(centerInPixels.get<1>() + g_vertex_ring[j + 1]);
			opaque_vertex_buffer[3 * decalage + j + 2] = 0.1f + g_vertex_ring[j + 2];
		}
		else
		{
			opaque_vertex_buffer[3 * decalage + j] = 0.0f;
			opaque_vertex_buffer[3 * decalage + j + 1] = 0.0f;
			opaque_vertex_buffer[3 * decalage + j + 2] = 0.0f;
		}
	}

	for (int j = 0; j < 3 * numVerticesRing; ++j)
	{
		opaque_index_buffer[j + 3 * decalage] = ringIndices[j];/*+ numVerticesPolygon*/;
	}
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
    {
        name = newName;
        renderCachedNameImages(); // au ratio de base (précisé en attribut constant dans la classe)
    }
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
            //std::cout << fillOpacity << std::endl;
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



