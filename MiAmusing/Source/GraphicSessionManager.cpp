/*
==============================================================================

GraphicSessionManager.cpp
Created: 28 Mar 2016 5:27:18pm
Author:  Gwendal Le Vaillant

==============================================================================
*/

#include "GraphicSessionManager.h"


// Other includes

#include "IPresenter.h"
#include "Presenter.h"
#include "View.h"

#include "AreaEvent.h"
#include "Exciter.h"

#include "SceneEvent.h"

#include "ControlEvent.h"

#include "MultiAreaEvent.h"

#include "AmusingScene.h"
#include "AnimatedPolygon.h"
#include "EditableEllipse.h"
#include "CompletePolygon.h"
#include <cmath>

#include "MultiCanvasComponentAmusing.h"

using namespace Amusing;
using namespace Miam;





// ========== CONSTRUCTION and DESTRUCTION ==========

GraphicSessionManager::GraphicSessionManager(Presenter* presenter_, View* view_) ://IPresenter* presenter_, View* view_) :
	IGraphicSessionManager(presenter_),
	view(view_),
	myPresenter(presenter_),
	myMultiCanvasComponent(new MultiCanvasComponentAmusing(this)),
	deleting(false)
{

	editScene = view->GetMainContentComponent()->GetSceneEditionComponent();
	// ICI ON CHARGE DES TRUCS
	// Canvases const count defined here
	// 1 SEUL CANVAS PAR EXEMPLE
	// On doit créer les sous-objets graphiques de canevas (View) avant de
	// les transmettre au sous-module de gestion de canevas (Presenter) que l'on crée
	// d'ailleurs ici aussi.
	canvasManagers.push_back(std::make_shared<MultiSceneCanvasManager>(this, myMultiCanvasComponent->AddCanvas(), SceneCanvasComponent::Id::Canvas1));
	myMultiCanvasComponent->CompleteInitialization();
	DBG("pushed");
//	canvasManagers.back()->CompleteInitialization(canvasManagers.back());
	DBG("initialized");
	
	/*
	canvasManagers.push_back(new MultiSceneCanvasEditor(this, multiCanvasComponent->AddCanvas(),  SceneCanvasComponent::Id::Canvas2));
	*/
	//canvasManagers.back()
	for (size_t i = 0; i<canvasManagers.size(); i++)
	{
		// After canvases are created : scenes creation
		// DEFAULT SCENES, TO BE CHANGED
		canvasManagers[i]->AddScene("Scene 1");
		canvasManagers[i]->AddScene("Scene 2");
		canvasManagers[i]->AddScene("Scene 3",true);
	}
	
	// Links to the view module
	view->CompleteInitialization(this, myMultiCanvasComponent);
	editScene->CompleteInitialization(this);// , multiCanvasComponent);
	DBG("view + edit scene initialized");
	// And states of the canvases are forced
	for (size_t i = 0; i<canvasManagers.size(); i++)
		canvasManagers[i]->SetMode(CanvasManagerMode::Unselected);
	DBG("mode set");

	// SÉLECTION/CHARGEMENT D'UN TRUC PAR DÉFAUT
	nextAreaId = 0; // plus tard : valeur contenue dans le fichier de sauvegarde
	canvasManagers.front()->SetMode(CanvasManagerMode::SceneOnlySelected);
	DBG("front mode selected");


	//OnLoad("test.xml");
}

GraphicSessionManager::~GraphicSessionManager()
{
	DBG("GraphicSessionManager destructor");
	delete myMultiCanvasComponent;
}


// Testing purposes only
void GraphicSessionManager::__LoadDefaultTest()
{
	//srand(2016); // GRAINE fixée ici


	//for (size_t i = 0; i<canvasManagers.size(); i++)
	//	canvasManagers[i]->__AddTestAreas();
}

void GraphicSessionManager::SetFromTree(bptree::ptree& /*graphicSessionTree*/)
{

}


// ===== SETTERS AND GETTERS =====

std::shared_ptr<IEditableArea> GraphicSessionManager::GetSelectedArea()
{
	if (selectedCanvas)
		return getSelectedCanvasAsManager()->GetSelectedArea();
	else
		return nullptr;
}


void GraphicSessionManager::SetAllChannels()
{
	for (int i = 0; i < (int)canvasManagers.size(); i++)
	{
		std::shared_ptr<MultiSceneCanvasManager> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[i]);
		canvasPtr->SetAllChannels();
	}
}

