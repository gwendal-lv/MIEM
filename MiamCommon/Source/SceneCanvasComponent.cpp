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
	openGLLabel = std::shared_ptr<OpenGLTextObject>(new OpenGLTextObject("ExportedFont2_png", 100.0f, 100.0f, 20.0f, -30.0f, 12));
	ofs.open("testFps.txt", std::ofstream::out | std::ofstream::app);
	EunderTime = 0.0;
	previousMaxSize = 0;
	needToResetBufferParts = false;

	Nshapes = new const int(numShapesMax);

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

	g_canvasOutlineIndex_buffer_data[0] = 0;
	g_canvasOutlineIndex_buffer_data[1] = 4;
	g_canvasOutlineIndex_buffer_data[2] = 5;
	g_canvasOutlineIndex_buffer_data[3] = 5;
	g_canvasOutlineIndex_buffer_data[4] = 0;
	g_canvasOutlineIndex_buffer_data[5] = 1;
	g_canvasOutlineIndex_buffer_data[6] = 5;
	g_canvasOutlineIndex_buffer_data[7] = 1;
	g_canvasOutlineIndex_buffer_data[8] = 2;
	g_canvasOutlineIndex_buffer_data[9] = 5;
	g_canvasOutlineIndex_buffer_data[10] = 2;
	g_canvasOutlineIndex_buffer_data[11] = 6;
	g_canvasOutlineIndex_buffer_data[12] = 6;
	g_canvasOutlineIndex_buffer_data[13] = 2;
	g_canvasOutlineIndex_buffer_data[14] = 7;
	g_canvasOutlineIndex_buffer_data[15] = 2;
	g_canvasOutlineIndex_buffer_data[16] = 7;
	g_canvasOutlineIndex_buffer_data[17] = 3;
	g_canvasOutlineIndex_buffer_data[18] = 3;
	g_canvasOutlineIndex_buffer_data[19] = 7;
	g_canvasOutlineIndex_buffer_data[20] = 4;
	g_canvasOutlineIndex_buffer_data[21] = 3;
	g_canvasOutlineIndex_buffer_data[22] = 4;
	g_canvasOutlineIndex_buffer_data[23] = 0;

	releaseResources = false;

	openGlContext.setComponentPaintingEnabled(true); // default behavior, lower perfs
													 // OpenGL final initialization will happen in the COmpleteInitialization method
}

void SceneCanvasComponent::ReleaseOpengGLResources()
{
	releaseResources = true;
	//openGLLabel->release();
}

SceneCanvasComponent::~SceneCanvasComponent()
{
	ofs.close();
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
	redrawCanvasOutline = true;
	if (redrawCanvasOutline)
		computeCanvasOutline();
}




