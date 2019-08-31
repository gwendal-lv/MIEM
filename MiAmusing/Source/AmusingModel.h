/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
