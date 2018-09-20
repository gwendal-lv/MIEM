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

#include "OpenGLTextObject.h"

#include <fstream>

#include <string>

#include <thread>

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
	SceneCanvasComponent(int numShapesMax, int numPointsMax = 64);

	void init(int numShapesMax, int numPointsMax);
	void ReleaseOpengGLResources();

    ~SceneCanvasComponent();
    
	/// \brief Also called from Miam::View::CompleteInitialization
    void CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager);

    // - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -
    void resized() override;
    
    // - - - - - - - - OpenGL specific - - - - - - - - -
    virtual void newOpenGLContextCreated() override;
    virtual void renderOpenGL() override; // ! in background-thread !
	virtual void DrawOnSceneCanevas(std::shared_ptr<Miam::MultiSceneCanvasInteractor> &manager);
	void DrawShapes();
	void DrawCanvasOutline();
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

    
protected :
	OpenGLShaderProgram::Attribute *position, *colour, *positionCopy, *colourCopy;

private:
	int numFrame;
	double EunderTime;
	OpenGLTextObject *openGLLabel, *openGLLabelCopy;

	bool needToResetBufferParts;
	int previousMaxSize; // utilisé pour remettre à 0 les parties de buffer qui étaient utilisées à la frame précédente et qui ne le sont plus mtn

	OpenGLShaderProgram *shaderProgram, *shaderProgramCopy;
	OpenGLShaderProgram::Uniform *projectionMatrix, *viewMatrix, *modelMatrix, *projectionMatrixCopy, *viewMatrixCopy, *modelMatrixCopy;

	String myVertexShader = "attribute vec4 position;\n"
#if JUCE_OPENGL_ES
		"attribute lowp vec4 colour;\n"
#else
		"attribute vec4 colour;\n"
#endif
		"\n"
		"uniform mat4 modelMatrix;\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"\n"
#if JUCE_OPENGL_ES
		"varying lowp vec4 fragmentColor;\n"
#else
		"varying vec4 fragmentColor;\n"
#endif
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
#if JUCE_OPENGL_ES
		"varying lowp vec4 fragmentColor;\n"
#else
		"varying vec4 fragmentColor;\n"
#endif
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

	/*static const int*/const int*  numPointsPolygon;// = 64;
	static const int numPointsRing = 64;
	static const int numPointCircle = 64;

	/*static const int*/const int* numVerticesPolygon;// = numPointsPolygon + 1;
	static const int numVerticesRing = 2 * numPointsRing;
	static const int numVerticesCircle = numPointCircle + 1;

	static const int dottedLineNparts = 20;
	static const int dottedLineVertexes = 4 * dottedLineNparts;
	static const int dottedLineIndices = 6 * dottedLineNparts;

	//											forme					centre							points									contour				manipulationLine	manipulationPoint
	const int* numVertexShape;// = numVerticesPolygon	+		numVerticesRing +	 (numPointsPolygon * numVerticesCircle) +			numPointsPolygon + dottedLineVertexes		+ numVerticesRing;
	const int* shapeVertexBufferSize;// = 3 * numVerticesPolygon + (3 * numVerticesRing) + numPointsPolygon * (3 * numVerticesCircle) + (3 * numPointsPolygon) + 3 * dottedLineVertexes + 3 * numVerticesRing;
	const int* shapeColorBufferSize;// = 4 * (numVerticesPolygon + (numVerticesRing)+numPointsPolygon * (numVerticesCircle)+(numPointsPolygon)+dottedLineVertexes + numVerticesRing);
	const int* shapeIndicesSize;// = 3 * numVerticesPolygon + (3 * numVerticesRing) + numPointsPolygon * (3 * numPointCircle) + (3 * 2 * numPointsPolygon) + dottedLineIndices + (3 * numVerticesRing);

	static const int Npolygons = 10;
	const int* Nshapes;// = Npolygons + Npolygons * (Npolygons + 1) / 2;
	const int* vertexBufferSize;// = Nshapes * shapeVertexBufferSize;
	const int* colorBufferSize;// = Nshapes * shapeColorBufferSize;
	const int* indicesSize;// = Nshapes * shapeIndicesSize;


	int shift2[35+1]; // tableau contenant les séparations entre les différentes parties d'une forme (forme = 1, centre = 1, points = 32, contour = 1)

	GLfloat g_vertex_ring[3 * numVerticesRing];
	unsigned int ringIndices[3 * numVerticesRing];

	GLfloat g_vertex_circle[3 * numVerticesCircle];
	unsigned int circleIndices[3 * numPointCircle];

	GLuint vertexBuffer;
	GLfloat *g_vertex_buffer_data;//[vertexBufferSize]; // forme + vertex

	GLuint colorBuffer;
	GLfloat *g_color_buffer_data;// [colorBufferSize];// [3 * numVerticesPolygon + 3 * numVerticesRing];

	GLuint elementBuffer;
	unsigned int *indices;// [indicesSize];

	GLuint canvasOutlineVertexBuffer;
	GLfloat g_canvasOutlineVertex_buffer_data[8*3];

	GLuint canvasOutlineCoulourBuffer;
	GLfloat g_canvasOutlineCoulour_buffer_data[8 * 4];

	GLuint canvasOutlineIndexBuffer;
	unsigned int g_canvasOutlineIndex_buffer_data[24];

	void computeCanvasOutline();
	bool redrawCanvasOutline;

	
	GLfloat g_vertex_dotted_line[3 * dottedLineVertexes];
	GLuint g_indices_dotted_line[dottedLineIndices];

	void CreateShapeBuffer(std::shared_ptr<IDrawableArea> area, int positionInBuffer);

	
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


	std::atomic<bool> releaseResources;



    std::atomic<bool> destructionDone;
	std::thread openGLDestructionThread;
	void openGLDestructionFunc();

	public :
	void waitForOpenGLResourcesRealeased();
	private:
	void computeManipulationLine(float Ox, float Oy, float Mx, float My, float width, float height);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneCanvasComponent)
};


#endif  // SCENECANVASCOMPONENT_H_INCLUDED
