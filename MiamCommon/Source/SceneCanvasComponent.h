/*
  ==============================================================================

    SceneCanvasComponent.h
    Created: 28 Mar 2016 3:39:35pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SCENECANVASCOMPONENT_H_INCLUDED
#define SCENECANVASCOMPONENT_H_INCLUDED

#include <memory>

#include "JuceHeader.h"

#include "DrawableArea.h"

#include "FrequencyMeasurer.h"



using namespace Miam;

namespace Miam {
    
    class MultiSceneCanvasInteractor;
}

//==============================================================================
/// \brief Initially empty component dedicated to the drawing of several Miam::DrawableArea
///
/// This component does not have any children UI controls built within the Projucer.
class SceneCanvasComponent    : public OpenGLRenderer,
                                public Component

{
public:
    
    
    // = = = = = = = = = = ENUMS = = = = = = = = = =
    
    /// \brief IDs for describing a particular canvas, or canvases in general
    ///
    enum Id : int { // enum *class* brings cast issues... (wait for C++14 ?)
        None = -1, ///< No canvas currently selected (useless now with smart ptrs)
        
        Canvas1 = 0, ///< First canvas...
        Canvas2, ///< Second canvas...
        CanvasesCount ///< Automatically contains the amount of accessible canvases
    };
    
    
    
    
    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    
    protected :
    /// \brief Link back to parent module. Thread-safe access to member functions
    /// (guaranteed by std::shared_ptr)
    std::weak_ptr<MultiSceneCanvasInteractor> canvasManager;
    
    bool selectedForEditing;
    
    
    // - - - - - OpenGL - - - - -
    OpenGLContext openGlContext;
    const int swapInterval = 1; // synced on vertical frequency
    bool isSwapSynced;
    
    // Les pointeurs sur les 2è copies des objets du canvas interactor
    // (pour savoir lesquels avaient changé) -> ATTENTION tout l'algorithme thread-safe
    // ici fonctionne tant que le CanvasInteractor construit bien un nouvel objet à chaque fois
    // que l'objet de base est modifié.
    // On stocke ça dans un simple vecteur (alors que les objets de la seconde
    // copie sont dans une liste)
    std::vector<std::shared_ptr<IDrawableArea>> canvasAreasPointersCopies;
    // La 3è copie des objets (celle pour faire le rendu à tout moment),
    // qui sera éventuellement actualisée lorsque nécessaire
    std::vector<std::shared_ptr<IDrawableArea>> duplicatedAreas;
    
    // - - - - - Time measures - - - - -
    const double desiredFrequency_Hz = 60.0; // actual freq will actually be greater
    const double desiredPeriod_ms = 1000.0/desiredFrequency_Hz;
    FrequencyMeasurer displayFrequencyMeasurer;
    
    protected :
    
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    
    SceneCanvasComponent();
    ~SceneCanvasComponent();
    
	/// \brief Also called from Miam::View::CompleteInitialization
    void CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager);

    // - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -
    void resized() override;
    
    // - - - - - - - - OpenGL specific - - - - - - - - -
    virtual void newOpenGLContextCreated() override;
    virtual void renderOpenGL() override; // ! in background-thread !
    virtual void openGLContextClosing() override;
    
    // - - - - - - - - Actual painting codes - - - - - - - - -
    protected :
    
    // - - - - - - - - Juce events - - - - - - - - -
    public :
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    
    // Getters and Setters
    float GetRatio() {return ((float)getWidth()) / ((float)getHeight()) ; }
    void SetIsSelectedForEditing(bool isSelected);
    

private:

	ScopedPointer<OpenGLShaderProgram> shaderProgram;
	ScopedPointer<OpenGLShaderProgram::Attribute> position, colour;

	ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, modelMatrix;

	String myVertexShader = "attribute vec4 position;\n"
		"attribute vec4 colour;\n"
		"\n"
		"uniform mat4 modelMatrix;\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
		"out vec4 fragmentColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"\n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;\n"
		"    fragmentColor = colour;"
		"}\n";

	String myFragmentShader =
#if JUCE_OPENGL_ES
		"varying lowp vec4 destinationColour;\n"
#else
		"varying vec4 destinationColour;\n"
#endif
		"\n"
		"in vec4 fragmentColor;\n"
		"\n"
		"void main()\n"
		"{\n"
#if JUCE_OPENGL_ES
		"   highp float l = 0.3;\n"

#else
		"   float l = 0.3;\n"

#endif
		"    gl_FragColor = fragmentColor;\n"
		"}\n";
	enum Layers
	{
		Shapes,
		ShapesOverlay
	};

	static const int numPointsPolygon = 32;
	static const int numPointsRing = 32;
	static const int numPointCircle = 32;

	static const int numVerticesPolygon = numPointsPolygon + 1;
	static const int numVerticesRing = 2 * numPointsRing;
	static const int numVerticesCircle = numPointCircle + 1;

	static const int vertexBufferSize = 3 * numVerticesPolygon + (3 * numVerticesRing) + numPointsPolygon * (3 * numVerticesCircle);
	static const int colorBufferSize = 3 * numVerticesPolygon +  (3 * numVerticesRing) + numPointsPolygon * (3 * numVerticesCircle);
	static const int indicesSize = 3 * numVerticesPolygon + (3 * numVerticesRing) + numPointsPolygon * (3 * numPointCircle);

	GLfloat g_vertex_ring[3 * numVerticesRing];
	unsigned int ringIndices[3 * numVerticesRing];

	GLfloat g_vertex_circle[3 * numVerticesCircle];
	unsigned int circleIndices[3 * numPointCircle];

	GLuint vertexBuffer;
	GLfloat g_vertex_buffer_data[vertexBufferSize]; // forme + vertex

	GLuint colorBuffer;
	std::vector<GLfloat> g_color_buffer_data;// [3 * numVerticesPolygon + 3 * numVerticesRing];

	GLuint elementBuffer;
	unsigned int indices[indicesSize];

	///// vertex de toutes les formes dans g_vertex_buffer
	//GLuint vertexBuffer; 
	//GLfloat g_vertex_buffer_data[3 * 3 * 32 * 20 * 3]; // taille = 3 composants * 32 triangles (max pour un cercle) *  3 sommets du triangle * 20 formes/scenes (10 curseurs + 10 polygones) * 3 scenes

	///// contient les indices pour dessiner les formes sans stocker plusieurs fois le mm sommet
	//GLuint indexVertexBuffer;
	//GLuint g_index_vertex_buffer[3 * 32 * 20 * 3]; // 3 sommets par triangle * 32 triangles (max pour un cercle) * 20 formes * 3 scenes

	///// contient les positions et rotation de chaque forme pour construire la modelMatrix de chaque forme
	//GLuint modelPositionBuffer;
	//GLfloat g_model_position[3 * 20 * 3]; // 3 composants (x, y, theta) * 20 formes * 3 scenes

	///// indices pour que tout les triangles d'une même forme subissent la même transformation de la matrice model
	//GLuint indexPositionBuffer;
	//GLuint g_index_model_position[20 * 3]; // 1 matrices * 20 formes * 3 scenes

	///// couleurs
	//GLuint colourBuffer;
	//GLfloat g_colour_buffer[3 * 3 * 32 * 20 * 3]; // 1 pour chaque vertex

	Matrix3D<float> lookAt(Vector3D<float> eye, Vector3D<float> center, Vector3D<float> up)
	{
		/*Vector3D<float> F = center - eye;
		Vector3D<float> s = F.normalised() ^ up;
		Vector3D<float> u = s.normalised() ^ F.normalised();
		return Matrix3D<float>(s.x, s.y, s.z, -eye * s,
		u.x, u.y, u.z, -eye * u,
		-F.normalised().x, -F.normalised().y, -F.normalised().z, -eye * F,
		0, 0.0f, 0.0f, 1.0f);*/
		Vector3D<float> zaxis = (eye - center).normalised();
		Vector3D<float> xaxis = (zaxis ^ up).normalised();
		Vector3D<float> yaxis = -(xaxis ^ zaxis);

		/*Matrix3D<float> viewMatrix(xaxis.x, xaxis.y, xaxis.z, -(xaxis * eye),
			yaxis.x, yaxis.y, yaxis.z, -(yaxis * eye),
			zaxis.x, zaxis.y, zaxis.z, -(zaxis * eye),
			0, 0, 0, 1);*/

		Matrix3D<float> viewMatrixTr(xaxis.x, yaxis.x, zaxis.x, 0,
			xaxis.y, yaxis.y, zaxis.y, 0,
			xaxis.z, yaxis.z, zaxis.z, 0,
			-(xaxis * eye), -(yaxis * eye), -(zaxis*eye), 1);
		return viewMatrixTr;
	}

	Matrix3D<float> perspective(float width, float height, float near, float far)
	{
		/*float top = tan(fovy / 2.0f) * near;
		float bottom = -top;*/
		//return Matrix3D<float>::fromFrustum(-top * width / height, top * width / height, bottom, top, near, far);

		float r = width;
		float l = 0.0f;
		float b = 0.0f;
		float t = height;
		Matrix3D<float> orthoProj(2.0f / (r - l), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (t - b), 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / (far - near), 0.0f,
			-(r + l) / (r - l), -(t + b) / (t - b), -(far + near) / (far - near), 1.0f);
		Matrix3D<float> autre = Matrix3D<float>::fromFrustum(0, width / 2, 0, height / 2, near, far);

		return orthoProj;

	}

	float getLayerRatio(Layers layers)
	{
		switch (layers)
		{
		case SceneCanvasComponent::Shapes:
			return 1.0f;
			break;
		case SceneCanvasComponent::ShapesOverlay:
			return 0.9f;
			break;
		default:
			break;
		}
	}


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneCanvasComponent)
};


#endif  // SCENECANVASCOMPONENT_H_INCLUDED
