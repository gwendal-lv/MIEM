/*
  ==============================================================================

    AmusingSceneComponent.cpp
    Created: 7 Jul 2017 3:39:08pm
    Author:  ayup1

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmusingSceneComponent.h"
#include "MultiSceneCanvasManager.h"

#include <memory>

using namespace Miam;

//==============================================================================
AmusingSceneComponent::AmusingSceneComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	currentSideBarType = SideBarType::None;
	isOptionShowed = false;

	showOptionsButton = new TextButton();
	showOptionsButton->setButtonText("opt");
	showOptionsButton->setSize(40, 20);
	showOptionsButton->setTopLeftPosition(0, 0);
	showOptionsButton->addListener(this);
	openGlContext.attachTo(*showOptionsButton);
	addAndMakeVisible(showOptionsButton);

	openGlContext.attachTo(areaOptions);
	addAndMakeVisible(areaOptions); // test pour voir si on le voit bien
	areaOptions.setBounds(0 , 0, 300, 200);
	areaOptions.CompleteInitialization(this);
	areaOptions.setVisible(false);
}

AmusingSceneComponent::~AmusingSceneComponent()
{
	//openGlContext.detach(areaOptions);
	//openGlContext.detach();
	delete showOptionsButton;
}

void AmusingSceneComponent::setSamplesColor(int Nsamples, Colour colorCode[])
{
	areaOptions.setSamplesColor(Nsamples, colorCode);
}

void AmusingSceneComponent::addColourSample(int index, Colour colour)
{
	areaOptions.addColourSample(index, colour);
}

void AmusingSceneComponent::renderOpenGL()
{
	//SceneCanvasComponent::renderOpenGL();

	/*auto manager = canvasManager.lock();

	const double desktopScale = openGlContext.getRenderingScale();
	std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGlContext,
		roundToInt(desktopScale * getWidth()),
		roundToInt(desktopScale * getHeight())));
	Graphics g(*glRenderer);

	g.addTransform(AffineTransform::scale((float)desktopScale));

	*/

	

	

	auto manager = canvasManager.lock();

	const double desktopScale = openGlContext.getRenderingScale();
	std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGlContext,
		roundToInt(desktopScale * getWidth()),
		roundToInt(desktopScale * getHeight())));
	Graphics g(*glRenderer);

	g.addTransform(AffineTransform::scale((float)desktopScale));

	// Pure black background
	g.fillAll(Colours::black);

	//side bar if needed
	switch (currentSideBarType)
	{
	case GrayScale:
		g.setGradientFill(ColourGradient::vertical(Colours::lightgrey, 0, Colours::darkgrey, getHeight()));
		g.fillRect(getWidth() - 100, 4, 100 - 4, getHeight() - 8);
		break;
	case TextScale:
		break;
	case ColourButtons:
		break;
	}

	// White interior contour 2px line to show when the canvas is active
	if (selectedForEditing)
	{
		g.setColour(Colours::white);
		g.drawRect(1, 1, getWidth() - 2, getHeight() - 2, 2);
	}


	// - - - - - THIRD Duplication of the drawable objects for thread-safe rendering, - - - - -
	// Lorsque nécessaire seulement !
	manager->LockAsyncDrawableObjects();


	bool areasCountChanged = (manager->GetAsyncDrawableObjects().size() != duplicatedAreas.size());
	// POUR L'INSTANT ALGO BÊTE
	// on resize le vecteur : la construction des shared n'est pas grave, leur bloc de contrôle reste
	// en mémoire finalement (on utilisera reset() )
	if (areasCountChanged)
	{
		canvasAreasPointersCopies.resize(manager->GetAsyncDrawableObjects().size());
		duplicatedAreas.resize(manager->GetAsyncDrawableObjects().size());
	}
	// Vérification simple de chaque aire 1 par 1
	size_t itIndex = 0;
	for (auto it = manager->GetAsyncDrawableObjects().begin();
		it != manager->GetAsyncDrawableObjects().end();
		++it)
	{
		// S'il y a eu un changement : on re-crée un pointeur déjà, puis
		// on fait effectivement la copie d'un nouvel objet
		if (canvasAreasPointersCopies[itIndex] != (*it))
		{
			canvasAreasPointersCopies[itIndex] = (*it);
			duplicatedAreas[itIndex] = (*it)->Clone();
		}
		// Double compteur
		itIndex++;
	}

	manager->UnlockAsyncDrawableObjects();


	// - - - - - Areas painting (including exciters if existing) - - - - -
	// Sans bloquer, du coup, les autres threads (pour réactivité max...)
	for (size_t i = 0; i<duplicatedAreas.size(); i++)
	{
		// Peut mettre à jour des images et autres (si l'échelle a changé)
		duplicatedAreas[i]->SetRenderingScale(desktopScale);
		// Dessin effectif
		duplicatedAreas[i]->Paint(g);
	}


	// Call to a general Graphic update on the whole Presenter module
	if (!manager->isUpdatePending())
		manager->triggerAsyncUpdate();

	// Time measures just before swap (or the closer that we can get to the swaps)
	displayFrequencyMeasurer.OnNewFrame();
	/*
	if (selectedForEditing)
	{
	if (displayFrequencyMeasurer.IsFreshAverageAvailable())
	DBG(displayFrequencyMeasurer.GetInfo());
	}
	*/
	// Forced sleep if drawing is too fast
	double underTime = desiredPeriod_ms - displayFrequencyMeasurer.GetLastDuration_ms();
	if (underTime > 0.0)
	{
		Thread::sleep((int)std::floor(underTime));
	}
	
}

