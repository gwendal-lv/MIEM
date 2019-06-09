/*
  ==============================================================================

    IDrawableArea.h
    Created: 7 Oct 2016 11:10:46am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/



// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =
// Warning d'h�ritage virtuel... Alors que c'est pr�vu normalement en C++11 !
#ifdef _MSC_VER // indique que c'est une version de visual studio
	#pragma warning( disable : 4250 )
#endif
// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =




#ifndef IDRAWABLEAREA_H_INCLUDED
#define IDRAWABLEAREA_H_INCLUDED

#include <memory>

#include "JuceHeader.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/polygon.hpp"
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;
typedef boost::geometry::model::polygon<bpt> bpolygon;

#include "OpenGLTextObject.h"

#include "MiemVector.hpp"

// Simple declaration for a pointer
class SceneCanvasComponent;



namespace Miam
{
    // Mise ici parce que les aires seulement sont concern�es par l'opacit�...
    enum class OpacityMode {
        Independent,
        DependingOnExcitement,
        
        Low, ///< Lowest possible opacity for an area
        Mid, ///< Medium opacity
        High ///< High opacity, but not totally opaque
    };
    
    
    /// /brief For menus creation by IDs, etc
    struct AreaDefaultType
    {
        
        enum : int
        {
            // Corresponds the return value of a Juce popup menu,
            // when nothing was selected
            None = 0,
            
            Ellipse,
            
            // Number of sides minimum = 3
            Polygon = 3,
            // Such that Polygon+1 is a square
            // Polygon+2 is a pentagon, etc...
        };
    };
    
    
    /// \brief Interface of the Miam::DrawableArea (which is an abtract object)
    class IDrawableArea
    {
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        IDrawableArea(){}
        virtual ~IDrawableArea() {}
        
        /// \brief To be overriden within any concrete area that inherits from this.
        virtual std::shared_ptr<IDrawableArea> Clone() = 0;
        protected :
        /// \brief Applique toutes les modifications n�cessaires suite au clonage d'un objet
        ///
        /// Par exemple : supprime les liens avec d'autres objets graphiques (s'ils existent),
        /// en supposant que ces liens ont �t� copi�s
        /// simplement lors d'un clonage qui vient tout juste d'�tre fait.
        ///
        /// \warning To be called from any Clone() overriden method
        virtual void onCloned() = 0;
        
        
        
        public :
        /// \brief Function called by a SceneCanvasComponent to draw the Area
        ///
        /// The whole painting job is at the moment performed by the CPU only, but should be
        /// replaced by OpenGL drawing soon.
        virtual void Paint(Graphics& g) = 0;
        /// \brief This function should be called when the area is associated to a (new) canvas,
        /// when its canvas is resized, or when a drawing refresh is needed.
        ///
        /// It updates real-screen pixel coordinates from internally stored normalized coordinates
        /// of all points.
        virtual void CanvasResized(SceneCanvasComponent* parentCanvas) = 0;
        
		/// \brief This function is used to refresh the vertices, the indices, and the coulours buffers
		/// before sending them to OpenGL renderer
		virtual void RefreshOpenGLBuffers() = 0;
		
		//virtual Matrix3D<float> GetModelMatrix() = 0;
        
        // =============== SETTERS & GETTERS ===============
        public :
        /// \brief Returns the position (in pixels) of the center of the shape
        virtual bpt GetCenterPosition() const = 0; // public copy
        protected :
        // ----- Internal reference-based getters (to solve inheritance issues) -----
        virtual bpt& getCenterInPixels() = 0;
        virtual std::vector<bpt>& getContourPointsInPixels() = 0;
        virtual SceneCanvasComponent* getParentCanvas() = 0;
        virtual Vector<GLfloat> & getRingVertexBuffer() = 0;
        virtual Vector<GLuint> & getRingIndexBuffer() = 0;
        public : // temp ?
        virtual Vector<GLfloat> & getVerticesBuffer() = 0;
        virtual Vector<GLuint> & getIndicesBuffer() = 0;
        virtual Vector<GLfloat> & getColoursBuffer() = 0;
        
        // ----- Public Sets and Gets -----
        public :
        virtual int64_t GetId() const = 0;
        virtual void SetId(int64_t _Id) = 0;
        virtual Colour GetFillColour() const = 0;
        virtual void SetFillColour(Colour newColour) = 0;
		virtual void SetAlpha(float newAlpha) = 0;
        virtual float GetAlpha() const = 0;
        virtual void SetOpacityMode(OpacityMode enable) = 0;
        virtual OpacityMode GetOpacityMode() const = 0;
        virtual void SetRenderingScale(double renderingScale_) = 0;

        /*
		virtual float* GetVerticesBufferPtr() = 0;
		virtual float* GetCoulourBufferPtr() = 0;
		virtual unsigned int* GetIndicesBufferPtr() = 0;
         */

		virtual void setVisible(bool shouldBeVisible) = 0;
		virtual bool isVisible() = 0;
        virtual bool IsNameVisible() const = 0;
        virtual std::shared_ptr<OpenGLTextObject> GetGLTextObject() = 0;



        // - - - - - Constant caracteristic values for VBOs - - - - -
        protected :
        // size of the different buffer parts
        // (Not static for multi-threading safety)
