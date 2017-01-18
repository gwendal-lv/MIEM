/*
  ==============================================================================

    FrameTriggerComponent.h
    Created: 17 Jan 2017 3:15:33pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef FRAMETRIGGERCOMPONENT_H_INCLUDED
#define FRAMETRIGGERCOMPONENT_H_INCLUDED


#include "JuceHeader.h"

#include "IGraphicSessionManager.h"


namespace Miam
{
    
    // Pre-declarations for pointer members
    class IGraphicSessionManager;
    
    
    /// \brief An almost empty, unvisible component, that trigger the graphic updates.
    /// It MUST attached to the main component of the Juce program.
    ///
    /// The trigger should in the future be bypassed if OpenGL buffer's swap is
    /// synchronized with the screen refresh rate (if "OpenGL is master")
    class FrameTriggerComponent : public AnimatedAppComponent
    {
        
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        protected :
        IGraphicSessionManager* graphicSessionManager;

        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        

        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        FrameTriggerComponent(IGraphicSessionManager* graphicSessionManager_);

        virtual void update() override;
        
    };
    
    
    
}


#endif  // FRAMETRIGGERCOMPONENT_H_INCLUDED
