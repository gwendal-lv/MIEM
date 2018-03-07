/*
  ==============================================================================

    IGraphicSessionManager.h
    Created: 21 Nov 2016 12:10:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IGRAPHICSESSIONMANAGER_H_INCLUDED
#define IGRAPHICSESSIONMANAGER_H_INCLUDED

#include <vector>
#include <memory>
#include <string>

#include "JuceHeader.h"

#include "MultiSceneCanvasInteractor.h"
#include "MultiCanvasComponent.h"

#include "GraphicEvent.h"

#include "MiamExceptions.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;


namespace Miam
{
    // pre-declarations for pointer members
    class IPresenter;
    class MultiSceneCanvasInteractor;
    
    
    /// \brief Interface for any manager of a Miam graphic session presenter
    ///
    /// Owns the unique MultiCanvasComponent
    class IGraphicSessionManager// : private Timer
    {
        // At init, for the component to get the canvas managers while keeping them
        // private
        friend MultiCanvasComponent;
        
        
        
        // = = = = = = = = = = COMMON ATTRIBUTES = = = = = = = = = =
        
        protected :
        // Children canvas managers (Declared at first, to be destroyed at last)
        std::vector< std::shared_ptr<MultiSceneCanvasInteractor> > canvasManagers;
        std::shared_ptr<MultiSceneCanvasInteractor> selectedCanvas = 0;
  
        
        // Unique parent/children pointers, so this is the easiest working way...
        private :
        IPresenter* presenter; // classes that inherit should get their own *real* presenter ?
        protected :
        MultiCanvasComponent* multiCanvasComponent;
        
        
        // Graphic objects management
        
        /// \brief Unique Id of the next created (or pasted, or loaded...) area
        int64_t nextAreaId;
        
        
        // states backup
        private :
        bool mouseResizingCanvas = false;
        
        
        
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :

        /// \brief When a new area is to be created, this gets the value of the next area ID, and increments it
        virtual uint64_t GetNextAreaId();
        
        virtual MultiCanvasComponent* GetMultiCanvasComponent() {return  multiCanvasComponent;}
        
        std::chrono::time_point<std::chrono::steady_clock> GetCommonTimePoint() const;
        
        /// \brief !!!!!!!!! À CHANGER pour l'instant toujours faux : pas d'OpenGL... !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        virtual bool IsOpenGlSwapSynced() {return false;}
        
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        /// - - - - - Construction and destruction - - - - -
        /// \brief Initializes pointers to other modules.
        ///
        /// ATTENTION pour l'instant, le nombre de canevas doit être défini dans le
        /// constructeur de la classe réelle qui hérite de celle-ci
        /// Puis au chargement XML, le nombre de canevas chargés devra correspondre...
        IGraphicSessionManager(IPresenter* presenter_);
        virtual ~IGraphicSessionManager();
        
        
        
        
        /// - - - - - Running mode - - - - -
        public :
        
        /// \brief The reaction of this manager when one its canvas sub-components
        /// changes itself its working mode
        virtual void CanvasModeChanged(CanvasManagerMode) = 0;
        
        
        
        
        
        // - - - - - - canvases managing - - - - - -
        
        public :
        /// \brief Sets the new active canvas and updates corresponding graphic objects. Called by the newly selected canvas itself. Can/Must be called by the newly selected canvas itself.
        ///
        /// Tells other canvases to unselect any previously selected area
        virtual void SetSelectedCanvas(std::shared_ptr<MultiSceneCanvasInteractor> selectedCanvas_);
        
        /// \brief Overload (for convenience)
        //void SetSelectedCanvas(MultiSceneCanvasInteractor* canvasInteractor);
        
        
        
        
        /// \brief Displays an informative message sent by one of the managed
        /// objects (Miam::EditableScene, Miam::MultiSceneCanvasEditor,
        /// Miam::InteractiveArea, ...)
        ///
        /// Les infos de + forte priorité écraseront celles de moins forte priorité.
        virtual void DisplayInfo(String info, int priority = 0) = 0;
        
        
        // - - - - - Events from a member of the Presenter module itself - - - - -
        
        /// \brief Receives, processes graphic events from any drawable object,
        /// then interprets it in terms of "audio features" to be transmitted to the
        /// Miam::IModel via the Miam::IPresenter
        virtual void HandleEventSync(std::shared_ptr<GraphicEvent> event_) = 0;

        void CallPresenterUpdate();
      
        
        // - - - - - Mouse Events - - - - -
        
        /// \brief Management of the Miam::MultiCanvasComponent
        ///
        /// May move to a separate manager in the future
        virtual void OnBackgroundMouseDown(const MouseEvent &event);
        /// \brief Management of the Miam::MultiCanvasComponent
        ///
        /// May move to a separate manager in the future
        virtual void OnBackgroundMouseDrag(const MouseEvent &event);
        /// \brief Management of the Miam::MultiCanvasComponent
        ///
        /// May move to a separate manager in the future
        virtual void OnBackgroundMouseUp(const MouseEvent &event);
        
        
        
        // - - - - - Area Events (from managed canvases) - - - - -
        
        
        // - - - - - XML import/export - - - - -
        public :
        virtual std::shared_ptr<bptree::ptree> GetCanvasesTree();
        virtual void SetFromTree(bptree::ptree& graphicSessionTree) = 0;
        /// \brief Récupère les sous-arbres (les scènes) des canevas ; chaque sous-
        /// arbre du vecteur commence par un noeud <canvas>.
        ///
        /// Les canevas sont créés dans le constructeur de l'implémentation réelle
        /// de cette interface IGraphicSessionManager, et leur taille est fixée à la
        /// création.
        /// Donc on ne créé pas les canevas ici... Par contre on vérifie (exception
        /// lancée sinon) que le nombre est cohérent dans le fichier XML.
        std::vector< std::shared_ptr<bptree::ptree> >
        ExtractCanvasesSubTrees(bptree::ptree& canvasesTree);
    };
    
}





#endif  // IGRAPHICSESSIONMANAGER_H_INCLUDED
