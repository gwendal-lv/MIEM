/*
  ==============================================================================

    Rythme.cpp
    Created: 11 Jan 2017 5:07:47pm
    Author:  ayup1

  ==============================================================================
*/

#include "Rythme.h"

Rythme::Rythme(Component* m_mother, int m_bpm, int duration) : 
	mother(m_mother),
	record(false), t(0), buffer(nullptr), position(0)
{
	Ttimer = 30; // dependra de BPM 
	maxT =  (int)round(((double)duration)/((double)Ttimer));
	currentOn.on = false;
	currentOn.duration = 0;
	//buffer = new int[maxT * sampleRate];
}

void Rythme::setSampleRate(double m_sampleRate)
{
	if (!record)
	{
		sampleRate = m_sampleRate;
		if (buffer != nullptr)
			delete[] buffer;

		L = (int)round(maxT * ((double)Ttimer / 1000) * sampleRate);
		buffer = new int[L];
	}
}

Rythme::~Rythme()
{
	delete[] buffer;
}

void Rythme::mouseDown(const MouseEvent &event)
{
	if (record)
	{
		//DBG("clic en : (" + (String)event.getPosition().getX() + ")");
		if (currentOn.on == false) // si avant on ne cliquait pas -> envoyer le temps de silence, puis creer le son
		{
			//DBG("down");
			// verifier si on a depasser le temps maximum !
			rythme.push_back(currentOn);
			currentOn.on = true;
			currentOn.duration = 0;
		}
	}
}

void Rythme::mouseUp(const MouseEvent &event)
{
	if (record)
	{
		if (currentOn.on == true)
		{
			DBG("up");
			// verifier si on a depasser le temps maximum !
			rythme.push_back(currentOn);
			currentOn.on = false;
			currentOn.duration = 0;
		}
	}
}

void Rythme::timerCallback()
{
	if (record)
	{
		if (t > maxT)
		{
			record = false;
			rythme.push_back(currentOn);
			fillBuffer();
			stopTimer();
		}
		++currentOn.duration;
		++t;
	}
}

void Rythme::fillBuffer()
{
	int sum = 0;

	rythme[rythme.size() - 1].duration -= 1;

	DBG("BufferSize = " + (String)(L));
	String R;
	String S;
	int *bufTest;
	bufTest = new int[L];
	int k = 0;
	for (int i = 0; i < (int)rythme.size(); ++i)
	{
		R += (String)rythme[i].duration;
		R += " ";
		S += (String)rythme[i].on;
		S += " ";
		DBG((String)rythme[i].on);
		double tms = rythme[i].duration * Ttimer; // duree de la note en ms
		long int N = (long int)round( (tms /1000) * sampleRate); // nbr d'ech pour avoir la bonne duree

		DBG((String)Ttimer);
		DBG((String)rythme[i].duration);
		DBG((String)tms);
		DBG((sampleRate));
		DBG("N = " + (String)N);
		
		for (int j = 0; j < N; ++j)
		{
			if (rythme[i].on)
				buffer[k] = 1;
			else
				buffer[k] = 0;
			
			sum += buffer[k];

			if ((buffer[k] != 0) && (buffer[k] != 1))
				DBG("probleme");


			++k;

			if (k > L)
			{
				DBG("Depassement en : " + (String)k + " " + (String)(maxT * Ttimer * sampleRate));
				DBG(S);
				DBG(R);
				DBG("1er = " + (String)(buffer[0]));
				DBG("2eme = " + (String)buffer[1]);
				return;
			}
		}
		
	}
	DBG(S);
	DBG(R);
	DBG("1er = " + (String)(buffer[0]));
	DBG("2eme = " + (String)buffer[1]);
	DBG("sum = " + (String)sum);
	DBG("temps 1 = " + (String)(sum / sampleRate));
}


void Rythme::paint(Graphics& g)
{
	DBG("Rythme::paint");
	g.fillAll(Colours::darkblue);
	//g.drawRect(getLocalBounds(), 1);
	//g.setColour(Colours::lightblue);
	//g.setFont(14.0f);
	//g.drawText("Recorder", getLocalBounds(),
	//	Justification::centred, true);   // draw some placeholder text
}

void Rythme::resized()
{
	DBG("Rythme::resized");
	//setBounds(mother->getWidth(), mother->getHeight())
}

void Rythme::startRecord()
{
	if (rythme.size() != 0)
	{
		rythme.clear();
		delete[] buffer;

		currentOn.on = false;
		currentOn.duration = 0;
		buffer = new int[(int)(maxT * (Ttimer / 1000) * sampleRate)];
	}

	record = true;
	startTimer(Ttimer);
}

void Rythme::stopRecord()
{
	record = false;
	stopTimer();
}

int* Rythme::getBuffer()
{
	if (rythme.size() == 0)
		return nullptr;
	else
		return buffer;
}

int Rythme::getNextSample()
{
	if (record == false)
	{
		if (position >= L)
		{
			//position = 0;
			//DBG("end");
			return 0;
		}
		//DBG(position);
		int sample = buffer[position];
		//DBG((String)sample);

		if (sample == 1)
			DBG("MAINTENANT !!!");

		++position;
		return sample;
	}
	else
		return 0;
}

void Rythme::setBPM(int newBPM)
{
	if (!record)
	{
		bpm = newBPM;
		Ttimer = 300;
	}
}

void Rythme::setMaxT(int newDuration)
{
	if (!record)
	{
		maxT = newDuration;
	}
}

