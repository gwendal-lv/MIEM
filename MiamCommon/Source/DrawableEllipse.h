/*
  ==============================================================================

    DrawableEllipse.h
    Created: 2 Feb 2017 2:17:07pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef DRAWABLEELLIPSE_H_INCLUDED
#define DRAWABLEELLIPSE_H_INCLUDED

#include "JuceHeader.h"
#include "DrawableArea.h"

namespace Miam
{
	class DrawableEllipse : public DrawableArea
	{
        // =============== ATTRIBUTES ===============
        
        private :
        /// \brief Nombre de points définissant la forme pseudo-elliptique dessinée
        /// par Juce via OpenGL. Désactivé : on utlisera uniquement la référence polygone.
        //const int ellipsePointsCount = numPointsPolygon;
        
        
		protected :
        bpolygon contourPoints;
        Path contour;

        /// \brief DEMI grand axe et DEMI petit axe. Relative sizes, percentage of parent canvas
        double a = 0.0;
        double b = 0.0;
        /// \brief HALF big and small axes length, in pixels
        int aInPixels = 0;
        int bInPixels = 0;
        
        double rotationAngle = 0.0; // angle d'inclinaison de l'ellipse
		float xScale = 1.0;
		float yScale = 1.0;

        
        
        // =============== SETTERS & GETTERS ===============
        public :
        
        // ----- VBO sizes -----
        // on ajoute la surface de la forme + les lignes extérieures
        virtual int GetVerticesBufferElementsCount() const override
        { return DrawableArea::GetVerticesBufferElementsCount()
            + numVerticesPolygon // car même nombre de points
            + numVerticesRing; } // contour ~= ring en fait
        virtual int GetIndicesBufferElementsCount() const override
        { return DrawableArea::GetIndicesBufferElementsCount()
            + 3 * numPointsPolygon // surface de l'ellipse (idem polygone)
            + 3 * numVerticesRing; } // contour
        
        
        
        // =============== METHODS ===============
		public :
        DrawableEllipse(bptree::ptree & areaTree);
        DrawableEllipse(int64_t _Id);
        DrawableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio = 1.77777777f);
        DrawableEllipse(int64_t _Id, bpt _center, int _radiusInPixels, Colour _fillColour);

		private:
        void createJucePolygon(int width = 160, int height = 90);
        /// \brief Performs general graphical initialisations
        void init();
        /// \brief Inits the sub-parts of the index buffers that will never change.
        void initSubBuffers();

	public:
		/// \brief Destructor.
		virtual ~DrawableEllipse();
        
        /// \brief Clonage
        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<DrawableEllipse>(*this);
            clone->onCloned();
            return clone;
        }
        
        // - - - - - XML import/export - - - - -
        /// \returns "Ellipse" even if it is actually a circle.
        virtual std::string GetTypeAsString() const override {return "Ellipse";};
        virtual std::shared_ptr<bptree::ptree> GetTree() override;


		// Display functions
	public:
		virtual void Paint(Graphics& g) override;
		virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
		virtual void RefreshOpenGLBuffers() override;
	
	private:
		void recreateContourPoints(int width, int height);

		protected:
        virtual double computeXScale(float _canvasRatio);
        virtual double computeYScale(float _canvasRatio);


	};
}



#endif  // DRAWABLEELLIPSE_H_INCLUDED
