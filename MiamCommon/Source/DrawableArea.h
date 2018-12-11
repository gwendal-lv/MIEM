/*
  ==============================================================================

    DrawableArea.h
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef DRAWABLEAREA_H_INCLUDED
#define DRAWABLEAREA_H_INCLUDED

#include "JuceHeader.h"

#include "IDrawableArea.h"

#define MIEM_CENTRAL_RING_Z         (-0.1f) // negative values are in front
#define MIEM_SHAPE_SURFACE_Z        (0.0f)
#define MIEM_SHAPE_CONTOUR_Z        (-0.1f)

#define MIEM_UNVISIBLE_COORDINATE   (-10.0f)

// Simple declaration for a pointer
class SceneCanvasComponent;


namespace Miam
{
    
    /// \brief Abstract class from which all Miam areas will be derived.
    ///
    /// Common graphics managing features for all possible areas.
    class DrawableArea : public virtual IDrawableArea
    {
        
        // =============== ATTRIBUTS ===============
        
        // General data
        protected :
        uint64 Id; ///< Unique ID
        SceneCanvasComponent* parentCanvas = 0; ///< Parent that draws this(needed to get display properties)
        
        // Geometric data
        protected :
        bpt center; ///< Normalized center coordinates (x, y in [0.0,1.0])
        bpt centerInPixels; ///< Center coordinates in pixels (relative to a canvas)
        bool displayCenter;
        bpolygon contourPointsInPixels; ///< Caracteristic points (coordinates in pixels) along the contour of the area (not necessarily drawn). Last point is a duplicate of the first for closed polygons.
        
        // Display data
        protected :
        
        double renderingScale;
        /// \brief Pour tout ce qui est en cache, on fait de base du rendu
        /// en taille x2 mais aussi en taille x1 maintenant.
        // Ensuite, si besoin, on fait un resize au moment du rendu final OpenGL
        /// (c'est Juce qui se débrouille avec...)
        // const double baseRenderingScale = 2.0; // plus utilisé...
        
        Colour fillColour; ///< Solid colour for filling the area (opacity should be 0xFF)
        float fillOpacity; ///< Opacity applied to the solid fill colour (in [0.0,1.0])
        bool isFilled = true; // correction bug suite fusion OpenGL... Ok ?
        OpacityMode opacityMode;
        
        /// \brief The lowest opacity of a displayed area
        const uint8 lowFillOpacityUint8 = 40;
        virtual float getLowFillOpacity() const override { return (float)(lowFillOpacityUint8) / 255.0f; }
        
        Colour contourColour; ///< Solid color of of the external shape of the 2D area.
        float contourWidth; ///< Width (in pixels) of of the external shape of the 2D area.
        float centerContourWidth; ///< Width (in pixels) of the center circle drawing.
        
        int centerCircleRadius; ///< Radius (in pixels) of the center circle drawing.
        
        String name;
        bool isNameVisible;
        // - - - Juce Images for CPU rendering - - -
        Image nameImage; // In VBO-mode : will be default constructed only
        Image nameImage2; // (null image)
        // - - - Advanced custom object for OpenGL/VBO/texture rendering - - -
        // Warning : contents of the pointer will be multi-threaded accessed.
        // After each modification, a new text object must be created (and this should be the only
        // access to this text object from the Juce Message thread).
        //
        // The OpenGL Thread will then read the contents of the object
        // for GPU-based rendering.
        std::shared_ptr<OpenGLTextObject> glTextObject;

        const int nameWidth = 200; // pixels (fera souvent moins que ça en pratique)
        const int nameHeight = 25; // pixels
        const unsigned int nameCharsCountMax = 15; //
        
        bool keepRatio;
		bool areaVisible = true;
        
         
        Vector<GLfloat> g_vertex_ring;
        Vector<GLuint> ringIndices;
        
		float mainZoffset;

		// Buffers to be copied in OpenGL VBOs
        // All child classes will enlarge and fill them with their own
        // data.
		Vector<GLfloat> vertices_buffer;
		Vector<GLuint> indices_buffer;
		Vector<GLfloat> coulours_buffer;
        
        // =============== SETTERS & GETTERS ===============
        protected :
        // ----- Internal reference-based getters (to solve inheritance issues) -----
        virtual bpt& getCenterInPixels() override { return centerInPixels; }
        virtual std::vector<bpt>& getContourPointsInPixels() override { return contourPointsInPixels.outer(); }
        virtual SceneCanvasComponent* getParentCanvas() override {
            assert(parentCanvas != 0);
            return parentCanvas;
        }
        virtual Vector<GLfloat> & getRingVertexBuffer() override { return g_vertex_ring; }
        virtual Vector<GLuint> & getRingIndexBuffer() override { return ringIndices; }
        
        public : // temporaire ???? risque de rester longtemps...
        virtual Vector<GLfloat> & getVerticesBuffer() override { return vertices_buffer; }
        virtual Vector<GLuint> & getIndicesBuffer() override { return indices_buffer; }
        virtual Vector<GLfloat> & getColoursBuffer() override { return coulours_buffer; }
        
        public :
        // ----- VBO sizes and elements' counts -----
        virtual int GetVerticesBufferElementsCount() const override
        { return numVerticesRing; } // only the central donut is common to any DrawableArea
        
        virtual int GetIndicesBufferElementsCount() const override
        { return  3 * numVerticesRing; } // equal number of vertices and triangles for the central ring
      

        
        virtual void setZoffset(float newOffset) override;
        virtual float getZoffset() const override {return mainZoffset;}
        
        public :
        /// \returns Unique ID of the area
        virtual int64_t GetId() const override {return Id;}
        /// \param _Id Unique ID of the area
        virtual void SetId(int64_t _Id) override {Id = _Id;}
        /// \returns See DrawableArea::fillColour
        virtual Colour GetFillColour() const override {return fillColour;}
        /// \param _fillColour See DrawableArea::fillColour
        virtual void SetFillColour(Colour newColour) override;
        
        void setIsFilled(bool shouldBeFilled);
        
        /// \param _fillOpacity See DrawableArea::fillOpacity
        virtual void SetAlpha(float newAlpha) override;
        virtual float GetAlpha() const override;
        
        
        virtual void SetOpacityMode(OpacityMode opacityMode_) override;
        virtual OpacityMode GetOpacityMode() const override {return opacityMode;}

		void setVisible(bool shoulBeVisible) override { areaVisible = shoulBeVisible; }
        bool isVisible() override { return areaVisible; }
        virtual bool IsNameVisible() const override {return isNameVisible;}
        virtual std::shared_ptr<OpenGLTextObject> GetGLTextObject() override { return glTextObject; }
        
        /// \brief Sets the name that could be displayed on screen next to the center
        virtual void SetName(String newName) override;
        
        void SetNameVisible(bool isVisible) {isNameVisible = isVisible;}
        
        void KeepRatio(bool _keepRatio);
        
        /// \brief Pour régler les problèmes d'écrans type "rétina"
        virtual void SetRenderingScale(double renderingScale_) override;
        
        // =============== MÉTHODES ===============

        
        public :
        
        /// \Brief Contruction from the unique ID, center, and fill colour that must
        /// be found in the given <area> sub-tree inner content.
        DrawableArea(bptree::ptree& areaTree);
        
        /// \brief Minimal constructor when building a new area
        ///
        /// \param _Id Unique ID of the area
        /// \param _center Normalized center coordinates (x, y in [0.0,1.0])
        /// \param _fillColour Solid colour for filling the area (opacity should be 0xFF)
		DrawableArea(int64_t _Id, bpt _center, Colour _fillColour);
        
        /// \brief Virtual destructor.
        virtual ~DrawableArea();
        
        // Clonage : rien de spé à faire ici...
        protected :
        virtual void onCloned() override {}
        
        private :
        void init();
        
        /// \brief Computes a ring made of numPointRing vertices for the internal circle,
        /// and other numPointRing vertices for the internal circle.
		void ComputeRing();
        
        void resetImages();
        void renderCachedNameImages();
        void resetTextureBasedName();
        public :
        
        /*
			float* GetVerticesBufferPtr() override
			{
				return vertices_buffer.data();
			}
			float* GetCoulourBufferPtr() override
			{
				return coulours_buffer.data();
			}
			unsigned int* GetIndicesBufferPtr() override
			{
				return indices_buffer.data();
			}
*/
        
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
		virtual void RefreshOpenGLBuffers() override;
        
        protected :
        void initSurfaceAndContourIndexSubBuffer(int vertexElmtOffset,
                                                 int indexElmtOffset,
                                                 int actualPointsCount);
        void initSurfaceAndContourColourSubBuffer(int vertexElmtOffset);

        // - - - - - XML import/export - - - - -
        public :
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
        
    };
    
}



#endif  // DRAWABLEAREA_H_INCLUDED
