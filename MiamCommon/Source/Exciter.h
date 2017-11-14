/*
  ==============================================================================

    Exciter.h
    Created: 11 Jan 2017 2:36:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef EXCITER_H_INCLUDED
#define EXCITER_H_INCLUDED


#include <vector>
#include <string>
#include <chrono>

#include "EditablePolygon.h"
#include "EditableEllipse.h"


namespace Miam
{
    
    /// \brief 
    class Exciter : public EditableEllipse
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        double volume;
        
        /// \brief Ids of the Mouse/touch inputs manipulating this area
        std::vector<int> touchInputSources;
        
        // Pointeur car doit être connu des CanvasManagers....
        std::shared_ptr<EditableEllipse> volumeControlArea;
        
        // Display attributes
        
        // Clignotement
        typedef std::chrono::steady_clock clock;
        std::chrono::time_point<clock> startTimePt;
        std::chrono::time_point<clock> commonStartTimePt;
        double const omega = 2.0 * M_PI * 1.0; // 1 Hz
        double const deltaBrightnessAmplitude = 0.3;
        double const deltaBrightnessOffset = 0.7;
        
        // Pour synchronisation du clignotement de tous les excitateurs ensemble.
        private :
        bool isAnimationSynchronized;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual std::string GetTypeAsString() const override {return "Exciter";}
        
        /// \brief Sets whether this exciter is animated the same as the others exciters that
        /// have this option activated.
        ///
        /// The synchronized clock must have been initialized before
        void SetIsAnimationSynchronized(bool isSynchronized);
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction/Destruction + polymorphic cloning - - - - -
        
        Exciter(bptree::ptree & areaTree, std::chrono::time_point<clock> commonStartTimePoint_);
        /// \brief Will automatically define the shape of the exciter
        Exciter(uint64_t uniqueId, std::chrono::time_point<clock> commonStartTimePoint_);
        virtual ~Exciter() {}
        virtual IDrawableArea* Clone() const override {return new Exciter(*this);}
        
        // - - - - - Ction helpers - - - - -
        private :
        void init();

		double computeXScale(float _canvasRatio) override;
		double computeYScale(float _canvasRatio) override;
        
        // - - - - - Display - - - - -
        public :
        /// \brief Paints specific elements over the elements painted
        /// by the EditableEllipse::Paint() method
        virtual void Paint(Graphics& g) override;

        
        // - - - - - XML import/export - - - - -
        virtual std::shared_ptr<bptree::ptree> GetTree() override;
        
    };
    
    
    
}




#endif  // EXCITER_H_INCLUDED
