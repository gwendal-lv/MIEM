/*
  ==============================================================================

    MouseSimulator.h
    Created: 29 Sep 2017 2:24:49pm
    Author:  ayup1

  ==============================================================================
*/

#pragma once

#include <thread>
#include "JuceHeader.h"
#include "SceneCanvasComponent.h"
#include "MultiSceneCanvasInteractor.h"
#include "MultiSceneCanvasManager.h"

enum MouseEventType
{
	isMouseDown,
	isMouseDrag,
	isMouseUp,
};

struct MovedArea
{
	/*
	class used to retain the result of each oparation on an area
	because the computation of the click order is made offline
	*/
	bpt center; //normalized
	bpt manipulationPointInPixels; // pixels
	bpolygon contourPointsInPixels; //normalized


};

class MouseSimulator : public Component
{
public:
	MouseSimulator(SceneCanvasComponent *m_sceneComponent, std::weak_ptr<MultiSceneCanvasInteractor> canvasManager);
	~MouseSimulator();

	void addClick(Point<float> location, int eventTime);

	void addResize(int areaId, float factor, int eventTime, float speed); // translate the resize in mouseDown and mouseUp events then add it to the events vector
	void addTranslation(int areaId, bpt translation, int eventTime, float speed); // translate the translation in mouseDown and mouseUp events then add it to the events vector
	void addRotation(int areaId, float rotation, int eventTime, float speed); // translate the rotation in mouseDown and mouseUp events then add it to the events vector

	void addMouseDown(Point<float> position, int eventTime); // add a mouseDown event
	void addMouseUp(Point<float> position, int eventTime, float incD, bool random); // add a mouseUp event and search for the previous mouseDown to create some mouseDrag event between
	void addMouseUpRotate(Point<float> position, Point<float> pivot, float rotation, int _eventTime, float incD, bool random); // idem as addMouseUp, but with circular trajectory for the dragPoints

private:
	void executeEvents();
	std::thread T;
	bool runThread;
	int64 Tposition;

	void lookForArea(int areaId);
	std::map<int, MovedArea> storedArea;
	std::list<MouseEvent> events;
	std::list<MouseEventType> types;

	SceneCanvasComponent* sceneComponent; // needed to send the events and to know the size of the scene
	std::weak_ptr<MultiSceneCanvasInteractor> canvasInteractor; // needed to translate the 

};

