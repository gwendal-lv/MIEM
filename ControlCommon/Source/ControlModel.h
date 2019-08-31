/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
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

#include "AppPurpose.h"


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
        // A master gain that gain be applied or not to the final state
        double masterGain = 1.0;
        bool masterGainEnabled = false;
        
        // Communication
        std::vector< std::shared_ptr<ControlStateSender<double>> > stateSenders;
        std::shared_ptr<MiamOscSender<double>> miamOscSender; // for quick easy access...
        
        // Time Management
        std::chrono::time_point<std::chrono::steady_clock> commonStartTimePt;
        
        
        AsyncParamChange::ParamType playState;
        
        // Rafraîchissements forcés
        int refreshFramesCounter = 0;
        const int refreshPeriod_frames = 250; // unité = frames
        bool continuousBackgroundBlobMatrixRefresh = true;
        bool continuousBackgroundSingleMatrixCoeffRefresh = false;
        bool wasSomethingUpdated = false;
        
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        
        public :
        /// \brief If gain is disabled, its value becomes automatically 1.0 to be neutral
        ///
        /// The value of the gain must be send through the AsyncParamChange packets via the lock-free queue.
        void SetIsMasterGainEnabled(bool isEnabled);
        bool GetIsMasterGainEnabled() {return masterGainEnabled;}
        
        
        protected :
        /// \brief For internal quick access. Might be deleted eventually.
        std::shared_ptr<MiamOscSender<double>>& getMainSpatSender()
        { return miamOscSender; };
        
        public :
		/// \brief Par défaut, on retourne l'attribut statique global
		virtual AppPurpose GetSessionPurpose() const { return App::GetPurpose(); }

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
        ControlModel(ControlPresenter* presenter_,
                     bool startUpdateThread = false,
                     double updateFrequency_Hz = 500.0);
        virtual ~ControlModel();


        
        // - - - - - Periodic updates - - - - -
        protected :
			/// \brief HUGE warning : VS2015 does not properly runs polymorphism
		/// if the thread that calls this function is created from this 
		/// class' contructor. The "this" pointer is not considered as a real
		/// type and an abstract method is called -> bizarre undefined random
		/// behavior then...
        virtual void update();
        ///  \brief Virtual function called from update() a the very beginning of the loop.
        ///
        /// To be overriden by child classes
        virtual void onUpdateStarts() {}
        ///  \brief Virtual function called from update() a the end of the loop, after
        /// updates were actually applied but before putting the Model thread to sleep.
        ///
        /// To be overriden by child classes
        virtual void onUpdateFinished() {}
        
        
        // - - - - - Internal events - - - - -
        protected :
        virtual void onPlay();
        
        
        // - - - - - Events from Presenter - - - - -
        public :
        /// \brief Resets the OSC sender using the property tree.
        /// \returns Whether it could connect or not using the UDP/IP data from the property tree.
        virtual bool ResetOscConfigurationFromTree(bptree::ptree& tree);
        
        
        // - - - - - Property tree (for XML) import/export - - - - -
        public :
        /// \brief Returns the property tree describing all model settings data
        std::shared_ptr<bptree::ptree> GetConfigurationTree();
        virtual void SetConfigurationFromTree(bptree::ptree& tree);
        
    };
    
}




#endif  // CONTROLMODEL_H_INCLUDED
