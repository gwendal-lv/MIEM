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

void SceneCanvasComponent::init(int numShapesMax, int /*numPointsMax*/)
{
    releaseResources = false;
	releaseDone = false; // ?? par encore init les resources....
    
	EunderTime = 0.0;

	Nshapes = numShapesMax;
    
    // création d'une aire par défaut pour connaître ses tailles de buffer
    // ATTENTION ça ne corrige pas tout : la taille max est référencée par rapport à une EDITABLE AREA
    auto dummyArea = std::make_shared<EditablePolygon>(0); // UID = 0

    numVertexShape = dummyArea->GetVerticesBufferElementsCount();
    shapeVertexBufferSize = dummyArea->GetVerticesBufferSize();
    shapeColorBufferSize = dummyArea->GetVerticesBufferSize();
    shapeIndicesSize = dummyArea->GetIndicesBufferSize();
    
    sceneVertexBufferData.resize(Nshapes * shapeVertexBufferSize);
    sceneColourBufferData.resize(Nshapes * shapeColorBufferSize);
	sceneIndicesBufferData.resize(Nshapes * shapeIndicesSize);
    std::string debugString = "Resize (possible reallocation) of scene's internal buffers. vertex:";
    debugString += std::to_string(sceneVertexBufferData.size()); debugString += "bytes ; index: ";
    debugString += std::to_string(sceneIndicesBufferData.size()); debugString += "bytes ; colour: ";
    debugString += std::to_string(sceneColourBufferData.size()); debugString += "bytes.\n";
    DBG(debugString);

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
    
    // Grooooosse optimisation (avant d'attacher le contexte)
#ifdef __MIEM_VBO
    openGlContext.setComponentPaintingEnabled(false);
#else
    openGlContext.setComponentPaintingEnabled(true); // default behavior, lower perfs
    // OpenGL final initialization will happen in the COmpleteInitialization method
#endif
}

void SceneCanvasComponent::TriggerOpengGLResourcesRelease()
{
#ifdef __MIAM_DEBUG
    postReleaseRenderRequestsCount = 0;
#endif
	releaseResources = true;
}

void SceneCanvasComponent::waitForOpenGLResourcesReleased()
{
#ifdef __MIEM_VBO
    DBG("[UI Thread][SceneCanvasComponent] Waiting for OpenGL resources to be released...");
    
    // MANIERE DEGUEULASSE D'ATTEEEEENDRE
    // --------- >>>>>>> VARIABLE CONDITIONNELLE STD plutôt
    // --------- >>>>>>> VARIABLE CONDITIONNELLE STD plutôt
    // --------- >>>>>>> VARIABLE CONDITIONNELLE STD plutôt
    // --------- >>>>>>> VARIABLE CONDITIONNELLE STD plutôt
    // --------- >>>>>>> VARIABLE CONDITIONNELLE STD plutôt
    // https://fr.cppreference.com/w/cpp/thread/condition_variable
    // pas posix....
	while (!releaseDone)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
#else
    DBG("[VIEW] This function should not be called in non-VBO mode..."); // à vérifier quand même...
    assert(false);
#endif
}

SceneCanvasComponent::~SceneCanvasComponent()
{
    openGlContext.detach();
    DBG("[SceneCanvasComponent] GL Content detached.");
}

