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



namespace Miam
{
    class PlayerModel;
    
    class PlayerView;
    class GraphicSessionPlayer;
    
    
    class PlayerPresenter : public ControlPresenter
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
        
        
        
        /* Pour l'instant, cet objet est détenu directement par le Presenter
         * Car il s'affiche en force, dans tous les cas, bien au-dessus de tout le reste....
         *
         * À changer peut-être dans une version future
         */
        LoadFileChooser loadFileChooser; // configuré à la construction
        
        
        // To fully control the delay before playing (and the VBO/textures
        // openGL resources acquisition/Release)
        bool isViewPreparingToPlay = false;
        
        
        
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
        /// Finished self-contruction, and also the construction of sub-modules
        virtual void CompleteInitialisation(PlayerModel* _model);
        
        void TryLoadFirstSession(std::string commandLine);
        
        
        
        // - - - - - General Management - - - - -
        
        /// \brief Might be called from the Presenter itself, or from the View
        ///
        /// Not supposed to be called from the Model, but this might happen in the future...
        virtual PlayerAppMode appModeChangeRequest(PlayerAppMode newAppMode);
        
        /// \brief Callback invoked when any FileChooser has asynchronously returned.
        void OnFileChooserReturn(const FileChooser& chooser);
        
        virtual void Update() override;
        
        
        
        
        // - - - - - Events from the View - - - - -
        /// \brief
        void OnMainMenuButtonClicked();
        void OnViewIsPreparingToPlay(bool _isPreparingToPlay);
        
        
        
        // - - - - - Events from the Model - - - - -
        
        /// \brief Processes the data then displays it. An empty tree means
        /// that the connection failed.
        void OnNewConnectionStatus(bool isConnectionEstablished, std::shared_ptr<bptree::ptree> connectionParametersTree);
        
        
        // = = = = = XML loading only = = = = =
        
        /// \brief Override qui permet de démarrer le Modèle lorsque le chargement de session est terminé
        /// (et qui arrête le modèle au début du chargement de session)
        virtual void LoadSession(std::string filename) override;
        
        virtual void SetConfigurationFromTree(bptree::ptree&) override;
        
        
        
    };
}
