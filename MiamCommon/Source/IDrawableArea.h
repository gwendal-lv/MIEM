/*
  ==============================================================================

    IDrawableArea.h
    Created: 7 Oct 2016 11:10:46am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/



// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =
// Warning d'héritage virtuel... Alors que c'est prévu normalement en C++11 !
#ifdef _MSC_VER // indique que c'est une version de visual studio
	#pragma warning( disable : 4250 )
#endif
// = = = = = = = = = SUPPRESSION D'UN WARNING Visual Studio = = = = = = = = = = =




#ifndef IDRAWABLEAREA_H_INCLUDED
#define IDRAWABLEAREA_H_INCLUDED

#include <memory>

#include "JuceHeader.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/polygon.hpp"
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> bpt;
typedef boost::geometry::model::polygon<bpt> bpolygon;


// Simple declaration for a pointer
class SceneCanvasComponent;



namespace Miam
{
    // Mise ici parce que les aires seulement sont concernées par l'opacité...
    enum class OpacityMode {
        Independent,
        DependingOnExcitement,
        
        Low, ///< Lowest possible opacity for an area
        Mid, ///< Medium opacity
        High ///< High opacity, but not totally opaque
    };
    
    
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
        virtual std::shared_ptr<IDrawableArea> Clone() = 0;
        protected :
        /// \brief Applique toutes les modifications nécessaires suite au clonage d'un objet
        ///
        /// Par exemple : supprime les liens avec d'autres objets graphiques (s'ils existent),
        /// en supposant que ces liens ont été copiés
        /// simplement lors d'un clonage qui vient tout juste d'être fait.
        ///
        /// \warning To be called from any Clone() overriden method
        virtual void onCloned() = 0;
        
        
        
        public :
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
        
		virtual bool IsActive() { return true; }
		virtual bool ShowCenter() { return true; }
		virtual bool hasVerticesChanged() = 0;
		virtual bool hasPositionChanged() = 0;
		virtual bpt GetManipulationPoint() { return bpt(0.0, 0.0); }
		virtual int GetOpaqueVerticesCount() = 0;
		virtual int GetIndexCount() = 0;
		virtual int GetOpaqueColourCount() = 0;
		virtual float GetOpaqueColour(int idx) = 0;
		virtual float GetOpaqueVertices(int idx) = 0;
		virtual int GetIndex(int idx) = 0;
		virtual Vector3D<float> GetModelParameters() = 0; // return parameter for ModelMatrix
		//virtual Matrix3D<float> GetModelMatrix() = 0;
        
        // ----- Setters and Getters -----
        
        virtual int64_t GetId() const = 0;
        virtual void SetId(int64_t _Id) = 0;
        virtual Colour GetFillColour() const = 0;
        virtual void SetFillColour(Colour newColour) = 0;
		virtual void SetAlpha(float newAlpha) = 0;
        virtual float GetAlpha() const = 0;
        virtual void SetOpacityMode(OpacityMode enable) = 0;
        virtual OpacityMode GetOpacityMode() const = 0;
        virtual void SetRenderingScale(double renderingScale_) = 0;
        
        /// \brief Sets the name that could be displayed on screen next to the center
        virtual void SetName(String newName) = 0;
        protected :
        virtual float getLowFillOpacity() const = 0;
        public :
        
        
        // - - - - - XML import/export - - - - -
        virtual std::string GetTypeAsString() const = 0;
        virtual std::shared_ptr<bptree::ptree> GetTree() = 0;
        // XML import is made directly from the constructor
        
    };
    
    
}


#endif  // IDRAWABLEAREA_H_INCLUDED