void GraphicSessionManager::setSamplesColor(int Nsamples, Colour colorCode[])
{
	myMultiCanvasComponent->setSamplesColor(Nsamples, colorCode);
}



// ===== UTILITIES =====




std::shared_ptr<MultiSceneCanvasManager> GraphicSessionManager::getSelectedCanvasAsManager()
{
	std::shared_ptr<MultiSceneCanvasManager> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasManager>(selectedCanvas);
	if (canvasPtr)
		return canvasPtr;
	else throw std::runtime_error("Canvas cannot be casted as a Miam::MultiSceneCanvasManager");
}


std::shared_ptr<MultiSceneCanvasEditor> GraphicSessionManager::getSelectedCanvasAsEditable()
{
	std::shared_ptr<MultiSceneCanvasEditor> canvasPtr = std::dynamic_pointer_cast<MultiSceneCanvasEditor>(selectedCanvas);
	if (canvasPtr)
		return canvasPtr;
	else throw std::runtime_error("Canvas not selected, or canvas cannot be casted a Miam::MultiSceneCanvasEditor");
}

int GraphicSessionManager::circleToNote(int numCirc)
{
	//int gamme[7] = { 60, 62, 64, 65, 67, 69, 71 }; // juste
	//int gamme[7] = { 36, 38, 39, 41, 43, 45, 46 }; // 2 bemols
	//int gamme[7] = { 48, 50, 52, 53, 55, 57, 59 };
	int gamme[7] = { 72, 74, 76, 77, 79, 81, 83 };
	//int gamme[7] = { 100, 100, 100, 100, 100, 100, 100 };
	return gamme[numCirc];
}

