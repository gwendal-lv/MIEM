/*
  ==============================================================================

    DrawableArea.cpp
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <codecvt> // codecvt_utf8_utf16

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

DrawableArea::DrawableArea(int64_t _Id, bpt _center, Colour _fillColour) : mainZoffset(0.0f)
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
    // TEEEEEMMMMMMMMPPPPPPPPP
    glTextObject = std::make_shared<OpenGLTextObject>(20.0f, 160.0f, 20.0f, -35.0f, 12);
    
    keepRatio = false;

	areaVisible = true;
    
#ifdef __MIEM_VBO
    // taille des VBOs
    g_vertex_ring.resize(3 * numVerticesRing);
    ringIndices.resize(3 * numVerticesRing);
    
    vertices_buffer.resize(GetVerticesBufferSize());
    indices_buffer.resize(GetIndicesBufferSize());
    coulours_buffer.resize(GetColoursBufferSize());
    
	ComputeRing();
    
#else  // not defined __MIEM_VBO
    resetImages();
    
#endif
}


void DrawableArea::setZoffset(float newOffset)
{
    mainZoffset = newOffset;
    RefreshOpenGLBuffers();
}

void DrawableArea::ComputeRing()
{
	// calcul d'un anneau de centre 0, de rayon XX pixels et avec une épaisseur de XX pixels
	const float radius = 5.0f;
	const float width = 2.0f;

	float ri = radius - width / 2.0f;
	float re = radius + width / 2.0f;

	double currentAngle = 0.0;
	double incAngle = 2 * M_PI / (double)ringResolution;
	for (int i = 0; i < ringResolution; ++i) // pour chaque "segment"
	{
        // Cercle intérieur : indices 0 à 31
        g_vertex_ring[i * 3 + 0] = ri * (float)cos(currentAngle);
        g_vertex_ring[i * 3 + 1] = ri * (float)sin(currentAngle);
        g_vertex_ring[i * 3 + 2] = 0.0f;
        // cercle extérieur : indices 32 à 61
        g_vertex_ring[ringResolution * 3 + i * 3 + 0] = re * (float)cos(currentAngle);
        g_vertex_ring[ringResolution * 3 + i * 3 + 1] = re * (float)sin(currentAngle);
        g_vertex_ring[ringResolution * 3 + i * 3 + 2] = 0.0f;
        
		currentAngle += incAngle;
	}
	for (int i = 0; i < ringResolution; ++i) // résolution 32 -> 64 points et 64 triangles
	{
        // triangle avec côté extérieur
		ringIndices[i * 6 + 0] = i; // pt intérieur
        ringIndices[i * 6 + 1] = ringResolution + i; // pt extérieur correspondant
        // dernier point : l'extérieur suivant, ou le 1ier extérieur si on revient au départ
		ringIndices[i * 6 + 2] = (ringResolution + i + 1) >= (2 * ringResolution) ?
                                 ringResolution : (ringResolution + i + 1);
        
        // triangle avec côté intérieur
        // en repartant du point "extérieur suivant" calculé juste avant
        ringIndices[i * 6 + 3] = ringIndices[i * 6 + 2];
		ringIndices[i * 6 + 4] = i;
		ringIndices[i * 6 + 5] = (i + 1) >= ringResolution ? 0 : (i + 1);
	}
}


void DrawableArea::resetImages()
{
#ifdef __MIEM_VBO
    assert(false); // this function should not be called in VBO mode
#endif
    
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
#ifdef __MIEM_VBO
    assert(false); // Images are not initialized in VBO mode, they can't be rendered.
#endif
    
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


void DrawableArea::resetTextureBasedName()
{
    // conversion utf-8 (générée nativement dans le programme, peu importe la plateforme)
    // pour avoir le vrai bon nombre de caractères
    std::u16string nameU16 = u"";
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    auto wideString = converter.from_bytes(name.toStdString().c_str());
    for (auto singleWideChar : wideString)
        nameU16 += (char16_t) singleWideChar;
    // Sending the right number of utf16 chars (not too much)
    if (nameU16.length() > nameCharsCountMax)
    {
        nameU16.resize(nameCharsCountMax);
        nameU16[nameU16.size()-1] = '.'; // caractère de chaîne raccourcie
    }
    
    // Choix du placement en X. Idée de base :
    // décaler de 1/3 de la taille estimée des caractères
    const double nameWidthEstimation = 0.7 * (double)(nameWidth) * nameU16.length()
                                            / (double)(nameCharsCountMax);
    const double desiredXOffset = - nameWidthEstimation / 3.0;
    double xPosition = 0.0f;
    // Si on près du bord gauche : on décale pour que la texture ne touche pas le bord
    const double minLeftMargin = 2.0 * (double)centerCircleRadius;
    if ( minLeftMargin > (centerInPixels.get<0>() + desiredXOffset) )
        xPosition = minLeftMargin;
    // si on est près du bord droit : on ne connaît pas la vraie taille de la texture tant qu'on
    // ne l'a pas construite.... Estimation du coup
    else if (parentCanvas &&
             ((centerInPixels.get<0>() + nameWidthEstimation + desiredXOffset)
                > parentCanvas->getWidth()))
        xPosition = parentCanvas->getWidth() - nameWidthEstimation;
    // Sinon c'est bon !
    else
        xPosition = centerInPixels.get<0>() + desiredXOffset;
    
    
    // reconstruction complète d'un nouvel objet (pour thread-safety)
    glTextObject = std::make_shared<OpenGLTextObject>((float)xPosition,
                                                      (float)(centerInPixels.get<1>() + centerCircleRadius + (double)nameHeight * 1.1),
                                                      std::roundf((float)(nameWidth) / (float)(nameCharsCountMax)),
		(float)nameHeight,
		nameCharsCountMax);
    // vérification pour l'UTF 16... en debug seulement
#ifdef __MIAM_DEBUG
	assert(sizeof(wchar_t) >= 2); // our UTF 16 converter won't properly work on this platform....)
#endif
    
    glTextObject->SetText(nameU16);
}



void DrawableArea::Paint(Graphics& g)
{
    // Dessin du centre (couleur du contour) si nécessaire
    if (displayCenter)
    {
        Colour actualContourColour = Colour(contourColour.getRed(), contourColour.getGreen(), contourColour.getBlue(), GetAlpha() );        
        g.setColour(actualContourColour);
        g.drawEllipse((float)centerInPixels.get<0>()-(float)centerCircleRadius,
                      (float)centerInPixels.get<1>()- (float)centerCircleRadius,
					(float)centerCircleRadius*2.0f,
					centerCircleRadius*2.0f,
					centerContourWidth);
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
            deltaX = -deltaX - name.length()*4.0; // le 4 est totalement arbitraire...
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
    
#ifdef __MIEM_VBO
    // Reconstruction complète de l'objet à chaque fois.... (pour assurer la multithread-safety....)
    resetTextureBasedName();
#endif
    
}

void DrawableArea::RefreshOpenGLBuffers()
{
#ifndef __MIEM_VBO
    assert(false); // this function should not be called !!
#endif
    
    // - - - Vertices - - -
	const float Xoffset = float(centerInPixels.get<0>());
	const float Yoffset = float(centerInPixels.get<1>());
	const float Zoffset = mainZoffset + MIEM_CENTRAL_RING_Z;
	if (displayCenter)
	{
        for (int j = 0; j < 3 * numVerticesRing ; j += 3)
		{
			vertices_buffer[j + 0] = Xoffset + g_vertex_ring[j + 0];
			vertices_buffer[j + 1] = Yoffset + g_vertex_ring[j + 1];
			vertices_buffer[j + 2] = Zoffset + g_vertex_ring[j + 2];
		}
	}
	else
	{
		for (int j = 0; j < 3 * numVerticesRing ; j += 3)
		{
			vertices_buffer[j + 0] = MIEM_UNVISIBLE_COORDINATE;
			vertices_buffer[j + 1] = MIEM_UNVISIBLE_COORDINATE;
			vertices_buffer[j + 2] = 0.0f;
		}
	}

    // - - - Indices - - -
    for (int jj = 0; jj < 3 * numVerticesRing ; ++jj) // autant de triangles que de vertices
	{
		indices_buffer[jj] = ringIndices[jj];
	}
    
    // - - - Couleur - - -
    const GLfloat R = contourColour.getRed() / 255.0f;
    const GLfloat G = contourColour.getGreen() / 250.0f;
    const GLfloat B = contourColour.getBlue() / 250.0f;
    const GLfloat A = GetAlpha(); // dynamic opacity
    
    for (int i = 0; i < 4*numVerticesRing; i+=4)
    {
        coulours_buffer[i + 0] = R;
        coulours_buffer[i + 1] = G;
        coulours_buffer[i + 2] = B;
        coulours_buffer[i + 3] = A;
    }
}

void DrawableArea::initSurfaceAndContourIndexSubBuffer(int vertexElmtOffset, int indexElmtOffset, int actualPointsCount)
{
    // surface interne : définition des indices des triangles
    for (int i = 0; i < actualPointsCount ; ++i)
    {
        // sera le centre par défaut
        indices_buffer[indexElmtOffset + i * 3 + 0] = vertexElmtOffset + 0;
        // 1ier point extérieur (tjs dans les bornes)
        indices_buffer[indexElmtOffset + i * 3 + 1] = vertexElmtOffset + i + 1;
        // 2nd point extérieur, qui peut être le dernier ou le premier du contour
        indices_buffer[indexElmtOffset + i * 3 + 2] = (i + 2) > actualPointsCount ?
        (vertexElmtOffset + 1) : (vertexElmtOffset + i + 2);
    }
    // En tout : on a autant de triangles que de points sur le contour du polygone
    for (int ii = 3 * actualPointsCount; ii < 3 * numPointsPolygon; ++ii) // points du VBO alloués en mémoire mais inutiles
    indices_buffer[indexElmtOffset + ii] = 0;

    // indices pour dessiner le contour
    // on avait "numPointsPolygon" triangles à dessiner au max
    const int contourIndexElmtOffset = indexElmtOffset + 3 * numPointsPolygon;
    // indices du ring pour dessiner le contour
    const int contourRingVertexElmtOffset = vertexElmtOffset + numVerticesPolygon;
    const int biggerContourRingVertexElmtOffset = contourRingVertexElmtOffset + numPointsPolygon;
    for (int i = 0; i < numPointsPolygon; ++i)
    {
        if (i < actualPointsCount)
        {
            // -> Triangle avec un côté en bordure intérieure
            // 1ier point de contour intérieur (tjs dans les bornes)
            indices_buffer[contourIndexElmtOffset + i * 6 + 0] = contourRingVertexElmtOffset + i;
            // 2nd point de contour, qui peut être le dernier ou le premier du contour
            indices_buffer[contourIndexElmtOffset + i * 6 + 1] = (i + 1) >= actualPointsCount ?
            (contourRingVertexElmtOffset + 0) : (contourRingVertexElmtOffset + i + 1);
            // pt de contour extérieur (tjs dans les bornes)
            indices_buffer[contourIndexElmtOffset + i * 6 + 2] = biggerContourRingVertexElmtOffset + i;
            
            // -> Triangle avec un côté en bordure extérieure
            // on repart du 2nd point de contour non-agrandi
            indices_buffer[contourIndexElmtOffset + i * 6 + 3] =
                        indices_buffer[contourIndexElmtOffset + i * 6 + 1];
            // pt contour agrandi tjs dans les bornes
            indices_buffer[contourIndexElmtOffset + i * 6 + 4] =
                        indices_buffer[contourIndexElmtOffset + i * 6 + 2];
            // pt contour agrandi avec possible dépassement (retour au départ)
            indices_buffer[contourIndexElmtOffset + i * 6 + 5] = (i + 1) >= actualPointsCount ?
            (biggerContourRingVertexElmtOffset+0) : biggerContourRingVertexElmtOffset + i + 1;
        }
        else
        {
            indices_buffer[contourIndexElmtOffset + i * 6 + 0] = 0;
            indices_buffer[contourIndexElmtOffset + i * 6 + 1] = 0;
            indices_buffer[contourIndexElmtOffset + i * 6 + 2] = 0;
            indices_buffer[contourIndexElmtOffset + i * 6 + 3] = 0;
            indices_buffer[contourIndexElmtOffset + i * 6 + 4] = 0;
            indices_buffer[contourIndexElmtOffset + i * 6 + 5] = 0;
        }
    }
}
void DrawableArea::initSurfaceAndContourColourSubBuffer(int vertexElmtOffset)
{
    // - - - couleur de la forme - - -
    const float A = GetAlpha();
    const float R = fillColour.getRed() / 255.0f;
    const float G = fillColour.getGreen() / 255.0f;
    const float B = fillColour.getBlue() / 255.0f;
    for (int i = 0; i < numVerticesPolygon; ++i)
    {
        coulours_buffer[4 * (vertexElmtOffset + i) + 0] = R;
        coulours_buffer[4 * (vertexElmtOffset + i) + 1] = G;
        coulours_buffer[4 * (vertexElmtOffset + i) + 2] = B;
        coulours_buffer[4 * (vertexElmtOffset + i) + 3] = A;
    }
    
    // - - - couleur du contour - - -
    const int contourLineVertexElmtOffset = vertexElmtOffset + numVerticesPolygon;
    const float A2 = GetAlpha();
    const float R2 = contourColour.getRed() / 255.0f;
    const float G2 = contourColour.getGreen() / 255.0f;
    const float B2 = contourColour.getBlue() / 255.0f;
    for (int i = 0; i < numVerticesRing; ++i)
    {
        coulours_buffer[4 * (contourLineVertexElmtOffset + i) + 0] = R2;
        coulours_buffer[4 * (contourLineVertexElmtOffset + i) + 1] = G2;
        coulours_buffer[4 * (contourLineVertexElmtOffset + i) + 2] = B2;
        coulours_buffer[4 * (contourLineVertexElmtOffset + i) + 3] = A2;
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
#ifdef __MIEM_VBO
        resetTextureBasedName();
#else
        renderCachedNameImages(); // au ratio de base (précisé en attribut constant dans la classe)
#endif
    }
}

void DrawableArea::setIsFilled(bool shouldBeFilled)
{
    isFilled = shouldBeFilled;
}


void DrawableArea::SetAlpha(float newAlpha)
{
	fillOpacity = newAlpha;
    
    // =================== à optimiser ? =====================
    // =================== à optimiser ? =====================
    // car cette fonction sera appelée extrêment souvent.... Inutile de recalculer tout le VBO
    // recalculer les indices de couleurs (même si on les calcule tous) suffira largement
#ifdef __MIEM_VBO
    RefreshOpenGLBuffers();
#endif
    // =================== à optimiser ? =====================
    // =================== à optimiser ? =====================
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
    if (opacityMode != opacityMode_)
    {
        opacityMode = opacityMode_;
#ifdef __MIEM_VBO
        if (parentCanvas)
            RefreshOpenGLBuffers();
#endif
    }
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



