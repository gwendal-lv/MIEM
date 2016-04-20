/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_CE75C11E02E72D3E__
#define __JUCE_HEADER_CE75C11E02E72D3E__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include <vector>

#include "SceneCanvasComponent.h"
#include "DrawableArea.h"

namespace Miam
{
    class SceneEditionManager;
}
using namespace Miam;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

 // \brief An important component that corresponds to an Miam::AppMode of the
 // Miam::Presenter.
 //
 // An auto-generated component, created by the Projucer.
 //

                 //[/Comments]
*/
class SceneEditionComponent  : public Component,
                               public ButtonListener,
                               public SliderListener
{
public:
    //==============================================================================
    SceneEditionComponent ();
    ~SceneEditionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    // Construction helps
    void CompleteInitialization(SceneEditionManager* _sceneEditionManager);

    // General Setters and Getters
    SceneCanvasComponent* GetSceneCanvasComponent() {return sceneCanvasComponent;}


    // ----- Functions that obey orders sent from the Presenter -----
    public :
    // Buttons visibility & enablement
    void enablePolygonEditionControls();
    void disablePolygonEditionControls();
    void disableAllButtonsBut(juce::String onlyActiveButtonName);
    // Parameters setting
    void SetAreaColourValue(juce::Colour colour);

    // Helpers
    private :
    void setEnabledAllControls(bool areEnabled);
    void setVisibleSelectedAreaControls(bool areVisible);
    void setEnabledSelectedAreaControls(bool areEnabled);
    void colourSliderMoved();

    public :



    public :
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SceneCanvasComponent* sceneCanvasComponent;

    SceneEditionManager* sceneEditionManager;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> groupComponent;
    ScopedPointer<GroupComponent> groupComponent2;
    ScopedPointer<TextButton> addPointTextButton;
    ScopedPointer<TextButton> deletePointTextButton;
    ScopedPointer<TextButton> copyTextButton;
    ScopedPointer<TextButton> pasteTextButton;
    ScopedPointer<TextButton> addAreaTextButton;
    ScopedPointer<TextButton> deleteAreaTextButton;
    ScopedPointer<Label> labelR;
    ScopedPointer<Slider> sliderR;
    ScopedPointer<Label> labelG;
    ScopedPointer<Slider> sliderG;
    ScopedPointer<Label> labelB;
    ScopedPointer<Slider> sliderB;
    ScopedPointer<TextButton> sendBackwardTextButton;
    ScopedPointer<TextButton> bringForwardTextButton;
    ScopedPointer<TextButton> bringToFrontTextButton;
    ScopedPointer<TextButton> sendToBackTextButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneEditionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_CE75C11E02E72D3E__
