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

#include "IModel.h"

#include "SpatType.h"
#include "Speaker.h"


namespace Miam
{
    // pre-declarations for pointers
    class IPresenter;
    class SpatSender;
    
    template< typename T >
    class SpatInterpolator;
    
    
    /// \brief
    class SpatModel : public IModel
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        
        private :
        // Link to presenter module, children classes will have their own ptr
        IPresenter* presenter = 0;
        
        
        protected :
        /// \brief Model update thread running at an almost constant frequency
        std::thread updateThread;
        double updateThreadF_Hz;
        int updateThreadT_us;
        std::atomic<bool> continueUpdate;
        /// \brief Time measuring helper for thread frequency stabilisation
        FrequencyMeasurer updateThreadMeasurer;
        
        
        
        // The model itself manages the speakers, for now
        // May be changed... But it remains important low-level data
        std::vector< std::shared_ptr<Speaker<double>> > speakers;
        
        // Spatialisation computer, that owns the spat states
        SpatInterpolator<double>* spatInterpolator = 0;
        
        // Internal descriptor
        SpatType spatType;
        
        // Communication
        std::vector< std::shared_ptr<SpatSender> > spatSenders;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        
        SpatType GetSpatType() {return spatType;}
        SpatInterpolator<double>* GetSpatInterpolator() {return spatInterpolator;}
        
        size_t GetOutputsCount() {return speakers.size();}
        std::string GetOutputName(size_t i_) {return speakers[i_]->GetName();}
        
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
        virtual void update() = 0;
        
        
    };
    
}




#endif  // SPATMODEL_H_INCLUDED
