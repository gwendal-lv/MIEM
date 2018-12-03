/*
  ==============================================================================

    EditableArea.cpp
    Created: 12 Apr 2016 4:39:52pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "EditableArea.h"


using namespace Miam;



EditableArea::EditableArea() : bmanipulationPointInPixels(0,0)
{
    enableTranslationOnly = false;
    
#ifdef __MIEM_VBO
    // Proper resize of OpenGL buffer for VBO rendering
    g_vertex_dotted_line.resize(3 * dottedLineVertexesCount);
    g_indices_dotted_line.resize(dottedLineIndicesCount);
    
    g_vertex_circle.resize(3 * numVerticesCircle);
    circleIndices.resize(3 * numPointCircle);
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



