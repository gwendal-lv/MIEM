/*
  ==============================================================================

    SceneCanvasComponent.cpp
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>
#include <algorithm>

#include "SceneCanvasComponent.h"

#include "MultiSceneCanvasInteractor.h"

//==============================================================================
SceneCanvasComponent::SceneCanvasComponent() : 
	selectedForEditing(false),
	displayFrequencyMeasurer("SceneCanvasComponent display")
{
	init(20,64);
}


SceneCanvasComponent::SceneCanvasComponent(int numShapesMax, int numPointsMax) :
    selectedForEditing(false),
    displayFrequencyMeasurer("SceneCanvasComponent display")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	init(numShapesMax, numPointsMax);
}

void SceneCanvasComponent::init(int numShapesMax, int numPointsMax)
{
	EunderTime = 0.0;
	previousMaxSize = 0;
	needToResetBufferParts = false;

	Nshapes = new const int(numShapesMax/*Npolygons + Npolygons * (Npolygons + 1) / 2*/);

	numPointsPolygon = new const int(numPointsMax);

	numVerticesPolygon = new const int((*numPointsPolygon) + 1);

	numVertexShape = new const int(*numVerticesPolygon + numVerticesRing + ((*numPointsPolygon) * numVerticesCircle) + *numPointsPolygon + dottedLineVertexes + numVerticesRing);
	shapeVertexBufferSize = new const int(3 * (*numVerticesPolygon) + (3 * numVerticesRing) + (*numPointsPolygon) * (3 * numVerticesCircle) + (3 * (*numPointsPolygon)) + 3 * dottedLineVertexes + 3 * numVerticesRing);
	shapeColorBufferSize = new const int(4 * (*numVerticesPolygon + numVerticesRing + (*numPointsPolygon) * (numVerticesCircle)+(*numPointsPolygon) + dottedLineVertexes + numVerticesRing));
	shapeIndicesSize = new const int(3 * (*numVerticesPolygon) + (3 * numVerticesRing) + (*numPointsPolygon) * (3 * numPointCircle) + (3 * 2 * (*numPointsPolygon)) + dottedLineIndices + (3 * numVerticesRing));

	vertexBufferSize = new const int(*Nshapes * (*shapeVertexBufferSize));
	colorBufferSize = new const int(*Nshapes * (*shapeColorBufferSize));
	indicesSize = new const int(*Nshapes * (*shapeIndicesSize));

	g_vertex_buffer_data = new GLfloat[*vertexBufferSize];
	g_color_buffer_data = new GLfloat[*colorBufferSize];
	indices = new GLuint[*indicesSize];

	for (int i = 0; i < *Nshapes; ++i)
	{
		for (int j = i * (*shapeColorBufferSize); j < i* (*shapeColorBufferSize) + 4 * (*numVerticesPolygon); j += 4)
		{
			g_color_buffer_data[j] = 0.5f;
			g_color_buffer_data[j + 1] = 0.5f;
			g_color_buffer_data[j + 2] = 0.5f;
			g_color_buffer_data[j + 3] = 1.0f;
		}
		for (int j = i * (*shapeColorBufferSize) + 4 * (*numVerticesPolygon); j < (i + 1)*(*shapeColorBufferSize); j += 4)
		{
			g_color_buffer_data[j] = 1.0f;
			g_color_buffer_data[j + 1] = 0.0f;
			g_color_buffer_data[j + 2] = 0.0f;
			g_color_buffer_data[j + 3] = 1.0f;
		}
	}

	// calcul d'un anneau de centre 0, de rayon 5 pixels et avec une épaisseur de 2 pixels
	float radius = 5.0f;
	float width = 3.0f;
	int numPoints = numPointsRing;
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

	// calcul d'un cercle de centre 0 et de rayon 5 pixels
	radius = 5.0f;
	currentAngle = 0.0;
	incAngle = 2 * M_PI / (double)numPoints;
	g_vertex_circle[0] = 0.0f;
	g_vertex_circle[1] = 0.0f;
	g_vertex_circle[2] = 0.0f;
	for (int i = 0; i < numPointCircle; ++i)
	{
		g_vertex_circle[(i + 1) * 3] = radius * (float)cos(currentAngle);
		g_vertex_circle[(i + 1) * 3 + 1] = radius * (float)sin(currentAngle);
		g_vertex_circle[(i + 1) * 3 + 2] = 0.0f;
		currentAngle += incAngle;
	}
	for (int i = 0; i < numPointCircle; ++i)
	{
		circleIndices[i * 3] = i + 1;
		circleIndices[i * 3 + 1] = 0;
		circleIndices[i * 3 + 2] = i + 2 > numPointCircle ? 1 : i + 2;
	}

	// mise à 0 des buffers
	for (int i = 0; i < 3 * (*numVerticesPolygon) + 3 * numVerticesRing; ++i)
		g_vertex_buffer_data[i] = 0;

	for (int i = 0; i < 3 * (*numVerticesPolygon) + 3 * (*numVerticesPolygon); ++i)
		indices[i] = 0;

	shift2[0] = 0;
	shift2[1] = *numVerticesPolygon;
	shift2[2] = *numVerticesPolygon + numVerticesRing;


	for (int k = 0; k < *numPointsPolygon; ++k)
	{
		shift2[3 + k] = shift2[2] + (k + 1) * numVerticesCircle;
	}

	shift2[2 + *numPointsPolygon + 1] = shift2[2 + *numPointsPolygon] + *numPointsPolygon;
	shift2[2 + *numPointsPolygon + 2] = shift2[2 + *numPointsPolygon + 1] + dottedLineVertexes;

	openGlContext.setComponentPaintingEnabled(true); // default behavior, lower perfs
													 // OpenGL final initialization will happen in the COmpleteInitialization method
}

