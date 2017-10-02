/*
  ==============================================================================

    MouseSimulator.cpp
    Created: 29 Sep 2017 2:24:49pm
    Author:  ayup1

  ==============================================================================
*/

#include "MouseSimulator.h"
#include <memory>
#include <chrono>
#include "AmusingScene.h"
#include "CompletePolygon.h"
#include "juce_gui_basics\juce_gui_basics.h"

class juce::MouseInputSourceInternal;

using namespace Amusing;

MouseSimulator::MouseSimulator(SceneCanvasComponent *m_sceneComponent, std::weak_ptr<MultiSceneCanvasInteractor> canvasManager)
{
	/// write the events to execute here

	/// end of the events

	// start of the thead
	runThread = true;
	T = std::thread(&MouseSimulator::executeEvents, this);
}

MouseSimulator::~MouseSimulator()
{
	runThread = false;
	T.join();
}

void MouseSimulator::addTranslation(int areaId, bpt translation, int eventTime, float speed)
{
	if (auto canvasManager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasInteractor.lock()))
	{
		if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(canvasManager->GetSelectedScene()))
		{
			if (auto completePolygon = std::dynamic_pointer_cast<CompletePolygon>(amusingScene->GetEditableArea(areaId)))
			{
				// transform the Translation in mouseDown and mouseUp
				//completePolygon->getPolygon() from polygon and center -> choose a point to click down and the point to click up
				bpolygon poly = completePolygon->getPolygon();
				bpt center = completePolygon->getCenter();

				// mouse down point : middle of the segment center-poly[1]
				bpt midPt = poly.outer().at(1);
				boost::geometry::subtract_point(midPt, center);
				boost::geometry::divide_value(midPt, 2.0);
				boost::geometry::add_point(midPt, center);
				addMouseDown(Point<float>((double)midPt.get<0>(),(double)midPt.get<1>()),eventTime);

				// mouse up point : mouse down + translation
				int upTime = eventTime + round( boost::geometry::length(translation) / speed);
				boost::geometry::add_point(midPt, translation);
				addMouseUp(Point<float>((double)midPt.get<0>(), (double)midPt.get<1>()), upTime, 0.001, false);
			}
		}
	}
}

void MouseSimulator::addRotation(int areaId, float rotation, int eventTime, float speed)
{
	if (auto canvasManager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasInteractor.lock()))
	{
		if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(canvasManager->GetSelectedScene()))
		{
			if (auto completePolygon = std::dynamic_pointer_cast<CompletePolygon>(amusingScene->GetEditableArea(areaId)))
			{
				// transform the Rotation in mouseDown and mouseUp
			}
		}
	}
}

void MouseSimulator::addMouseDown(Point<float> position, int eventTime)
{
	juce::Time t();
	
	
	auto mainSource = Desktop::getInstance().getMainMouseSource();
	const juce::MouseEvent me(mainSource
		, Point<float>(0,0), juce::ModifierKeys::Flags::noModifiers, MouseInputSource::invalidPressure,
		MouseInputSource::invalidOrientation, MouseInputSource::invalidRotation,
		MouseInputSource::invalidTiltX, MouseInputSource::invalidTiltY,
		this, this, juce::Time((int64)eventTime), Point<float>(0, 0), juce::Time((int64)eventTime), 0,false);
	
	
	events.push_back(me);
}

void MouseSimulator::addMouseUp(Point<float> position, int eventTime, float incD, bool)
{
	// add a mouseUp event and search for the previous mouseDown to create some mouseDrag event between
}

void MouseSimulator::executeEvents()
{
	while (runThread)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // period of the timer = 100ms
	}
}