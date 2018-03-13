/*
  ==============================================================================

    ControlModel.h
    Created: 28 Nov 2016 11:02:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef CONTROLMODEL_H_INCLUDED
#define CONTROLMODEL_H_INCLUDED


#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>

#include "IModel.h"
#include "PeriodicUpdateThread.h"

#include "StatesInterpolator.hpp"
#include "MiamOscSender.hpp"

#include "ControlMatrix.hpp"
#include "AudioDefines.h"


namespace Miam
{
    // pre-declarations for pointers
    class ControlPresenter;
    
    
    
    
    
    /// \brief
    class ControlModel : public IModel, protected PeriodicUpdateThread
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        
        private :
        // Link to presenter module, children classes will have their own ptr
        ControlPresenter* presenter = 0;
        
        
        protected :
        
        // States computer/interpolator, that owns the states
        std::shared_ptr<StatesInterpolator<double>> interpolator;
        
        // Communication
        std::vector< std::shared_ptr<ControlStateSender<double>> > stateSenders;
        
        // Time Management
        std::chrono::time_point<std::chrono::steady_clock> commonStartTimePt;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        virtual InterpolationType GetInterpolatorType_Atomic () override;
        InterpolationType GetInterpolatorType() {return interpolator->GetType();}
        std::shared_ptr<StatesInterpolator<double>> GetInterpolator() {return interpolator;}
        /// \brief Only 1 sender (index 0) available at the moment.
        std::shared_ptr<ControlStateSender<double>> GetStateSender(size_t index);
        
        /// \brief See Miam::StatesInterpolator
        size_t GetOutputsCount();
        
        //std::string GetOutputName(size_t i_) {return speakers[i_]->GetName();}
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction / destruction - - - - -
        ControlModel(ControlPresenter* presenter_, double updateFrequency_Hz = 500.0);
        virtual ~ControlModel();
        
        
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
        virtual void SetConfigurationFromTree(bptree::ptree& tree);
        
    };
    
}




#endif  // CONTROLMODEL_H_INCLUDED
