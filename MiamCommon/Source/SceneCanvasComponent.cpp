/*
  ==============================================================================

    SceneCanvasComponent.cpp
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <cmath>

#include "SceneCanvasComponent.h"

#include "MultiSceneCanvasInteractor.h"

//==============================================================================
SceneCanvasComponent::SceneCanvasComponent() :
    selectedForEditing(false),
    displayFrequencyMeasurer("SceneCanvasComponent display")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	// calcul d'un anneau de centre 0, de rayon 5 pixels et avec une épaisseur de 2 pixels
	int radius = 20;
	int width = 2;
	int numPoints = numPointsRing;
	int ri = radius - width / 2;
	int re = radius + width / 2;

	double currentAngle = 0;
	double incAngle = 2 * M_PI / (double)numPoints;
	for (int i = 0; i < numPoints; ++i)
	{
		g_vertex_ring[i * 3] = ri * cos(currentAngle);
		g_vertex_ring[i * 3 + 1] =  ri * sin(currentAngle);
		g_vertex_ring[i * 3 + 2] = 0;
		g_vertex_ring[numPoints*3 + i*3] = re * cos(currentAngle);
		g_vertex_ring[numPoints*3 + i*3 + 1] = re * sin(currentAngle);
		g_vertex_ring[numPoints*3 + i*3 + 2] = 0;
		currentAngle += incAngle;
	}
	for (int i = 0; i < numPoints; ++i)
	{
		ringIndices[i * 6] = i;
		ringIndices[i * 6 + 1] = numPoints + i;
		ringIndices[i * 6 + 2] = numPoints + i + 1 >= 2 * numPoints? numPoints : numPoints + i + 1;
		ringIndices[i * 6 + 3] = numPoints + i + 1 >= 2 * numPoints ? numPoints : numPoints + i + 1;
		ringIndices[i * 6 + 4] = i;
		ringIndices[i * 6 + 5] = i + 1 >= numPoints? 0 : i+1;
	}

	//g_vertex_buffer_data[3 * 3 * numVerticesPolygon + 3 * 3 * numVerticesRing];
	for (int i = 0; i < 3 * numVerticesPolygon + 3 * numVerticesRing; ++i)
		g_vertex_buffer_data[i] = 0;

	for (int i = 0; i < 3 * numVerticesPolygon + 3 * numVerticesPolygon; ++i)
		indices[i] = 0;
    
    openGlContext.setComponentPaintingEnabled(true); // default behavior, lower perfs
    // OpenGL final initialization will happen in the COmpleteInitialization method
    
}

SceneCanvasComponent::~SceneCanvasComponent()
{
    openGlContext.detach();
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
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
		g_vertex_buffer_data, GL_STATIC_DRAW);

	// TRIANGLE COULEUR
	// pareil pour les buffers de couleurs des deux
	openGlContext.extensions.glGenBuffers(1, &colorBuffer);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// TRIANGLE INDEX
	openGlContext.extensions.glGenBuffers(1, &elementBuffer);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, (3 * numVerticesPolygon + 3 * numVerticesRing) * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// déclaration des attributs et uniforms : !!! aux noms et leurs utilisations dans les shaders !!!
	position = new OpenGLShaderProgram::Attribute(*shaderProgram, String("position").toRawUTF8());
	colour = new OpenGLShaderProgram::Attribute(*shaderProgram, "colour");

	projectionMatrix = new OpenGLShaderProgram::Uniform(*shaderProgram, "projectionMatrix");
	viewMatrix = new OpenGLShaderProgram::Uniform(*shaderProgram, "viewMatrix");
	modelMatrix = new OpenGLShaderProgram::Uniform(*shaderProgram, "modelMatrix");
}
void SceneCanvasComponent::renderOpenGL()
{


	//DBG("render : " + getName());
    auto manager = canvasManager.lock();
	OpenGLHelpers::clear(Colours::black);
    const double desktopScale = (float)openGlContext.getRenderingScale();
    /*std::unique_ptr<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (openGlContext,
                                                                                    roundToInt (desktopScale * getWidth()),
                                                                                    roundToInt (desktopScale * getHeight())));*/
   // Graphics g(*glRenderer);
    
    //g.addTransform(AffineTransform::scale((float) desktopScale));
    
    // Pure black background
   // g.fillAll (Colours::black);
    
    // White interior contour 2px line to show when the canvas is active
    /*if (selectedForEditing)
    {
        g.setColour(Colours::white);
        g.drawRect(1, 1, getWidth()-2, getHeight()-2, 2);
    }*/

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
        canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
        duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
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
    for (size_t i=0;i<duplicatedAreas.size();i++)
    {
    //    // Peut mettre à jour des images et autres (si l'échelle a changé)
    //    duplicatedAreas[i]->SetRenderingScale(desktopScale);
    //    // Dessin effectif
    //    duplicatedAreas[i]->Paint(g);
		int decalage = numPointsRing + 1;
		if (duplicatedAreas[i]->GetVerticesCount() >= 3)
		{
			
			std::vector<float> newVertex = duplicatedAreas[i]->GetVertices();
			for (int j = 0; j < newVertex.size(); ++j)
			{
				g_vertex_buffer_data[j] = 0;//newVertex[j];//*10
			}
			
			for (int j = 0; j < 3 * numVerticesRing; j+= 3)
			{
				g_vertex_buffer_data[3 *decalage/*numVerticesPolygon*/ + j] = getWidth()/2 + g_vertex_ring[j];
				g_vertex_buffer_data[3 *decalage/*numVerticesPolygon*/ + j + 1] = getHeight()/2 + g_vertex_ring[j + 1];
				g_vertex_buffer_data[3 *decalage/*numVerticesPolygon*/ + j + 2] = g_vertex_ring[j + 2];
			}
		}

		if (duplicatedAreas[i]->GetIndexCount() >= 3)
		{
			
			std::vector<int> newIndex = duplicatedAreas[i]->GetIndex();
			for (int j = 0; j < newIndex.size(); ++j)
			{
				indices[j] = (unsigned int)newIndex[j];
			}
			
			for (int j = 0; j < 3 * numVerticesRing; ++j)
			{
				indices[j/*+ numVerticesPolygon*/] = ringIndices[j] + decalage;/*+ numVerticesPolygon*/;
			}
		}

    }
    



	/// calcul des matrices
	Matrix3D<float> testView = lookAt(Vector3D<float>(0, 0, 1), Vector3D<float>(0, 0, 0), Vector3D<float>(0, -1, 0));
	Matrix3D<float> testProject = perspective(getWidth(), getHeight(), 0.1f, 100.0f);
	if (projectionMatrix != nullptr)
		projectionMatrix->setMatrix4(perspective(getWidth(), getHeight(), 0.5f, 1.1f).mat, 1, false);

	if (viewMatrix != nullptr)
		viewMatrix->setMatrix4(testView.mat, 1, false);

	Matrix3D<float> model(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, getHeight(), 0.0f, 1.0f);//10*getHeight()

	if (modelMatrix != nullptr)
		modelMatrix->setMatrix4(model.mat, 1, false);

	/// Draw triangle
	openGlContext.extensions.glEnableVertexAttribArray(position->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	openGlContext.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
	openGlContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);
	//openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);

	openGlContext.extensions.glEnableVertexAttribArray(colour->attributeID);
	openGlContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	openGlContext.extensions.glVertexAttribPointer(colour->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);

	
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	//openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	openGlContext.extensions.glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (3 * numVerticesPolygon + 3 * numVerticesRing) * sizeof(unsigned int), &indices[0]);

	glDrawElements(GL_TRIANGLES, 3 * numVerticesPolygon + 3 * numVerticesRing /*+ 3 * 64*/, GL_UNSIGNED_INT, (void*)0);
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
    double underTime = desiredPeriod_ms - displayFrequencyMeasurer.GetLastDuration_ms();
    if (underTime > 0.0)
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

