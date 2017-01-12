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
    class Presenter;
    class SpatSender;
    
    template< typename T >
    class SpatInterpolator;
    
    
    /// \brief
    class SpatModel : public IModel
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        
        // Link to presenter module
        Presenter* presenter = 0;
        
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
        std::string GetOutputName(size_t _i) {return speakers[_i]->GetName();}
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        // - - - - - Construction / destruction - - - - -
        SpatModel(Presenter* _presenter);
        virtual ~SpatModel();
        
        // - - - - - Speakers management - - - - -
        virtual void AddSpeaker();
        virtual void RemoveSpeaker(size_t _id);
        
        
    };
    
}




#endif  // SPATMODEL_H_INCLUDED
