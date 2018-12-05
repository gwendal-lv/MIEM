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

#include "TextUtils.h"

#include "EditablePolygon.h"

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
	releaseDone = false;
	EunderTime = 0.0;
	previousMaxSize = 0;
	needToResetBufferParts = false;

	Nshapes = numShapesMax;
    
    // création d'une aire par défaut pour connaître ses tailles de buffer
    // ATTENTION ça ne corrige pas tout : la taille max est référencée par rapport à une EDITABLE AREA
    auto dummyArea = std::make_shared<EditablePolygon>(0); // IUD = 0

    numVertexShape = dummyArea->GetVerticesBufferElementsCount();
    shapeVertexBufferSize = dummyArea->GetVerticesBufferSize();
    shapeColorBufferSize = dummyArea->GetVerticesBufferSize();
    shapeIndicesSize = dummyArea->GetIndicesBufferSize();

	vertexBufferSize = Nshapes * shapeVertexBufferSize;
	colorBufferSize = Nshapes * shapeColorBufferSize;
	indicesSize = Nshapes * shapeIndicesSize;

    deleteBuffers();
	sceneVertexBufferData = new GLfloat[vertexBufferSize];
	sceneColourBufferData = new GLfloat[colorBufferSize];
	sceneIndicesBufferData = new GLuint[indicesSize];

    // code qui a l'air de servir à remplir les couleurs non-utiles à des valeurs par défaut....
    /*
	for (int i = 0; i < Nshapes; ++i)
	{
		for (int j = i * shapeColorBufferSize; j < i* shapeColorBufferSize + 4 * numVerticesPolygon; j += 4)
		{
			g_color_buffer_data[j] = 0.5f;
			g_color_buffer_data[j + 1] = 0.5f;
			g_color_buffer_data[j + 2] = 0.5f;
			g_color_buffer_data[j + 3] = 1.0f;
		}
		for (int j = i * shapeColorBufferSize + 4 * numVerticesPolygon; j < (i + 1)*shapeColorBufferSize; j += 4)
		{
			g_color_buffer_data[j] = 1.0f;
			g_color_buffer_data[j + 1] = 0.0f;
			g_color_buffer_data[j + 2] = 0.0f;
			g_color_buffer_data[j + 3] = 1.0f;
		}
	}
     */

    // Initialisation du IBO interne propre à la scène
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
	openGLLabel = nullptr;
	openGlContext.setComponentPaintingEnabled(true); // default behavior, lower perfs
													 // OpenGL final initialization will happen in the COmpleteInitialization method
}

void SceneCanvasComponent::ReleaseOpengGLResources()
{
	rendering_mutex.lock();
	releaseResources = true;
	/*openGLLabel->release();*/
	shaderProgram = nullptr;
	projectionMatrix = nullptr;
	viewMatrix = nullptr;
	modelMatrix = nullptr;
	positionShaderAttribute = nullptr;
	colourShaderAttribute = nullptr;
	rendering_mutex.unlock();
	//openGLDestructionThread = std::thread(&SceneCanvasComponent::openGLDestructionAfterLastFrame, this);

		
}

void SceneCanvasComponent::waitForOpenGLResourcesRealeased()
{
#ifdef __MIEM_VBO
	while (!releaseDone) {}
	while (!openGLDestructionThread.joinable()) {}
	openGLDestructionThread.join();
	releaseDone = false;
#else
    DBG("[VIEW] This function should not be called in non-VBO mode..."); // à vérifier quand même...
    assert(false);
#endif
}

SceneCanvasComponent::~SceneCanvasComponent()
{
    openGlContext.detach();
    
    deleteBuffers();

    DBG("SceneCanvasComponent : destruction arrive à son terme");
}

void SceneCanvasComponent::deleteBuffers()
{
    if (sceneVertexBufferData != nullptr)
    {
        delete[] sceneVertexBufferData;
        sceneVertexBufferData = nullptr;
    }
    if (sceneColourBufferData != nullptr)
    {
        delete[] sceneColourBufferData;
        sceneColourBufferData = nullptr;
    }
    if (sceneIndicesBufferData != nullptr)
    {
        delete[] sceneIndicesBufferData;
        sceneIndicesBufferData = nullptr;
    }
}

void SceneCanvasComponent::CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager)
{
    canvasManager = _canvasManager;
    
    // OpenGL final initialization here
    openGlContext.setRenderer(this);
    
    SetupGLResources();
}







// - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -

