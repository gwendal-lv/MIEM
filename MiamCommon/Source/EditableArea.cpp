/*
  ==============================================================================

    EditableArea.cpp
    Created: 12 Apr 2016 4:39:52pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "EditableArea.h"

#include "SceneCanvasComponent.h"

#include "MiamMath.h"

using namespace Miam;



EditableArea::EditableArea() : bmanipulationPointInPixels(0,0)
{
    enableTranslationOnly = false;
    
#ifdef __MIEM_VBO
    // Proper resize of OpenGL buffer for VBO rendering
    g_vertex_dotted_line.resize(3 * dottedLineVertexesCount);
    g_indices_dotted_line.resize(dottedLineIndicesCount);
    
    g_vertex_circle.resize(3 * numVerticesCircle);
    g_circle_indices.resize(3 * numPointCircle);
#endif
}


EditableArea::~EditableArea()
{

}


void EditableArea::SetActive(bool activate)
{
    isActive = activate;
    
    if (isActive)
        SetOpacityMode(OpacityMode::High);
    else
        SetOpacityMode(OpacityMode::Mid);
    
}


// ========== Display computations ==========
void EditableArea::computeManipulationPoint()
{
    float manipulationLineLengthLeft = 0.25f*(getParentCanvas()->getWidth()+getParentCanvas()->getHeight())/2.0f,
    manipulationLineLengthRight = manipulationLineLengthLeft; //px
    if (getCenterInPixels().get<0>() + manipulationLineLengthRight + manipulationPointRadius
        <= getParentCanvas()->getWidth())
        bmanipulationPointInPixels = bpt(getCenterInPixels().get<0>()
                                         + manipulationLineLengthRight,
                                         getCenterInPixels().get<1>());
    else
        bmanipulationPointInPixels = bpt(getCenterInPixels().get<0>()
                                         - manipulationLineLengthLeft,
                                         getCenterInPixels().get<1>());
}

void EditableArea::computeManipulationLineBuffer(float Ox, float Oy, float Mx, float My, float width, float height)
{
    int N = 20;
    float length = (float)boost::geometry::distance(bpt(Ox, Oy), bpt(Mx, My));//0.25 * (getWidth() + getHeight()) / 2.0;
    if (length / (2 * height) > 20.0f)
        height = (length / 20.0f) / 2.0f;
    else
        N = int(length / (2 * height));
    
    float sina = (My - Oy) / length;
    float cosa = (Mx - Ox) / length;
    
#ifndef __MIEM_VBO
    throw std::logic_error("Cannot manipulate VBOs in non-VBO mode");
#endif
    for (int i = 0; i < dottedLineNparts; ++i)
    {
        if (i < N)
        {
            // 0 - up_left
            g_vertex_dotted_line[i * 3 * 4 + 0] = Ox + i * 2 * height * cosa - (width / 2.0f) * sina;
            g_vertex_dotted_line[i * 3 * 4 + 1] = Oy + i * 2 * height * sina + (width / 2.0f) * cosa;
            g_vertex_dotted_line[i * 3 * 4 + 2] = 0.1f;
            // 1 - down_left
            g_vertex_dotted_line[i * 3 * 4 + 3] = Ox + i * 2 * height * cosa + (width / 2.0f) * sina;
            g_vertex_dotted_line[i * 3 * 4 + 4] = Oy + i * 2 * height * sina - (width / 2.0f) * cosa;
            g_vertex_dotted_line[i * 3 * 4 + 5] = 0.1f;
            // 2 - up_right
            g_vertex_dotted_line[i * 3 * 4 + 6] = Ox + (2 * i + 1)  * height * cosa - (width / 2.0f) * sina;
            g_vertex_dotted_line[i * 3 * 4 + 7] = Oy + (2 * i + 1) * height * sina + (width / 2.0f) * cosa;
            g_vertex_dotted_line[i * 3 * 4 + 8] = 0.1f;
            // 3 - down_right
            g_vertex_dotted_line[i * 3 * 4 + 9] = Ox + (2 * i + 1) * height * cosa + (width / 2.0f) * sina;
            g_vertex_dotted_line[i * 3 * 4 + 10] = Oy + (2 * i + 1) * height * sina - (width / 2.0f) * cosa;
            g_vertex_dotted_line[i * 3 * 4 + 11] = 0.1f;
            
            g_indices_dotted_line[i * 6 + 0] = i * 4 + 0;
            g_indices_dotted_line[i * 6 + 1] = i * 4 + 1;
            g_indices_dotted_line[i * 6 + 2] = i * 4 + 2;
            g_indices_dotted_line[i * 6 + 3] = i * 4 + 1;
            g_indices_dotted_line[i * 6 + 4] = i * 4 + 2;
            g_indices_dotted_line[i * 6 + 5] = i * 4 + 3;
        }
        else
        {
            for (int j = 0; j < 12; ++j)
            {
                g_vertex_dotted_line[i * 12 + j] = 0.0f;
            }
            for (int j = 0; j < 6; ++j)
            {
                g_indices_dotted_line[i * 6 + j] = 0;
            }
        }
    }
    
}

void EditableArea::computeSmallDiskBuffers()
{
    // Vertices of the small disk
    float radius = 3.0f; // A MODIFIER SELON DESKTOP SCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAALE
    double currentAngle = 0.0;
    double incAngle = 2 * M_PI / (double)numPointsSmallCircle;
    g_vertex_circle[0] = 0.0f;
    g_vertex_circle[1] = 0.0f;
    g_vertex_circle[2] = 0.0f;
    for (int i = 1; i < numVerticesSmallCircle; ++i)
    {
        g_vertex_circle[i * 3 + 0] = radius * (float)cos(currentAngle);
        g_vertex_circle[i * 3 + 1] = radius * (float)sin(currentAngle);
        g_vertex_circle[i * 3 + 2] = 0.0f;
        currentAngle += incAngle;
    }
    // Indexes of all triangles of a small disk
    for (int i = 0; i < numPointsSmallCircle; ++i)
    {
        g_circle_indices[i * 3 + 1] = 0; // centre
        g_circle_indices[i * 3 + 0] = i + 1; // next point
        // next++ or first point
        g_circle_indices[i * 3 + 2] = (i + 2) > numPointsSmallCircle ? 1 : (i + 2);
    }
    /// COULEUR A REPRENDRE PLUS TARD
    /// COULEUR A REPRENDRE PLUS TARD
    /// COULEUR A REPRENDRE PLUS TARD
    /// COULEUR A REPRENDRE PLUS TARD
    /// couleur
    // points
    /*
    int decalage = InteractivePolygon::GetVerticesBufferElementsCount();
    for (int i = 0; i < (numPointsPolygon * numVerticesCircle); ++i)
    {
        coulours_buffer[decalage + 4 * i + 0] = editingElementsColour.getRed() / 255.0f;
        coulours_buffer[decalage + 4 * i + 1] = editingElementsColour.getGreen() / 255.0f;
        coulours_buffer[decalage + 4 * i + 2] = editingElementsColour.getBlue() / 255.0f;
        coulours_buffer[decalage + 4 * i + 3] = GetAlpha();
    }
    // manipulationLine
    decalage += 4 * (numPointsPolygon * numVerticesCircle);
    for (int i = 0; i < dottedLineVertexesCount; ++i)
    {
        coulours_buffer[decalage + 4 * i + 0] = editingElementsColour.getRed() / 255.0f;
        coulours_buffer[decalage + 4 * i + 1] = editingElementsColour.getGreen() / 255.0f;
        coulours_buffer[decalage + 4 * i + 2] = editingElementsColour.getBlue() / 255.0f;
        coulours_buffer[decalage + 4 * i + 3] = GetAlpha();
    }
    // manipulationPoint
    decalage += 4 * dottedLineVertexesCount;
    for (int i = 0; i < numVerticesRing; ++i)
    {
        coulours_buffer[decalage + 4 * i + 0] = editingElementsColour.getRed() / 255.0f;
        coulours_buffer[decalage + 4 * i + 1] = editingElementsColour.getGreen() / 255.0f;
        coulours_buffer[decalage + 4 * i + 2] = editingElementsColour.getBlue() / 255.0f;
        coulours_buffer[decalage + 4 * i + 3] = GetAlpha();
    }
    */
    /// COULEUR A REPRENDRE PLUS TARD
    /// COULEUR A REPRENDRE PLUS TARD
    /// COULEUR A REPRENDRE PLUS TARD
    /// COULEUR A REPRENDRE PLUS TARD
}