//void AmusingSceneComponent::mouseDown(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseDown");
//}
//
//void AmusingSceneComponent::mouseDrag(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseDrag");
//}
//
//void AmusingSceneComponent::mouseUp(const juce::MouseEvent &event)
//{
//	DBG("AmusingSceneComponent::mouseUp");
//}

void AmusingSceneComponent::OnSpeedChanged(double newSpeed)
{
	//DBG("newSpeed = " + (String)newSpeed);
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeSpeed(newSpeed);
		}
	}
}

void AmusingSceneComponent::OnVelocityChanged(double newVelocity)
{
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeVelocity(newVelocity);
		}
	}
}

void AmusingSceneComponent::OnColourChanged(Colour newColour, int colourIdx)
{
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeColour(newColour,colourIdx);
		}
	}
}

void AmusingSceneComponent::OnBaseNoteChanged(int newBaseNote)
{
	DBG("newBaseNote = " + (String)newBaseNote);
	//canvasManager.lock()->
	//if(auto canvasManagerAsManager = std::dynamic_pointer_cast)
	if (auto manager = canvasManager.lock())
	{
		if (auto managerAsManager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(manager))
		{
			managerAsManager->ChangeBaseNote(newBaseNote);
		}
	}
}

void AmusingSceneComponent::SetAreaOptionsCenter(bpt center)
{
	showOptionsButton->setTopLeftPosition((int)center.get<0>()+100,(int) center.get<1>() - 150);
	areaOptions.setCentrePosition((int)center.get<0>(), (int)center.get<1>());
	
}

void AmusingSceneComponent::SetAreaOptionsVisible(bool show)
{
	if (show)
	{
		isOptionShowed = true;
		areaOptions.setVisible(true);
		showOptionsButton->setVisible(true);
	}
	else
	{
		isOptionShowed = false;
		areaOptions.setVisible(false);
		showOptionsButton->setVisible(false);
	}
}

void AmusingSceneComponent::SetAreaOptionsVisible(bool show,double speed, double velocity, int currentOctave, int colorIdx)
{
	if (show)
	{
		areaOptions.setSpeedSliderValue(speed);
		areaOptions.setVelocitySliderValue(velocity);
		areaOptions.setOctaveSlider(currentOctave);
		areaOptions.setCurrentColorSelected(colorIdx);
		showOptionsButton->setVisible(true);
	}
	//else
	//	areaOptions.setVisible(false);
}

void AmusingSceneComponent::buttonClicked(Button * buttonClicked)
{
	if (buttonClicked == showOptionsButton)
	{
		if (isOptionShowed)
		{
			areaOptions.setVisible(false);
			isOptionShowed = false;
		}
		else
		{
			areaOptions.setVisible(true);
			isOptionShowed = true;
		}
	}
}

void AmusingSceneComponent::mouseDown(const juce::MouseEvent & event)
{
	SceneCanvasComponent::mouseDown(event);
}

void AmusingSceneComponent::mouseDoubleClick(const juce::MouseEvent & event)
{
	DBG("doubleClick");
	if (auto manager = std::dynamic_pointer_cast<Amusing::MultiSceneCanvasManager>(canvasManager.lock()))
		manager->OnCanvasMouseDoubleClick(event);
}

void AmusingSceneComponent::ShowSideBar(SideBarType sideBarType)
{
	currentSideBarType = sideBarType;
}