void SceneCanvasComponent::CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager)
{
    canvasManager = _canvasManager;
    
    // OpenGL final initialization here
    openGlContext.setRenderer(this);
    
    SetupGLContext();
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

void SceneCanvasComponent::SetupGLContext()
{
    openGlContext.attachTo(*this);

    openGlContext.setContinuousRepainting(true);
    setVisible(true);
    
}

void SceneCanvasComponent::ReleaseGLResources_NoVBO()
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
	// Must be called after context is created (not just attached)
	/*
	isSwapSynced = openGlContext.setSwapInterval(swapInterval);
	if (isSwapSynced)
		DBG("[MIEM OpenGL] évènement 'New Context Created'. (supports synced OpenGL swaps)");
	else
		DBG("[MIEM OpenGL] évènement 'New Context Created'. (does not support synced OpenGL swaps)");
	*/
	openGlContext.setSwapInterval(0);
	DBG("[MIEM OpenGL] évènement 'New Context Created', swap sync disabled.");

	redrawCanvasOutline = true;
	numFrame = 0;

	// Will init the counter
	displayFrequencyMeasurer.OnNewFrame();


#ifdef __MIEM_VBO
    DBG("Création des shaders, buffers, etc.");
    releaseDone = false;
    
	shaderProgram = std::make_unique<OpenGLShaderProgram>(openGlContext);
	shaderProgram->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(myVertexShader));
	shaderProgram->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(myFragmentShader));
	shaderProgram->link();

	shaderProgram->use();

	/// - - - - - canvas outline buffers - - - - -
	openGlContext.extensions.glGenBuffers(1, &canvasOutlineVertexBufferName);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineVertexBufferName);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat),
                                          g_canvasOutlineVertex_buffer_data,
                                          GL_DYNAMIC_DRAW); // modified often, used many times

	openGlContext.extensions.glGenBuffers(1, &canvasOutlineCoulourBufferName);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineCoulourBufferName);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, 8 * 4 * sizeof(GLfloat),
                                          g_canvasOutlineCoulour_buffer_data,
                                          GL_DYNAMIC_DRAW);

	openGlContext.extensions.glGenBuffers(1, &canvasOutlineIndexBufferName);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasOutlineIndexBufferName);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * 4 * sizeof(GLuint),
                                          g_canvasOutlineIndex_buffer_data,
                                          GL_DYNAMIC_DRAW);

	/// - - - - - shapes buffers - - - - -

	// TRIANGLE BUFFER
	openGlContext.extensions.glGenBuffers(1, &vertexBufferGlName); // génération du nom du buffer (pas de l'object càd le VBO)
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGlName); // lie le nom à un VBO
    // Réservation de la mémoire du VBO :
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER,
                                          sceneVertexBufferData.size() * sizeof(GLfloat),
                                          sceneVertexBufferData.data(),
                                          GL_DYNAMIC_DRAW); // data modified by application (DRAW) many times and used often (DYNAMIC)
    // ??? pourquoi ne pas utiliser named buffer data ???? on laisse comme ça marchait +/- à un moment donné...
    DBG("Vertex buffer object allocated. Size in GLfloat:" + std::to_string(sceneVertexBufferData.size()));

	// TRIANGLE COULEUR
	// pareil pour les buffers de couleurs des deux
	openGlContext.extensions.glGenBuffers(1, &colorBufferGlName);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBufferGlName);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER,
                                          sceneColourBufferData.size() * sizeof(GLfloat),
                                          sceneColourBufferData.data(),
                                          GL_DYNAMIC_DRAW);
    DBG("Vertex colour buffer object allocated. Size in GLfloat:" + std::to_string(sceneColourBufferData.size()));


	// TRIANGLE INDEX
	openGlContext.extensions.glGenBuffers(1, &elementBufferGlName);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferGlName);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                          sceneIndicesBufferData.size() * sizeof(GLuint),
                                          sceneIndicesBufferData.data(),
                                          GL_DYNAMIC_DRAW);
    DBG("Index buffer object allocated. Size in GLuint:" + std::to_string(sceneColourBufferData.size()));

	///uniforms

	// déclaration des attributs et uniforms : !!! aux noms et leurs utilisations dans les shaders !!!
	positionShaderAttribute = std::make_unique<OpenGLShaderProgram::Attribute>(*shaderProgram.get(), String("position").toRawUTF8());
	colourShaderAttribute = std::make_unique<OpenGLShaderProgram::Attribute>(*shaderProgram.get(), "colour");

	projectionMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "projectionMatrix");
	viewMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "viewMatrix");
	modelMatrix = std::make_unique<OpenGLShaderProgram::Uniform>(*shaderProgram.get(), "modelMatrix");


	/// calcul des matrices
	Matrix3D<float> testView = Math::GenerateLookAtMatrix(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
	//Matrix3D<float> testProject = perspective((float)getWidth(), (float)getHeight(), 0.1f, 100.0f);
	if (projectionMatrix != nullptr)
        projectionMatrix->setMatrix4(Math::GenerateOrthoPerspectiveMatrix((float)getWidth(),
                                                                          (float)getHeight(),
                                                                          cameraNearZ, // near
                                                                          cameraFarZ).mat, // far
                                     1, false);

	if (viewMatrix != nullptr)
		viewMatrix->setMatrix4(testView.mat, 1, false);

	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, (float)/*desktopScale **/ getHeight(), 0.0f, 1.0f);//10*getHeight()

	if (modelMatrix != nullptr)
		modelMatrix->setMatrix4(model.mat, 1, false);

	computeCanvasOutline();

    
    // = = = = = loading of shared data for all text objects = = = = =
    InitGLFontResources(openGlContext); // from OpenGLFontManager mother class
    
    // - - - - label de display des fps - - - -
    if(openGLInfoLabel == nullptr)
        openGLInfoLabel = std::make_unique<OpenGLTextObject>(20.0f, 90.0f, 20.0f, +35.0f, 12);
    std::u16string texteInfo = u"Ho, huitre !" ;
    openGLInfoLabel->SetText(texteInfo);
    // init du text selon le contexte
    openGLInfoLabel->Initialise(openGlContext, this);
	

