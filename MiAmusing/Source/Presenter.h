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
#include<boost/bimap.hpp>
#include <map>

#include "IPresenter.h"
#include "AppMode.h"
#include "SpatType.h"

#include "GraphicSessionManager.h"


class Cursor;

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
		float masterVolume;

        AppMode appMode;
        
        
        // Sub-modules
        GraphicSessionManager graphicSessionManager;
        
        // Conversion from areas to sources
		//Presenter myPresenter;
		//int gamme[7] = {0,2,4,5,7,9,11};
		int gamme[5] = { 0,3,4,6,7};
		struct Class1Compare
		{
			bool operator() (const Colour& lhs, const Colour& rhs) const
			{
				if (lhs.getRed() < rhs.getRed())
					return true;
				else if (lhs.getRed() > rhs.getRed())
					return false;
				else
				{
					if (lhs.getGreen() < rhs.getGreen())
						return true;
					else if (lhs.getGreen() > rhs.getGreen())
						return false;
					else
					{
						if (lhs.getBlue() < rhs.getBlue())
							return true;
						else
							return false;
					}
				}
				
			}
		};

		std::map<std::shared_ptr<IEditableArea>, int> areaToInitSurface;
		std::map<Colour, int,Class1Compare> colourToIdx;
		std::map<std::shared_ptr<IEditableArea>, int> colorIdx;
		std::map<std::shared_ptr<IEditableArea>, int> octave;
		std::map<std::shared_ptr<IEditableArea>, double> phase;
		std::map<std::shared_ptr<IEditableArea>, double> areaToSpeed;
		std::map<std::shared_ptr<IEditableArea>, double> areaToVelocity;
		boost::bimap<std::shared_ptr<IEditableArea>,int> areaToSourceMulti;
		boost::bimap<std::shared_ptr<Cursor>, int> cursorToReadingHead;
		std::map<std::shared_ptr<EditableScene>, int> sceneToChannel;
		int Nsources, Ncursors;
		void SetAllChannels();


		public :
			void addOctave(std::shared_ptr<IEditableArea> newArea);
			int getNote(std::shared_ptr<IEditableArea> area, int circle);
			void setOctave(std::shared_ptr<IEditableArea> currentArea, int newOctave);
			int getColorIdx(std::shared_ptr<IEditableArea> currentArea);
			void setColorIdx(std::shared_ptr<IEditableArea> currentArea, int idx);
			int getOctave(std::shared_ptr<IEditableArea> area);
			int getReadingHeadID(std::shared_ptr<Cursor> cursor);
			int getTimeLineID(std::shared_ptr<IEditableArea> area);
			void deleteReadingHeadRef(std::shared_ptr<Cursor> cursor);
			std::shared_ptr<Cursor> getCursor(int m_Id);
			std::shared_ptr<IEditableArea> getAreaFromSource(int source);
			void setSpeedArea(std::shared_ptr<IEditableArea> scene, double speed);
			double getSpeedArea(std::shared_ptr<IEditableArea> area);
			void setVelocityArea(std::shared_ptr<IEditableArea> scene, double velocity);
			double getVelocityArea(std::shared_ptr<IEditableArea> area);
			void setColorPath(int idx, Colour concernedColour, String pathAssociated);
			int getPathIdx(Colour color);
			void setInitSize(std::shared_ptr<IEditableArea> newArea,int surface);

			double computeFrequency(std::shared_ptr<IEditableArea> newArea,double surface);

			int getTempo();
			void setTempo(int newTempo);
			float getMasterVolume();
			void setMasterVolume(float newVolume);
			
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
		//double lastPosition;
		std::map<int,double> lastPositions;
		std::map<int, bool> positionChanged;
    };
    
    
    
}



#endif  // PRESENTER_H_INCLUDED
