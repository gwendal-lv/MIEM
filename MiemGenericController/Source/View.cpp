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

#include "View.h"

#include "Presenter.h"

#include "PlayerHelpContent.h"

using namespace Miam;


// Default constructor
View::View(DocumentWindow* mainWindow_, MainContentComponent* mainContentComponent_)
:
PlayerView(mainWindow_, mainContentComponent_->GetBackgroundComponent())
{
    mainContentComponent = mainContentComponent_;
    
    mainContentComponent->SetMiamView(this);
}


// Default destructor
View::~View()
{
}

// Called by the Presenter during its own construction, after the View is constructed
// ?? Règle précise pour appeler les "compléments d'initialisation" ??
//
void View::CompleteInitialization(Presenter* _presenter)
{
    presenter = _presenter;
    
    mainContentComponent->CompleteInitialization(presenter);
}
void View::CompleteInitialization(GraphicSessionPlayer* /*_graphicSessionManager*/, MultiCanvasComponent* _multiCanvasComponent)
{
    mainContentComponent->CompleteInitialization(_multiCanvasComponent);
    
    DisplayComplementaryInfo("");
    backgroundComponent->GetMainMenuComponent()->SetHelpString(PlayerHelpContent::GetMainHelp(AppPurpose::GenericController));
}


void View::TriggerGLResourcesRelease()
{
#ifdef __MIEM_VBO
    // VBO / textures
    mainContentComponent->GetBackgroundComponent()->GetMultiCanvasComponent()->ReleaseOpengGLResources();
#else
    // Nettoyage OpenGL/Juce de base si nécessaire (attention en conflit avec release VBO)
    view->GetMainContentComponent()->GetSceneEditionComponent()->PrepareUnvisible(); // équivalent pour le miem controller ???
#endif
}


void View::ForceResized()
{
    mainContentComponent->resized();
}

