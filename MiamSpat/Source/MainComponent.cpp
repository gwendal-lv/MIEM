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

#include "MainComponent.h"
#include "View.h"

#include <string>

//#include "MultiCanvasComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    // Ajout des composants enfant
    backgroundComponent.reset(new PlayerBackgroundComponent());
    addAndMakeVisible(backgroundComponent.get());
    
    setSize (600, 400);
    
    setLookAndFeel(&lookAndFeel);
}

MainContentComponent::~MainContentComponent()
{
	backgroundComponent = nullptr;

    setLookAndFeel(nullptr);
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour::fromRGB(48, 48, 48));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    // Many assertion failures with this specific text drawing.... --> ????? (glyph position asked from another thread
    // with the message thread running ????)
    /*
    g.drawText (std::string(ProjectInfo::projectName) + " " + std::to_string(ProjectInfo::versionNumber),
                getLocalBounds(), Justification::centred, true);
     */
}

void MainContentComponent::resized()
{
    if (view)
        backgroundComponent->setBounds(view->GetSafeBackgroundBounds(getLocalBounds()));
    else
        backgroundComponent->setBounds(getLocalBounds());
    backgroundComponent->resized(); //  forcé....
}




// ============== SETTERS AND GETTERS ==============
void MainContentComponent::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    assert(view != 0);
    backgroundComponent->CompleteInitialization(presenter, view);
}
void MainContentComponent::CompleteInitialization(MultiCanvasComponent* multiCanvasComponent_)
{
    backgroundComponent->CompleteInitialization(multiCanvasComponent_);
}

void MainContentComponent::SetMiamView(Miam::View* _view)
{
    view = _view;
    
}

