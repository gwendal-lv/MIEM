/*
  ==============================================================================

    DrawableEllipse.cpp
    Created: 2 Feb 2017 2:17:07pm
    Author:  ayup1

  ==============================================================================
*/

#include "DrawableEllipse.h"
#include "SceneCanvasComponent.h"

using namespace Miam;


DrawableEllipse::DrawableEllipse(int64_t _Id) :
	DrawableEllipse(_Id, bpt(0.5f, 0.5f),0.2f,0.2f,Colours::darkgrey)
{
	rotationAngle = 0;
}

DrawableEllipse::DrawableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio) :
	DrawableArea(_Id,_center,_fillColour), a(_a), b(_b)
{
	rotationAngle = 0;
	//float xScale, yScale;
	if (_canvasRatio > 1.0f) // ratio of an landscape-oriented window
	{
		xScale = 1.0f / _canvasRatio;
		yScale = 1.0f;
	}
	else // ratio of an portrait-oriented window
	{
		xScale = 1.0f;
		yScale = 1.0f*_canvasRatio;
	}

	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() + (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>() - (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(contourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));

	createJucePolygon();
}

void DrawableEllipse::createJucePolygon(int width, int height)
{
	contour.clear();

	if (keepRatio)
	{
		float newCanvasRatio = (float)width / (float)height;
		float newXScale;
		float newYScale;
		if (newCanvasRatio > 1.0f) // ratio of an landscape-oriented window
		{
			xScale = 1.0f / newCanvasRatio;
			yScale = 1.0f;
		}
		else // ratio of an portrait-oriented window
		{
			xScale = 1.0f;
			yScale = 1.0f*newCanvasRatio;
		}
		recreateContourPoints();
	}

	contour.addEllipse((float)center.get<0>() -((float)a*xScale/2), (float)center.get<1>() -((float)b*yScale/2), (float)a*xScale, (float)b*yScale);

	//contour.applyTransform(AffineTransform::rotation(rotationAngle,(float)center.get<0>(),(float)center.get<1>()));

	//if (keepRatio)
	//{
		//float newCanvasRatio = (float)width / (float)height;
		//float newXScale;
		//float newYScale;
		//if (newCanvasRatio > 1.0f) // ratio of an landscape-oriented window
		//{
		//	newXScale = 1.0f / newCanvasRatio;
		//	newYScale = 1.0f;
		//}
		//else // ratio of an portrait-oriented window
		//{
		//	newXScale = 1.0f;
		//	newYScale = 1.0f*newCanvasRatio;
		//}
		//if (xScale != newXScale)
		//	contour.applyTransform(AffineTransform::scale((float)height, (float)height));
		//else if(yScale != newYScale)
		//	contour.applyTransform(AffineTransform::scale((float)width, (float)width));
		//else
		//	contour.applyTransform(AffineTransform::scale((float)width, (float)height));
		//contour.applyTransform(AffineTransform::scale((float)width/2.0f, (float)height/2.0f));
	//}
	//else
		contour.applyTransform(AffineTransform::scale((float)width, (float)height));

	contour.applyTransform(AffineTransform::rotation(rotationAngle, (float)center.get<0>() * (float)width, (float)center.get<1>() * (float)height));
}

DrawableEllipse::~DrawableEllipse()
{
}

// Called by the parent component (which is a canvas)
void DrawableEllipse::Paint(Graphics& g)
{
	g.setColour(fillColour);
    g.setOpacity(enableLowOpacityMode ? getLowFillOpacity() : fillOpacity);
	g.fillPath(contour);

	g.setColour(contourColour);
    g.setOpacity(enableLowOpacityMode ? getLowFillOpacity() : fillOpacity);
	g.strokePath(contour, PathStrokeType(contourWidth));

	// Parent's drawings on top of these ones
	DrawableArea::Paint(g);
}

void DrawableEllipse::CanvasResized(SceneCanvasComponent* _parentCanvas)
{
	DrawableArea::CanvasResized(_parentCanvas);

	createJucePolygon(parentCanvas->getWidth(), parentCanvas->getHeight());
}

void DrawableEllipse::recreateContourPoints()
{
	bpolygon newContourPoints;
	boost::geometry::append(newContourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));
	boost::geometry::append(newContourPoints.outer(), bpt(center.get<0>() + (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(newContourPoints.outer(), bpt(center.get<0>(), center.get<1>() + (b / 2)*yScale));
	boost::geometry::append(newContourPoints.outer(), bpt(center.get<0>() - (a / 2)*xScale, center.get<1>()));
	boost::geometry::append(newContourPoints.outer(), bpt(center.get<0>(), center.get<1>() - (b / 2)*yScale));

	boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> Rot(rotationAngle);

	contourPoints.clear();
	boost::geometry::transform(newContourPoints, contourPoints, Rot);

}