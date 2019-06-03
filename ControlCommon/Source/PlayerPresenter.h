/*
  ==============================================================================

    PlayerPresenter.h
    Created: 1 Mar 2018 3:01:42pm
    Author:  Gwendal

  ==============================================================================
*/

#pragma once


#include "ControlPresenter.h"

#include "PlayerAppMode.h"

#include "RemoteControlServer.h"


namespace Miam
{
    class PlayerModel;
    
    class PlayerView;
    class GraphicSessionPlayer;
    
    
    class PlayerPresenter : public ControlPresenter,
                            private Timer // for auto-updates when necessary (when Juce does not update & render)
    {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        PlayerView* view;
        PlayerModel* model;
        
        protected :
        
        PlayerAppMode appMode;
        /* Permet de savoir, lorsque l'on se balade qqpart dans le
         * programme (dans les menus, etc....), si on a le modèle qui joue en tâche de fond ou pas.
         */
        PlayerAppMode previousPlayerStatus;
        
        /// \brief Will store wether the app has play at least once since it has been opened.
        bool appHasBeenPlayingOnce = false;
        
        
        
        /* Pour l'instant, cet objet est détenu directement par le Presenter
         * Car il s'affiche en force, dans tous les cas, bien au-dessus de tout le reste....
         *
         * À changer peut-être dans une version future
         */
        LoadFileChooser loadFileChooser; // configuré à la construction
        
        
        // To fully control the delay before playing (and the VBO/textures
        // openGL resources acquisition/Release)
        bool isViewPreparingToPlay = false;
        
        // UDP OSC information (from Model)
        int udpPort = -1;
        /// \brief The TCP listening port for opening a secured socket for remote control
        /// of the Presenter
        const int tcpServerPortOffset = 20; // e.g. if OSC on 8001, then TCP remote on 8021
        /// \brief TCP server for Presenter remote control
        RemoteControlServer remoteControlServer;
        
        
        
        // = = = = = = = = = = Setters and Getters = = = = = = = = = =
        public :
        
        PlayerAppMode getAppMode() {return appMode;};
        
        /// \brief Chaque classe fille pourra décider de son propre GraphicSessionPlayer
        virtual GraphicSessionPlayer* GetGraphicSessionPlayer() = 0;
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        PlayerPresenter(PlayerView* _view);
        // virtual dtor
        virtual ~PlayerPresenter() {}
        /// \brief To be called from the Miam::Model when it is being
        /// constructed (happens after the construction of this class)
        ///
        /// Finishes self-contruction, and also the construction of sub-modules
        void CompleteInitialisation(PlayerModel* _model);
        
        void TryLoadFirstSession(std::string commandLine);
        
        
        
        // - - - - - General Management - - - - -
        
        /// \brief Might be called from the Presenter itself, or from the View
        ///
        /// Not supposed to be called from the Model, but this might happen in the future...
        virtual PlayerAppMode appModeChangeRequest(PlayerAppMode newAppMode);
        
        /// \brief Callback invoked when any FileChooser has asynchronously returned.
        void OnFileChooserReturn(const FileChooser& chooser);
        
        
        private :
        /// \brief Timer callback associated to the Update function
        virtual void timerCallback() override;
        
        
        
        protected :
        /// \brief Called by a parent class when emptying the lock-free queue
        // -> override disabled while unused !
        
        //virtual void processParamChangeFromModel(AsyncParamChange const & paramChange) override;
        
        
        
        
        
        // - - - - - Events from the View - - - - -
        public :
        /// \brief
        void OnMainMenuButtonClicked();
        void OnViewIsPreparingToPlay(bool _isPreparingToPlay);
        void OnHelpButtonClicked(bool isHelpCurrentlyDisplayed);
        virtual void OnLoadDefaultSession() = 0;
        /// \brief When the main slider (which is not always displayed or used) has been moved
        virtual void OnMainSliderValueChanged_dB(double /*newValue_dB*/) {} // no default action !
        /// \brief Returns wether the fullscreen mode will be activated or not
        bool OnFullscreenButtonClicked();
        
        /// \brief Re-connection request from View. Always accepted... for now
        /// \returns Whether the request is accepted (new connection params allowed) or not
        bool OnOscConfigurationEditionRequest() {return true;}
        /// \brief Re-connection parameters validated by user (after edition was granted by this).
        /// This will stop the model, reconnect to new parameters, then stop the model, but only
        /// if given parameters are OK for connection
        ///
        /// ipAddress must be either valid or empty.
        /// udpPort must be either valid or -1
        ///
        /// \returns Whether the re-connection was successful or nort
        bool OnOscConfigurationEditionFinished(std::string ipAddress, int udpPort);
        
        
        
        // - - - - - Events from the Model - - - - -
        
        /// \brief Processes the data then displays it. An empty tree means
        /// that the connection failed.
        void OnNewConnectionStatus(bool isConnectionEstablished, std::shared_ptr<bptree::ptree> connectionParametersTree);
        
        
        
        
        // - - - - - Remote control - - - - -
        void ReinitRemoteControlServer();
        // events from the connection object
        void OnRemoteControlConnectionMade();
        void OnRemoteControlConnectionLost();
        
        
        
        
        // = = = = = XML loading only = = = = =
        
        /// \brief Override qui permet de démarrer le Modèle lorsque le chargement de session est terminé
        /// (et qui arrête le modèle au début du chargement de session)
        virtual void LoadSession(std::string filename) override;
        
        virtual void SetConfigurationFromTree(bptree::ptree&) override;
        
        
        
    };
}
