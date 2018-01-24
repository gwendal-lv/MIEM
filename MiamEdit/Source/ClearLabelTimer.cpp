/*
  ==============================================================================

    ClearLabelTimer.cpp
    Created: 13 Apr 2016 2:57:58pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
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