// ===== EVENTS FROM THE PRESENTER ITSELF =====
void GraphicSessionManager::HandleEventSync(std::shared_ptr<GraphicEvent> event_)
{
	
	
	////////////////////
	int ADSR = 1; //////
	///////////////////

	Miam::AsyncParamChange param;
	
	if (auto myTestEvent = std::dynamic_pointer_cast<AreaEvent>(event_))
	{
		if (myTestEvent->GetType() == AreaEventType::AnotherMonoTouchPointDragAlreadyBegun)
		{
			if (myTestEvent->GetConcernedArea() == nullptr)
				if (myTestEvent->GetConcernedScene() == nullptr)
				{
					param.Id1 = 0;
					param.Type = Miam::AsyncParamChange::InputsCount;
					myPresenter->SendParamChange(param);
					return;
				}
		}
	}
	
	// Event about an Area
	
	if (auto areaE = std::dynamic_pointer_cast<AreaEvent>(event_))
	{
		if (auto multiE = std::dynamic_pointer_cast<MultiAreaEvent>(event_))
		{
			//DBG("multiareaevent");
			//getSelectedCanvasAsEditable()->CallRepaint
			//getSelectedCanvasAsManager()->handleUpdateNowIfNeeded
			if (multiE->GetType() == AreaEventType::Selected)
				DBG("deselection + selection");
		}

		// Event about an Exciter in particular : we'll have to update the spat mix !
		//if (auto exciter = std::dynamic_pointer_cast<Exciter>(areaE->GetConcernedArea()))
		//{
		//	//std::cout << "mix à mettre à jour" << std::endl;
		//}
		else if (auto area = std::dynamic_pointer_cast<EditableArea>(areaE->GetConcernedArea()))
		{
			int i;
			switch (areaE->GetType())
			{
			case AreaEventType::NothingHappened:
				//DBG("NothingHappened");
				break;
			case AreaEventType::Added:
				DBG("Area Added");
				//param.Type = Miam::AsyncParamChange::ParamType::Activate;
				
				
				if (auto circle = std::dynamic_pointer_cast<EditableEllipse>(area))
				{
					param.Id2 = 20;
					if (ADSR == 1)
						param.DoubleValue = 20;
				}
				
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					DBG("Complete Area");
					if (areaE->GetMessage() != "intersection")
					{
						param.Id1 = myPresenter->getTimeLineID(area);
						//if (param.Id1 > 10)
						//	DBG("trop grand pour mon nombre d'aire");
						myPresenter->setInitSize(complete,complete->GetSurface());
						myPresenter->addOctave(complete);
						param.Id2 = 1024;
						param.Type = Miam::AsyncParamChange::ParamType::Activate;
						param.IntegerValue = 1; // 1 pour activer la source, 0 pour la supprimer
						/*if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
						{
							param.IntegerValue = myPresenter->getChannel(amusingScene);
						}
						else*/
							//param.IntegerValue = 0;
						param.FloatValue = 1; // set initial speed to 1;
						param.DoubleValue = myPresenter->getColorIdx(area);
						myPresenter->SendParamChange(param); // envoie l'ordre de creer/ detruire une source audio
						//DBG("GSM : construct a new audio polygon, please. Id : " + (String)param.Id1);
						param.Type = Miam::AsyncParamChange::ParamType::Source;
						i = 0;
						while (complete->getAllPercentages(i, param.DoubleValue) && complete->getAllDistanceFromCenter(i, param.IntegerValue))
						{
							//param.IntegerValue = 60 + 2* (param.IntegerValue);
							param.IntegerValue = myPresenter->getNote(complete, param.IntegerValue); //circleToNote(param.IntegerValue);
							//DBG("noteToSend = " + (String)param.IntegerValue);
							param.Id2 = i;
							param.FloatValue = (float)myPresenter->getVelocityArea(complete);
							myPresenter->SendParamChange(param);
							++i;
						}
						i = 0;
					}
				
				}
				
				if (auto cursorArea = std::dynamic_pointer_cast<Cursor>(area))
				{
					DBG("Cursor");
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					//param.Id1 = 1024;
					if (auto amusingS = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					{
						if (auto associateArea = cursorArea->getAssociateArea())//amusingS->getAssociateArea(cursorArea))
						{
							if (auto associatePolygon = std::dynamic_pointer_cast<CompletePolygon>(associateArea))
								param.Id1 = myPresenter->getTimeLineID(associatePolygon);
						}
					}
					param.Id2 = myPresenter->getReadingHeadID(cursorArea);
					param.IntegerValue = 1;
					param.DoubleValue = cursorArea->getSpeed();
					myPresenter->SendParamChange(param);
				}
				//myPresenter->SendParamChange(param);
				DBG("Send");
				break;
			case AreaEventType::Deleted:
				if (auto cursorArea = std::dynamic_pointer_cast<Cursor>(area))
				{
					param.Type = Miam::AsyncParamChange::ParamType::Activate;
					param.Id1 = 1024;
					param.Id2 = myPresenter->getReadingHeadID(cursorArea);

					param.IntegerValue = 0;
					myPresenter->deleteReadingHeadRef(cursorArea);
					myPresenter->SendParamChange(param);
				}
				else if(auto completeArea = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					if (areaE->GetMessage() != "intersection")
					{
						param.Type = Miam::AsyncParamChange::ParamType::Activate;
						param.Id1 = myPresenter->getTimeLineID(area);
						//if (param.Id1 > 10)
						//	DBG("trop grand pour mon nombre d'aire");
						param.Id2 = 1024;
						param.IntegerValue = 0;
						myPresenter->SendParamChange(param);
					}
				}
				
				
				break;
			case AreaEventType::PointDragBegins :
				DBG("PointDragBegins");
				break;
			case AreaEventType::PointDragStops :
				DBG("PointDragStops");
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					param.Type = AsyncParamChange::ParamType::None;
					//param.Id1 = myPresenter->getSourceID(area);
					//param.Type = Miam::AsyncParamChange::ParamType::Activate;
					//param.Id2 = 1; // 1 pour activer la source, 0 pour la supprimer
					//if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					//{
					//	DBG("channel I send : " + (String)myPresenter->getChannel(amusingScene));
					//	param.IntegerValue = myPresenter->getChannel(amusingScene);
					//}
					//else
					//	param.IntegerValue = 1;
					//myPresenter->SendParamChange(param); // envoie l'ordre de creer/ detruire une source audio

					param.Id1 = myPresenter->getTimeLineID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					////DBG("before while");
					while (complete->getAllPercentages(i, param.DoubleValue) && complete->getAllDistanceFromCenter(i, param.IntegerValue))
					{
						//param.IntegerValue = 60 + (2*param.IntegerValue);
						param.IntegerValue = myPresenter->getNote(complete, param.IntegerValue);
						//DBG("noteToSend = " + (String)param.IntegerValue);
						param.Id2 = i;
						param.FloatValue = (float)myPresenter->getVelocityArea(complete);
						myPresenter->SendParamChange(param);
						++i;
					}
					//i = 0;


					//param.Id2 = 1000; // indiquera que l'on a envoye la position de tous les points
					//myPresenter->SendParamChange(param);
					//}
					//DBG("finish");
				}
				//myPresenter->SendParamChange(param);
				break;
			case AreaEventType::ShapeChanged :
				DBG("shapeChanged");
				if (auto cursor = std::dynamic_pointer_cast<Cursor>(area))
				{
					DBG("Cursor's shape changed");
					param.Type = AsyncParamChange::ParamType::Play;
					param.IntegerValue = 0;
					param.Id1 = myPresenter->getReadingHeadID(cursor);
					param.DoubleValue = cursor->getSpeed();
					myPresenter->SendParamChange(param);
				}
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					
					/// envoie des infos sur les notes à jouer

					param.Id1 = myPresenter->getTimeLineID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					while (complete->getAllPercentages(i, param.DoubleValue) && complete->getAllDistanceFromCenter(i, param.IntegerValue))
					{
						if (param.DoubleValue == 0 && i != 0)
							DBG("aaaaaaarg");
						
						param.IntegerValue = myPresenter->getNote(complete, param.IntegerValue);
						
						param.Id2 = i;
						param.FloatValue = (float)myPresenter->getVelocityArea(complete);
						myPresenter->SendParamChange(param);
						++i;
					}
					i = 0;

					/// envoie des infos sur les accords
					param.Type = Miam::AsyncParamChange::ParamType::Position;
					std::shared_ptr<CompletePolygon> chordArea = nullptr;
					param.Id2 = param.Id1;
					myPresenter->SendParamChange(param); // to reset chords

					

					i = 0;
					if (auto amusingS = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					{
						while (auto intersection = amusingS->getNextChildOf(complete, i))
						{
							auto otherParent = intersection->getOtherParent(complete);

							// send new chords belonging to polygon "complete" already reset before the loop
							param.Id1 = myPresenter->getTimeLineID(complete);
							param.Id2 = myPresenter->getTimeLineID(otherParent);
							for (int k = 0; k < intersection->getApexesCount(complete); ++k)
							{
								param.DoubleValue = intersection->getApexesAngle(complete,k);
								myPresenter->SendParamChange(param);
							}

							// first reset chords belonging to the other parent
							int tmp = param.Id1;
							param.Id1 = param.Id2;
							myPresenter->SendParamChange(param);

							// send new chords belonging to the other parent
							param.Id2 = tmp;
							for (int k = 0; k < intersection->getApexesCount(otherParent); ++k)
							{
								param.DoubleValue = intersection->getApexesAngle(otherParent, k);
								myPresenter->SendParamChange(param);
							}


							++i;
						}
					}
				}				
				break;
			case AreaEventType::Translation :
				//DBG("Translation");
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					/// envoie des infos sur les accords
					param.Type = Miam::AsyncParamChange::ParamType::Position;
					param.Id1 = myPresenter->getTimeLineID(area);
					std::shared_ptr<CompletePolygon> chordArea;
					param.Id2 = param.Id1;
					myPresenter->SendParamChange(param); // to reset chords

					// send new chords
					i = 0;
					if (auto amusingS = std::dynamic_pointer_cast<AmusingScene>(areaE->GetConcernedScene()))
					{
						while (auto intersection = amusingS->getNextChildOf(complete, i))
						{
							auto otherParent = intersection->getOtherParent(complete);

							// send new chords belonging to polygon "complete"
							param.Id1 = myPresenter->getTimeLineID(complete);
							param.Id2 = myPresenter->getTimeLineID(otherParent);
							for (int k = 0; k < intersection->getApexesCount(complete); ++k)
							{
								param.DoubleValue = intersection->getApexesAngle(complete, k);
								myPresenter->SendParamChange(param);
							}

							// first reset chords belonging to the other parent
							int tmp = param.Id1;
							param.Id1 = param.Id2;
							myPresenter->SendParamChange(param);

							// send new chords belonging to the other parent
							param.Id2 = tmp;
							for (int k = 0; k < intersection->getApexesCount(otherParent); ++k)
							{
								param.DoubleValue = intersection->getApexesAngle(otherParent, k);
								myPresenter->SendParamChange(param);
							}


							++i;
						}
					}
				}
				//area-> get center height --> volume
				break;
			case AreaEventType::RotScale :
				DBG("RotScale");
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					param.Type = AsyncParamChange::Frequency;
					param.Id1 = myPresenter->getTimeLineID(complete);
					param.DoubleValue = myPresenter->computeFrequency(complete,complete->GetSurface());
					DBG((String)param.DoubleValue);
					myPresenter->SendParamChange(param);



					param.Type = Miam::AsyncParamChange::ParamType::Source;
					i = 0;
					////DBG("before while");
					while (complete->getAllPercentages(i, param.DoubleValue) && complete->getAllDistanceFromCenter(i, param.IntegerValue))
					{
						//param.IntegerValue = 60 + (2*param.IntegerValue);
						param.IntegerValue = myPresenter->getNote(complete, param.IntegerValue);
						//DBG("noteToSend = " + (String)param.IntegerValue);
						param.Id2 = i;
						param.FloatValue = (float)myPresenter->getVelocityArea(complete);
						myPresenter->SendParamChange(param);
						++i;
					}
				}
				break;
			case AreaEventType::Selected :
				DBG("selection");
				if (auto cursor = std::dynamic_pointer_cast<Cursor>(area))
				{
					param.Type = AsyncParamChange::Play;
					param.Id1 = myPresenter->getReadingHeadID(cursor);
					param.IntegerValue = 1;
					param.DoubleValue = cursor->getSpeed();
					myPresenter->SendParamChange(param);
				}
				break;
				//case AreaEventType::
			case AreaEventType::ParentChanged:
				if (auto cursor = std::dynamic_pointer_cast<Cursor>(area))
				{
					param.Type = AsyncParamChange::Update;
					param.Id1 = myPresenter->getTimeLineID(cursor->getAssociateArea());
					param.Id2 = myPresenter->getReadingHeadID(cursor);
					param.DoubleValue = cursor->getPositionInAssociateArea(); //cursor->getSpeed();
					param.FloatValue = (float)cursor->getSpeed();//cursor->getPositionInAssociateArea();
					DBG("phi : " + (String)param.FloatValue);
					myPresenter->SendParamChange(param);
				}
				break;
			case AreaEventType::ColorChanged :
				if (auto complete = std::dynamic_pointer_cast<CompletePolygon>(area))
				{
					param.Id1 = myPresenter->getTimeLineID(area);
					param.Type = Miam::AsyncParamChange::ParamType::Volume;
					param.Id2 = 0;
					param.FloatValue = (float)myPresenter->getVelocityArea(complete);
					myPresenter->SendParamChange(param);

					param.Type = Miam::AsyncParamChange::ParamType::InputsCount;
					param.Id2 = myPresenter->getPathIdx(complete->GetFillColour());
					DBG("set path : " + (String)param.Id2);
					myPresenter->SendParamChange(param);
					
				}
				break;
			default:
				break;
			}
		}
	}
	else if (auto sceneE = std::dynamic_pointer_cast<SceneEvent>(event_))
	{
		DBG("scene event type = " + (String)(int)sceneE->GetType());
		switch (sceneE->GetType())
		{
		case SceneEventType::Added:
			DBG("Scene added = " + sceneE->GetNewScene()->GetName());
			//DBG((String)param.Type)
			
			
			
			break;
		case SceneEventType::Deleted:
			DBG("Scene deleted");
			break;
		case SceneEventType::NothingHappened:
			DBG("Nothing happened to scene");
			if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(sceneE->GetNewScene()))
			{
				//myPresenter->setChannel(amusingScene, 1);
			}
			break;
		case SceneEventType::SceneChanged:
			DBG("Scene Changed");
			if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(sceneE->GetNewScene()))
			{
				
				/*if(amusingScene->GetDrawableObjectsCount() > 0)
					editScene->setMidiChannel(myPresenter->getChannel(sceneE->GetNewScene()));*/
				if (amusingScene->getNumberArea() < 10)
					editScene->showAddPolygon();
				else
					editScene->hideAddPolygon();
					
				//for (int i = 0; i < (int)amusingScene->GetDrawableObjectsCount(); i++)
				//{
				//	if (auto area = amusingScene->GetDrawableObject(i))//myPresenter->getSceneArea(amusingScene, i))
				//	{
				//		if (auto areaC = std::dynamic_pointer_cast<CompletePolygon>(area))
				//		{
				//			param.Type = AsyncParamChange::ParamType::UdpPort;
				//			param.Id1 = myPresenter->getTimeLineID(areaC);
				//			param.IntegerValue = 0;//myPresenter->getChannel(sceneE->GetNewScene());
				//			myPresenter->SendParamChange(param);
				//		}
				//		
				//	}
				//}

			}
			break;
		default:
			break;
		}
	}
	else if(auto controlE = std::dynamic_pointer_cast<ControlEvent>(event_ ))
	{
		switch (controlE->GetType())
		{
		case ControlEventType::Play:
			DBG("play clicked");
			param.Type = Miam::AsyncParamChange::ParamType::Duration;
			param.IntegerValue = myPresenter->getTempo();
			param.FloatValue = myPresenter->getMasterVolume();
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Pause:
			DBG("pause clicked");
			param.Type = Miam::AsyncParamChange::ParamType::Pause;
			myPresenter->SendParamChange(param);
			break;
		case ControlEventType::Stop:
			DBG("stop clicked");
			param.Type = Miam::AsyncParamChange::ParamType::Stop;
			param.Id1 = 255;
			myPresenter->SendParamChange(param);
			break;
		default:
			break;
		}
	}
}


