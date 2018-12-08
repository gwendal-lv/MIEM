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

		private:
			void createJucePolygon(int width = 160, int height = 90);
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
        // ATTENTION a et b ne sont pas le grand rayon et le petit rayon
        // (demi grand axe et demi petit axe)... à corriger.
			double a, b; // grand axe et petit axe
			double rotationAngle; // angle d'inclinaison de l'ellipse
			float xScale, yScale;

			virtual double computeXScale(float _canvasRatio);
			virtual double computeYScale(float _canvasRatio);


	};
}



#endif  // DRAWABLEELLIPSE_H_INCLUDED
