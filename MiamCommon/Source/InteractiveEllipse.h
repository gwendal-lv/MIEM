/*
  ==============================================================================

    InteractiveEllipse.h
    Created: 2 Feb 2017 11:52:00am
    Author:  ayup1

  ==============================================================================
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