#endif // __MIEM_VBO
	//shaderProgram->use(); // on utilise qu'un seul shader program pour le moment donc on appelle une seule fois cette fonction
}

void SceneCanvasComponent::renderOpenGL()
{
	auto manager = canvasManager.lock();
    const double desktopScale = openGlContext.getRenderingScale();
    
    
    
    
    
    // - - - - - THIRD Duplication of the drawable objects for thread-safe rendering, - - - - -
    // Pour garder trace des nouvelles aires pour OpenGL (allocations de ressources)
    // et des anciennes pour lesquelles il faut libérer des ressources
    std::vector<std::shared_ptr<IDrawableArea>> areasToGlInit;
    areasToGlInit.reserve(100); // arbitraire...
    std::vector<std::shared_ptr<IDrawableArea>> areasToGlRelease;
    areasToGlRelease.reserve(100);
    
    // Lock le + tard possible, lorsque nécessaire seulement !
    manager->LockAsyncDrawableObjects();
    
    long areasCountDifference = (long)(manager->GetAsyncDrawableObjects().size())
								- (long)(duplicatedAreas.size());
    // Pour l'instant algo assez bête....
    // Si décalage d'indice : tout sera à refaire.... mais bon c'est + simple.
    if (areasCountDifference != 0)
    {
        // If areas are deleted : we store this information
        if (areasCountDifference < 0)
        {
            for (size_t i = manager->GetAsyncDrawableObjects().size();
                 i < duplicatedAreas.size();
                 i++)
            {
                areasToGlRelease.push_back(duplicatedAreas[i]);
            }
        }
        
        // Actual resize. Warning for later : some pointers might point to NULL
        canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
        duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
    }
    // Vérification simple de chaque aire 1 par 1 après resize.
    // ******************************************************
    // OPTIMISATION POSSIBLE POUR LES VBO, POUR LIMITER LA QUANTITE DE COPIES EFFECTUEES
    // Même si cette optimisation est pour l'instant négligeable par rapport
    // au nombre d'allocations mémoire nécessaires pour dupliquer les aires (et les VBO internes...)
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
            // Sauvegarde aire à release (might be nullptr, not initialized by Clone() yet)
            if (duplicatedAreas[itIndex])
                areasToGlRelease.push_back(duplicatedAreas[itIndex]);
            // Changement pour de vrai
            canvasAreasPointersCopies[itIndex] = (*it);
            duplicatedAreas[itIndex] = (*it)->Clone();
            // Sauvegarde aire à init
            areasToGlInit.push_back(duplicatedAreas[itIndex]);
        }
        // Double compteur
        itIndex++;
    }
    
    manager->UnlockAsyncDrawableObjects();
    

    // DEBUG TEMP
    //std::cout << areasToGlInit.size() << "init ; " << areasToGlRelease.size() << "release" <<std::endl;
    
    
    
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

    
    
    
    // =========================== Rendu GPU++ avec VBO ==========================
