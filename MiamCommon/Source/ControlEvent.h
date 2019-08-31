/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
