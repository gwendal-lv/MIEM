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

struct ColorGenerator
{
	int count;
	GLfloat color[3];
	ColorGenerator(GLfloat R, GLfloat G, GLfloat B) : count(0)
	{
		color[0] = R;
		color[1] = G;
		color[2] = B;
	}
	GLfloat operator() ()
	{
		GLfloat ret = color[count % 3];
		++count;
		return ret;
	}
};

//==============================================================================
SceneCanvasComponent::SceneCanvasComponent() :
    selectedForEditing(false),
    displayFrequencyMeasurer("SceneCanvasComponent display")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	
	g_color_buffer_data = std::vector<GLfloat>(colorBufferSize,0.0f);
	for (int i = 0; i < Nshapes;++i)
	{
		ColorGenerator g(0.5f, 0.5f, 0.5f);
		std::generate(g_color_buffer_data.begin() + i * shapeColorBufferSize, g_color_buffer_data.begin() + i * shapeColorBufferSize + 3 * numVerticesPolygon, g);
		ColorGenerator f(1.0f, 0.0f, 0.0f);
		std::generate(g_color_buffer_data.begin() + i * shapeColorBufferSize + 3 * numVerticesPolygon + 1, g_color_buffer_data.begin() + (i+1) * shapeColorBufferSize, f);
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
	
	// calcul d'un cercle de centre 0 et de rayon 5 pixels
	radius = 5.0f;
	currentAngle = 0.0;
	incAngle = 2 * M_PI / (double)numPoints;
	g_vertex_circle[0] = 0.0f;
	g_vertex_circle[1] = 0.0f;
	g_vertex_circle[2] = 0.0f;
	for (int i = 0; i < numPointCircle; ++i)
	{
		g_vertex_circle[(i+1) * 3] = radius * cos(currentAngle);
		g_vertex_circle[(i+1) * 3 + 1] = radius * sin(currentAngle);
		g_vertex_circle[(i+1) * 3 + 2] = 0;
		currentAngle += incAngle;
	}
	for (int i = 0; i < numPointCircle; ++i)
	{
		circleIndices[i * 3] = i + 1;
		circleIndices[i * 3 + 1] = 0;
		circleIndices[i * 3 + 2] = i + 2 > numPointCircle ? 1 : i + 2;
	}

	//g_vertex_buffer_data[3 * 3 * numVerticesPolygon + 3 * 3 * numVerticesRing];
	for (int i = 0; i < 3 * numVerticesPolygon + 3 * numVerticesRing; ++i)
		g_vertex_buffer_data[i] = 0;

	for (int i = 0; i < 3 * numVerticesPolygon + 3 * numVerticesPolygon; ++i)
		indices[i] = 0;

	shift2[0] = 0;
	shift2[1] = numVerticesPolygon;
	shift2[2] = numVerticesPolygon + numVerticesRing;

	
	for (int k = 0; k < numPointsPolygon; ++k)
	{
		shift2[3 + k] = shift2[2] + (k+1) * numVerticesCircle;
	}
    
    openGlContext.setComponentPaintingEnabled(false); // default behavior, lower perfs
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
	openGlContext.extensions.glBufferData(GL_ARRAY_BUFFER, colorBufferSize* sizeof(GLfloat[3]), &g_color_buffer_data[0], GL_STATIC_DRAW);

	// TRIANGLE INDEX
	openGlContext.extensions.glGenBuffers(1, &elementBuffer);
	openGlContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	openGlContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		/*
		int decalage = numPointsPolygon + 1;
		if (duplicatedAreas[i]->GetVerticesCount() >= 3)
		{
			/// vertex
			//1. forme
			std::vector<float> newVertex = duplicatedAreas[i]->GetVertices();
			for (int j = 0; j < newVertex.size(); ++j)
			{
				g_vertex_buffer_data[j] = newVertex[j];//*10
			}
			
			//2. centre
			for (int j = 0; j < 3 * numVerticesRing; j+= 3)
			{
				g_vertex_buffer_data[3 *decalage + j] = 1.0* (newVertex[0] + g_vertex_ring[j]);
				g_vertex_buffer_data[3 *decalage + j + 1] = 1.0*(newVertex[1] + g_vertex_ring[j + 1]);
				g_vertex_buffer_data[3 *decalage + j + 2] = 0.1 + g_vertex_ring[j + 2];
			}
			decalage += numVerticesRing;

			//3. points
			int numApexes = (newVertex.size() - 3) / 3;
			for (int k = 0; k < numApexes; ++k)
			{
				for (int j = 0; j < 3 * numVerticesCircle; j += 3)
				{
					g_vertex_buffer_data[3 * decalage + j] = 1.0* (newVertex[3 + k*3] + g_vertex_circle[j]);
					g_vertex_buffer_data[3 * decalage + j + 1] = 1.0*(newVertex[3 + k*3 +1] + g_vertex_circle[j + 1]);
					g_vertex_buffer_data[3 * decalage + j + 2] = 0.1 + g_vertex_circle[j + 2];
				}
				decalage += numVerticesCircle;
			}

			decalage = numVerticesPolygon + (numVerticesRing) + numPointsPolygon * (numVerticesCircle);

			//4. contour
			std::vector<float> newOutline = duplicatedAreas[i]->GetOutline();
			for (int j = 0; j < newOutline.size(); ++j)
			{
				g_vertex_buffer_data[3 * decalage + j] = newOutline[j];
			}
			decalage += numPointsPolygon;
		}

		/// indices
		decalage = numPointsPolygon + 1;
		if (duplicatedAreas[i]->GetIndexCount() >= 3)
		{
			//1. forme
			std::vector<int> newIndex = duplicatedAreas[i]->GetIndex();
			for (int j = 0; j < newIndex.size(); ++j)
			{
				indices[j] = (unsigned int)newIndex[j];
			}
			
			//2. centre
			for (int j = 0; j < 3 * numVerticesRing; ++j)
			{
				indices[j + 3 * decalage] = ringIndices[j] + decalage;;
			}
			decalage += numVerticesRing;

			//3. points
			int numApexes = newIndex.size() / 3;
			for (int k = 0; k < numApexes; ++k)
			{
				for (int j = 0; j < 3 * numPointCircle; ++j)
					indices[j + 3 * decalage] = circleIndices[j] + decalage + k;
				decalage += numPointCircle;
			}

			decalage = numVerticesPolygon + (numVerticesRing) + numPointsPolygon * (numPointCircle+1);

			//4. contour
			for (int i = 0; i < numApexes; ++i)
			{
				indices[3 * decalage + i * 6] = i+1;
				indices[3 * decalage + i * 6 + 1] = decalage + i;
				indices[3 * decalage + i * 6 + 2] = decalage + i + 1 >= decalage + numApexes ? decalage : decalage + i + 1;
				indices[3 * decalage + i * 6 + 3] = decalage + i + 1 >= decalage + numApexes ? decalage : decalage + i + 1;
				indices[3 * decalage + i * 6 + 4] = i+1;
				indices[3 * decalage + i * 6 + 5] = i + 2 >= numApexes + 1 ? 0 : i + 2;
			}

			decalage += 2 * numPointsPolygon;
		}*/
		//if(duplicatedAreas.size() > 1)
			DrawShape(duplicatedAreas[i], i * numVertexShape);
		//DrawShape(duplicatedAreas[1], 0 * numVertexShape);
    }
    

	glOrtho(0,getWidth(),0, getHeight(), 0.5f, 1.1f);

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
	openGlContext.extensions.glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesSize * sizeof(unsigned int), &indices[0]);

	glDrawElements(GL_TRIANGLES, indicesSize /*+ 3 * 64*/, GL_UNSIGNED_INT, (void*)0);
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

