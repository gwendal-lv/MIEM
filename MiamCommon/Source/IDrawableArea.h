/*
  ==============================================================================

    IDrawableArea.h
    Created: 7 Oct 2016 11:10:46am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/



// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =
// Warning d� � l'h�ritage virtuel... Alors que c'est pr�vu normalement en C++11 !
#ifdef _MSC_VER // indique que c'est une version de visual studio
	#pragma warning( disable : 4250 )
#endif
// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =




#ifndef IDRAWABLEAREA_H_INCLUDED
#define IDRAWABLEAREA_H_INCLUDED

#include "JuceHeader.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


// Simple declaration for a pointer
class SceneCanvasComponent;

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/polygon.hpp"

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;

namespace Miam
{
    /// /brief For menus creation by IDs, etc
    struct AreaDefaultType
    {
        
        enum : int
        {
            // Corresponds the return value of a Juce popup menu,
            // when nothing was selected
            None = 0,
            
            Ellipse,
            
            // Number of sides minimum = 3
            Polygon = 3,
            // Such that Polygon+1 is a square
            // Polygon+2 is a pentagon, etc...
        };
    };
    
    
    /// \brief Interface of the Miam::DrawableArea (which is an abtract object)
    class IDrawableArea
    {
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        IDrawableArea(){}
        virtual ~IDrawableArea() {}
        /// \brief To be overriden within any concrete area that inherits from this.
        virtual IDrawableArea* Clone() const = 0;
        
        
        
        
        /// \brief Function called by a SceneCanvasComponent to draw the Area
        ///
        /// The whole painting job is at the moment performed by the CPU only, but should be
        /// replaced by OpenGL drawing soon.
        virtual void Paint(Graphics& g) = 0;
        /// \brief This function should be called when the area is associated to a (new) canvas,
        /// when its canvas is resized, or when a drawing refresh is needed.
        ///
        /// It updates real-screen pixel coordinates from internally stored normalized coordinates
        /// of all points.
        virtual void CanvasResized(SceneCanvasComponent* parentCanvas) = 0;
        
        
        // ----- Setters and Getters -----
        
        virtual int64_t GetId() = 0;
        virtual void SetId(int64_t _Id) = 0;
        virtual Colour GetFillColour() = 0;
        virtual void SetFillColour(Colour newColour) = 0;
		virtual void SetAlpha(float newAlpha) = 0;
        
        /// \brief Sets the name that could be displayed on screen next to the center
        virtual void SetName(String newName) = 0;
        
        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const = 0;
        virtual std::shared_ptr<bptree::ptree> GetTree() = 0;
        // XML import is made directly from the constructor
        
    };
    
    
}


#endif  // IDRAWABLEAREA_H_INCLUDED
