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
        /// \brief Constructor of a disk, with the radius given in percentage of the smallest
        /// measure of canvas (width or height)
        EditableEllipse(int64_t _Id, bpt _center, double _r, Colour _fillColour, float _canvasRatio);
        /// \ßrief Construction of a classical ellipse
        EditableEllipse(int64_t _Id, bpt _center, double _a, double _b, Colour _fillColour, float _canvasRatio);
        /// \brief Construction of a disk with constant size in pixels
        EditableEllipse(int64_t _Id, bpt _center, int radiusInPixels, Colour _fillColour);

			virtual ~EditableEllipse() {/* DBG("ellipse deleted"); */}

        virtual std::shared_ptr<IDrawableArea> Clone() override
        {
            auto clone = std::make_shared<EditableEllipse>(*this);
            clone->onCloned();
            return clone;
        }

		private :
			void init();
			void graphicalInit();
			void behaviorInit();

		public :
			
			
			virtual void Paint(Graphics& g) override;
			virtual void CanvasResized(SceneCanvasComponent* _parentCanvas) override;
			virtual void RefreshOpenGLBuffers() override;
        
        
			// ----- Setters and Getters -----
		public:
			void SetActive(bool activate) override;
			double getRadius();

        
        // =============== Setters and Getters ===============
        public :
        // - - - - - VBOs - - - - -
        virtual int GetVerticesBufferElementsCount() const override {
            return InteractiveEllipse::GetVerticesBufferElementsCount()
            + getEditableAreaVerticesCount(); }
        virtual int GetIndicesBufferElementsCount() const override {
            return InteractiveEllipse::GetIndicesBufferElementsCount()
            + getEditableAreaIndexesCount(); }
        
        
        
		public :
			
			Miam::AreaEventType TryBeginPointMove(const Point<double>& hitPoint) override;
			Miam::AreaEventType TryMovePoint(const Point<double>& newLocation) override;
			Miam::AreaEventType EndPointMove() override;
			void Translate(const Point<double>& translation) override;
			
			bool SizeChanged(double sizeFactor, bool minSize);
			void Rotate(double Radian);
			void updateContourPoints();

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
