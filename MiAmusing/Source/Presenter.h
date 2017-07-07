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
//#include<boost\container\map.hpp>
#include<boost\bimap.hpp>
#include <map>

#include "IPresenter.h"
#include "AppMode.h"
#include "SpatType.h"

#include "GraphicSessionManager.h"




namespace Amusing {
    // Simple declaration : we don't need the entire description
    class AmusingModel;
    class View;
	class Follower;

    
    /// \brief One of the three main modules of the MiamSpat program, that defines the behavior
	/// between the user and displayed GUI elements. Miam-specific graphical objects belong to it.
	///
	/// Read <a href="http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter">MPV scheme</a>
	/// for a better understanding of MiamEdit functionnalities between the Model, the Presenter 
	/// and the View : "The presenter acts upon the model and the view. It retrieves data from 
	/// repositories (the model), and formats it for display in the view".
	///
	/// \remark Usual GUI controls (created from the Projucer) however belong to the View.
    class Presenter : public Miam::IPresenter
    {
        private :
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =

        // Pointer to the unique View module
        View* view = 0;
        // Pointer to the unique Model module
        AmusingModel* model = 0;
		int tempo;

        AppMode appMode;
        
        
        // Sub-modules
        GraphicSessionManager graphicSessionManager;
        
        // Conversion from areas to sources
		//Presenter myPresenter;
		

		boost::bimap<std::shared_ptr<IEditableArea>,int> areaToSourceMulti;
		std::map<std::shared_ptr<EditableScene>, int> sceneToChannel;
		int Nsources, Nfollower;
		void SetAllChannels();

		boost::bimap < std::shared_ptr<Follower>, int> followerToCtrlSource;
		std::map<int, double> test;

		public :
			void setChannel(std::shared_ptr<EditableScene> scene,int channel);
			int getChannel(std::shared_ptr<EditableScene> scene);
			int getSourceID(std::shared_ptr<IEditableArea> area);
			std::shared_ptr<IEditableArea> getAreaFromSource(int source);
			int getCtrlSourceId(std::shared_ptr<Follower> follower);
			std::shared_ptr<Follower> getFollowerFromCtrl(int ctrlId);

			int getTempo();
			void setTempo(int newTempo);
			void setAudioDeviceManager(AudioDeviceManager* deviceManager);
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        AppMode getAppMode() {return appMode;};
        GraphicSessionManager* getGraphicSessionManager() {return &graphicSessionManager;}
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        
        public :
        
        // - - - - - Contruction and Destruction - - - - -
        // Only constructor
        Presenter(View* _view);
        /// \brief To be called from the Miam::Model when it is being
        /// constructed (happens after the construction of this class)
        ///
        /// Finished self-contruction, and also the construction of sub-modules
        void CompleteInitialisation(AmusingModel* _model);

		virtual void Update() override;
        
		/*AudioDeviceManager* getAudioDeviceManager();*/
		void removeDeviceManagerFromOptionWindow();

        // Events from the View
        AppMode appModeChangeRequest(AppMode newAppMode);
        
		// Events from the view (from the editScene)
		
	private :
		double lastPosition;
    };
    
    
    
}



#endif  // PRESENTER_H_INCLUDED
