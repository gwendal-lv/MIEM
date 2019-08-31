/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
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

#ifndef AMUSINGMULTISCENECANVASINTERACTOR_H_INCLUDED
#define AMUSINGMULTISCENECANVASINTERACTOR_H_INCLUDED

#include "MultiSceneCanvasInteractor.h"
#include "MultiSceneCanvasEditor.h"



using namespace Miam;

namespace Miam
{
    class AmusingMultiSceneCanvasInteractor : public MultiSceneCanvasEditor//MultiSceneCanvasInteractor
    {
	public :
		// Constructor
		AmusingMultiSceneCanvasInteractor(IGraphicSessionManager* _graphicSessionManager, MultiSceneCanvasComponent* _canvasComponent, SceneCanvasComponent::Id _selfId);
		~AmusingMultiSceneCanvasInteractor();

		// Add AmusingScene
		void AddScene(std::string name) override;
	
		// Example
		void __AddTestAreas();

		// callback when one surface of a scene is changed
		//void OnSurfaceChanged(double newSurface) override;

	};
};


#endif  // AMUSINGMULTISCENECANVASINTERACTOR_H_INCLUDED
