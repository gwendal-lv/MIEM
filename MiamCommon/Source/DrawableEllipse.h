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

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/polygon.hpp"

//typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;
typedef boost::geometry::model::polygon<bpt> bpolygon;

namespace Miam
{
	class DrawableEllipse : public DrawableArea
	{
        private :
        /// \brief Nombre de points définissant la forme pseudo-elliptique dessinée
        /// par Juce via OpenGL
        int ellipseVerticesCount = 32;
        
		protected :
			bpolygon contourPoints;
			Path contour;

		public :
            DrawableEllipse(bptree::ptree & areaTree);
			DrawableEllipse(int64_t _Id);
			DrawableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio = 1.77777777f);

			int GetOpaqueVerticesCount() override {
				return DrawableArea::GetOpaqueVerticesCount() + numVerticesPolygon + numPointsPolygon;
			};

			int GetIndexCount() override;
			bool hasVerticesChanged() override;
			bool hasPositionChanged() override;
			Vector3D<float> GetModelParameters() override;

		private:
			void createJucePolygon(int width = 160, int height = 90);

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
		void setVerticesCount(int newVerticesCount);
		void setIsFilled(bool shouldBeFilled);
		virtual void Paint(Graphics& g) override;
		virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
	
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

			bool isFilled;

	};
}



#endif  // DRAWABLEELLIPSE_H_INCLUDED
