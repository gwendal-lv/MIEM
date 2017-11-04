/*
  ==============================================================================

    EditableEllipse.h
    Created: 2 Feb 2017 2:17:32pm
    Author:  ayup1

  ==============================================================================
*/

#ifndef EDITABLEELLIPSE_H_INCLUDED
#define EDITABLEELLIPSE_H_INCLUDED

#include <vector>

#include "InteractiveEllipse.h"
#include "EditableArea.h"

namespace Miam
{
	class EditableEllipse : public InteractiveEllipse, public Miam::EditableArea
	{
		public :
        
        EditableEllipse(bptree::ptree & areaTree);
        
			EditableEllipse(int64_t _Id);
			EditableEllipse(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio);
			EditableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);

			virtual ~EditableEllipse() {/* DBG("ellipse deleted"); */}

			virtual std::shared_ptr<IDrawableArea> Clone() const override
        { return std::make_shared<EditableEllipse>(*this); }

		private :
			void init();
			void graphicalInit();
			void behaviorInit();

		public :
			virtual void Paint(Graphics& g) override;
			virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;

		private:
			void computeManipulationPoint();

			// ----- Setters and Getters -----
		public:
			void SetActive(bool activate) override;
			double getRadius();

		public :
			
			Miam::AreaEventType TryBeginPointMove(const Point<double>& hitPoint) override;
			Miam::AreaEventType TryMovePoint(const Point<double>& newLocation) override;
			Miam::AreaEventType EndPointMove() override;
			void Translate(const Point<double>& translation) override;
			void setCenterPosition(bpt newCenter);
			
			bool SizeChanged(double sizeFactor, bool minSize);
			void Rotate(double Radian);

		protected:
			void recreateNormalizedPoints() override;

		private:
			bool isNewContourPointValid(const Point<double>& newLocation);
			bool isNewCenterValid(const Point<double>& newLocation);
        
        
        // - - - - - XML import/export - - - - -
        /// \returns "Ellipse" even if it is actually a circle.
        virtual std::string GetTypeAsString() const override {return "EditableEllipse";};
        // Pas d'override du GetTree : aucun paramètre spécifique pour cette classe
        //virtual std::shared_ptr<bptree::ptree> GetTree() override;
	};
}



#endif  // EDITABLEELLIPSE_H_INCLUDED