void SceneCanvasComponent::resized()
{
    // Actualization of all areas graphical objets, if Presenter is accessible (not at first time)
    if (auto manager = canvasManager.lock())
    {
        manager->OnResized();
    }
    
#ifdef __MIEM_VBO
    redrawCanvasOutline = true;
    if (redrawCanvasOutline)
        computeCanvasOutline();
#endif
}




// - - - - - - - - OpenGL specific - - - - - - - - -

void SceneCanvasComponent::SetupGLResources()
{
    openGlContext.attachTo(*this);
    openGlContext.setContinuousRepainting(true);
    isSwapSynced = openGlContext.setSwapInterval(swapInterval);
    if (isSwapSynced)
        DBG("Context attached (supports synced OpenGL swaps)");
    else
        DBG("Context attached (does not support synced OpenGL swaps)");
    setVisible(true);
}

void SceneCanvasComponent::ReleaseGLResources()
{
    if (! isVisible())
        return;
    
    DBG("Relâchement ressource GL d'un Canvas Component...");
    
    
    openGlContext.executeOnGLThread(
                                    [this] (OpenGLContext& openGlContext)
                                    {
                                        DBG("Arrêt du *continuous repainting* du contexte GL");
                                        openGlContext.setContinuousRepainting(false); DBG("OK");
                                    }                               ,
                                    true); // will block until finished
    
    //DBG("Détachement contexte");
    //openGlContext.detach(); DBG("OK");
    DBG("auto-invisibilité....");
    setVisible(false); DBG("OK");
}






void SceneCanvasComponent::newOpenGLContextCreated()
{
    //DBG("SceneCanvasComponent : init OpenGL");
	redrawCanvasOutline = true;
	numFrame = 0;

	// Will init the counter
	//displayFrequencyMeasurer.OnNewFrame();

	//if (releaseResources)
	//{
	//	shaderProgram.release();
	//	shaderProgram = nullptr;
	//	openGLLabel->release();
	//	openGLLabel = nullptr;
	//}

#ifdef __MIEM_VBO
    DBG("[MIEM OpenGL] évènement 'New Context Created'. Création des shaders, buffers, etc.");
    
	shaderProgram = std::make_unique<OpenGLShaderProgram>(openGlContext);
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
	openGlContext.extensions.glGenBuffers(1, &vertexBufferGlName);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGlName);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * sizeof(GLfloat)/*sizeof(g_vertex_buffer_data)*/,
		sceneVertexBufferData, GL_STREAM_DRAW);

	// TRIANGLE COULEUR
	// pareil pour les buffers de couleurs des deux
	openGlContext.extensions.glGenBuffers(1, &colorBufferGlName);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBufferGlName);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, colorBufferSize * sizeof(GLfloat), sceneColourBufferData, GL_STREAM_DRAW);

	// TRIANGLE INDEX
	openGlContext.extensions.glGenBuffers(1, &elementBufferGlName);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferGlName);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), sceneIndicesBufferData, GL_STREAM_DRAW);

	///uniforms

	// déclaration des attributs et uniforms : !!! aux noms et leurs utilisations dans les shaders !!!
	positionShaderAttribute = std::make_unique<OpenGLShaderProgram::Attribute>(*shaderProgram.get(), String("position").toRawUTF8());
	colourShaderAttribute = std::make_unique<OpenGLShaderProgram::Attribute>(*shaderProgram.get(), "colour");

	projectionMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "projectionMatrix");
	viewMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "viewMatrix");
	modelMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "modelMatrix");


	/// calcul des matrices
	Matrix3D<float> testView = lookAt(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
	//Matrix3D<float> testProject = perspective((float)getWidth(), (float)getHeight(), 0.1f, 100.0f);
	if (projectionMatrix != nullptr)
		projectionMatrix->setMatrix4(perspective((float)getWidth(), (float)getHeight(), 0.5f, 5.0f).mat, 1, false);

	if (viewMatrix != nullptr)
		viewMatrix->setMatrix4(testView.mat, 1, false);

	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, (float)/*desktopScale **/ getHeight(), 0.0f, 1.0f);//10*getHeight()

	if (modelMatrix != nullptr)
		modelMatrix->setMatrix4(model.mat, 1, false);

	computeCanvasOutline();

	if(openGLLabel == nullptr)
		openGLLabel = std::make_unique<OpenGLTextObject>("newFontImg_png"/*"C:\\Users\\ayup1\\PycharmProjects\\fontPNG\\newFontImg.png"*/, 100.0f, 300.0f, 100.0f, -100.0f, 12); // "ExportedFont2_png", "C:\\Users\\ayup1\\Downloads\\newFontImg.png"
	openGLLabel->initialiseText(openGlContext);

	

