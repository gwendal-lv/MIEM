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
#include "boost\geometry.hpp"
#include "boost\geometry\geometries\geometries.hpp"
#include "boost\geometry\geometries\polygon.hpp"
#include "IDrawableArea.h"

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;

// Simple declaration for a pointer
class SceneCanvasComponent;


namespace Miam
{
    
    /// \brief Abstract class from which all Miam areas will be derived.
    class DrawableArea : public virtual IDrawableArea
    {
        
        public :
        
        /// \brief Minimal constructor
        ///
        /// \param _Id Unique ID of the area
        /// \param _center Normalized center coordinates (x, y in [0.0,1.0])
        /// \param _fillColour Solid colour for filling the area (opacity should be 0xFF)

		DrawableArea(int64_t _Id, bpt _center, Colour _fillColour);
        
        /// \brief Virtual destructor.
        virtual ~DrawableArea() {}
        
        
        virtual void Paint(Graphics& g);
        virtual void CanvasResized(SceneCanvasComponent* _parentCanvas);

        
        // ----- Setters and Getters -----
        
        /// \returns Unique ID of the area
        int64_t GetId() {return Id;}
        /// \param _Id Unique ID of the area
        void SetId(int64_t _Id) {Id = _Id;}
        /// \returns See DrawableArea::fillColour
        Colour GetFillColour() {return fillColour;}
        /// \param _fillColour See DrawableArea::fillColour
        void SetFillColour(Colour newColour);
        
        void SetNameVisible(bool isVisible) {isNameVisible = isVisible;}
        
        
        // General data
        protected :
        uint64 Id; ///< Unique ID
        SceneCanvasComponent* parentCanvas; ///< Parent that draws this(needed to get display properties)
        
        // Geometric data
        protected :
		bpt center; ///< Normalized center coordinates (x, y in [0.0,1.0])
		bpt centerInPixels; ///< Center coordinates in pixels (relative to a canvas)
        
        
        // Display data
        protected :
        Colour fillColour; ///< Solid colour for filling the area (opacity should be 0xFF)
        float fillOpacity; ///< Opacity applied to the solid fill colour (in [0.0,1.0])
        
        Colour contourColour; ///< Solid color of of the external shape of the 2D area.
        float contourWidth; ///< Width (in pixels) of of the external shape of the 2D area.
        float centerContourWidth; ///< Width (in pixels) of the center circle drawing.
        
        int centerCircleRadius; ///< Radius (in pixels) of the center circle drawing.
            
        bool isNameVisible;
    };
    
}



#endif  // DRAWABLEAREA_H_INCLUDED
