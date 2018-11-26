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
        SceneCanvasComponent* parentCanvas; ///< Parent that draws this(needed to get display properties)
        
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
        static const uint8 lowFillOpacityUint8 = 40;
        virtual float getLowFillOpacity() const override { return (float)(lowFillOpacityUint8) / 255.0f; }
        
        Colour contourColour; ///< Solid color of of the external shape of the 2D area.
        float contourWidth; ///< Width (in pixels) of of the external shape of the 2D area.
        float centerContourWidth; ///< Width (in pixels) of the center circle drawing.
        
        int centerCircleRadius; ///< Radius (in pixels) of the center circle drawing.
        
        String name;
        bool isNameVisible;
        // Images bêtement comme ça pour l'instant...
        Image nameImage;
        Image nameImage2;
        static const int nameWidth = 120; // pixels
        static const int nameHeight = 15; // pixels
        
        bool keepRatio;
		bool areaVisible = true;
        
		// size of the different buffer parts
		static const int numPointsPolygon = 32;
		static const int numPointsRing = 32;
		static const int numPointCircle = 32;

		static const int numVerticesPolygon = numPointsPolygon + 1; // +1 pour le centre du polygone
		static const int numVerticesRing = 2 * numPointsRing;
		static const int numVerticesCircle = numPointCircle + 1; // +1 pour le centre du point
		
		int verticesBufferSize = 3 * numVerticesRing;//3 * numPointsPolygon + 3 * numVerticesRing; // par défaut : contour + centre
		int indicesBufferSize = 3 * numVerticesRing;//3 * 2 * numPointsPolygon + 3 * numVerticesRing;
		int couloursBufferSize = 4 * numVerticesRing;

        std::vector<GLfloat> g_vertex_ring;
        std::vector<unsigned int> ringIndices;
		float mainZoffset;

		// buffer used by OpenGL
		std::vector<GLfloat> vertices_buffer;
		std::vector<GLuint> indices_buffer;
		std::vector<GLfloat> coulours_buffer;
        
        // =============== SETTERS & GETTERS ===============
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
		void ComputeRing(int numPoints);
        void resetImages();
        void renderCachedNameImages();
        public :
        
			const int GetVerticesBufferSize() override { return numVerticesRing; }
        
        
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

			int GetIndicesBufferSize() override { return  3 * numVerticesRing; }

			int GetCouloursBufferSize() { return 4 * numVerticesRing; }

			void setZoffset(const float newOffset) override
			{
				mainZoffset = newOffset;
			}
        
        virtual void Paint(Graphics& g) override;
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
		virtual void RefreshOpenGLBuffers() override;

        // - - - - - XML import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
        
    };
    
}



#endif  // DRAWABLEAREA_H_INCLUDED