void GraphicSessionManager::CanvasModeChanged(CanvasManagerMode canvasMode)
{
	switch (canvasMode)
	{
	case Miam::CanvasManagerMode::Null:
		break;
	case Miam::CanvasManagerMode::Loading:
		break;
	case Miam::CanvasManagerMode::Loaded:
		break;
	case Miam::CanvasManagerMode::Unselected:
		DBG("area unselected");
		break;
	case Miam::CanvasManagerMode::SceneOnlySelected:
		DBG("scene selected");
		break;
	case Miam::CanvasManagerMode::CanvasSelected:
		break;
	case Miam::CanvasManagerMode::AreaSelected:
		DBG("area selected");
		break;
	case Miam::CanvasManagerMode::EditingArea:
		break;
	case Miam::CanvasManagerMode::WaitingForPointCreation:
		break;
	case Miam::CanvasManagerMode::WaitingForPointDeletion:
		break;
	default:
		break;
	}
}








// ===== EVENTS TO VIEW =====

void GraphicSessionManager::DisplayInfo(String info, int /*priority*/)
{
	view->DisplayInfo(info);
}

void GraphicSessionManager::OnAddArea()
{
	if (selectedCanvas)
	{
		if (auto canvas = std::dynamic_pointer_cast<AmusingScene>(getSelectedCanvasAsEditable()))
			canvas->AddAnimatedArea(GetNextAreaId());
		else
			getSelectedCanvasAsEditable()->AddDefaultArea(GetNextAreaId());
	}
}