void SceneCanvasComponent::DrawShape(std::shared_ptr<IDrawableArea> area, int positionInBuffer)
{
	int decalage = positionInBuffer;// + numPointsPolygon + 1;
	//std::vector<int> shift;
	//shift.push_back(decalage);
	if (area->GetVerticesCount() >= 3)
	{
		/// vertex
		//1. forme
		std::vector<float> newVertex = area->GetVertices();
		for (int j = 0; j < 3*(numPointsPolygon + 1); ++j)
		{
			/*if (3 * decalage + j > shapeVertexBufferSize)
				DBG("depasse");*/
			if (j < newVertex.size())
				g_vertex_buffer_data[3 * decalage + j] = newVertex[j];//*10
			else
				g_vertex_buffer_data[3 * decalage + j] = 0;
		}

		decalage += (numPointsPolygon + 1);
		//shift.push_back(decalage);

		//2. centre
		for (int j = 0; j < 3 * numVerticesRing; j += 3)
		{
			/*if (3 * decalage + j > shapeVertexBufferSize)
				DBG("depasse");*/
			g_vertex_buffer_data[3 * decalage + j] = 1.0* (newVertex[0] + g_vertex_ring[j]);
			g_vertex_buffer_data[3 * decalage + j + 1] = 1.0*(newVertex[1] + g_vertex_ring[j + 1]);
			g_vertex_buffer_data[3 * decalage + j + 2] = 0.1 + g_vertex_ring[j + 2];
		}
		decalage += numVerticesRing;
		//shift.push_back(decalage);

		//3. points
		int numApexes = (newVertex.size() - 3) / 3;
		for (int k = 0; k < numPointsPolygon; ++k)
		{
			if (k < numApexes)
			{
				for (int j = 0; j < 3 * numVerticesCircle; j += 3)
				{
					/*if (3 * decalage + j > shapeVertexBufferSize)
						DBG("depasse");*/
					g_vertex_buffer_data[3 * decalage + j] = 1.0* (newVertex[3 + k * 3] + g_vertex_circle[j]);
					g_vertex_buffer_data[3 * decalage + j + 1] = 1.0*(newVertex[3 + k * 3 + 1] + g_vertex_circle[j + 1]);
					g_vertex_buffer_data[3 * decalage + j + 2] = 0.1 + g_vertex_circle[j + 2];
				}
				decalage += numVerticesCircle;
			}
			else
			{
				for (int j = 0; j < 3 * numVerticesCircle; j++)
					g_vertex_buffer_data[3 * decalage + j] = 0;
				decalage += numVerticesCircle;
			}
			//shift.push_back(decalage);
		}
		/*for (int i = numApexes; i < numPointsPolygon; ++i)
			shift.push_back(positionInBuffer + numVerticesPolygon + numVerticesRing + (i+1)*numVerticesCircle);*/

		decalage = positionInBuffer + numVerticesPolygon + (numVerticesRing)+numPointsPolygon * (numVerticesCircle);//(numPointCircle);

		//4. contour
		std::vector<float> newOutline = area->GetOutline();
		for (int j = 0; j < 3*numPointsPolygon; ++j)
		{
			/*if (3 * decalage + j > shapeVertexBufferSize)
				DBG("depasse");*/
			if (j < newOutline.size())
				g_vertex_buffer_data[3 * decalage + j] = newOutline[j];
			else
				g_vertex_buffer_data[3 * decalage + j] = 0;
		}
		decalage += numPointsPolygon;
	}


	/// indices
	decalage = positionInBuffer;// +numPointsPolygon + 1;
	if (area->GetIndexCount() >= 3)
	{
		int last = 0;
		//1. forme
		std::vector<int> newIndex = area->GetIndex();
		for (int j = 0; j < 3*(numPointsPolygon + 1); ++j)
		{
			/*if (3 * decalage + j > shapeIndicesSize || 3 * decalage + j > 3*(decalage + numPointsPolygon + 1))
				DBG("depasse");*/
			if (j < newIndex.size())
				indices[3 * decalage + j] = (unsigned int)newIndex[j] + shift2[0] + positionInBuffer;
			else
				indices[3 * decalage + j] = 0;
			last = 3 * decalage + j;
		}

		decalage += numPointsPolygon + 1;
		/*if (last > 3 * decalage)
			DBG("depasse");*/

		//2. centre
		for (int j = 0; j < 3 * numVerticesRing; ++j)
		{
			/*if (3 * decalage + j > shapeIndicesSize || 3 * decalage + j > 3*(decalage + numVerticesRing))
				DBG("depasse");*/
			indices[j + 3 * decalage/*+ numVerticesPolygon*/] = ringIndices[j] + shift2[1] + positionInBuffer;/*+ numVerticesPolygon*/;
			last = j + 3 * decalage;
		}
		decalage += numVerticesRing;
		/*if (last > 3 * decalage)
			DBG("depasse");*/

		//3. points
		int numApexes = newIndex.size() / 3;
		for (int k = 0; k < numPointsPolygon; ++k)
		{
			if (k < numApexes)
			{
				for (int j = 0; j < 3 * numPointCircle; ++j)
				{
					/*if (3 * decalage + j > shapeIndicesSize || 3 * decalage + j > 3 * (decalage + numPointCircle))
						DBG("depasse");*/
					indices[j + 3 * decalage/*+ numVerticesPolygon*/] = circleIndices[j] + shift2[2 + k] + positionInBuffer;
					last = j + 3 * decalage;
				}
			}
			else
			{
				for (int j = 0; j < 3 * numPointCircle; ++j)
					indices[j + 3 * decalage/*+ numVerticesPolygon*/] = 0;
			}
			decalage += numPointCircle;
		}

		decalage = positionInBuffer + numVerticesPolygon + (numVerticesRing)+numPointsPolygon * (numPointCircle);
		/*if (last > 3 * decalage)
			DBG("depasse");*/
										
		
		int count = 0;
		//4. contour
		for (int i = 0; i < numPointsPolygon; ++i)
		{
			/*if (3 * decalage + i * 6 > shapeIndicesSize || 3 * decalage + i * 6 > 3*(decalage+2*numPointsPolygon))
				DBG("depasse");*/
			if (i < numApexes)
			{
				indices[3 * decalage + i * 6] = positionInBuffer + i + 1;
				indices[3 * decalage + i * 6 + 1] = shift2[2 + numPointsPolygon] + positionInBuffer + i;
				indices[3 * decalage + i * 6 + 2] = shift2[2 + numPointsPolygon] + positionInBuffer + i + 1 >= shift2[2 + numPointsPolygon] + positionInBuffer + numApexes ? shift2[2 + numPointsPolygon] + positionInBuffer : shift2[2 + numPointsPolygon] + positionInBuffer + i + 1;
				indices[3 * decalage + i * 6 + 3] = shift2[2 + numPointsPolygon] + positionInBuffer + i + 1 >= shift2[2 + numPointsPolygon] + positionInBuffer + numApexes ? shift2[2 + numPointsPolygon] + positionInBuffer : shift2[2 + numPointsPolygon] + positionInBuffer + i + 1;
				indices[3 * decalage + i * 6 + 4] = positionInBuffer + i + 1;
				indices[3 * decalage + i * 6 + 5] = positionInBuffer + i + 2 >= positionInBuffer + numApexes + 1 ? positionInBuffer + 1 : positionInBuffer + i + 2;
				count += 6;
			}
			else
			{
				indices[3 * decalage + i * 6] = 0;
				indices[3 * decalage + i * 6 + 1] = 0;
				indices[3 * decalage + i * 6 + 2] = 0;
				indices[3 * decalage + i * 6 + 3] = 0;
				indices[3 * decalage + i * 6 + 4] = 0;
				indices[3 * decalage + i * 6 + 5] = 0;
				count += 6;
			}
			last = 3 * decalage + i * 6 + 5;
		}

		decalage += 2 * numPointsPolygon;
		/*if (last > 3 * decalage)
			DBG("depasse");*/
	}


}