#else // if IS def __MIEM_VBO
    
    // Management of new/old areas
    for (auto area : areasToGlRelease)
    {
        if (area->IsNameVisible())
            area->GetGLTextObject()->releaseResourcesSync();
    }
    for (auto area : areasToGlInit)
    {
        if (area->IsNameVisible())
            area->GetGLTextObject()->Initialise(openGlContext, this);
    }
    
	if (releaseResources) // dernière frame, qui effectue le release
	{
        DBG("[MIEM OpenGL] 'Release resources' débute dans le thread OpenGL (dernière frame avec destruction)");
        postReleaseRenderRequestsCount = 0;
		openGLDestructionAtLastFrame();

		releaseDone = true;
		releaseResources = false;
	}
    else if (releaseDone) // on ne va pas faire le rendu sans les ressources !
    {
#ifdef __MIAM_DEBUG
        ++postReleaseRenderRequestsCount;
        if (postReleaseRenderRequestsCount == 1)
            DBG("[SceneCanvasComponent] Cancelled renders on calls to renderOpenGL after VBO/texture/shader resources were released.");
        else
            std::cout << "*";
#endif
    }
	else // sinon OK on fait le rendu
	{
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
		DrawOnSceneCanevas();
        // ***    ***
        
        
        // Affichage des FPS
		if (openGLInfoLabel != nullptr)
		{
            auto meanFps = displayFrequencyMeasurer.GetAverageFrequency_Hz();
			Matrix3D<float> testModel(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, (float)getHeight(), 0.0f, 1.0f);
            Matrix3D<float> testView = Math::GenerateLookAtMatrix(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
			Matrix3D<float> testProjecxtion = Math::GenerateOrthoPerspectiveMatrix((float)getWidth(),
                                                                                   (float)getHeight(),
                                                                                   cameraNearZ, // near
                                                                                   cameraFarZ); // far
            // Label "OpenGL"
			std::u16string testFPS = u"";
			testFPS += Miam::TextUtils::ConvertNumberToU16string(meanFps, 5);
            testFPS += u" fps";
            openGLInfoLabel->SetText(testFPS);
			openGLInfoLabel->drawOneTexturedRectangle(openGlContext, testModel, testView, testProjecxtion);
		}
    }
#endif // ndef __MIEM_VBO

    
    

    // Call to a general Graphic update on the whole Presenter module
    if (!manager->isUpdatePending())
        manager->triggerAsyncUpdate();

    // Time measures just before swap (or the closer that we can get to the swaps)
    displayFrequencyMeasurer.OnNewFrame();

    // Solution brutale...
    double lastDuration = displayFrequencyMeasurer.GetLastDuration_ms();
    double underTime = lastDuration > 0.0 ? desiredPeriod_ms - lastDuration : 0.0;


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
        EunderTime = 0.0;
        numFrame = 0;
    }
    if (underTime < 0.0)
        underTime = 0.0;
    
// NO FRAMERATE LIMIT FOR NOW

    if (underTime > 0.0)
    {
        //Thread::sleep((int)std::floor(underTime));
    }

}

void SceneCanvasComponent::openGLDestructionAtLastFrame()
{
    DBG("[MIEM OpenGL] Destruction sur 'last frame'");
    
    // objets texte avec texture
    openGLInfoLabel->releaseResourcesSync();
    openGLInfoLabel = nullptr;
    
    // vers la fin (objets managés sont partagés...)
    ReleaseGLFontResources();
    
    // propres ressources en dernier
    shaderProgram = nullptr;
    projectionMatrix = nullptr;
    viewMatrix = nullptr;
    modelMatrix = nullptr;
    positionShaderAttribute = nullptr;
    colourShaderAttribute = nullptr;
}

void SceneCanvasComponent::DrawOnSceneCanevas()
{
	if (shaderProgram != nullptr)
		shaderProgram->use();

	//		/|\
	//	   / | \		IL FAUT METTRE LES UPDATES DE MATRICES APRES LE USE ET AVANT LE DRAW !!!!
	//	  /  |  \
	//	 /   O   \
	//	-----------

	if (projectionMatrix != nullptr)
        projectionMatrix->setMatrix4(Math::GenerateOrthoPerspectiveMatrix((float)/*desktopScale **/ getWidth(),
                                                                          (float)/*desktopScale **/ getHeight(),
                                                                          cameraNearZ, // near
                                                                          cameraFarZ).mat, // far
                                     1, false);

	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, (float)getHeight(), 0.0f, 1.0f);
	if (modelMatrix != nullptr)
		modelMatrix->setMatrix4(model.mat, 1, false);

	// - - - - - Areas addition to VBO (including exciters if existing) - - - - -
	// Sans bloquer, du coup, les autres threads (pour réactivité max...)
    currentVertexBufferArrayPos = 0;
    currentColourBufferArrayPos = 0; // total reinit of the GL buffer objects
    currentIndexBufferArrayPos = 0;
	for (size_t i = 0; i < duplicatedAreas.size(); i++)
	{
        // Re-création de tous les buffers à chaque frame
        // --------- OPTIMISATION POSSIBLE SI UN JOOOOUUUUURRRRRRR çA MARCHE ------------
        // même si la quantité de données (environ 100ko au total) ne vaut peut être pas le coup de
        // faire plusieurs appels de fonctions........ pour n'actualiser que des parties de VBO
		AddShapeToBuffers(duplicatedAreas[i]);
	}

	DrawCanvasOutline();

	DrawShapes();
    
    DrawShapesNames();
}

