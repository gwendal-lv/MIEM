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

#include "EditableArea.h"

#include "SceneCanvasComponent.h"

#include "MiamMath.h"

using namespace Miam;



EditableArea::EditableArea() : bmanipulationPointInPixels(0,0)
{
    enableTranslationOnly = false;
    
    contourPointsRadius = 4.0f;
    
#ifdef __MIEM_VBO
    // Proper resize of OpenGL buffer for VBO rendering
    g_vertex_dotted_line.resize(3 * dottedLineVertexesCount);
    g_indices_dotted_line.resize(dottedLineIndicesCount);
    
    initSmallDiskBuffers();
#endif
}


EditableArea::~EditableArea()
{

}


void EditableArea::SetActive(bool activate)
{
    if (activate != isActive)
    {
        isActive = activate;
        
        if (isActive)
            SetOpacityMode(OpacityMode::High);
        else
            SetOpacityMode(OpacityMode::Mid);
    }
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
            g_vertex_dotted_line[i * 3 * 4 + 2] = 0.0f;
            // 1 - down_left
            g_vertex_dotted_line[i * 3 * 4 + 3] = Ox + i * 2 * height * cosa + (width / 2.0f) * sina;
            g_vertex_dotted_line[i * 3 * 4 + 4] = Oy + i * 2 * height * sina - (width / 2.0f) * cosa;
            g_vertex_dotted_line[i * 3 * 4 + 5] = 0.0f;
            // 2 - up_right
            g_vertex_dotted_line[i * 3 * 4 + 6] = Ox + (2 * i + 1)  * height * cosa - (width / 2.0f) * sina;
            g_vertex_dotted_line[i * 3 * 4 + 7] = Oy + (2 * i + 1) * height * sina + (width / 2.0f) * cosa;
            g_vertex_dotted_line[i * 3 * 4 + 8] = 0.0f;
            // 3 - down_right
            g_vertex_dotted_line[i * 3 * 4 + 9] = Ox + (2 * i + 1) * height * cosa + (width / 2.0f) * sina;
            g_vertex_dotted_line[i * 3 * 4 + 10] = Oy + (2 * i + 1) * height * sina - (width / 2.0f) * cosa;
            g_vertex_dotted_line[i * 3 * 4 + 11] = 0.0f;
            
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

void EditableArea::initSmallDiskBuffers()
{
    g_vertex_circle.resize(3 * numVerticesSmallCircle);
    g_circle_indices.resize(3 * numPointsSmallCircle);
    
    // Vertices of the small disk
    double currentAngle = 0.0;
    double incAngle = 2 * M_PI / (double)numPointsSmallCircle;
    g_vertex_circle[0] = 0.0f;
    g_vertex_circle[1] = 0.0f;
    g_vertex_circle[2] = 0.0f;
    for (int i = 1; i < (numPointsSmallCircle+1); ++i)
    {
        g_vertex_circle[i * 3 + 0] = contourPointsRadius * (float)cos(currentAngle);
        g_vertex_circle[i * 3 + 1] = contourPointsRadius * (float)sin(currentAngle);
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
}

void EditableArea::refreshOpenGLSubBuffers(int vertexBufElmtOffset, int indexBufElmtOffset)
{
    if (!isActive || enableTranslationOnly)
    {
        // inutile de remplir tout ça de zéros... On n'ira juste pas dessiner tous ces éléments
        actualVerticesBufferElementsCount = vertexBufElmtOffset;
        actualIndicesBufferElementsCount = indexBufElmtOffset;
        
        // Remplissage toujours ici mais inutile (si besoin pour debug par exemple...)
        /*for (int i = 0; i < getEditableAreaIndexesCount(); ++i)
             getIndicesBuffer()[indexBufElmtOffset + i] = 0;
         */
    }
    
    // We draw additionnal elements only if the polygon is active
    else
    {
        const float Zoffset = getZoffset() + MIEM_EDITION_ELEMENTS_Z;
        
        // buffers accessible via virtual function calls only
        Vector<GLfloat>& vertexBuffer = getVerticesBuffer();
        Vector<GLfloat>& colourBuffer = getColoursBuffer();
        Vector<GLuint>& indexBuffer = getIndicesBuffer();
        
        /// ---------------- manipulationLine + manipulationPoint -----------------------
        const int manipLineVertexBufElmtOffset = vertexBufElmtOffset;
        const int manipLineIndexBufElmtOffset = indexBufElmtOffset;
        
        // - - - - points - - - -
        // Manipulation LINE
        computeManipulationLineBuffer((float)getCenterInPixels().get<0>(), // does all translations required
                                      (float)getCenterInPixels().get<1>(),
                                      (float)bmanipulationPointInPixels.get<0>(),
                                      (float)bmanipulationPointInPixels.get<1>(),
                                      4.0f, 4.0f);
        for (int i = 0; i < dottedLineVertexesCount; ++i)
        {
            // no need to translate x or y
            vertexBuffer[3 * (manipLineVertexBufElmtOffset + i) + 0] = g_vertex_dotted_line[3 * i + 0];
            vertexBuffer[3 * (manipLineVertexBufElmtOffset + i) + 1] = g_vertex_dotted_line[3 * i + 1];
            vertexBuffer[3 * (manipLineVertexBufElmtOffset + i) + 2] = Zoffset;
        }
        
        // Offset for the ring (which is the next thing to fill)
        const int manipRingVertexBufElmtOffset = manipLineVertexBufElmtOffset + dottedLineVertexesCount;
        // Manipulation RING (handle)
        const float Xoffset = (float)bmanipulationPointInPixels.get<0>();
        const float Yoffset = (float)bmanipulationPointInPixels.get<1>();
        for (int j = 0; j < numVerticesRing; ++j)
        {
            vertexBuffer[3 * (manipRingVertexBufElmtOffset + j) + 0] = Xoffset + getRingVertexBuffer()[j*3 + 0];
            vertexBuffer[3 * (manipRingVertexBufElmtOffset + j) + 1] = Yoffset + getRingVertexBuffer()[j*3 + 1];
            vertexBuffer[3 * (manipRingVertexBufElmtOffset + j) + 2] = Zoffset;
        }

        
        // - - - - indexes - - - -
        // Manipulation LINE
        for (int i = 0; i < dottedLineIndicesCount; ++i)
            indexBuffer[manipLineIndexBufElmtOffset + i] = manipLineVertexBufElmtOffset + g_indices_dotted_line[i];
        // Offset for the ring (which is the next thing to fill)
        const int manipRingIndexBufElmtOffset = manipLineIndexBufElmtOffset + dottedLineIndicesCount;
        // Manipulation RING (handle)
        for (int jj = 0; jj < getRingIndexBuffer().size(); ++jj)
            indexBuffer[manipRingIndexBufElmtOffset + jj] = manipRingVertexBufElmtOffset + getRingIndexBuffer()[jj];
        
        
        
        /// ---------------- contours disks on contour points -----------------------
        const int disksVertexBufElmtOffset = manipRingVertexBufElmtOffset + numVerticesRing;
        const int actualContourPointsCount = (int)getContourPointsInPixels().size() - 1;
        
        /// - - - - points - - - -
        // Actual vertices
        for (int i = 0; i < actualContourPointsCount; ++i)
        {
            const float contourPointXoffset = (float)getContourPointsInPixels()[i].get<0>();
            const float contourPointYoffset = (float)getContourPointsInPixels()[i].get<1>();
            for (int j = 0; j < numVerticesSmallCircle; ++j)
            {
                vertexBuffer[3 * (disksVertexBufElmtOffset + i * numVerticesSmallCircle + j) + 0]
                = contourPointXoffset + g_vertex_circle[j*3 + 0];
                vertexBuffer[3 * (disksVertexBufElmtOffset + i * numVerticesSmallCircle + j) + 1]
                = contourPointYoffset + g_vertex_circle[j*3 + 1];
                vertexBuffer[3 * (disksVertexBufElmtOffset + i * numVerticesSmallCircle + j) + 2]
                = Zoffset;
            }
        }
        // filling of the remaining space with zeroes
        // Unnecessary after optimization of "actual indices count"
        /*
        for (int i = actualContourPointsCount; i < numPointsPolygon; ++i)
            for (int jj = 0 ; jj < (3*numVerticesSmallCircle) ; ++jj)
                getVerticesBuffer()[3 * (disksVertexBufElmtOffset + i * numVerticesSmallCircle) + jj] = 0.0f;
         */
        
        
        // - - - - indexes - - - -
        const int disksIndexBufElmtOffset = manipRingIndexBufElmtOffset + (int)getRingIndexBuffer().size();
        for (int i = 0; i < actualContourPointsCount; ++i) // pour chaque petit disque de contour
        {
            // on aura autant de triangles que de points
            for (int jj = 0; jj < numIndicesSmallCircle; ++jj)
                indexBuffer[disksIndexBufElmtOffset + i * numIndicesSmallCircle + jj] = disksVertexBufElmtOffset
                                + i * numVerticesSmallCircle // passage au disque suivant (en comptant les points)
                                + g_circle_indices[jj]; // point du cercle courant
        }
        // remaining indices to zero
        /*
        for (int i = actualContourPointsCount; i < numPointsPolygon; ++i)
            for (int jj = 0; jj < numIndicesSmallCircle; ++jj)
                indexBuffer[disksIndexBufElmtOffset + i * numIndicesSmallCircle + jj] = 0;
        */
        // useless with the optimization...
        
        
        // - - - - Definition of the actual indices count - - - -
        actualVerticesBufferElementsCount = disksVertexBufElmtOffset // début des disques
                    + actualContourPointsCount * numVerticesSmallCircle; // taille des disques en VBO
        actualIndicesBufferElementsCount = disksIndexBufElmtOffset
                    + actualContourPointsCount * numIndicesSmallCircle; // idem
        
        
        // COULEUR TEMP - A FAIRE UNIQUEMENT 1 FOIS AU DEPART ET PLUS JAMAIS APRES
        for (int i=vertexBufElmtOffset ; i<GetVerticesBufferElementsCount() ; i++)
        {
            colourBuffer[4 * i + 0] = editingElementsColour.getRed() / 255.0f;
            colourBuffer[4 * i + 1] = editingElementsColour.getGreen() / 255.0f;
            colourBuffer[4 * i + 2] = editingElementsColour.getBlue() / 255.0f;
            colourBuffer[4 * i + 3] = MIEM_EDITION_ELEMENTS_ALPHA;
        }
    }
}


