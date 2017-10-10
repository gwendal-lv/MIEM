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
	//addTranslation(0, bpt(-0.1,-0.1), 1200,0.5); // speed = 1screen/s
	
	addResize(0, 2.0f, 1200, 1.0);
	addResize(0, 1.0f / 2.0f, 2000, 0.5);

	addRotation(0, 3.14f/2.0f, 3200, 1.0);
	addTranslation(0, bpt(-0.1, -0.1), 6500, 0.5);
	addTranslation(0, bpt(0.1, 0.1), 8500, 0.5);
	addRotation(0, -3.14f / 2.0f, 9500, 1.0);


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

void MouseSimulator::addResize(int areaId, float factor, int eventTime, float speed)
{
	lookForArea(areaId);
	bpolygon poly = storedArea.at(areaId).contourPointsInPixels;
	bpt center = storedArea.at(areaId).center;
	bpt bmanipulationPointInPixels = storedArea.at(areaId).manipulationPointInPixels;

	// transform the Rotation in mouseDown and mouseUp

	//mouseDown point = manipulation point
	Point<float> manipulationPt(bmanipulationPointInPixels.get<0>(), bmanipulationPointInPixels.get<1>());
	addMouseDown(manipulationPt, eventTime);

	// mouseUp point = manipulation point translated
	Point<float> upPoint = manipulationPt;
	bpt C(center.get<0>() * (double)sceneComponent->getWidth(), center.get<1>() * (double)sceneComponent->getHeight());
	upPoint.addXY((float)-C.get<0>(), (float)-C.get<1>());
	upPoint.setX(factor * upPoint.x);
	upPoint.addXY((float)C.get<0>(), (float)C.get<1>());
	Point<float> upPointNormalized(upPoint.x / (float)sceneComponent->getWidth(), upPoint.y / (float)sceneComponent->getHeight());
	Point<float> manipulationPtNormalized(storedArea.at(areaId).manipulationPointInPixels.get<0>() / (float)sceneComponent->getWidth(), storedArea.at(areaId).manipulationPointInPixels.get<1>() / (float)sceneComponent->getHeight());
	int incT = round(upPointNormalized.getDistanceFrom(manipulationPtNormalized) / (speed / 1000.0f));
	int upTime = eventTime + incT;

	addMouseUp(upPoint, upTime, 2, false);


	///update the contourPoints
	bool returnValue = false;
	//// --- size if polygon is still big enough only ---
	double minDistanceFromCenter = 0.0;
	bool wasSizeApplied = false;
	bpolygon testBoost2, testBoost;
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> trans(-C.get<0>(), -C.get<1>());
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtrans(C.get<0>(), C.get<1>());
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> resizer(factor, factor);
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaleToPixel(sceneComponent->getWidth(), sceneComponent->getHeight());
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaleToNorm(1.0 / (double)sceneComponent->getWidth(), 1.0 / (double)sceneComponent->getHeight());
	

	bpolygon contourPointsInPixels;
	boost::geometry::transform(storedArea.at(areaId).contourPointsInPixels, contourPointsInPixels, scaleToPixel);
	boost::geometry::transform(contourPointsInPixels, testBoost, trans);
	boost::geometry::transform(testBoost, testBoost2, resizer);
	boost::geometry::transform(testBoost2, testBoost, invtrans);
	


	for (size_t i = 0; i < testBoost.outer().size(); i++)
	{
		//if (testBoost.outer().at(i).get<0>() < 0 || testBoost.outer().at(i).get<1>() < 0)
		//	DBG("probleme");
		if (boost::geometry::distance(testBoost.outer().at(i), C) > minDistanceFromCenter)
			minDistanceFromCenter = boost::geometry::distance(testBoost.outer().at(i), C);
	}
	//std::vector<bpolygon> comparaison;
	//boost::geometry::difference(bnewContourPoints, testBoost, comparaison);
	float minimumSizePercentage = 0.03f;
	bool minSize = true;
	if ((!minSize) || (minDistanceFromCenter >=
		minimumSizePercentage*(sceneComponent->getWidth() + sceneComponent->getHeight()) / 2.0))
	{
		wasSizeApplied = true;
		contourPointsInPixels.clear(); // test;
		contourPointsInPixels = testBoost;//bnewContourPoints;
										  //bmanipulationPointInPixels.set<0>(centerInPixels.get<0>() + manipulationPointRadius*size); //= bnewLocation;
										  //bmanipulationPointInPixels.set<1>(centerInPixels.get<1>() + manipulationPointRadius*size);
										  //bmanipulationPointInPixels = newManipulationPt;
		returnValue = true;
		
	}
	storedArea.at(areaId).contourPointsInPixels.clear();
	boost::geometry::transform(contourPointsInPixels, storedArea.at(areaId).contourPointsInPixels, scaleToNorm);
}

