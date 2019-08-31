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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Presenter
#include "Presenter.h"
#include "PlayerAppMode.h"



// View
#include "MiamLookAndFeel.h"
#include "PlayerBackgroundComponent.h"


using namespace Miam;
namespace Miam
{
    class Presenter; // cross-inclusion issue
    class View; // cross-inclusion issue
    class MultiCanvasComponent;
}


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
private:
    // Links back to modules
    View* view = 0;
    Presenter* presenter = 0;
    
    // Graphical component
    std::unique_ptr<PlayerBackgroundComponent> backgroundComponent;
    //MultiCanvasComponent* multiCanvasComponent = 0; // belongs to the presenter
    
    // Display attributes
    MiamLookAndFeel lookAndFeel;
    
    // ============== SETTERS AND GETTERS ==============
    public :
    /// \brief Function called after both Miam::View and Miam::Presenter are contructed
    void CompleteInitialization(Presenter* _presenter);
    /// \brief Function called after both View and Presenter are contructed
    void CompleteInitialization(MultiCanvasComponent*);
    
    /// \brief Necessary for the Miam::View to reference itself, because this class is always
    /// constructed by the MainWindow before the View module.
    void SetMiamView(View* _view);
    
    PlayerBackgroundComponent* GetBackgroundComponent() const {return backgroundComponent.get(); }
    
    
    // = = = = = = = = = = METHODS (JUCE AND USER-DEFINED) = = = = = = = = = =
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
