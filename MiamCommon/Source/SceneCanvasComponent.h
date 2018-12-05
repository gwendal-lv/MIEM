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
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

#include "JuceHeader.h"

#include "MiemVector.hpp"
#include "FrequencyMeasurer.h"

#include "DrawableArea.h"
#include "OpenGLTextObject.h"




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
    
    // - - - - - Time measures - - - - -
    const double desiredFrequency_Hz = 60.0; // actual freq will actually be greater
    const double desiredPeriod_ms = 1000.0/desiredFrequency_Hz;
    FrequencyMeasurer displayFrequencyMeasurer;
    
    int numFrame;
    double EunderTime;
    
    // - - - - - Areas duplicates for multi-threaded rendering - - - - -
    
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
    
    
    // - - - - - OpenGL - - - - -
    OpenGLContext openGlContext;
    const int swapInterval = 1; // synced on vertical frequency
    bool isSwapSynced;
    
    std::unique_ptr<OpenGLShaderProgram::Attribute> positionShaderAttribute, colourShaderAttribute;
    
    std::unique_ptr<OpenGLTextObject> openGLLabel;
    
    std::atomic<bool> releaseResources;
    std::atomic<bool> releaseDone;
    std::mutex rendering_mutex;
    
    std::thread openGLDestructionThread;
    
    std::unique_ptr<OpenGLShaderProgram> shaderProgram;
    std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, modelMatrix;
    
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
    
    
    // - - - General Vertex, Colour and Element Buffer Objects and Data - - -
    
    // à dégager à terme (seule les formes devront préciser leur taille !!!!!!!!)
    int numVertexShape;
    int shapeVertexBufferSize;
    int shapeColorBufferSize;
    int shapeIndicesSize;
    
    // Précédemment : version avec des pointeurs constant... c'était un peu n'importe quoi...
    int Nshapes;
    // à mettre dans une structure (ou un vecteur...)
    /*
    int vertexBufferSize = 0;
    int colorBufferSize = 0;
    int indicesSize = 0;
     */
    
    
    GLuint vertexBufferGlName;
    Vector<GLfloat> sceneVertexBufferData;
    GLuint colorBufferGlName;
    Vector<GLfloat> sceneColourBufferData;
    GLuint elementBufferGlName;
    Vector<GLuint> sceneIndicesBufferData;
    
    // Next buffer position to be written (= last filled data pos + 1)
    size_t currentVertexBufferArrayPos = 0;
    size_t currentColourBufferArrayPos = 0;
    size_t currentIndexBufferArrayPos = 0;
    
    
    
    // - - - Local buffers for the scene itself - - -
    
    GLuint canvasOutlineVertexBufferName;
    GLfloat g_canvasOutlineVertex_buffer_data[8*3];
    GLuint canvasOutlineCoulourBufferName;
    GLfloat g_canvasOutlineCoulour_buffer_data[8 * 4];
    GLuint canvasOutlineIndexBufferName;
    GLuint g_canvasOutlineIndex_buffer_data[24];
    
    
    
    
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    
    SceneCanvasComponent();
    
    /// \brief Initialisation pour OpenGL, avec des tailles max de VBO
    ///
    /// Nombre de points max par forme correspond au nombre de points
    /// du contour. Le nombre de points des VBO OpenGL sera potentiellement
    /// beaucoup plus grand que ce paramètre.
    /// Le nombre de points max par forme est important pour les ellipses par exemple.
	SceneCanvasComponent(int numShapesMax, int numPointsMax = 64);

    
	void init(int numShapesMax, int numPointsMax);
    private :
    /// \brief tests first if buffers are null or not
    void deleteBuffers();
    public :
	void ReleaseOpengGLResources();

    ~SceneCanvasComponent();
    
	/// \brief Also called from Miam::View::CompleteInitialization
    void CompleteInitialization(std::shared_ptr<MultiSceneCanvasInteractor> _canvasManager);

    // - - - - - - - - Juce usual paint/resized component methods - - - - - - - - -
    void resized() override;
    
    // - - - - - - - - OpenGL specific - - - - - - - - -
    virtual void newOpenGLContextCreated() override;
    virtual void renderOpenGL() override; // ! in background-thread !
	virtual void openGLDestructionAtLastFrame();
    
    /// \brief Draws the shapes only, and not the text on them (names, etc...)
    ///
    /// Cette fonction s'occupe du dessin sur le canevas, le texte se fait après, dans le renderOpenGL
	virtual void DrawOnSceneCanevas();
	void DrawShapes();
    /// \brief Will be drawn using separate VBOs and IBOs
	void DrawCanvasOutline();
    virtual void openGLContextClosing() override;
    
    void SetupGLResources();
    void ReleaseGLResources();
    
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

		return orthoProj;

	}

    
    
    //  - - - - - Méthodes VBOs - - - - -
    private :
    
    void computeCanvasOutline();
    bool redrawCanvasOutline;
    
    /// \brief à renommer/refactorer : fait un FILL en réalité (le buffer reste créé)
    // argument POSITION IN BUFFER doit etre SUPPRIME (il faudra garder un compteur interne)
	void AddShapeToBuffers(std::shared_ptr<IDrawableArea> area);

	
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


	public :
		virtual void openGLDestructionAfterLastFrame();
		void waitForOpenGLResourcesRealeased();
	private:
	void computeManipulationLine(float Ox, float Oy, float Mx, float My, float width, float height);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneCanvasComponent)
};


#endif  // SCENECANVASCOMPONENT_H_INCLUDED
