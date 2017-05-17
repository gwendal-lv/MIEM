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

			AudioDeviceManager& getAudioDeviceManager();

			AudioManager *audioManager;
			Presenter *presenter;
        
    };
}


#endif  // MODEL_H_INCLUDED
