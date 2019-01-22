/*
  ==============================================================================

    View.cpp
    Created: 11 Mar 2016 5:22:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
    
    backgroundComponent->GetMainMenuComponent()->SetHelpString(PlayerHelpContent::GetMainHelp(AppPurpose::Spatialisation));
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

void View::OnNewVolumes(double lowCorrVolume, double highCorrVolume)
{
    String message = TRANS("Plug-in output volume: ");
    message += String(TextUtils::GetAmplitude_dB_string_from_Linear(highCorrVolume, 3));
    message += " dB FS [";
    message += TRANS("correlated inputs");
    message += "], ";
    message += String(TextUtils::GetAmplitude_dB_string_from_Linear(lowCorrVolume, 3));
    message += " dB FS [";
    message += TRANS("decorrelated inputs");
    message += "]";
    DisplayComplementaryInfo(message);
}