static int sceneCanvasFramesCounter = 0;
void SceneCanvasComponent::DrawShapes()
{
#ifndef __MIEM_VBO
    throw std::logic_error("Cannot manipulate VBOs in non-VBO mode");
#endif
    
	if (positionShaderAttribute != nullptr && colourShaderAttribute != nullptr)
	{
#ifdef __MIAM_DEBUG
        if ((sceneCanvasFramesCounter++) > 1000)
        {
            //std::cout << "[Draw GL shapes] " << currentVertexBufferArrayPos << " vertex GLfloat ; " << currentColourBufferArrayPos << " colour GLfloat ; " << currentIndexBufferArrayPos << " index GLuint" << std::endl;
            sceneCanvasFramesCounter = 0;
            
        }
#endif
        
        // VERTICES array
		openGlContext.extensions.glEnableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGlName);
        // GLBufferSubData fait effectivement la copie dans le VBO
        assert(currentVertexBufferArrayPos < sceneVertexBufferData.size());
		openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER,
                                                 0, // vertex data starting at [0] in the VBO
                                                 currentVertexBufferArrayPos * sizeof(GLfloat), // size of modification in bytes
                                                 sceneVertexBufferData.data());
        // cette fonction peut servir si plusieurs données (pos et colour par exemple) sont dans le même array... pas de pb ici
		openGlContext.extensions.glVertexAttribPointer(positionShaderAttribute->attributeID,
                                                       3, // number of components
                                                       GL_FLOAT, // type de données
                                                       GL_FALSE, // truc avec la normalisation des nombres entiers/virgule fixe...
			sizeof(float[3]) /*sizeof(float[3])*/, // "stride". 0 <=> data tightly packed in array
                                                       0); // **pointer** to the first component (0 should be OK....)
        
		//openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);

        // COLOURS array
		openGlContext.extensions.glEnableVertexAttribArray(colourShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBufferGlName);
        assert(currentColourBufferArrayPos < sceneColourBufferData.size());
		openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0,
                                                 currentColourBufferArrayPos * sizeof(GLfloat),
                                                 sceneColourBufferData.data());
		openGlContext.extensions.glVertexAttribPointer(colourShaderAttribute->attributeID,
                                                       4, GL_FLOAT, GL_FALSE, sizeof(float[4]), 0);

        // INDICES (ELEMENT) array
		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferGlName);
        assert(currentIndexBufferArrayPos < sceneIndicesBufferData.size());
		openGlContext.extensions.glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                                                 currentIndexBufferArrayPos * sizeof(GLuint),
                                                 sceneIndicesBufferData.data());

        
		glDrawElements(GL_TRIANGLES, (GLsizei) currentIndexBufferArrayPos, GL_UNSIGNED_INT,
                       (void*)0); // pointer location ???
        /*
        glDrawElements(GL_TRIANGLES, (GLsizei) 192, GL_UNSIGNED_INT,
                       (void*)0); // pointer location ???
         */
        
		openGlContext.extensions.glDisableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glDisableVertexAttribArray(colourShaderAttribute->attributeID);

		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