#ifdef JUCE_ANDROID
        // lower values for Android... Because low perfs with Juce
        // VERY VERY LOW VALUES, JUST FOR TEST ON ANDROID
        const int ringResolution = 8;
#else
        // actual resolution of any donut ring, for good performances for medium object such as exciters
        const int ringResolution = 16;

#endif
        // Common values for all platforms :

        // Max number of contour points of a polygon... AND actual number of points of an ellipse ?
        const int numPointsPolygon = 24; // must be the same for all platforms
        // WARNING there is a remaining issue in the OpenGL rendering code...
        // the ringResolution and the numPointsPolygon are mixed at some point...
        // So they SHOULD REMAIN THE SAME, or else OpenGL should be debuggued
        // to get the bug : numPointsPOlygon ==  24 and ringResolution == 12
        // -> OK, bug must be solved... to be properly tested
        const int numPointsPolygonContour = 2 * numPointsPolygon; // 2 polygons filled

        const int numPointCircle = 32; // actual resolution of any circle (not often used, small circles are preferred)

        // Small circles : around 10 points
        const int numVerticesPolygon = numPointsPolygon + 1; // +1 pour le centre du polygone
        const int numVerticesRing = 2 * ringResolution; // donut = 2 circles of 32 points
        const int numVerticesCircle = numPointCircle + 1; // +1 pour le centre du disque



        // - - - - - VBO methods - - - - -
        // - - - Following getters return the number of elements or array size of each kind of OpenGL buffer
        public :
        
        /// \brief Returns the number of vertices within the buffer. Each  vertex is made of several coordinates.
        /// To get the size of the buffer, please see Miam::DrawableArea::GetVerticesBufferSize()
		virtual int GetVerticesBufferElementsCount() const = 0;
        virtual int GetVerticesBufferActualElementsCount() const
        { return GetVerticesBufferElementsCount(); } // default behavior, to be overriden
        
        /// \brief Returns the number of indices within the buffer. For example, 1 triangles requires
        /// a total count of 3 indices. The number of indices should be the same as the size of the buffer.
		virtual int GetIndicesBufferElementsCount() const = 0;
        /// \brief The actual (variable) number of elements required to draw the whole shape
        virtual int GetIndicesBufferActualElementsCount() const
        { return GetIndicesBufferElementsCount(); } // default behavior, to be overriden
        
        //int GetColoursBufferElementsCount() { return 1 * GetVerticesBufferElementsCount(); }
        // deleted for security reasons....  issues with virtual functions !!!
        // Impossible telle qu'écrite comme ça de récupérer le nb de couleur d'une classe mère en particulier
        
        
        // - - - Following functions should not be overriden. Arrays' sizes are always proportionnal
        // to the number of contained elements !
    
        int GetVerticesBufferSize() const {return 3 * GetVerticesBufferElementsCount(); } // x, y, z float coords
        int GetIndicesBufferSize() const {return GetIndicesBufferElementsCount(); } // 1 index is a 1D coordinate
        int GetColoursBufferSize() const {return 4 * GetVerticesBufferElementsCount(); } // ARGB float coords for each vertex
        
        
        virtual void setZoffset(float newOffset) = 0;
        virtual float getZoffset() const = 0;
        
        /// \brief Sets the name that could be displayed on screen next to the center
        virtual void SetName(String newName) = 0;
        protected :
        virtual float getLowFillOpacity() const = 0;
        public :
        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const = 0;
        virtual std::shared_ptr<bptree::ptree> GetTree() = 0;
        // XML import is made directly from the constructor
        
    };
    
    
}


#endif  // IDRAWABLEAREA_H_INCLUDED