void GraphicSessionManager::OnFollowerTranslation(std::shared_ptr<GraphicEvent> graphicE)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->OnFollowerTranslation(graphicE);
}

void GraphicSessionManager::OnAudioPosition(double position)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->OnAudioPosition(position);
}

void GraphicSessionManager::SetAllAudioPositions(double position)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->SetAllAudioPositions(position);
}

void GraphicSessionManager::SetAudioPositions(std::shared_ptr<Cursor> area, double position)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->SetAudioPositions(area, position);
}

void GraphicSessionManager::lookForAreasConcerned(Colour colourConcerned)
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->lookForAreasConcerned(colourConcerned);
}

void GraphicSessionManager::OnAddSquare()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddNedgeArea(GetNextAreaId(), 4);
	}
}

void GraphicSessionManager::OnAddTriangle()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddNedgeArea(GetNextAreaId(), 3);
	}
}

void GraphicSessionManager::OnAddHexa()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddNedgeArea(GetNextAreaId(), 6);
	}
}

void GraphicSessionManager::OnAddCircle()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddNedgeArea(GetNextAreaId(), 20);
	}
}

void GraphicSessionManager::OnAddTrueCircle()
{
	if (selectedCanvas)
		getSelectedCanvasAsManager()->AddTrueCircle(GetNextAreaId());
}