#endif // __MIEM_VBO
	//shaderProgram->use(); // on utilise qu'un seul shader program pour le moment donc on appelle une seule fois cette fonction
}

void SceneCanvasComponent::renderOpenGL()
{
	auto manager = canvasManager.lock();
    const double desktopScale = openGlContext.getRenderingScale();
    
    
    
    
    
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
            previousMaxSize = (int)duplicatedAreas.size(); // VBOs only
            needToResetBufferParts = true; // VBOs only
        }
        else
        {
            previousMaxSize = (int)manager->GetAsyncDrawableObjects().size(); // VBOs only
            needToResetBufferParts = false; // VBOs only
        }
        canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
        duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
    }
    else
    {
        previousMaxSize = (int)duplicatedAreas.size(); // VBOs only
        needToResetBufferParts = false; // VBOs only
    }
    // Vérification simple de chaque aire 1 par 1
    // ******************************************************
    // OPTIMISATION POSSIBLE POUR LES VBO, POUR LIMITER LA QUANTITE DE COPIES EFFECTUEES
    // ******************************************************
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
    

    
    
    
    
    
    // =========================== Rendu classique CPU++ sans VBO ==========================
#ifndef __MIEM_VBO
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

	// - - - - - Areas painting (including exciters if existing) - - - - -
	// Sans bloquer, du coup, les autres threads (pour réactivité max...)
	for (size_t i = 0; i<duplicatedAreas.size(); i++)
	{
		// Peut mettre à jour des images et autres (si l'échelle a changé)
		duplicatedAreas[i]->SetRenderingScale(desktopScale);
		// Dessin effectif
		if(duplicatedAreas[i]->isVisible())
			duplicatedAreas[i]->Paint(g);
	}

    
    
    
    // =========================== Rendu classique GPU++ avec VBO ==========================
