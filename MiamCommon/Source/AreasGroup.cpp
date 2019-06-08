/*
  ==============================================================================

    AreasGroup.cpp
    Created: 8 Jun 2019 5:26:57pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "AreasGroup.h"

#include "InteractiveArea.h"

using namespace Miam;

// static rand gen
std::mt19937 AreasGroup::randomGenerator = std::mt19937();

AreasGroup::AreasGroup()
{
    colour = juce::Colour::fromRGB((randomGenerator() % 230) + 26,
                                   (randomGenerator() % 230) + 26,
                                   (randomGenerator() % 230) + 26);
}


void AreasGroup::MarkAsDuplicateOf(std::shared_ptr<AreasGroup>& _originalGroup)
{
    originalGroup = _originalGroup;
}

