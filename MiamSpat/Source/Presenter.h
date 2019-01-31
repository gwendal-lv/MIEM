/*
  ==============================================================================

    Presenter.h
    Created: 11 Mar 2016 4:45:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef PRESENTER_H_INCLUDED
#define PRESENTER_H_INCLUDED

#include <iostream>

#include "PlayerPresenter.h"

#include "SpatType.h"

#include "GraphicSessionPlayer.h"

#include "MiemFileChoosers.h"


namespace Miam {
    // Simple declaration : we don't need the entire description
    class Model;
    class View;
    

    
    /// \brief One of the three main modules of the MiamSpat program, that defines the behavior
	/// between the user and displayed GUI elements. Miam-specific graphical objects belong to it.
	///
	/// Read <a href="http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter">MPV scheme</a>
	/// for a better understanding of MiamEdit functionnalities between the Model, the Presenter 
	/// and the View : "The presenter acts upon the model and the view. It retrieves data from 
	/// repositories (the model), and formats it for display in the view".
	///
	/// \remark Usual GUI controls (created from the Projucer) however belong to the View.
    class Presenter : public PlayerPresenter
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        // Pointer to the unique Model module
        Model* model = 0;
        // Pointer to the unique View module
        View* view = 0;
        
        
        // Sub-modules
        GraphicSessionPlayer graphicSessionManager;
        
        
        // Backups of data from Model
        double lastLowCorrelationVolume = 0.0;
        double lastHighCorrelationVolume = 0.0;
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        virtual GraphicSessionPlayer* GetGraphicSessionPlayer() override
        { return &graphicSessionManager; }
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - Contruction and Destruction - - - - -
        // Only constructor
        Presenter(View* _view);
        virtual ~Presenter() {}
        void CompleteInitialisation(Model* _model);
        
        virtual void OnLoadDefaultSession() override;
        virtual AppPurpose GetSessionPurpose() const override {return AppPurpose::Spatialisation;}
        

        // - - - - - SPAT-specific events from View - - - - -
        virtual void OnMainSliderValueChanged_dB(double newValue_dB) override;

        
        
        // - - - - - Periodic updates - - - - -
        virtual void Update() override;

        
        
    };
    
    
    
}



#endif  // PRESENTER_H_INCLUDED