#else // if IS def __MIEM_VBO
	if (releaseResources)
	{
        DBG("[MIEM OpenGL] 'Release resources' débute dans le thread OpenGL");
		openGLDestructionAtLastFrame();

		releaseDone = false;
		releaseResources = false;
		openGLDestructionThread = std::thread(&SceneCanvasComponent::openGLDestructionAfterLastFrame, this);
	}
	else
	{
		rendering_mutex.lock();
		++numFrame;

		OpenGLHelpers::clear(Colours::black);

		glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // peut-être à vérifier/modifier ???????????
		//glBlendFunc(GL_ONE, GL_ZERO);
		openGlContext.extensions.glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

        // *** Fonction principale de DESSIN DE FRAME OPEN GL PAR VBOs ***
		DrawOnSceneCanevas(manager);
        // ***    ***
        
        // Affichage des FPS
		int fps = (int)displayFrequencyMeasurer.GetAverageFrequency_Hz();
		if (openGLLabel != nullptr)
		{
			Matrix3D<float> testModel(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, (float)getHeight(), 0.0f, 1.0f);
			Matrix3D<float> testView = lookAt(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
			Matrix3D<float> testProjecxtion = perspective((float)/*desktopScale **/ getWidth(), (float)/*desktopScale **/ getHeight(), 0.5f, 1.1f);

			std::u16string testFPS[]{ u"" };
			Miam::TextUtils::intToU16string(fps, testFPS);

			openGLLabel->drawOneTexturedRectangle(openGlContext, testModel, testView, testProjecxtion, testFPS/*std::to_string(fps)*/);
		}
        
        // Fin du rendu...
		rendering_mutex.unlock();
    }
#endif // ndef __MIEM_VBO

		// Call to a general Graphic update on the whole Presenter module
		if (!manager->isUpdatePending())
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
		double underTime = lastDuration > 0.0 ? desiredPeriod_ms - lastDuration : 0.0;
#else
		double underTime = desiredPeriod_ms - displayFrequencyMeasurer.GetLastDuration_ms();
#endif


		double last = displayFrequencyMeasurer.GetLastDuration_ms();
#ifdef __MIAM_DEBUG
		if (last < 0.0)
        {
			DBG("probleme lastDuration: " + (String)last);
            assert(false);
        }
#endif
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

void SceneCanvasComponent::openGLDestructionAtLastFrame()
{
    DBG("[MIEM OpenGL] Destruction sur 'last frame'");
	openGLLabel->release();
	std::u16string dummyString[]{ u"" };
	Matrix3D<float> dummyMatrix(Vector3D<float>(0.0f, 0.0f, 0.0f));
	openGLLabel->drawOneTexturedRectangle(openGlContext, dummyMatrix, dummyMatrix, dummyMatrix, dummyString/*std::to_string(fps)*/);
	openGLLabel->waitForOpenGLResourcesRealeased();
	openGLLabel = nullptr;
}

void SceneCanvasComponent::DrawOnSceneCanevas(std::shared_ptr<Miam::MultiSceneCanvasInteractor> &manager)
{
	if (shaderProgram != nullptr)
		shaderProgram->use();

	//		/|\
	//	   / | \		IL FAUT METTRE LES UPDATES DE MATRICES APRES LE USE ET AVANT LE DRAW !!!!
	//	  /  |  \
	//	 /   O   \
	//	-----------

	if (projectionMatrix != nullptr)
		projectionMatrix->setMatrix4(perspective((float)/*desktopScale **/ getWidth(), (float)/*desktopScale **/ getHeight(), 0.5f, 1.1f).mat, 1, false);


	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, (float)getHeight(), 0.0f, 1.0f);

	if (modelMatrix != nullptr)
	{
		modelMatrix->setMatrix4(model.mat, 1, false);
	}


	// - - - - - Areas painting (including exciters if existing) - - - - -
	// Sans bloquer, du coup, les autres threads (pour réactivité max...)
    currentVertexBufferArrayPos = 0;
    currentColourBufferArrayPos = 0;
    currentIndexBufferArrayPos = 0;
	for (size_t i = 0; i < duplicatedAreas.size(); i++)
	{
        // Re-création de tous les buffers à chaque frame
        // --------- OPTIMISATION POSSIBLE SI UN JOOOOUUUUURRRRRRR çA MARCHE ------------
		AddShapeToBuffers(duplicatedAreas[i]);
	}

    // Pour l'instant on oublie ça... On remplit juste tout le buffer jusqu'au bout
	if (/*needToResetBufferParts*/false)
	{
		std::fill(sceneVertexBufferData + (int)duplicatedAreas.size() * shapeVertexBufferSize, sceneVertexBufferData + previousMaxSize * shapeVertexBufferSize, 0.0f);
		std::fill(sceneColourBufferData + (int)duplicatedAreas.size() * shapeColorBufferSize, sceneColourBufferData + previousMaxSize * shapeColorBufferSize, 0.0f);
		std::fill(sceneIndicesBufferData + (int)duplicatedAreas.size() * shapeIndicesSize, sceneIndicesBufferData + previousMaxSize * shapeIndicesSize, 0);
	}
    // Remplissage du reste du buffer d'indices par des zéros


    // ** SUPPRIME POUR L"INSTANT **
	//DrawCanvasOutline();

	DrawShapes();
}

void SceneCanvasComponent::DrawShapes()
{
#ifndef __MIEM_VBO
    throw std::logic_error("Cannot manipulate VBOs in non-VBO mode");
#endif
    
	if (positionShaderAttribute != nullptr && colourShaderAttribute != nullptr)
	{
        std::cout << "[Draw GL shapes] " << currentVertexBufferArrayPos << " vertex GLfloat ; " << currentColourBufferArrayPos << " colour GLfloat ; " << currentIndexBufferArrayPos << " index GLuint" << std::endl;
        
        // VERTICES array
		openGlContext.extensions.glEnableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGlName);
		openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0,
                                                 currentVertexBufferArrayPos * sizeof(GLfloat),
                                                 sceneVertexBufferData);
		openGlContext.extensions.glVertexAttribPointer(positionShaderAttribute->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0); // ??? Commentaires explicatifs ???
		//openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);

        // COLOURS array
		openGlContext.extensions.glEnableVertexAttribArray(colourShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBufferGlName);
		openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0,
                                                 currentColourBufferArrayPos * sizeof(GLfloat),
                                                 sceneColourBufferData);
		openGlContext.extensions.glVertexAttribPointer(colourShaderAttribute->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(float[4]), 0);

        // INDICES (ELEMENT) array
		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferGlName);
		openGlContext.extensions.glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                                                 currentIndexBufferArrayPos * sizeof(GLuint),
                                                 sceneIndicesBufferData);

		glDrawElements(GL_TRIANGLES, (GLsizei) currentIndexBufferArrayPos, GL_UNSIGNED_INT, (void*)0);
        //glDrawElements(GL_TRIANGLES, (GLsizei) 192, GL_UNSIGNED_INT, (void*)0);
        // juste le premier ring ... qq'un écrit dans le VBO du 1ier ring...
        // *********************
        
		openGlContext.extensions.glDisableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glDisableVertexAttribArray(colourShaderAttribute->attributeID);

		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void SceneCanvasComponent::DrawCanvasOutline()
{
#ifndef __MIEM_VBO
    throw std::logic_error("Cannot manipulate VBOs in non-VBO mode");
#endif
    
	if (positionShaderAttribute != nullptr && colourShaderAttribute != nullptr)
	{
		if (redrawCanvasOutline)
        {
			computeCanvasOutline();
            redrawCanvasOutline = false;
        }

		openGlContext.extensions.glEnableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineVertexBuffer);
		if (redrawCanvasOutline)
			openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 3 * sizeof(GLfloat), g_canvasOutlineVertex_buffer_data);
		openGlContext.extensions.glVertexAttribPointer(positionShaderAttribute->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);

		openGlContext.extensions.glEnableVertexAttribArray(colourShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineCoulourBuffer);
		if (redrawCanvasOutline)
			openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 4 * sizeof(GLfloat), g_canvasOutlineCoulour_buffer_data);
		openGlContext.extensions.glVertexAttribPointer(colourShaderAttribute->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(float[4]), 0);

		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasOutlineIndexBuffer);

		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)0);

		openGlContext.extensions.glDisableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glDisableVertexAttribArray(colourShaderAttribute->attributeID);

		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


