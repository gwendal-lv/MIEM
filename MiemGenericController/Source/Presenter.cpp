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

#include "Presenter.h"

#include "Model.h"
#include "View.h"

#include "JuceHeader.h"


#include "AppPurpose.h"
#include "MiemDefaultSessions.h"

using namespace Miam;


// - - - - - Contruction and Destruction - - - - -
AppPurpose App::appPurpose = AppPurpose::GenericController;

Presenter::Presenter(View* _view) :
    PlayerPresenter(_view),

    view(_view),

    graphicSessionManager(this, _view)
{
    // After all sub-modules are built, the presenter refers itself to the View
    view->CompleteInitialization(this);
    //view->GetMainContentComponent()->resized();
    
    appModeChangeRequest(PlayerAppMode::Loading);
}

void Presenter::CompleteInitialisation(Model* _model)
{
    // - - - Init des attributs privés puis des parents - - -
    model = _model;
    PlayerPresenter::CompleteInitialisation(_model);
}

void Presenter::OnLoadDefaultSession()
{
    LoadSession(DefaultSessions::GetDefaultSessionCode_mcs());
}


// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =



// = = = = = = = = = = METHODES = = = = = = = = = =
void Presenter::processParamChangeFromModel(AsyncParamChange const & paramChange)
{
	// no specific event treated specifically here... Control/Player/Presenter will do the job
    PlayerPresenter::processParamChangeFromModel(paramChange);
}