// - - - - - - - - OpenGL specific - - - - - - - - -
void SceneCanvasComponent::newOpenGLContextCreated()
{
    //DBG("SceneCanvasComponent : init OpenGL");
	redrawCanvasOutline = true;
	numFrame = 0;

	// Will init the counter
	//displayFrequencyMeasurer.OnNewFrame();

	shaderProgram = new OpenGLShaderProgram(openGlContext);
	shaderProgram->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(myVertexShader));
	shaderProgram->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(myFragmentShader));
	shaderProgram->link();

	shaderProgram->use();

	/// canvas outline buffer
	openGlContext.extensions.glGenBuffers(1, &canvasOutlineVertexBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineVertexBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat),
		g_canvasOutlineVertex_buffer_data, GL_STREAM_DRAW);

	openGlContext.extensions.glGenBuffers(1, &canvasOutlineCoulourBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineCoulourBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, 8 * 4 * sizeof(GLfloat), g_canvasOutlineCoulour_buffer_data, GL_STREAM_DRAW);

	openGlContext.extensions.glGenBuffers(1, &canvasOutlineIndexBuffer);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasOutlineIndexBuffer);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * 4 * sizeof(unsigned int), g_canvasOutlineIndex_buffer_data, GL_STREAM_DRAW);

	/// shapes buffer

	// TRIANGLE BUFFER
	// openGL génère 1 buffer, avec l'ID vertexBuffer, et qui contiendra g_vertex_buffer_data
	openGlContext.extensions.glGenBuffers(1, &vertexBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, *vertexBufferSize * sizeof(GLfloat)/*sizeof(g_vertex_buffer_data)*/,
		g_vertex_buffer_data, GL_STREAM_DRAW);

	// TRIANGLE COULEUR
	// pareil pour les buffers de couleurs des deux
	openGlContext.extensions.glGenBuffers(1, &colorBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, *colorBufferSize * sizeof(GLfloat), g_color_buffer_data, GL_STREAM_DRAW);

	// TRIANGLE INDEX
	openGlContext.extensions.glGenBuffers(1, &elementBuffer);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, *indicesSize * sizeof(unsigned int), indices, GL_STREAM_DRAW);

	///uniforms

	// déclaration des attributs et uniforms : !!! aux noms et leurs utilisations dans les shaders !!!
	position = new OpenGLShaderProgram::Attribute(*shaderProgram, String("position").toRawUTF8());
	colour = new OpenGLShaderProgram::Attribute(*shaderProgram, "colour");

	projectionMatrix.reset(new OpenGLShaderProgram::Uniform(*shaderProgram, "projectionMatrix"));
	viewMatrix.reset(new OpenGLShaderProgram::Uniform(*shaderProgram, "viewMatrix"));
	modelMatrix.reset(new OpenGLShaderProgram::Uniform(*shaderProgram, "modelMatrix"));


	/// calcul des matrices
	Matrix3D<float> testView = lookAt(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
	//Matrix3D<float> testProject = perspective((float)getWidth(), (float)getHeight(), 0.1f, 100.0f);
	if (projectionMatrix.get() != nullptr)
		projectionMatrix->setMatrix4(perspective((float)getWidth(), (float)getHeight(), 0.5f, 5.0f).mat, 1, false);

	if (viewMatrix.get() != nullptr)
		viewMatrix->setMatrix4(testView.mat, 1, false);

	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, (float)/*desktopScale **/ getHeight(), 0.0f, 1.0f);//10*getHeight()

	if (modelMatrix.get() != nullptr)
		modelMatrix->setMatrix4(model.mat, 1, false);

	computeCanvasOutline();
	openGLLabel->initialiseText(openGlContext);
	//shaderProgram->use(); // on utilise qu'un seul shader program pour le moment donc on appelle une seule fois cette fonction
}
void SceneCanvasComponent::renderOpenGL()
{
#if !defined(OPENGL_RENDERING) || (OPENGL_RENDERING == 0)

	auto manager = canvasManager.lock();

	const double desktopScale = openGlContext.getRenderingScale();
	std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGlContext,
		roundToInt(desktopScale * getWidth()),
		roundToInt(desktopScale * getHeight())));
	Graphics g(*glRenderer);

	g.addTransform(AffineTransform::scale((float)desktopScale));

	// Pure black background
	g.fillAll(Colours::black);

	// White interior contour 2px line to show when the canvas is active
	if (selectedForEditing)
	{
		g.setColour(Colours::white);
		g.drawRect(1, 1, getWidth() - 2, getHeight() - 2, 2);
	}


	// - - - - - THIRD Duplication of the drawable objects for thread-safe rendering, - - - - -
	// Lorsque nécessaire seulement !
	manager->LockAsyncDrawableObjects();


	bool areasCountChanged = (manager->GetAsyncDrawableObjects().size() != duplicatedAreas.size());
	// POUR L'INSTANT ALGO BÊTE
	// on resize le vecteur : la construction des shared n'est pas grave, leur bloc de contrôle reste
	// en mémoire finalement (on utilisera reset() )
	if (areasCountChanged)
	{
		canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
		duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
	}
	// Vérification simple de chaque aire 1 par 1
	size_t itIndex = 0;
	for (auto it = manager->GetAsyncDrawableObjects().begin();
		it != manager->GetAsyncDrawableObjects().end();
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
	for (size_t i = 0; i<duplicatedAreas.size(); i++)
	{
		// Peut mettre à jour des images et autres (si l'échelle a changé)
		duplicatedAreas[i]->SetRenderingScale(desktopScale);
		// Dessin effectif
		duplicatedAreas[i]->Paint(g);
	}

#else // !OPENGL_RENDERING || OPENGL_RENDERING == 0

	if (releaseResources)
	{
		openGLLabel->release();
		openGLLabel = nullptr;
		releaseResources = false;
	}
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
	
	//		/|\
	//	   / | \		IL FAUT METTRE LES UPDATES DE MATRICES APRES LE USE ET AVANT LE DRAW !!!!
	//	  /  |  \
	//	 /   O   \
	//	-----------
    
	projectionMatrix->setMatrix4(perspective((float)/*desktopScale **/ getWidth(), (float)/*desktopScale **/ getHeight(), 0.5f, 1.1f).mat, 1, false);

	
	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, (float)getHeight(), 0.0f, 1.0f);

	if (modelMatrix.get() != nullptr)
		modelMatrix->setMatrix4(model.mat, 1, false);
    
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
	const int duplicatedSize = (int)duplicatedAreas.size();
	for (size_t i=0;i<duplicatedSize;i++)
    {
		CreateShapeBuffer(duplicatedAreas[i], (int)i);
    }

	if (needToResetBufferParts)
	{
		std::fill(g_vertex_buffer_data + (int)duplicatedAreas.size() * *shapeVertexBufferSize, g_vertex_buffer_data + previousMaxSize * *shapeVertexBufferSize/**vertexBufferSize*/, 0.0f);
		std::fill(g_color_buffer_data + (int)duplicatedAreas.size() * *shapeColorBufferSize, g_color_buffer_data + previousMaxSize * *shapeColorBufferSize/**colorBufferSize*/, 0.0f);
		std::fill(indices + (int)duplicatedAreas.size() * *shapeIndicesSize, indices + previousMaxSize * *shapeIndicesSize/**indicesSize*/, 0);
	}


	DrawCanvasOutline();
	

	if (!(!areasCountChanged && duplicatedSize == 0))
	{
		DrawShapes();
	}

	int fps = displayFrequencyMeasurer.GetAverageFrequency_Hz();

	if (openGLLabel != nullptr)
	{
		Matrix3D<float> testView = lookAt(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
		Matrix3D<float> testProjecxtion = perspective((float)/*desktopScale **/ getWidth(), (float)/*desktopScale **/ getHeight(), 0.5f, 1.1f);
		openGLLabel->drawOneTexturedRectangle(openGlContext, model, testView, testProjecxtion, std::to_string(fps));
	}

#endif // !OPENGL_RENDERING || OPENGL_RENDERING == 0
    
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
	if (numFrame > 100 * 60)
	{
		//DBG("60 frames, underTime : "+ (String)underTime +" [underTime]" + (String)(EunderTime/600.0));
		//DBG("probleme underTime : " + (String)underTime);
		//ofs << String(EunderTime / 6000.0).toStdString() << std::endl;
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

void SceneCanvasComponent::DrawShapes()
{
	/// Draw triangle
	openGlContext.extensions.glEnableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, /**vertexBufferSize*/previousMaxSize * *shapeVertexBufferSize * sizeof(GLfloat)/*sizeof(g_vertex_buffer_data)*/, g_vertex_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);
	//openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);

	openGlContext.extensions.glEnableVertexAttribArray(colour->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, /**colorBufferSize*/previousMaxSize * *shapeColorBufferSize * sizeof(GLfloat), g_color_buffer_data);
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
}

void SceneCanvasComponent::DrawCanvasOutline()
{
	if (redrawCanvasOutline)
		computeCanvasOutline();

	openGlContext.extensions.glEnableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineVertexBuffer);
	if (redrawCanvasOutline)
		openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 3 * sizeof(GLfloat), g_canvasOutlineVertex_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);

	openGlContext.extensions.glEnableVertexAttribArray(colour->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineCoulourBuffer);
	if (redrawCanvasOutline)
		openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 4 * sizeof(GLfloat), g_canvasOutlineCoulour_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(colour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(float[4]), 0);

	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasOutlineIndexBuffer);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)0);

	openGlContext.extensions.glDisableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glDisableVertexAttribArray(colour->attributeID);

	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (redrawCanvasOutline)
		redrawCanvasOutline = false;
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

