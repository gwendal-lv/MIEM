/*
  ==============================================================================

    SpatModel.h
    Created: 28 Nov 2016 11:02:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATMODEL_H_INCLUDED
#define SPATMODEL_H_INCLUDED


#include <vector>
#include <memory>
#include <thread>
#include <atomic>

#include "IModel.h"
#include "PeriodicUpdateThread.h"

#include "SpatType.h"
#include "SpatInterpolator.hpp"
#include "Speaker.h"

#include "SpatMatrix.hpp"
#include "AudioDefines.h"


namespace Miam
{
    // pre-declarations for pointers
    class IPresenter;
    class SpatSender;
    
    
    
    
    
    /// \brief
    class SpatModel : public IModel, protected PeriodicUpdateThread
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        
        private :
        // Link to presenter module, children classes will have their own ptr
        IPresenter* presenter = 0;
        
        
        protected :
        // The model itself manages the speakers, for now
        // May be changed... But it remains important low-level data
        std::vector< std::shared_ptr<Speaker<double>> > speakers;
        
        // Spatialisation computer, that owns the spat states
        std::shared_ptr<SpatInterpolator<double>> spatInterpolator;
        
        // Communication
        std::vector< std::shared_ptr<SpatSender> > spatSenders;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        SpatType GetSpatType() {return spatInterpolator->GetSpatType();}
        std::shared_ptr<SpatInterpolator<double>> GetSpatInterpolator() {return spatInterpolator;}
        
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// plus valable..... le spat interpolator doit tout contrôler
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        size_t GetOutputsCount() {return speakers.size();}
        //std::string GetOutputName(size_t i_) {return speakers[i_]->GetName();}
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction / destruction - - - - -
        SpatModel(IPresenter* presenter_, double updateFrequency_Hz = 500.0);
        virtual ~SpatModel();
        
        // - - - - - Speakers management - - - - -
        virtual void AddSpeaker();
        virtual void RemoveSpeaker(size_t id_);
        
        
        // - - - - - Periodic updates - - - - -
        protected :
			/// \brief HUGE warning : VS2015 does not properly runs polymorphism
		/// if the thread that calls this function is created from this 
		/// class' contructor. The "this" pointer is not considered as a real
		/// type and an abstract method is called -> bizarre undefined random
		/// behavior then...
		///
		/// Marked as private for now to avoid that bug
	private:
        virtual void update() = 0;
        
        
    };
    
}




#endif  // SPATMODEL_H_INCLUDED