void GraphicSessionManager::hideAddPolygon()
{
	editScene->hideAddPolygon();
}

void GraphicSessionManager::showAddPolygon()
{
	editScene->showAddPolygon();
}

void GraphicSessionManager::OnDelete()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->OnDelete();
		deleting = true;
	}
}

void Amusing::GraphicSessionManager::OnTestChangeSound()
{
	HandleEventSync(std::shared_ptr<AreaEvent>(new AreaEvent(nullptr, AreaEventType::AnotherMonoTouchPointDragAlreadyBegun, nullptr)));
}

void GraphicSessionManager::OnSave(std::string filename)
{
	auto wholeAmusingTree = std::make_shared<bptree::ptree>();
	bptree::xml_writer_settings<std::string> xmlSettings(' ', 4);
	auto canvasesTree = GetCanvasesTree(); // OK
	auto viewTree = view->GetAudioSettingsTree();
	
	wholeAmusingTree->add_child("canvas", *canvasesTree);
	wholeAmusingTree->add_child("view", *viewTree);
	
	
	boost::property_tree::write_xml(filename, *wholeAmusingTree,std::locale(),xmlSettings);
}

void GraphicSessionManager::OnLoad(std::string filename)
{
	bptree::ptree xmlTree, graphicSessionManagerTree, viewTree;
	try 
	{
		// Lecture
		bptree::read_xml(filename, xmlTree);
		// puis Séparation des grandes parties du fichier
		graphicSessionManagerTree = xmlTree.get_child("canvas");
		viewTree = xmlTree.get_child("view");
		
		std::vector< std::shared_ptr<bptree::ptree> > canvasTrees;
		if (!graphicSessionManagerTree.empty())
		{
			try {
				canvasTrees = IGraphicSessionManager::ExtractCanvasesSubTrees(graphicSessionManagerTree);
			}
			// S'il y a une erreur : on quitte la fonction directement
			catch (bptree::ptree_error& e) {
				throw XmlReadException("<graphicsession> : error extracting <canvas> nodes : ", e);
			}
		}
		int64_t biggestAreaUid = -1;
		if (!canvasTrees.empty())
		{
			for (size_t i = 0; i < canvasManagers.size(); i++)
			{
				if (auto currentCanvas = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[i]))
				{
					currentCanvas->UnselectScene();
					myPresenter->setTempo(canvasTrees[i]->get_child("canvas").get<int>("<xmlattr>.BPM"));
					editScene->setTempoSlider(canvasTrees[i]->get_child("canvas").get<int>("<xmlattr>.BPM"));
					HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
				}
				auto sceneTrees = canvasManagers[i]->SetScenesFromTree<AmusingScene>(*(canvasTrees[i]));
				{
					int W = canvasTrees[i]->get_child("canvas").get<int>("<xmlattr>.widthOnSaveTime");
					int H = canvasTrees[i]->get_child("canvas").get<int>("<xmlattr>.heightOnSaveTime");
					for (size_t j = 0; j < sceneTrees.size(); j++)
					{
						std::string canvasAndSceneString = "Canvas " + boost::lexical_cast<std::string>(i) + ", Scene " + boost::lexical_cast<std::string>(j) + ": ";

						// Première passe de lecture
						size_t areasCount;
						try {
							areasCount = XmlUtils::CheckIndexes(*(sceneTrees[j]), "scene.areas", "area");
						}
						catch (XmlReadException &e) {
							throw XmlReadException(canvasAndSceneString, e);
						}

						// Pré-chargement des aires
						std::vector<std::shared_ptr<CompletePolygon>> areas; // y compris les excitateurs
						areas.resize(areasCount);

						for (auto& area : sceneTrees[j]->get_child("scene.areas"))
						{

							try 
							{
								auto index = area.second.get<size_t>("<xmlattr>.index");
								auto type = area.second.get<std::string>("<xmlattr>.type");

								/*area.second.get_child("geometry").put("<xmlattr>.width", W);
								area.second.get_child("geometry").put("<xmlattr>.height", H);*/
								area.second.put("<xmlattr>.width", W);
								area.second.put("<xmlattr>.height", H);

								// Spat Polygones
								if (type == "DrawablePolygon")
								{
									try {
										areas[index] = std::make_shared<CompletePolygon>(area.second);
										bptree::ptree areaAudioParameterTree = area.second.get_child("optionsParameter");
										setSpeedArea(areas[index], areaAudioParameterTree.get<double>("<xmlattr>.speed"));
										setOctave(areas[index], areaAudioParameterTree.get<int>("<xmlattr>.octave"));
										setVelocityArea(areas[index], areaAudioParameterTree.get<double>("<xmlattr>.velocity"));
									}
									catch (XmlReadException &e) {
										throw XmlReadException(canvasAndSceneString + e.what());
									}
								}
								
								// Recherche de l'UID le plus grand utilisé jusqu'ici
								if (areas[index]->GetId() > biggestAreaUid)
									biggestAreaUid = areas[index]->GetId();
							}
							catch (bptree::ptree_error &e) 
							{
								throw XmlReadException(canvasAndSceneString, e);
							}

						}

						// Finalement, ajout effectif des aires dans le bon ordre
						for (size_t k = 0; k<areas.size(); k++)
						{
							// Fonction commune à toutes les aires
							canvasManagers[i]->AddAreaToScene(j, areas[k]);
						}

						if (auto currentCanvas = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[i]))
						{
							currentCanvas->AddAllIntersectionsToScene((int)j);
						}

					}
				}
			}
			nextAreaId = biggestAreaUid + 1;
			// Actualisations pour chaque canevas
			/*for (auto& canvas : canvasManagers)
				canvas->OnXmlLoadFinished();*/
			//SelectScene(0);
		}



		if (!viewTree.empty())
		{
			view->setSoundsSettings(viewTree);
			bptree::ptree subTree = viewTree.get_child("sound");
			//bptree::ptree soundTree = subTree.get_child("soundTree");
			for (auto& sound : subTree.get_child("soundTree"))
			{
				if (!sound.second.empty())
				{
					auto index = sound.second.get<int>("<xmlattr>.index");
					
					try
					{
						
						auto color = sound.second.get<std::string>("<xmlattr>.color");
						auto path = sound.second.get<std::string>("<xmlattr>.soundFilePath");
						myPresenter->setColorPath(index, Colour::fromString(StringRef(color)), path);

						for (int j = 0; j < (int)canvasManagers.size(); ++j)
						{
							if (auto currentManager = std::dynamic_pointer_cast<MultiSceneCanvasManager>(canvasManagers[j]))
								currentManager->lookForAreasConcerned(Colour::fromString(StringRef(color)));
						}

					}
					catch (bptree::ptree_error &e) {
						throw XmlReadException("Sound " + boost::lexical_cast<std::string>(index) + ": ", e);
					}
				}
			}
				
			
			
		}

	}
	catch (bptree::ptree_error &e) {
		throw XmlReadException("Cannot load session: ", e);
	}
}

