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
#include "AudioPlayer.h"

namespace Miam
{
    
    
    class Model : public IModel
    {
        public :
        
            // constructeur et destructeur
            Model(Presenter* _presenter);
            ~Model();
        
			AudioPlayer *audioPlayer;
            
            
        
    };
}


#endif  // MODEL_H_INCLUDED
