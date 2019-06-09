/*
  ==============================================================================

    GraphicControlSessionManager.h
    Created: 13 Jun 2017 9:11:20pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <memory>

#include "IGraphicSessionManager.h"
#include "ControlArea.h"

#include "StatesInterpolator.hpp"

#include "AsyncParamChange.h"


namespace Miam
{
    // Forward declarations
    class ControlPresenter;
    
    class GraphicControlSessionManager : public IGraphicSessionManager
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        
        private :
        ControlPresenter* presenter;
        
        protected :
        
        // For communication with Model
        std::shared_ptr<StatesInterpolator<double>> spatInterpolator;
        
        
        
        
        
        // = = = = = = = = = = Getters and Setters = = = = = = = = = =
        public :
        
        
        
        // = = = = = = = = = = = = = = METHODS = = = = = = = = = = = = = =
        
        public :
        
        // - - - - - Construction/destruction, and helpers - - - - -
        
        /// \brief Construction (the whole Presenter module is built after the View).
        GraphicControlSessionManager(ControlPresenter* _presenter);
        
        /// \Brief to be called after Model construction (which occurs
        /// after Presenter construction)
        void CompleteInitialisation(std::shared_ptr<StatesInterpolator<double>> _spatInterpolator);
        
        /// \brief Destruction and the editor and the canvases
        virtual ~GraphicControlSessionManager() {}
        
        protected :
        AsyncParamChange buildExcitementParamChange(std::shared_ptr<ControlArea> area);

        
        // - - - - - XML import/export - - - - -
        public :
        virtual void SetFromTree(bptree::ptree& graphicSessionTree) override;
        /// \Brief Finishes to XML-load spat attributes of an area, from the inner
        /// content of the <spatstate> tag.
        ///
        /// \param spatStateTree may be a std::nullptr
        virtual void LoadSpatAreaLinks(std::shared_ptr<ControlArea> area, std::shared_ptr<bptree::ptree> spatStateTree);
        
    };
}