void SceneCanvasComponent::DrawShapesNames()
{
    Matrix3D<float> testModel(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, (float)getHeight(), 0.0f, 1.0f);
    Matrix3D<float> testView = Math::GenerateLookAtMatrix(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
    Matrix3D<float> testProjecxtion = Math::GenerateOrthoPerspectiveMatrix((float)getWidth(),
                                                                           (float)getHeight(),
                                                                           cameraNearZ, // near
                                                                           cameraFarZ); // far
    
    for (auto area : duplicatedAreas)
    {
        if (area->isVisible() && area->IsNameVisible())
        {
            area->GetGLTextObject()->drawOneTexturedRectangle(openGlContext,
                                                              testModel, testView, testProjecxtion,
                                                              area->GetAlpha());
        }
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
			computeCanvasOutline();
        

		openGlContext.extensions.glEnableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineVertexBufferName);
        if (redrawCanvasOutline) // copie des nouvelles données dans le VBO
			openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER,
                                                     0, 8 * 3 * sizeof(GLfloat),
                                                     g_canvasOutlineVertex_buffer_data);
		openGlContext.extensions.glVertexAttribPointer(positionShaderAttribute->attributeID, 3, GL_FLOAT, GL_FALSE,
                                                       sizeof(float[3]), // ne devrait pas être zéro ?
                                                       0);

		openGlContext.extensions.glEnableVertexAttribArray(colourShaderAttribute->attributeID);
		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, canvasOutlineCoulourBufferName);
		if (redrawCanvasOutline) // copie des nouvelles données dans le VBO
			openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 4 * sizeof(GLfloat), g_canvasOutlineCoulour_buffer_data);
		openGlContext.extensions.glVertexAttribPointer(colourShaderAttribute->attributeID, 4, GL_FLOAT, GL_FALSE,
                                                       sizeof(float[4]),
                                                       0);

		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasOutlineIndexBufferName);

		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)0);

		openGlContext.extensions.glDisableVertexAttribArray(positionShaderAttribute->attributeID);
		openGlContext.extensions.glDisableVertexAttribArray(colourShaderAttribute->attributeID);

		openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
		openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        
        // Canvas outline est maintenant à jour dans les VBOs
        if (redrawCanvasOutline)
            redrawCanvasOutline = false;
	}
}


void SceneCanvasComponent::openGLContextClosing()
{
    // Méthode même pas appelée dans Android... D'après la doc Juce au 30 octobre 2017
    DBG("SceneCanvasComponent : closing OpenGL Context (only informative, doing nothing).");
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
        // Vérification de l'optimisation - A IMPLEMENTER ICI, AIRE PAR AIRE
        //std::cout << "[Rendu OpenGL] " << area->GetVerticesBufferActualElementsCount() << " verts et " << area->GetIndicesBufferActualElementsCount() << " indices" << std::endl;
     
        // buffers accessible via function calls only
        auto areaVertexBuffer = area->getVerticesBuffer();
        auto areaColourBuffer = area->getColoursBuffer();
        auto areaIndexBuffer = area->getIndicesBuffer();
        
		/// vertices
        const size_t shapeVertexBufferOffset = currentVertexBufferArrayPos;
        const size_t shapeVerticesBufferActualSize = 3 * area->GetVerticesBufferActualElementsCount();
#ifdef __MIEM_DEBUG_TRACE // dans area : Miem::Vector qui checke en debug les indices
        for (size_t i=0 ; i < shapeVerticesBufferActualSize ; i++)
        {
            sceneVertexBufferData[currentVertexBufferArrayPos] = vertexBuffer[i];
            ++currentVertexBufferArrayPos;
        }
#else // sinon pas de trace dans le vecteur, copie optimisée
        std::copy(areaVertexBuffer.begin(),
                  areaVertexBuffer.begin() + shapeVerticesBufferActualSize,
                  sceneVertexBufferData.begin() + currentVertexBufferArrayPos);
        currentVertexBufferArrayPos += shapeVerticesBufferActualSize;
#endif
        // colours
        const size_t shapeColoursBufferActualSize = 4 * area->GetVerticesBufferActualElementsCount();
#ifdef __MIEM_DEBUG_TRACE
        for (size_t i=0 ; i < shapeColoursBufferActualSize ; i++)
        {
            sceneColourBufferData[currentColourBufferArrayPos] = colourBuffer[i];
            ++currentColourBufferArrayPos;
        }
#else
        std::copy(areaColourBuffer.begin(),
                  areaColourBuffer.begin() + shapeColoursBufferActualSize,
                  sceneColourBufferData.begin() + currentColourBufferArrayPos);
        currentColourBufferArrayPos += shapeColoursBufferActualSize;
#endif
		/// indices
        assert((shapeVertexBufferOffset % 3) == 0); // sinon, on a mis des points non terminés....
        const GLuint shapeVertexBufferElmtOffset = (GLuint) shapeVertexBufferOffset / 3;
        const size_t shapeIndicesBufferActualSize = area->GetIndicesBufferActualElementsCount();
        for (size_t i=0 ; i < shapeIndicesBufferActualSize ; i++)
        {
            // les indices doivent être décalés de l'offset de vertex buffer de la forme
            // donc : pas de copie optimisée possible
            sceneIndicesBufferData[currentIndexBufferArrayPos] = areaIndexBuffer[i]
                                                + shapeVertexBufferElmtOffset;
            ++currentIndexBufferArrayPos;
        }
	}
}