SceneCanvasComponent::~SceneCanvasComponent()
{
    openGlContext.detach();
	delete[] g_vertex_buffer_data;
	delete[] g_color_buffer_data;
	delete[] indices;

	delete vertexBufferSize;
	delete colorBufferSize;
	delete indicesSize;

	delete numVertexShape;
	delete shapeVertexBufferSize;
	delete shapeColorBufferSize;
	delete shapeIndicesSize;

	delete numVerticesPolygon;

	delete Nshapes;
	delete numPointsPolygon;
    DBG("SceneCanvasComponent : destruction arrive à son terme");
}

void SceneCanvasComponent::CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager)
{
    canvasManager = _canvasManager;
    
    // OpenGL final initialization here
    openGlContext.setRenderer(this);
    openGlContext.attachTo(*this);
    openGlContext.setContinuousRepainting(true);
    isSwapSynced = openGlContext.setSwapInterval(swapInterval);
    if (isSwapSynced)
        DBG("Platform supports synced OpenGL swaps");
    else
        DBG("Platform does not support synced OpenGL swaps");
}







// - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (auto manager = canvasManager.lock())
    {
        manager->OnResized();
    }
}




// - - - - - - - - OpenGL specific - - - - - - - - -
void SceneCanvasComponent::newOpenGLContextCreated()
{
    //DBG("SceneCanvasComponent : init OpenGL");
	numFrame = 0;

	// Will init the counter
	//displayFrequencyMeasurer.OnNewFrame();

	shaderProgram = new OpenGLShaderProgram(openGlContext);
	shaderProgram->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(myVertexShader));
	shaderProgram->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(myFragmentShader));
	shaderProgram->link();

	shaderProgram->use();

	// TRIANGLE BUFFER
	// openGL génère 1 buffer, avec l'ID vertexBuffer, et qui contiendra g_vertex_buffer_data
	openGlContext.extensions.glGenBuffers(1, &vertexBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, *vertexBufferSize * sizeof(GLfloat)/*sizeof(g_vertex_buffer_data)*/,
		g_vertex_buffer_data, GL_STATIC_DRAW);

	// TRIANGLE COULEUR
	// pareil pour les buffers de couleurs des deux
	openGlContext.extensions.glGenBuffers(1, &colorBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, *colorBufferSize * sizeof(GLfloat), g_color_buffer_data, GL_STATIC_DRAW);

	// TRIANGLE INDEX
	openGlContext.extensions.glGenBuffers(1, &elementBuffer);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, *indicesSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// déclaration des attributs et uniforms : !!! aux noms et leurs utilisations dans les shaders !!!
	position = new OpenGLShaderProgram::Attribute(*shaderProgram, String("position").toRawUTF8());
	colour = new OpenGLShaderProgram::Attribute(*shaderProgram, "colour");

	projectionMatrix = new OpenGLShaderProgram::Uniform(*shaderProgram, "projectionMatrix");
	viewMatrix = new OpenGLShaderProgram::Uniform(*shaderProgram, "viewMatrix");
	modelMatrix = new OpenGLShaderProgram::Uniform(*shaderProgram, "modelMatrix");


	/// calcul des matrices
	Matrix3D<float> testView = lookAt(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
	//Matrix3D<float> testProject = perspective((float)getWidth(), (float)getHeight(), 0.1f, 100.0f);
	if (projectionMatrix != nullptr)
		projectionMatrix->setMatrix4(perspective((float)getWidth(), (float)getHeight(), 0.5f, 1.1f).mat, 1, false);

	if (viewMatrix != nullptr)
		viewMatrix->setMatrix4(testView.mat, 1, false);

	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, (float)getHeight(), 0.0f, 1.0f);//10*getHeight()

	if (modelMatrix != nullptr)
		modelMatrix->setMatrix4(model.mat, 1, false);

	//shaderProgram->use(); // on utilise qu'un seul shader program pour le moment donc on appelle une seule fois cette fonction
}
void SceneCanvasComponent::renderOpenGL()
{
	++numFrame;

	//DBG("render : " + getName());
    auto manager = canvasManager.lock();
	OpenGLHelpers::clear(Colours::black);
    const double desktopScale = (float)openGlContext.getRenderingScale();
    
    // Pure black background
   // g.fillAll (Colours::black);
    
    // White interior contour 2px line to show when the canvas is active
    /*if (selectedForEditing)
    {
        g.setColour(Colours::white);
        g.drawRect(1, 1, getWidth()-2, getHeight()-2, 2);
    }*/
	glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ZERO);
	openGlContext.extensions.glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	

	glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));
    
	shaderProgram->use();
    
    // - - - - - THIRD Duplication of the drawable objects for thread-safe rendering, - - - - -
    // Lorsque nécessaire seulement !
    manager->LockAsyncDrawableObjects();
    
    
    bool areasCountChanged = (manager->GetAsyncDrawableObjects().size() != duplicatedAreas.size());
    // POUR L'INSTANT ALGO BÊTE
    // on resize le vecteur : la construction des shared n'est pas grave, leur bloc de contrôle reste
    // en mémoire finalement (on utilisera reset() )
    if (areasCountChanged)
    {
		if (manager->GetAsyncDrawableObjects().size() < duplicatedAreas.size())
		{
			previousMaxSize = (int)duplicatedAreas.size();
			needToResetBufferParts = true;
		}
		else
		{
			previousMaxSize = (int)manager->GetAsyncDrawableObjects().size();
			needToResetBufferParts = false;
		}
        canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
        duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
    }
	else
	{
		previousMaxSize = (int)duplicatedAreas.size();
	}
    // Vérification simple de chaque aire 1 par 1
    size_t itIndex = 0;
    for (auto it = manager->GetAsyncDrawableObjects().begin() ;
         it != manager->GetAsyncDrawableObjects().end() ;
         ++it)
    {
        // S'il y a eu un changement : on re-crée un pointeur déjà, puis
        // on fait effectivement la copie d'un nouvel objet
        if (canvasAreasPointersCopies[itIndex] != (*it))
        {
            canvasAreasPointersCopies[itIndex] = (*it);
            duplicatedAreas[itIndex] = (*it)->Clone();
        }
        // Double compteur
        itIndex++;
    }
    
    manager->UnlockAsyncDrawableObjects();
    
    
    // - - - - - Areas painting (including exciters if existing) - - - - -
    // Sans bloquer, du coup, les autres threads (pour réactivité max...)
	const int duplicatedSize = duplicatedAreas.size();
	for (size_t i=0;i<duplicatedSize;i++)
    {
		DrawShape(duplicatedAreas[i], (int)i);
    }

	if (needToResetBufferParts)
	{
		std::fill(g_vertex_buffer_data + (int)duplicatedAreas.size() * *shapeVertexBufferSize, g_vertex_buffer_data + previousMaxSize * *shapeVertexBufferSize/**vertexBufferSize*/, 0.0f);
		std::fill(g_color_buffer_data + (int)duplicatedAreas.size() * *shapeColorBufferSize, g_color_buffer_data + previousMaxSize * *shapeColorBufferSize/**colorBufferSize*/, 0.0f);
		std::fill(indices + (int)duplicatedAreas.size() * *shapeIndicesSize, indices + previousMaxSize * *shapeIndicesSize/**indicesSize*/, 0);
	}
	/*for (int i = (int)duplicatedAreas.size() * *shapeVertexBufferSize; i < *vertexBufferSize; ++i)
	{
		g_vertex_buffer_data[i] = 0.0f;
	}
	for (int i = (int)duplicatedAreas.size() * *shapeColorBufferSize; i < *colorBufferSize; ++i)
	{
		g_color_buffer_data[i] = 0.0f;
	}
	for (int i = (int)duplicatedAreas.size() * *shapeIndicesSize; i < *indicesSize; ++i)
	{
		indices[i] = 0;
	}*/

	//glOrtho(0,getWidth(),0, getHeight(), 0.5f, 1.1f);



	/// Draw triangle
	openGlContext.extensions.glEnableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, /**vertexBufferSize*/ previousMaxSize * *shapeVertexBufferSize * sizeof(GLfloat)/*sizeof(g_vertex_buffer_data)*/, g_vertex_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);
	//openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);

	openGlContext.extensions.glEnableVertexAttribArray(colour->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, /**colorBufferSize*/ previousMaxSize * *shapeColorBufferSize * sizeof(GLfloat), g_color_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(colour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(float[4]), 0);

	
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	//openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	openGlContext.extensions.glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, /**indicesSize*/ previousMaxSize * *shapeIndicesSize * sizeof(unsigned int), indices);

	glDrawElements(GL_TRIANGLES, previousMaxSize * *shapeIndicesSize/**indicesSize*/ /*+ 3 * 64*/, GL_UNSIGNED_INT, (void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	openGlContext.extensions.glDisableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glDisableVertexAttribArray(colour->attributeID);

	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    // Call to a general Graphic update on the whole Presenter module
	if ( ! manager->isUpdatePending() )
		manager->triggerAsyncUpdate();
    
    // Time measures just before swap (or the closer that we can get to the swaps)
    displayFrequencyMeasurer.OnNewFrame();
    /*
    if (selectedForEditing)
    {
        if (displayFrequencyMeasurer.IsFreshAverageAvailable())
            DBG(displayFrequencyMeasurer.GetInfo());
    }
     */
    // Forced sleep if drawing is too fast
#if __AMUSINGMOBILE
	double lastDuration = displayFrequencyMeasurer.GetLastDuration_ms();
	double underTime = lastDuration > 0.0? desiredPeriod_ms - lastDuration : 0.0;
#else
	double underTime = desiredPeriod_ms - displayFrequencyMeasurer.GetLastDuration_ms();
#endif

	
	double last = displayFrequencyMeasurer.GetLastDuration_ms();
	if (last < 0.0)
		DBG("probleme lastDuration: " + (String)last);
	EunderTime += underTime;
	if (numFrame > 6000)
	{
		//DBG("60 frames, underTime : "+ (String)underTime +" [underTime]" + (String)(EunderTime/600.0));
		//DBG("probleme underTime : " + (String)underTime);
		EunderTime = 0.0;
		numFrame = 0;
	}
	if (underTime < 0.0)
	{
		//DBG("probleme underTime : " + (String)underTime);
		underTime = 0.0;
	}
	//if (last > desiredPeriod_ms)
	//	DBG("probleme : last > desiredPeriod_ms");

    if (underTime >= 0.0)
    {
        Thread::sleep((int)std::floor(underTime));
    }
}


void SceneCanvasComponent::openGLContextClosing()
{
    // Méthode même pas appelée dans Android... D'après la doc Juce au 30 octobre 2017
    //DBG("SceneCanvasComponent : closing OpenGL Context");
}





// - - - - - - - - Juce events - - - - - - - - -

void SceneCanvasComponent::mouseDown(const juce::MouseEvent& event)
{
    canvasManager.lock()->OnCanvasMouseDown(event);
}

void SceneCanvasComponent::mouseDrag(const juce::MouseEvent& event)
{
    canvasManager.lock()->OnCanvasMouseDrag(event);
}

void SceneCanvasComponent::mouseUp(const juce::MouseEvent& event)
{
    canvasManager.lock()->OnCanvasMouseUp(event);
}



// Getters and Setters

void SceneCanvasComponent::SetIsSelectedForEditing(bool isSelected)
{
    selectedForEditing = isSelected;
}

void SceneCanvasComponent::DrawShape(std::shared_ptr<IDrawableArea> area, int positionInBuffer)
{
	int decalage = 3 * ((int)positionInBuffer * *numVertexShape);// + numPointsPolygon + 1;
	area->RefreshOpenGLBuffers();
	//std::vector<int> shift;
	//shift.push_back(decalage);
	if (area->GetVerticesBufferSize() >= 3)
	{
		/// vertex
		//1. forme
		try
		{
			const int verticesCount = 3 * area->GetVerticesBufferSize();
			float* vertexPtr = area->GetVerticesBufferPtr();
			for (int j = 0; j < verticesCount; ++j)
			{
				if (j < verticesCount)
					g_vertex_buffer_data[decalage + j] = (*vertexPtr);//area->GetVerticesBufferElt(j);//*10
				else
					g_vertex_buffer_data[decalage + j] = 0.0f;
				++vertexPtr;
			}
		}
		catch (const std::out_of_range& e)
		{
			DBG(e.what());
		}

		//decalage += (*numPointsPolygon + 1);

		//2. centre
		
		//decalage += numVerticesRing;

	}


	/// indices
	decalage = positionInBuffer * *shapeIndicesSize;// différent du decalage pour les vertex et les couleurs !
	int beginShape = positionInBuffer * *numVertexShape;
	if (area->GetIndicesBufferSize() >= 3)
	{
		try
		{
			const int indicesCount = area->GetIndicesBufferSize();
			int* indicesPtr = area->GetIndicesBufferPtr();
			for (int i = 0; i < indicesCount; ++i)
			{
				indices[decalage + i] = (*indicesPtr) + beginShape;
				++indicesPtr;
			}
		}
		catch (const std::out_of_range& e)
		{
			DBG(e.what());
		}

		// colors
		decalage = 4 * ((int)positionInBuffer * *numVertexShape);

		try
		{
			const int couloursCount = area->GetCouloursBufferSize();
			float* couloursPtr = area->GetCoulourBufferPtr();
			for (int i = 0; i < couloursCount; ++i)
			{
				g_color_buffer_data[decalage + i] = (*couloursPtr);
				++couloursPtr;
			}
		}
		catch (const std::out_of_range& e)
		{
			DBG(e.what());
		}

	}


}

void SceneCanvasComponent::computeManipulationLine(float Ox, float Oy, float Mx, float My, float width, float height)
{
	int N = 20;
	float length = (float)boost::geometry::distance(bpt(Ox, Oy), bpt(Mx, My));//0.25 * (getWidth() + getHeight()) / 2.0;
	if (length / (2 * height) > 20.0f)
		height = (length / 20.0f) / 2.0f;
	else
		N = int(length / (2.0f * height));

	float sina = (My - Oy) / length;
	float cosa = (Mx - Ox) / length;

	for (int i = 0; i <  dottedLineNparts; ++i)
	{
		if (i < N)
		{
			// up_left
			g_vertex_dotted_line[i * 3 * 4] = Ox + i * 2 * height * cosa - (width/2.0f) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 1] = Oy + i * 2 * height * sina + (width / 2.0f) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 2] = 0.1f;
			// down_left
			g_vertex_dotted_line[i * 3 * 4 + 3] = Ox + i * 2 * height * cosa + (width / 2.0f) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 4] = Oy + i * 2 * height * sina - (width / 2.0f) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 5] = 0.1f;
			// up_right
			g_vertex_dotted_line[i * 3 * 4 + 6] = Ox + (2 * i + 1)  * height * cosa - (width / 2.0f) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 7] = Oy + (2 * i + 1) * height * sina + (width / 2.0f) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 8] = 0.1f;
			// down_right
			g_vertex_dotted_line[i * 3 * 4 + 9] = Ox + (2 * i + 1) * height * cosa + (width / 2.0f) * sina;
			g_vertex_dotted_line[i * 3 * 4 + 10] = Oy + (2* i + 1) * height * sina - (width / 2.0f) * cosa;
			g_vertex_dotted_line[i * 3 * 4 + 11] = 0.1f;

			g_indices_dotted_line[i * 6] = i * 4;
			g_indices_dotted_line[i * 6 + 1] = i * 4 + 1;
			g_indices_dotted_line[i * 6 + 2] = i * 4 + 2;
			g_indices_dotted_line[i * 6 + 3] = i * 4 + 1;
			g_indices_dotted_line[i * 6 + 4] = i * 4 + 2;
			g_indices_dotted_line[i * 6 + 5] = i * 4 + 3;
		}
		else
		{
			for (int j = 0; j < 12; ++j)
			{
				g_vertex_dotted_line[i * 12 + j] = 0.0f;
			}
			for (int j = 0; j < 6; ++j)
			{
				g_indices_dotted_line[i * 6 + j] = 0;
			}
		}
	}

}