void SceneCanvasComponent::computeCanvasOutline()
{
	float outlineWidth = 5.0f; //px
	g_canvasOutlineVertex_buffer_data[0] = ((float)getWidth() /** (1.0f + 0.5f * std::sin(2.0f * M_PI * (1.0f/60.0f) * (float)numFrame - 50.0f * 60.0f))*/) - outlineWidth;
	g_canvasOutlineVertex_buffer_data[1] = (float)getHeight() - outlineWidth;
	g_canvasOutlineVertex_buffer_data[2] = 0.0f;

	g_canvasOutlineVertex_buffer_data[3] = outlineWidth;
	g_canvasOutlineVertex_buffer_data[4] = (float)getHeight() - outlineWidth;
	g_canvasOutlineVertex_buffer_data[5] = 0.0f;

	g_canvasOutlineVertex_buffer_data[6] = outlineWidth;
	g_canvasOutlineVertex_buffer_data[7] = outlineWidth;
	g_canvasOutlineVertex_buffer_data[8] = 0.0f;

	g_canvasOutlineVertex_buffer_data[9] = ((float)getWidth() /** (1.0f + 0.5f * std::sin(2.0f * M_PI * (1.0f / 60.0f) * (float)numFrame - 50.0f * 60.0f))*/) - outlineWidth;
	g_canvasOutlineVertex_buffer_data[10] = outlineWidth;
	g_canvasOutlineVertex_buffer_data[11] = 0.0f;

	g_canvasOutlineVertex_buffer_data[12] = (float)getWidth();//* (1.0f + 0.5f * std::sin(2.0f * M_PI * (1.0f / 60.0f) * (float)numFrame - 50.0f * 60.0f));
	g_canvasOutlineVertex_buffer_data[13] = (float)getHeight();
	g_canvasOutlineVertex_buffer_data[14] = 0.0f;

	g_canvasOutlineVertex_buffer_data[15] = 0.0f;
	g_canvasOutlineVertex_buffer_data[16] = (float)getHeight();
	g_canvasOutlineVertex_buffer_data[17] = 0.0f;

	g_canvasOutlineVertex_buffer_data[18] = 0.0f;
	g_canvasOutlineVertex_buffer_data[19] = 0.0f;
	g_canvasOutlineVertex_buffer_data[20] = 0.0f;

	g_canvasOutlineVertex_buffer_data[21] = (float)getWidth();// *(1.0f + 0.5f * std::sin(2.0f * M_PI * (1.0f / 60.0f) * (float)numFrame - 50.0f * 60.0f));
	g_canvasOutlineVertex_buffer_data[22] = 0.0f;
	g_canvasOutlineVertex_buffer_data[23] = 0.0f;

	for(int i = 0; i < 32; ++i)
		g_canvasOutlineCoulour_buffer_data[i] = 1.0f;
}


