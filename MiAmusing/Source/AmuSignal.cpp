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

#include "AmuSignal.h"

AmuSignal::AmuSignal(double m_amplitude, double m_frequency) : 
	AudioSource(), amplitude(m_amplitude), frequency(m_frequency), state(Stopped)
{
}

AmuSignal::~AmuSignal()
{
}

bool AmuSignal::isPlaying()
{
	if (state != Stopped && state != Paused)
	{
		return true;
	}
	return false;
}

bool AmuSignal::isStopped()
{
	if (state == Stopped)
		return true;
	else
		return false;
}

