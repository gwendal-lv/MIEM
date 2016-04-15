/*
  ==============================================================================

    ClearLabelTimer.cpp
    Created: 13 Apr 2016 2:57:58pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ClearLabelTimer.h"


using namespace Miam;



void ClearLabelTimer::startTimer(Label* _label)
{
    label = _label;
    juce::Timer::startTimer(intervalInMilliseconds);
}

void ClearLabelTimer::timerCallback()
{
    stopTimer();
    label->setText("...", NotificationType::sendNotificationAsync);
}