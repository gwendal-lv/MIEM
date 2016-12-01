/*
  ==============================================================================

    SpatStateFadersDisplayComponent.h
    Created: 29 Nov 2016 8:15:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef SPATSTATEFADERSDISPLAYCOMPONENT_H_INCLUDED
#define SPATSTATEFADERSDISPLAYCOMPONENT_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

#include "SpatStatesEditionManager.h"

using namespace Miam;

//==============================================================================
/*
*/
class SpatStateFadersDisplayComponent    : public Component
{
public:
    SpatStateFadersDisplayComponent();
    void CompleteInitialization(SpatStatesEditionManager* _editionManager);
        ~SpatStateFadersDisplayComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    
    
    protected :
    // Link to the associated manager
    SpatStatesEditionManager* editionManager = 0;
    
    std::vector<ScopedPointer<Slider>> sliders;
    std::vector<ScopedPointer<DrawableText>> slidersLabels;
    void addSlider();
    void removeSlider();
    
    
private:
    // internal display attributes
    int sliderWidth = 0, sliderHeight = 0, textHeight = 0;
    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpatStateFadersDisplayComponent)
};


#endif  // SPATSTATEFADERSDISPLAYCOMPONENT_H_INCLUDED
