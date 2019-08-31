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

#include "ClearLabelTimer.h"


using namespace Miam;


void ClearLabelTimer::SetLabelToClearAfterTimeout(Label* label_)
{
    label = label_;
}

void ClearLabelTimer::StartTimer()
{
    juce::Timer::startTimer(IntervalInMilliseconds);
}

void ClearLabelTimer::timerCallback()
{
    stopTimer();
    
    if (label)
    {
        label->setText("...", NotificationType::sendNotificationAsync);
        currentDisplayedInfoPriority = -1; // on se met en attente d'une info
    }
    else
        throw std::runtime_error("Label to clear is not set within class ClearLabelTimer");
}