void SceneCanvasComponent::openGLContextClosing()
{
    // Méthode même pas appelée dans Android... D'après la doc Juce au 30 octobre 2017
    DBG("SceneCanvasComponent : closing OpenGL Context");
	/*shaderProgram.release();
	openGLLabel->release();
	openGLLabel = nullptr;*/
	
	//shaderProgram = nullptr;
	//openGLLabel->release();
	//delete openGLLabel;
	//openGLLabel = nullptr;
	
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
#ifndef __MIEM_VBO
    throw std::logic_error("Cannot manipulate VBOs in non-VBO mode");
#endif
    
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


void SceneCanvasComponent::AddShapeToBuffers(std::shared_ptr<IDrawableArea> area)
{
#ifndef __MIEM_VBO
    throw std::logic_error("Cannot manipulate VBOs in non-VBO mode");
#endif
    
	if (area->isVisible())
	{
		/// vertices
        const size_t shapeVertexBufferOffset = currentVertexBufferArrayPos;
        for (size_t i=0 ; i<area->getVerticesBuffer().size() ; i++)
        {
            // **************************************************************
            // REMPLACER EN RELEASE PAR UNE COPIE STL ULTRA OPTIMISEE PAR VECTEURS
            // **************************************************************
            assert(currentVertexBufferArrayPos < vertexBufferSize);
            // dans area : MIEM vector qui checke en debug les indices
            sceneVertexBufferData[currentVertexBufferArrayPos] = area->getVerticesBuffer()[i];
            ++currentVertexBufferArrayPos;
        }
        // colours
        for (size_t i=0 ; i<area->getColoursBuffer().size() ; i++)
        {
            // **************************************************************
            // REMPLACER EN RELEASE PAR UNE COPIE STL ULTRA OPTIMISEE PAR VECTEURS
            // **************************************************************
            assert(currentColourBufferArrayPos < colorBufferSize);
            sceneColourBufferData[currentColourBufferArrayPos] = area->getColoursBuffer()[i];
            ++currentColourBufferArrayPos;
        }
		/// indices
        assert((shapeVertexBufferOffset % 3) == 0); // sinon, on a mis des points non terminés....
        const GLuint shapeVertexBufferElmtOffset = (GLuint) shapeVertexBufferOffset / 3;
        for (size_t i=0 ; i<area->getIndicesBuffer().size() ; i++)
        {
            // !!!!! les indices doivent être décalés del'offset de vertex buffer pour cette forme !!
            assert(currentIndexBufferArrayPos < indicesSize);
            sceneIndicesBufferData[currentIndexBufferArrayPos] = area->getIndicesBuffer()[i] + shapeVertexBufferElmtOffset;
            ++currentIndexBufferArrayPos;
        }
	}
}

void SceneCanvasComponent::openGLDestructionAfterLastFrame()
{
	// à appeler tant que le context est encore actif afin de pouvoir supprimer correctement les ressources OpenGL
	
	shaderProgram = nullptr;
	viewMatrix = nullptr;
	projectionMatrix = nullptr;
	modelMatrix = nullptr;
	positionShaderAttribute = nullptr;
	colourShaderAttribute = nullptr;
	releaseDone = true;
}


