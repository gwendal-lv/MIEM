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
#include <chrono>

#include "IModel.h"
#include "PeriodicUpdateThread.h"

#include "SpatType.h"
#include "SpatInterpolator.hpp"
#include "MiamOscSender.hpp"
#include "Speaker.h"

#include "SpatMatrix.hpp"
#include "AudioDefines.h"


namespace Miam
{
    // pre-declarations for pointers
    class SpatPresenter;
    
    
    
    
    
    /// \brief
    class SpatModel : public IModel, protected PeriodicUpdateThread
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        
        private :
        // Link to presenter module, children classes will have their own ptr
        SpatPresenter* presenter = 0;
        
        
        protected :
        // The model itself manages the speakers, for now
        // May be changed... But it remains important low-level data
        std::vector< std::shared_ptr<Speaker<double>> > speakers;
        
        // Spatialisation computer/interpolator, that owns the spat states
        std::shared_ptr<SpatInterpolator<double>> spatInterpolator;
        
        // Communication
        std::vector< std::shared_ptr<SpatSender<double>> > spatSenders;
        
        // Time Management
        std::chrono::time_point<std::chrono::steady_clock> commonStartTimePt;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        SpatType GetSpatType() {return spatInterpolator->GetSpatType();}
        std::shared_ptr<SpatInterpolator<double>> GetSpatInterpolator() {return spatInterpolator;}
        /// \brief Only 1 spat sender (index 0) available at the moment.
        std::shared_ptr<SpatSender<double>> GetSpatSender(size_t index)
        { return spatSenders[0]; };
        
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
        SpatModel(SpatPresenter* presenter_, double updateFrequency_Hz = 500.0);
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
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        public :
        /// \brief Returns the property tree describing all model settings data
        std::shared_ptr<bptree::ptree> GetConfigurationTree();
        void SetConfigurationFromTree(bptree::ptree& tree);
        
    };
    
}




#endif  // SPATMODEL_H_INCLUDED