void GraphicSessionManager::OnAddComplete()
{
	if (selectedCanvas)
	{
		getSelectedCanvasAsManager()->AddCompleteArea();
	}
}

void GraphicSessionManager::OnSoundClick()
{
	view->ShowSoundManager();
}

void GraphicSessionManager::OnDeviceOptionsClicked()
{
	view->ShowDeviceOptionsDialog();
}

void GraphicSessionManager::SetMidiChannel(int /*ch*/)
{
	if (selectedCanvas)
	{
		if (auto amusingScene = std::dynamic_pointer_cast<AmusingScene>(getSelectedCanvasAsManager()->GetSelectedScene()))
		{
			//DBG("channel to send : " + (String)ch);
			//myPresenter->setChannel(amusingScene, ch);
			//getSelectedCanvasAsManager()->resendToModel();
			HandleEventSync(std::shared_ptr<SceneEvent>(new SceneEvent(getSelectedCanvasAsManager(), amusingScene, SceneEventType::SceneChanged)));
		}
	}
}

void GraphicSessionManager::setSpeedArea(std::shared_ptr<IEditableArea> area, double speed)
{
	myPresenter->setSpeedArea(area,speed);
}

void GraphicSessionManager::setVelocityArea(std::shared_ptr<IEditableArea> area, double speed)
{
	myPresenter->setVelocityArea(area, speed);
}

