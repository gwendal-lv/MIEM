/*
  ==============================================================================

    MusicalScene.cpp
    Created: 19 Dec 2016 8:22:55am
    Author:  ayup1

  ==============================================================================
*/

#include "MusicalScene.h"
#include "SquareSignal.h"
#include "TriangleSignal.h"

MusicalScene::MusicalScene(String m_name) : name(m_name)
{
}

MusicalScene::~MusicalScene()
{
}

void MusicalScene::addSource(int ID, int sourceType)
{
	switch (sourceType)
	{
	case 3 :
		trackArray.add(new TriangleSignal(1.25, 100, 8));
		// voir pour l'ID, stocker dans vecteur à part ou ajouter l'ID dans la classe Signal
		break;
	case 4 :
		trackArray.add(new SquareSignal(1.25,100,8));
		break;
	default:
		DBG("Unknow sourceType");
		break;
	}
}

void MusicalScene::setFrequency(int ID, double frequency)
{
	for (int i = 0; i < trackArray.size(); ++i)
	{
		if (trackArray[i]->getID() == ID)
			trackArray->setFrequency(frequency);
	}
}
void MusicalScene::setAmplitude(int ID, double amplitude)
{}
void MusicalScene::setPath(int ID, String path)
{}
void MusicalScene::setState(int ID, TransportState transportState) // gere l'etat de la source : play, start, stop
{}
void MusicalScene::setVolume(double amplitude)
{}
void MusicalScene::setReverse(bool enable)
{}