void SceneCanvasComponent::CreateShapeBuffer(std::shared_ptr<IDrawableArea> area, int positionInBuffer)
{
	int decalage = 3 * ((int)positionInBuffer * *numVertexShape);// + numPointsPolygon + 1;
	if (area->isVisible())
	{
		/// vertex
		try
		{
			const int verticesCount = 3 * area->GetVerticesBufferSize();
			GLfloat *destPtr = &g_vertex_buffer_data[decalage];
			float* vertexPtr = area->GetVerticesBufferPtr();
			for (int j = 0; j < verticesCount; ++j)
			{
				destPtr[j] = (*vertexPtr);//area->GetVerticesBufferElt(j);//*10
				++vertexPtr;
			}
		}
		catch (const std::out_of_range& e)
		{
			DBG(e.what());
		}

		/// indices
		decalage = positionInBuffer * *shapeIndicesSize;// différent du decalage pour les vertex et les couleurs !
		int beginShape = positionInBuffer * *numVertexShape;
		try
		{
			const int indicesCount = area->GetIndicesBufferSize();
			unsigned int *destIndicesPtr = &indices[decalage];
			unsigned int* indicesPtr = area->GetIndicesBufferPtr();
			for (int i = 0; i < indicesCount; ++i)
			{
				destIndicesPtr[i] = (*indicesPtr) + beginShape;
				++indicesPtr;
			}
		}
		catch (const std::out_of_range& e)
		{
			DBG(e.what());
		}

		/// colors
		decalage = 4 * ((int)positionInBuffer * *numVertexShape);
		try
		{
			const int couloursCount = area->GetCouloursBufferSize();
			float* couloursPtr = area->GetCoulourBufferPtr();
			float* destCouloursPtr = &g_color_buffer_data[decalage];
			for (int i = 0; i < couloursCount; ++i)
			{
				destCouloursPtr[i] = (*couloursPtr);
				++couloursPtr;
			}
		}
		catch (const std::out_of_range& e)
		{
			DBG(e.what());
		}
	}
	else
	{
		const int verticesCount = 3 * area->GetVerticesBufferSize();
		GLfloat *destPtr = &g_vertex_buffer_data[decalage];
		for (int j = 0; j < verticesCount; ++j)
			destPtr[j] = 0.0f;

		decalage = positionInBuffer * *shapeIndicesSize;// différent du decalage pour les vertex et les couleurs !

		const int indicesCount = area->GetIndicesBufferSize();
		unsigned int *destIndicesPtr = &indices[decalage];
		for (int i = 0; i < indicesCount; ++i)
			destIndicesPtr[i] = 0;

		decalage = 4 * ((int)positionInBuffer * *numVertexShape);
		const int couloursCount = area->GetCouloursBufferSize();
		float* destCouloursPtr = &g_color_buffer_data[decalage];
		for (int i = 0; i < couloursCount; ++i)
			destCouloursPtr[i] = 0.0f;
		
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