void GraphicSessionManager::setOctave(std::shared_ptr<IEditableArea> area, int newOctave)
{
	myPresenter->setOctave(area, newOctave);
}

double GraphicSessionManager::getSpeed(std::shared_ptr<IEditableArea> area)
{
	return myPresenter->getSpeedArea(area);
}

double GraphicSessionManager::getVelocity(std::shared_ptr<IEditableArea> area)
{
	return myPresenter->getVelocityArea(area);
}

int GraphicSessionManager::getOctave(std::shared_ptr<IEditableArea> area)
{
	return myPresenter->getOctave(area);
}

int GraphicSessionManager::getColor(std::shared_ptr<IEditableArea> area)
{
	return myPresenter->getColorIdx(area);
}

void GraphicSessionManager::setColor(std::shared_ptr<IEditableArea> area, int colourIdx)
{
	myPresenter->setColorIdx(area, colourIdx);
}

int GraphicSessionManager::getTempo()
{
	return myPresenter->getTempo();
}

void GraphicSessionManager::OnTempoChanged(int newTempo) // voir si laisser comme ca, pcq pas d'interpretation necessaire pour le tempo
{
	myPresenter->setTempo(newTempo);
	HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
}

void GraphicSessionManager::OnMasterVolumeChanged(float newVolume)
{
	myPresenter->setMasterVolume(newVolume);
	HandleEventSync(std::shared_ptr<ControlEvent>(new ControlEvent(ControlEventType::Play)));
}