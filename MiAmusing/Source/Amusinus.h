/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2017 Guillaume Villée.
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

#ifndef AMUSINUS_H_INCLUDED
#define AMUSINUS_H_INCLUDED

#define PI 3.14159265359

struct AmuSinusParameter 
{
	double m_amplitude;
	double m_frequency;
	double m_phase;
	double m_sampleRate;
	int m_bufferSize;
};

class AmuSinus
{
public:
	AmuSinus(double m_amplitude, double m_frequency, double m_phase, double m_sampleRate, int m_bufferSize);
	AmuSinus(AmuSinusParameter parameters);
	AmuSinus(AmuSinus *toCopy);
	~AmuSinus();

	// ----- setters and getters -------
	void setFrequency(double newFrequency);
	void setAmplitude(double newAmplitude);

	// to change the amplitude without gain ramp
	void resetAmplitude(double newAmplitude);

	//to change phase
	void setPhase(double newPhase);
	void resetPhase();
	void resetPhase(double newPhase);
	void shiftPhase(double shift);

	// get output
	double getSample();

	double getFrequency();
	double getAmplitude();
	double getPhase();
	double getSampleRate();
	int getBufferSize();

	double getCurrentSample();
	double getCurrentAngle();
	double getCurrentAmplitude();

private:
	// parameters
	double amplitude;
	double frequency;
	double phase;
	double sampleRate;
	double bufferSize;

	// intern variable
	double angle;

	// to update angle
	double angleDelta;
	void updateAngleDelta();
	
	// to change frequency
	double targetFrequency;
	void updateFrequency();

	// to change amplitude
	double targetAmplitude;
	void updateAmplitude();

};




#endif  // AMUSINUS_H_INCLUDED
