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

#ifndef GRAPHICEVENT_H_INCLUDED
#define GRAPHICEVENT_H_INCLUDED


#include <string>


namespace Miam
{
	
    
    class GraphicEvent {

		

        protected :
        /// \brief Message that can contain anything...
        std::string message;
		
        
        public :
        virtual void SetMessage(std::string message_) {message = message_;}
        virtual std::string GetMessage() {return message;}
        
		GraphicEvent(){}
        virtual ~GraphicEvent() {}
        
    };
    
}



#endif  // GRAPHICEVENT_H_INCLUDED
