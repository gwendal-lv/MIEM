/*
  ==============================================================================

    ControlEvent.h
    Created: 26 Jan 2017 4:25:21pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef CONTROLEVENT_H_INCLUDED
#define CONTROLEVENT_H_INCLUDED

#include "GraphicEvent.h"

namespace Miam
{

	enum ControlEventType
	{
		NothingHappened,
		Play,
		Pause,
		Stop,
	};

	class ControlEvent : public GraphicEvent
	{
	public:
		ControlEventType GetType(){return eventType;}

		ControlEvent(ControlEventType _eventType);
		virtual ~ControlEvent();


	protected:
		ControlEventType eventType = ControlEventType::NothingHappened;
	};


}


#endif  // CONTROLEVENT_H_INCLUDED