void MouseSimulator::addTranslation(int areaId, bpt translation, int eventTime, float speed)
{
	
	lookForArea(areaId);
	
	// transform the Translation in mouseDown and mouseUp
	//completePolygon->getPolygon() from polygon and center -> choose a point to click down and the point to click up
	bpolygon poly = storedArea.at(areaId).contourPointsInPixels;
	bpt center = storedArea.at(areaId).center;
	

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


	// update the storedArea
	bpt newCenter;
	bpolygon newContour;
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(translation.get<0>(), translation.get<1>());
	boost::geometry::transform(storedArea.at(areaId).contourPointsInPixels, newContour, tr);
	boost::geometry::transform(storedArea.at(areaId).center, newCenter, tr);
	boost::geometry::add_point(storedArea.at(areaId).manipulationPointInPixels,bpt(translation.get<0>() * sceneComponent->getWidth(),translation.get<1>() * sceneComponent->getHeight()));
	storedArea.at(areaId).contourPointsInPixels.clear();
	storedArea.at(areaId).contourPointsInPixels = newContour;
	storedArea.at(areaId).center = newCenter;
	
}



void MouseSimulator::addRotation(int areaId, float rotation, int eventTime, float speed)
{
	lookForArea(areaId);
	bpolygon poly = storedArea.at(areaId).contourPointsInPixels;
	bpt center = storedArea.at(areaId).center;
	bpt bmanipulationPointInPixels = storedArea.at(areaId).manipulationPointInPixels;

	// transform the Rotation in mouseDown and mouseUp

	//mouseDown point = manipulation point
	Point<float> manipulationPt(bmanipulationPointInPixels.get<0>(),bmanipulationPointInPixels.get<1>());
	addMouseDown(manipulationPt, eventTime);

	// mouseUp point = manipulation point rotated by the angle
				
	boost::geometry::multiply_point(center, bpt(sceneComponent->getWidth(), sceneComponent->getHeight()));
	boost::geometry::subtract_point(bmanipulationPointInPixels, center);
	bpt rotatedManipulationPt;
	boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> Rot(rotation);
	boost::geometry::transform(bmanipulationPointInPixels, rotatedManipulationPt, Rot);
	boost::geometry::add_point(rotatedManipulationPt, center);

	Point<float> pivot(center.get<0>(), center.get<1>());
	Point<float> position(rotatedManipulationPt.get<0>(), rotatedManipulationPt.get<1>());

	boost::geometry::divide_point(center, bpt((double)sceneComponent->getWidth(), (double)sceneComponent->getHeight()));
	boost::geometry::divide_point(bmanipulationPointInPixels, bpt((double)sceneComponent->getWidth(), (double)sceneComponent->getHeight()));
	double arc = abs(rotation) * boost::geometry::distance(bmanipulationPointInPixels, center); // pas en pixel car vitesse en ecran/seconde
	int incT = round(arc / (speed / 1000.0f));
	int upTime = eventTime + incT;
	addMouseUpRotate(position, pivot, rotation, upTime, 2, false);

	// update the storedArea
	bpt newCenter, newManipulationPt;
	bpolygon newContour;
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> tr(-storedArea.at(areaId).center.get<0>(), -storedArea.at(areaId).center.get<1>()); // normalized
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaleToPixel(sceneComponent->getWidth(), sceneComponent->getHeight());
	boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, double, 2, 2> rot(rotation);
	boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scaleToNorm(1.0/(double)sceneComponent->getWidth(), 1.0/(double)sceneComponent->getHeight());
	boost::geometry::strategy::transform::translate_transformer<double, 2, 2> invtr(storedArea.at(areaId).center.get<0>(), storedArea.at(areaId).center.get<1>());

	boost::geometry::transform(storedArea.at(areaId).contourPointsInPixels, newContour, tr);
	boost::geometry::transform(newContour, storedArea.at(areaId).contourPointsInPixels, scaleToPixel);
	boost::geometry::transform(storedArea.at(areaId).contourPointsInPixels, newContour, rot);
	boost::geometry::transform(newContour, storedArea.at(areaId).contourPointsInPixels, scaleToNorm);
	boost::geometry::transform(storedArea.at(areaId).contourPointsInPixels, newContour, invtr);

	storedArea.at(areaId).contourPointsInPixels.clear();
	storedArea.at(areaId).contourPointsInPixels = newContour;

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

void MouseSimulator::addMouseUpRotate(Point<float> position, Point<float> pivot, float alpha, int _eventTime, float incD, bool random)
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
		//position.setXY(position.x / (float)sceneComponent->getWidth(), position.y / (float)sceneComponent->getHeight());
		//Point<float> C(pivot.x / (float)sceneComponent->getWidth(), pivot.y / (float)sceneComponent->getHeight());
		float R = position.getDistanceFrom(pivot);
		float distance = abs(alpha) * R;// arc

		int numOfDragEvents = roundToInt(distance / incD);
		Point<float> inc = (prevEvt.position - pivot);

		int64 incT = round(((int64)_eventTime - prevEvt.eventTime.toMilliseconds()) / (int64)numOfDragEvents);
		for (int i = 0; i < numOfDragEvents - 1; ++i)
		{
			Point<float> dragPosition = inc;
			dragPosition = dragPosition.rotatedAboutOrigin(float(i + 1) * alpha / (float)numOfDragEvents);
			dragPosition.addXY(pivot.x, pivot.y);
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

void MouseSimulator::lookForArea(int areaId)
{
	if (storedArea.find(areaId) == storedArea.end())
	{
		if (auto canvasManager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasInteractor.lock()))
		{
			if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(canvasManager->GetSelectedScene()))
			{
				if (auto completePolygon = std::dynamic_pointer_cast<CompletePolygon>(amusingScene->GetEditableArea(areaId)))
				{
					MovedArea newMovedArea;
					newMovedArea.center = completePolygon->getCenter();
					boost::geometry::divide_point(newMovedArea.center, bpt(sceneComponent->getWidth(), sceneComponent->getHeight()));
					newMovedArea.contourPointsInPixels = completePolygon->getPolygon();

					bpt C = completePolygon->getCenter();

					bpt bmanipulationPointInPixels;
					float manipulationLineLengthLeft = 0.25f*(sceneComponent->getWidth() + sceneComponent->getHeight()) / 2.0f,
						manipulationLineLengthRight = manipulationLineLengthLeft; //px
					if (C.get<0>() + manipulationLineLengthRight + 8
						<= sceneComponent->getWidth())
						bmanipulationPointInPixels = bpt(C.get<0>()
							+ manipulationLineLengthRight,
							C.get<1>());
					else
						bmanipulationPointInPixels = bpt(C.get<0>()
							- manipulationLineLengthLeft,
							C.get<1>());
					newMovedArea.manipulationPointInPixels = bmanipulationPointInPixels;

					storedArea.insert(std::pair<int, MovedArea>(areaId, newMovedArea));
				}
			}
		}
	}
}
