/*
  ==============================================================================

    Presenter.h
    Created: 3 May 2017 3:16:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "IPresenter.h"

#include "PeriodicUpdateThread.h"

#include <cmath>
#include <memory>
#include <atomic>
#include <thread>

#include "JuceHeader.h"

#include "OscDebugger.h"

namespace Miam {
    
    
    // Forward declarations
    class MatrixRouterAudioProcessor;
    class NetworkModel;
    class MatrixRouterAudioProcessorEditor;
    class OscMatrixComponent;
    
    /// \brief Base class of the PRESENTER module (see MVP scheme)
    class Presenter : public IPresenter
    {
        
        
        
        // ================== ATTRIBUTES ===================
        
        // - - - - - links to other modules - - - - -
        private :
			/// \brief The plugin editor might be destroyed at any time by the user and/or DAW, but it will notify this class before doing so
			MatrixRouterAudioProcessorEditor * view = 0;
        MatrixRouterAudioProcessor& model;
        std::shared_ptr<NetworkModel> networkModel;
#ifdef __MIAM_DEBUG
		OscDebugger oscLocalhostDebugger;
#endif


		// - - - - - wake-up notification from model - - - - -
		// to compensate the non-working Juce Timers....... lol.
		std::atomic<bool> mustWakeUp;
		std::atomic<bool> continueWakeUpPooling;
		std::thread wakeUpPoolingThread;
		const int poolPeriod_ms = 2;
        
        // - - - - - graphical objects - - - - -
        /// \brief Kept within the presenter, sent to the ProcessorEditor (=View)
        /// when necessary (when recreated by the user) for the View to become its parent
        ScopedPointer<OscMatrixComponent> oscMatrixComponent;
        
        std::string oscAddressCopy;
        
        // - - - - - time management - - - - -
        const double updateFrequency_Hz = 15.0;
        const long updatePeriod_us = 66667; // rien de bien diabolique, correspond à 15Hz
        bool isModelReadyToReceive = false; // But model can force "false"
        
        // ================== METHODS ===================
        
        // - - - - - Construction & destruction - - - - -
        public :
        Presenter(MatrixRouterAudioProcessor& _model, std::shared_ptr<NetworkModel> _networkModel);
        ~Presenter();
        
        // Callback from the Model, after the constructor of the plugin editor
        void OnPluginEditorCreated(MatrixRouterAudioProcessorEditor* view);

        // - - - - - Lock-free Communication - - - - -
        virtual void SendParamChange(AsyncParamChange& paramChange) override;
        virtual bool TrySendParamChange(AsyncParamChange& paramChange) override;
        
        // - - - - - Setters and Getters - - - - -
        OscMatrixComponent* GetOscMatrixComponent() {return oscMatrixComponent;}
        
        
        // - - - - - Periodic Update - - - - -
        const int GetUpdateFrequency_Hz() const {return (int)round(updateFrequency_Hz);}
        // Called on the Juce UI thread from View's Timer
        void UpdateFromView(MatrixRouterAudioProcessorEditor* view);
        // Override method that should not be called here (particular situation : fresh
        // view pointer necessary
        virtual void Update() override;
	protected:
		void poolNotifications();
	public :
		void WakeUp();

		virtual AppPurpose GetSessionPurpose() const override { return AppPurpose::Spatialisation; }
        
    
        // - - - - - Callbacks from View - - - - -
		void OnViewConstructed(MatrixRouterAudioProcessorEditor* _view) { view = _view; }
		void OnViewDestructed() { view = 0; }

        /// \brief Translates the data into a Miam::AsyncParamChange, then sends the data
        /// to the Model.
        void OnSliderValueChanged(int row, int col, double value);
        void OnMatrixZeroed();
        void OnUdpPortChanged(int udpPort);
        void OnAttackDurationChanged(double attackDuration);
        
        // - - - - - Synchronous callbacks from Model - - - - -
        // Bien vérifier à un moment que tout ça passe bien sur le thread UI dans Juce !
        void OnNewUdpPort(int udpPort, bool isConnected);
    };
    
}
