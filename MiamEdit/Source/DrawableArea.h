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
    
    /// \brief Abstract class from which all Miam areas will be derived.
    class DrawableArea
    {
        
        public :
        
        /// \brief Minimal constructor
        ///
        /// \param _Id Unique ID of the area
        /// \param _center Normalized center coordinates (x, y in [0.0,1.0])
        /// \param _fillColour Solid colour for filling the area (opacity should be 0xFF)
        DrawableArea(int64_t _Id, Point<double> _center, Colour _fillColour);
        
        /// \brief Virtual destructor.
        virtual ~DrawableArea() {};
        
        /// \brief Function called by a SceneCanvasComponent to draw the Area
        ///
        /// The whole painting job is at the moment performed by the CPU only, but should be
        /// replaced by OpenGL drawing soon.
        virtual void Paint(Graphics& g) = 0;
        /// \brief This function should be called when canvas is resized,
        /// or when a drawing refresh is needed.
        ///
        /// It updates real-screen pixel coordinates from internally stored normalized coordinates
        /// of all points.
        virtual void CanvasResized(int width, int height) = 0;
        
        
        // ----- Setters and Getters -----
        
        /// \returns Unique ID of the area
        int64_t GetId() {return Id;}
        /// \param _Id Unique ID of the area
        void SetId(int64_t _Id) {Id = _Id;}
        /// \returns See DrawableArea::fillColour
        Colour GetFillColour() {return fillColour;}
        /// \param _fillColour See DrawableArea::fillColour
        void SetFillColour(Colour newColour);
        
        
        // General data
        protected :
        uint64 Id; ///< Unique ID
        
        // Geometric data
        protected :
        Point<double> center; ///< Normalized center coordinates (x, y in [0.0,1.0])
        Point<double> centerInPixels; ///< Center coordinates in pixels (relative to a canvas)
        
        
        // Display data
        protected :
        Colour fillColour; ///< Solid colour for filling the area (opacity should be 0xFF)
        float fillOpacity; ///< Opacity applied to the solid fill colour (in [0.0,1.0])
        
        Colour contourColour; ///< Solid color of of the external shape of the 2D area.
        float contourWidth; ///< Width (in pixels) of of the external shape of the 2D area.
        float centerContourWidth; ///< Width (in pixels) of the center circle drawing.
        
        int centerCircleRadius; ///< Radius (in pixels) of the center circle drawing.
    };
    
}



#endif  // DRAWABLEAREA_H_INCLUDED
