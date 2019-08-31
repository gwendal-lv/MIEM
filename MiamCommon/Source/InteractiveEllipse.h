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

#ifndef INTERACTIVEELLIPSE_H_INCLUDED
#define INTERACTIVEELLIPSE_H_INCLUDED

#include "DrawableEllipse.h"
#include "InteractiveArea.h"



namespace Miam
{
	class InteractiveEllipse : public DrawableEllipse, public InteractiveArea
	{
		public :
        
        InteractiveEllipse(bptree::ptree & areaTree);
        InteractiveEllipse(int64_t _Id);
        InteractiveEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);
        InteractiveEllipse(int64_t _Id, bpt _center, int _radiusInPixels, Colour _fillColour);

			virtual ~InteractiveEllipse() {}
		
        virtual std::shared_ptr<IDrawableArea> Clone() override;
        protected :
        virtual void onCloned() override;
        public :

		private :
			void init();

		public :
			virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const override {return "InteractiveEllipse";};
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
        
        
			// Interactions computing
		public:
			virtual bool HitTest(bpt T) const override;
			double ComputeInteractionWeight(bpt T) override;

		private :
			void computeSurface();
		public :
			double GetSurface() override
			{ return surface; }

		protected :
			bool isRound;
		public :
			void SetIsRound(bool _isround);

	
	};
}


#endif  // INTERACTIVEELLIPSE_H_INCLUDED