void EditableArea::refreshOpenGLSubBuffers(int vertexBufElmtOffset, int indexBufElmtOffset)
{
    if (!isActive)
    {
		// Attention : le std::fill ne fait aucun check...
		/*
		size_t fillSize = getEditableAreaIndexesCount();
        std::fill(getIndicesBuffer().begin() + indexBufElmtOffset,
                  getIndicesBuffer().begin() + indexBufElmtOffset + fillSize, 0);
				  */
		for (int i = 0; i < getEditableAreaIndexesCount(); ++i)
			getIndicesBuffer()[i] = 0;
    }
    // We draw additionnal elements only if the polygon is active
    else
    {
        /// ---------------- contours disks on contour points -----------------------
        const int disksVertexBufElmtOffset = vertexBufElmtOffset; // offset for the contour disks
        int actualContourPointsCount = (int)getContourPointsInPixels().size() - 1;
        
        /// - - - - points - - - -
        // Actual vertices
        for (int i = 0; i < actualContourPointsCount; ++i)
        {
            float Xoffset = (float)getContourPointsInPixels()[i].get<0>();
            float Yoffset = (float)getContourPointsInPixels()[i].get<1>();
            float Zoffset = getZoffset() + 0.1f;
            for (int j = 0; j < 3 * numVerticesSmallCircle; j += 3)
            {
                getVerticesBuffer()[(3 * disksVertexBufElmtOffset) + (i * 3 * numVerticesSmallCircle) + j + 0]
                = Xoffset + g_vertex_circle[j + 0];
                getVerticesBuffer()[(3 * disksVertexBufElmtOffset) + (i * 3 * numVerticesSmallCircle) + j + 1]
                = Yoffset + g_vertex_circle[j + 1];
                getVerticesBuffer()[(3 * disksVertexBufElmtOffset) + (i * 3 * numVerticesSmallCircle) + j + 2]
                = Zoffset + g_vertex_circle[j + 2];
            }
        }
        // filling of the remaining space with zeroes
        for (int jj = disksVertexBufElmtOffset; jj < vertexBufElmtOffset + contourCirclesTotalVerticesCount; ++jj)
            getVerticesBuffer()[(3 * disksVertexBufElmtOffset) + jj] = 0.0f;
        // we declare here the beginning pos for the next step
        const int manipLineVertexBufElmtOffset = disksVertexBufElmtOffset + contourCirclesTotalVerticesCount;
        
        
        // - - - - indexes - - - -
        const int disksIndexBufElmtOffset = indexBufElmtOffset; // to keep track of the logic
        for (int i = 0; i < actualContourPointsCount; ++i) // pour chaque petit disque de contour
        {
            // on aura autant de triangles que de points
            for (int jj = 0; jj < numIndicesSmallCircle; ++jj)
                getIndicesBuffer()[disksIndexBufElmtOffset + i * numIndicesSmallCircle + jj] = disksVertexBufElmtOffset + g_circle_indices[jj];
        }
        // remaining indices to zero
        for (int i = actualContourPointsCount; i < numPointsPolygon; ++i)
            for (int jj = 0; jj < numIndicesSmallCircle; ++jj)
                getIndicesBuffer()[disksIndexBufElmtOffset + i * numIndicesSmallCircle + jj] = 0;
        // we declare here the beginning pos for the next step
        const int manipLineIndexBufElmtOffset = disksIndexBufElmtOffset + contourCirclesTotalIndicesCount;
        
        
        
        
        /// ---------------- manipulationLine + manipulationPoint -----------------------
        /// - - - - points - - - -
         // Manipulation LINE
        computeManipulationLineBuffer((float)getCenterInPixels().get<0>(), // does all translations required
                                      (float)getCenterInPixels().get<1>(),
                                      (float)bmanipulationPointInPixels.get<0>(),
                                      (float)bmanipulationPointInPixels.get<1>(),
                                      4.0f, 4.0f);
        for (int ii = 0; ii < 3 * dottedLineVertexesCount; ++ii)
            getVerticesBuffer()[(3 * manipLineVertexBufElmtOffset) + ii] = g_vertex_dotted_line[ii]; // no need to translate
        // Offset for the ring (which is the next thing to fill)
        const int manipRingVertexBufElmtOffset = manipLineVertexBufElmtOffset + dottedLineVertexesCount;
        // Manipulation RING (handle)
        float Xoffset = (float)bmanipulationPointInPixels.get<0>();
        float Yoffset = (float)bmanipulationPointInPixels.get<1>();
        float Zoffset = getZoffset() + MIEM_EDITION_ELEMENTS_Z;
        for (int j = 0; j < 3 * numVerticesRing; j += 3)
        {
            getVerticesBuffer()[3 * manipRingVertexBufElmtOffset + j + 0] = Xoffset + getRingVertexBuffer()[j + 0];
            getVerticesBuffer()[3 * manipRingVertexBufElmtOffset + j + 1] = Yoffset + getRingVertexBuffer()[j + 1];
            getVerticesBuffer()[3 * manipRingVertexBufElmtOffset + j + 2] = Zoffset + getRingVertexBuffer()[j + 2];
        }

        
        // - - - - indexes - - - -
        // Manipulation LINE
        for (int i = 0; i < dottedLineIndicesCount; ++i)
            getIndicesBuffer()[manipLineIndexBufElmtOffset + i] = manipLineVertexBufElmtOffset + g_indices_dotted_line[i];
        // Offset for the ring (which is the next thing to fill)
        const int manipRingIndexBufElmtOffset = manipLineIndexBufElmtOffset + dottedLineIndicesCount;
        // Manipulation RING (handle)
        for (int jj = 0; jj < 3 * numVerticesRing; ++jj)
            getIndicesBuffer()[manipRingIndexBufElmtOffset + jj] = 0;
    }
}


