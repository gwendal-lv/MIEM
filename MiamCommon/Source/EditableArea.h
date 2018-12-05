/*
  ==============================================================================

    EditableArea.h
    Created: 12 Apr 2016 4:39:52pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef EDITABLEAREA_H_INCLUDED
#define EDITABLEAREA_H_INCLUDED


#include <vector>



#include "JuceHeader.h"

#include "IEditableArea.h"

#include "MiemVector.hpp"


#define MIEM_EDITION_ELEMENTS_Z         (0.2f)


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
        int getEditableAreaVerticesCount() {
            return contourCirclesTotalVerticesCount // petits disques blancs sur les points du contour
            + dottedLineVertexesCount + numVerticesRing; } // manipulationLine + manipulationPoint (small ring)
        /// \brief Additionnal count for this class only
        int getEditableAreaIndexesCount() {
            return contourCirclesTotalIndicesCount // disques sur points du contour
            + dottedLineIndicesCount + (3 * numVerticesRing); } // ligne + small ring au bout
        
        /// \brief Refreshes the sub-part of the GL buffers that concerns only this particular class.
        ///
        /// VBO additionnal data :
        /// + small disks (circles) on all possible contour points of the shape
        /// + manipulation dotted line
        /// + manipulation ring (= manipulation handle)
        ///
        /// \param vertexBufElmtOffset Element (not array) position of the next vertex or colour to put in the buffer
        /// \param vertexBufElmtOffset Element (not array) position of the next index to put in the buffer
        void refreshOpenGLSubBuffers(int vertexBufElmtOffset, int indexBufElmtOffset);
        
        void computeSmallDiskBuffers();
        
        
        
        // =============== COMMON ATTRIBUTES TO ALL EDITABLE AREAS =============
        
        // - - - - - Constant caracteristic values for VBOs - - - - -
        // might be optimized by using preprocessor defines..... these values will remain constant
        protected :
        const int dottedLineNparts = 20; // NOT static for multithreading safety
        const int dottedLineVertexesCount = 4 * dottedLineNparts; // 1 rectangle par partie
        const int dottedLineIndicesCount = 6 * dottedLineNparts; // chaque rectangle divisé en 2 triangles
        
        // (Not static for multi-threading safety)
        const int numPointsSmallCircle = 16; // actual resolution of any small circle
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
        float contourPointsRadius;
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
