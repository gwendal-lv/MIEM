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

MouseSimulator::MouseSimulator(SceneCanvasComponent *m_sceneComponent, std::weak_ptr<MultiSceneCanvasInteractor> canvasManager) :
	sceneComponent(m_sceneComponent), canvasInteractor(canvasManager)
{
	/// write the events to execute here
	addClick(Point<float>(267.0, 178.0), 1000);
	addTranslation(0, bpt(-0.1,-0.1), 1200,1.0); // speed = 1screen/s
	/// end of the events

	// start of the thead
	Tposition = (int64)0;
	runThread = true;
	T = std::thread(&MouseSimulator::executeEvents, this);
}

MouseSimulator::~MouseSimulator()
{
	runThread = false;
	T.join();
}

void MouseSimulator::addClick(Point<float> location, int eventTime)
{
	addMouseDown(location, eventTime);
	addMouseUp(location, eventTime + 10,0,false);
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
				boost::geometry::divide_point(center, bpt((double)sceneComponent->getWidth(), (double)sceneComponent->getHeight()));

				// mouse down point : middle of the segment center-poly[1]
				bpt midPt = poly.outer().at(1);
				boost::geometry::subtract_point(midPt, center);
				boost::geometry::divide_value(midPt, 2.0);
				boost::geometry::add_point(midPt, center);
				addMouseDown(Point<float>((double)midPt.get<0>() * sceneComponent->getWidth(),(double)midPt.get<1>()* sceneComponent->getHeight()),eventTime);

				// mouse up point : mouse down + translation
				
				
				int incT = round(boost::geometry::distance(translation, bpt(0, 0)) / (speed /1000.0f));
				int upTime = eventTime + incT;
				boost::geometry::add_point(midPt, translation);
				addMouseUp(Point<float>((double)midPt.get<0>()* sceneComponent->getWidth(), (double)midPt.get<1>() * sceneComponent->getHeight()), upTime, 2, false);
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

				//mouseDown point = manipulation point
				Point<float> manipulationPt = completePolygon->getManipulationPoint();
				addMouseDown(manipulationPt, eventTime);

				// mouseUp point = manipulation point rotated by the angle
				bpt center = completePolygon->getCenter();
				bpt bmanipulationPt(manipulationPt.getX(), manipulationPt.getY());
				boost::geometry::subtract_point(bmanipulationPt, center);
				bpt rotatedManipulationPt;
				boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> Rot(rotation);
				boost::geometry::transform(bmanipulationPt, rotatedManipulationPt, Rot);
				boost::geometry::add_point(rotatedManipulationPt, center);

				Point<float> pivot(center.get<0>(), center.get<1>());
				Point<float> position(rotatedManipulationPt.get<0>(), rotatedManipulationPt.get<1>());

				boost::geometry::divide_point(center, bpt((double)sceneComponent->getWidth(), (double)sceneComponent->getHeight()));
				double arc = rotation * boost::geometry::distance(bmanipulationPt, center); // pas en pixel car vitesse en ecran/seconde
				int incT = round(arc / (speed / 1000.0f));
				int upTime = eventTime + incT;
				addMouseUpRotate(position, pivot, upTime, 2, false);
			}
		}
	}
}

void MouseSimulator::addMouseDown(Point<float> position, int _eventTime)
{
	juce::Time t();
	
	
	juce::ModifierKeys modifier(juce::ModifierKeys::Flags::leftButtonModifier);
	int numOfClicks = 1;
	juce::Time eventTime((int64)_eventTime);
	bool wasDragged = false;

	auto mainSource = Desktop::getInstance().getMainMouseSource();
	const juce::MouseEvent me(mainSource
		, position, modifier, MouseInputSource::invalidPressure,
		MouseInputSource::invalidOrientation, MouseInputSource::invalidRotation,
		MouseInputSource::invalidTiltX, MouseInputSource::invalidTiltY,
		this, this, eventTime, position, eventTime, numOfClicks,wasDragged);
	
	
	events.push_back(me);
	types.push_back(isMouseDown);
}

void MouseSimulator::addMouseUp(Point<float> position, int _eventTime, float incD, bool random)
{
	auto mainSource = Desktop::getInstance().getMainMouseSource();
	bool wasDragged = false;
	// add a mouseUp event and search for the previous mouseDown to create some mouseDrag event between
	
	MouseEvent prevEvt(events.back());
	
	if (types.back() == isMouseDown && (prevEvt.position != position))
	{
		// previous was mouseDown -> need to create some MouseDrag between the two event
		wasDragged = true;
		juce::ModifierKeys modifier(juce::ModifierKeys::Flags::leftButtonModifier);
		int numOfClicks = 0;
		float distance = position.getDistanceFrom(prevEvt.position);
		
		int numOfDragEvents = roundToInt(distance / incD);
		Point<float> inc = (position - prevEvt.position) / (float)numOfDragEvents;
		
		int64 incT = round( ((int64)_eventTime - prevEvt.eventTime.toMilliseconds()) / (int64)numOfDragEvents);
		for (int i = 0; i < numOfDragEvents-1; ++i)
		{
			Point<float> dragPosition = (inc * (float)(i + 1));
			dragPosition.addXY(prevEvt.position.x, prevEvt.position.y);
			int64 t = prevEvt.eventTime.toMilliseconds();
			t += int64(i + 1)*incT;
			Time dragTime(t);
			

			const juce::MouseEvent me(mainSource
				, dragPosition, modifier, MouseInputSource::invalidPressure,
				MouseInputSource::invalidOrientation, MouseInputSource::invalidRotation,
				MouseInputSource::invalidTiltX, MouseInputSource::invalidTiltY,
				this, this, dragTime, dragPosition, dragTime, numOfClicks, wasDragged);
			events.push_back(me);
			types.push_back(isMouseDrag);
		}
	}
	// then create the mouseUp
	juce::ModifierKeys modifier(juce::ModifierKeys::Flags::noModifiers);
	int numOfClicks = 0;
	Time upTime((int64)_eventTime);

	const juce::MouseEvent me(mainSource
		, position, modifier, MouseInputSource::invalidPressure,
		MouseInputSource::invalidOrientation, MouseInputSource::invalidRotation,
		MouseInputSource::invalidTiltX, MouseInputSource::invalidTiltY,
		this, this, upTime, position, upTime, numOfClicks, wasDragged);

	events.push_back(me);
	types.push_back(isMouseUp);
}

void MouseSimulator::addMouseUpRotate(Point<float> position, Point<float> pivot, int _eventTime, float incD, bool random)
{

}

void MouseSimulator::executeEvents()
{
	DBG("begin thread");
	while (runThread)
	{
		DBG((String)Tposition);
		if(!events.empty())
		{
			
			for (int i = (int)Tposition; i < (int)(Tposition + 10); i++)
			{
				if (events.empty())
					break;
				MouseEvent evt(events.front());
				if (Time(i) == evt.eventTime)
				{
					const MessageManagerLock mmLock;
					switch (types.front())
					{
					case isMouseDown:
						sceneComponent->mouseDown(events.front());
						break;
					case isMouseDrag:
						sceneComponent->mouseDrag(events.front());
						break;
					case isMouseUp:
						sceneComponent->mouseUp(events.front());
						break;
					default:
						break;
					}
					events.pop_front();
					types.pop_front();
				}
			}
		}
		Tposition += (int64)10;
		std::this_thread::sleep_for(std::chrono::milliseconds(3)); // period of the timer = 100ms
	}
	DBG("End thread");
}