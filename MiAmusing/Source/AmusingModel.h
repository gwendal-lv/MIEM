/*
  ==============================================================================

    Model.h
    Created: 9 Dec 2016 9:24:21am
    Author:  ayup1

  ==============================================================================
*/

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED



#include "IModel.h"
//#include "AudioPlayer.h"
#include "AudioManager.h"
#include "Presenter.h"
#include <string>

#include<memory>

namespace Amusing
{
	// Pre-declaration for pointer members
	class AudioManager;
    
    class AmusingModel : public Miam::IModel
    {
        public :
        
            // constructeur et destructeur
			AmusingModel(Presenter* m_presenter);
            virtual ~AmusingModel();
        
			bool lookForParameter(Miam::AsyncParamChange &param);

			//AudioDeviceManager& getAudioDeviceManager();

			ScopedPointer<AudioManager> audioManager;//AudioManager *audioManager;
			Presenter *presenter;

			void removeDeviceManagerFromOptionWindow();
			std::shared_ptr<AudioDeviceManager> sharedAudioDeviceManager;
			//ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager;
			void addNewSoundPath(int idx, std::string newPath);
			String getSoundPath(int idx);
			
			MidiOutput* getMidiOutput();
			std::map<int, std::string> idxToPath;
    };
}


#endif  // MODEL_H_INCLUDED
