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

#ifndef EDITABLEAREA_H_INCLUDED
#define EDITABLEAREA_H_INCLUDED


#include <vector>



#include "JuceHeader.h"

#include "IEditableArea.h"

#include "MiemVector.hpp"


#define MIEM_EDITION_ELEMENTS_Z         (-0.2f)
#define MIEM_EDITION_ELEMENTS_ALPHA     (1.0f)


namespace Miam
{
    /// \brief Abstract class adding attributes for all areas that can be edited
    class EditableArea : public virtual IEditableArea
    {
        
        
        
        // = = = = = = = = = = ENUMS = = = = = = = = = =
        public :
        
        /// \brief Contains the integer Id of a contour or a special area point.
        ///
        /// Values from 0 to +inf refer to "normal" area contour points, negative values refer
        /// to area points associated to "special functions" (the center, or editing points).
        enum EditableAreaPointId : int { // enum *class* brings cast (to int) issues...
            
            TwoNeighbourPoints = -50, ///< Special value that indicates that two neighbour points = a side is being dragged
            
            ManipulationPoint = -40, ///< Point that allows rotation and scaling of the whole area
            Center = -30, ///< Center of interaction
            WholeArea = -20, ///< Special point representing the whole area
            
            None = -10 ///< No point selected
            
        };
        
        
        
        // ===== COMMON METHODS TO ALL EDITABLE AREAS =====
        
        // Construction/destruction
        public :
        EditableArea();
        virtual ~EditableArea();
        
        // ----- Setters and Getters -----
        public :
        void SetActive(bool activate) override;
        
        void SetEnableTranslationOnly(bool enable_) override {enableTranslationOnly = enable_;}
        
		int GetPointDraggedId() override { return pointDraggedId; }
        
        // ----- Display computations -----
        protected :
        void computeManipulationPoint();
        void computeManipulationLineBuffer(float Ox, float Oy, float Mx, float My, float width, float height);
        
        /// \brief Additionnal count for this class only
        int getEditableAreaVerticesCount() const {
            return contourCirclesTotalVerticesCount // petits disques blancs sur les points du contour
            + dottedLineVertexesCount + numVerticesRing; } // manipulationLine + manipulationPoint (small ring)
        /// \brief Additionnal count for this class only
        int getEditableAreaIndexesCount() const {
            return contourCirclesTotalIndicesCount // disques sur points du contour
            + dottedLineIndicesCount + (3 * numVerticesRing); } // ligne + small ring au bout
        
        /// \brief Refreshes the sub-part of the GL buffers that concerns only this particular class.
        ///
        /// VBO additionnal data :
        /// + manipulation dotted line
        /// + manipulation ring (= manipulation handle)
        ///
        /// at the end, with variable size (for optimized data transfer and GL drawing) :
        /// + small disks (circles) on all possible contour points of the shape
        ///
        /// \param vertexBufElmtOffset Element (not array) position of the next vertex or colour to put in the buffer
        /// \param vertexBufElmtOffset Element (not array) position of the next index to put in the buffer
        void refreshOpenGLSubBuffers(int vertexBufElmtOffset, int indexBufElmtOffset);
        
        
        void initSmallDiskBuffers();
        
        virtual int GetVerticesBufferActualElementsCount() const override
        { return actualVerticesBufferElementsCount; }
        virtual int GetIndicesBufferActualElementsCount() const override
        { return actualIndicesBufferElementsCount; }
        
        // =============== COMMON ATTRIBUTES TO ALL EDITABLE AREAS =============
        
        // Might contain an optimized value if all geometry is not necessary on screen
        int actualIndicesBufferElementsCount = 0;
        // idem
        int actualVerticesBufferElementsCount = 0;
        
        // - - - - - Constant caracteristic values for VBOs - - - - -
        // might be optimized by using preprocessor defines..... these values will remain constant
        protected :
        const int dottedLineNparts = 20; // NOT static for multithreading safety
        const int dottedLineVertexesCount = 4 * dottedLineNparts; // 1 rectangle par partie
        const int dottedLineIndicesCount = 6 * dottedLineNparts; // chaque rectangle divisé en 2 triangles
        
        // (Not static for multi-threading safety)
        const int numPointsSmallCircle = 10; // actual resolution of any small circle. Convient pour diam 14px ou -
        const int numVerticesSmallCircle = numPointsSmallCircle + 1; // +1 pour le centre du disque
        const int numIndicesSmallCircle = numPointsSmallCircle * 3;
        const int contourCirclesTotalVerticesCount = numVerticesSmallCircle * numPointsPolygon;
        const int contourCirclesTotalIndicesCount = (numPointsSmallCircle * 3) * numPointsPolygon;
        
        // - - - - - actual VBO and IBO - - - - -
        Vector<GLfloat> g_vertex_dotted_line;
        Vector<GLuint> g_indices_dotted_line;
        
        Vector<GLfloat> g_vertex_circle; // small circle
        Vector<GLuint> g_circle_indices; // small circle
        
        
        // Status
        bool isActive; ///< Wether this area is marking itself as actively editable
        bool enableTranslationOnly; ///< Wether any 2D geometric transformation, or the translation only is authorized
        
        // Basic drawing attributes
        
        
        // Additionnal graphical assets

		/// \brief Special point, only visible when area is active, which allows the 
		/// user to scale and rotate the whole area.
        Point<double> manipulationPointInPixels;
		bpt bmanipulationPointInPixels;
		///< For all editing graphical elements drawn on top of a basic DrawableArea
        Colour editingElementsColour;
        
		/// \brief On-screen display radius in pixels (see
		/// InteractiveArea::contourPointsInPixels)
        float contourPointsRadius = 0.0f; // init in ctor
		/// \brief On-screen display radius in pixels (see
		/// EditableArea::manipulationPointInPixels)
        float manipulationPointRadius;
        
        /// \brief ID of the only point of this area that is being moved.
        //int pointDraggedId;
        
        Point<double> lastLocation; ///< Back-up for EditableArea::pointDraggedId
        
        float minimumSizePercentage = 0.03f; ///< In % of mean of canvas width and height

		protected :
        int pointDraggedId;
        int sideDraggedId = -1; // utilisé uniquement lorsque pointDraggedId indique TwoPoints
        
        
    };
    
    
}



#endif  // EDITABLEAREA_H_INCLUDED
