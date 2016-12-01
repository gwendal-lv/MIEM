/*
  ==============================================================================

    SpatStateFadersDisplayComponent.cpp
    Created: 29 Nov 2016 8:15:45pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpatStateFadersDisplayComponent.h"

#include "SpatStatesEditionComponent.h"

//==============================================================================
SpatStateFadersDisplayComponent::SpatStateFadersDisplayComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}
void SpatStateFadersDisplayComponent::CompleteInitialization(SpatStatesEditionManager* _editionManager)
{
    editionManager = _editionManager;
}
SpatStateFadersDisplayComponent::~SpatStateFadersDisplayComponent()
{
}

void SpatStateFadersDisplayComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    
    
    // We add/remove faders if necessary...
    // Kind of disguting within the paint() function. Don't care for it now !...
    if (sliders.size() < editionManager->GetFadersCount())
    {
        while (sliders.size() != editionManager->GetFadersCount())
        {
            addSlider();
        }
        // update of all sliders values now
        // UPDATE ALL FADERS VALUES FROM MODEL DATA
        resized();
    }
    else if (sliders.size() > editionManager->GetFadersCount())
    {
        while (sliders.size() != editionManager->GetFadersCount())
        {
            removeSlider();
        }
        // graphical update only... or ?
        resized();
    }
             
    
    // Sliders drawing
    for (size_t i = 0 ; i<sliders.size() ; i++)
    {
        // ACTUALISATION DES VALEURS SI NÉCESSAIRE ????
    }
    
    // Text under faders
    AffineTransform transform = AffineTransform();
    transform = transform.rotated(M_PI_2);
    transform = transform.translated(0, sliderHeight);
    g.addTransform(transform);
    for (size_t i = 0 ; i<slidersLabels.size() ; i++)
    {
        AffineTransform translation;
        translation = translation.translated(0, -sliderWidth);
        g.addTransform(translation);
        
        slidersLabels[i]->paint(g);
    }
    
}

void SpatStateFadersDisplayComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    sliderWidth = roundToInt(floorf((float)(getWidth())/(float)(sliders.size())));
    sliderHeight = roundToInt((float)(getHeight())*0.6);
    textHeight = getHeight() - sliderHeight;
    for (size_t i = 0 ; i<sliders.size() ; i++)
    {
        sliders[i]->setBounds(i*sliderWidth, 0, sliderWidth, sliderHeight);
        slidersLabels[i]->setBoundingBox(RelativeParallelogram(Rectangle<float>(sliderHeight, i*sliderWidth, textHeight, sliderWidth)));
    }
}



void SpatStateFadersDisplayComponent::addSlider()
{
    sliders.push_back(new Slider());
    slidersLabels.push_back(new DrawableText());
    
    sliders.back()->setSliderStyle(Slider::SliderStyle::LinearVertical);
    slidersLabels.back()->setText(editionManager->GetFaderName(slidersLabels.size()-1));
    
    addAndMakeVisible(sliders.back());
    //addAndMakeVisible(slidersLabels.back());
}
void SpatStateFadersDisplayComponent::removeSlider()
{
    removeChildComponent(sliders.back());
    sliders.resize(sliders.size()-1);
    //removeChildComponent(slidersLabels.back());
    slidersLabels.resize(slidersLabels.size()-1);
}




