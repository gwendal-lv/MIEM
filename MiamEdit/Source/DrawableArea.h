/*
  ==============================================================================

    DrawableArea.h
    Created: 26 Mar 2016 12:04:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef DRAWABLEAREA_H_INCLUDED
#define DRAWABLEAREA_H_INCLUDED

#include "JuceHeader.h"


namespace Miam
{
    
    class DrawableArea
    {
        
        public :
        
        DrawableArea(int64_t _Id, Point<double> _center, Colour _fillColour);
        
        virtual ~DrawableArea() {};
        
        
        virtual void Paint(Graphics& g) = 0;
        virtual void CanvasResized(int width, int height) = 0;
        
        
        // Setters and Getters
        int64_t GetId() {return Id;}
        void SetId(int64_t _Id) {Id = _Id;}
        Colour GetFillColour() {return fillColour;}
        void SetFillColour(Colour newColour);
        
        
        // General data
        protected :
        uint64 Id;
        
        // Geometric data
        protected :
        Point<double> center;
        Point<double> centerInPixels;
        
        
        // Display data
        protected :
        Colour fillColour;
        float fillOpacity;
        
        Colour contourColour;
        float contourWidth;
        float centerContourWidth;
        
        int centerCircleRadius;
    };
    
}



#endif  // DRAWABLEAREA_H_INCLUDED
