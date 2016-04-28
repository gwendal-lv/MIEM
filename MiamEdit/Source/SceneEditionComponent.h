/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

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
                               public SliderListener,
                               public ComboBoxListener
{
public:
    //==============================================================================
    SceneEditionComponent ();
    ~SceneEditionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    // ----- Construction helpers -----
    void CompleteInitialization(SceneEditionManager* _sceneEditionManager);

    // ----- General Setters and Getters -----
    /// \brief Get a pointer to one of the canvases.
    SceneCanvasComponent* GetSceneCanvasComponent(int position)
    { return sceneCanvasComponents[position]; }


    // ----- Functions that obey orders sent from the Presenter -----
    public :
    void SetEnabledAllControls(bool areEnabled, bool controlsBackUp = true);
    // - - - - - Canvases & canvas group - - - - -
    void SetCanvasGroupHidden(bool _isHidden);
    void SetCanvasGroupReduced(bool _isReduced);
    // - - - - - Area group - - - - -
    void SetAreaGroupHidden(bool _isHidden);
    void SetAreaGroupReduced(bool _isReduced);
    void SetEnabledAreaEditionControls(bool areEnabled);
    void SetEnabledAreaMainControls(bool areEnabled);
    void SetVisibleAreaEditionControls(bool areVisible);
    void DisableAllButtonsBut(juce::String onlyActiveButtonName);
    void SetPasteEnabled(bool _isEnabled); // button with back-up function
    // - - - - - Spat group - - - - -
    void SetSpatGroupHidden(bool _isHidden);
    void SetSpatGroupReduced(bool _isReduced);
    // - - - - - Initial Scene State group - - - - -
    void SetInitialStateGroupHidden(bool _isHidden);
    void SetInitialStateGroupReduced(bool _isReduced);


    // ----- Other setters and getters -----
    void SetAreaColourValue(juce::Colour colour);


    // Helpers
    private :
    void setEnabledSelectedAreaControls(bool areEnabled);
    void setVisibleSpatControls(bool areVisible);
    void colourSliderMoved();

    void areaGroupTranslateY(int dY);
    void spatGroupTranslateY(int dY);
    void componentTranslateY(Component* component, int dY);


    public :
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    std::vector<SceneCanvasComponent*> sceneCanvasComponents;

    SceneEditionManager* sceneEditionManager;

    int canvasGroupReducedH, areaGroupReducedH, spatGroupReducedH;
    bool isCanvasGroupHidden = false,
            isAreaGroupReduced = false,
            isSpatGroupHidden = false;

    // When all buttons are disabled, to highlight that we can do only 1 action,
    // we need to back up the state of some buttons
    bool pasteTextButtonEnabledBackUp = false;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> areaGroupComponent;
    ScopedPointer<GroupComponent> spatGroupComponent;
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
    ScopedPointer<GroupComponent> canvasGroupComponent;
    ScopedPointer<ComboBox> speakersGroupComboBox;
    ScopedPointer<Label> spatLabel;
    ScopedPointer<TextButton> addSceneTextButton;
    ScopedPointer<TextButton> deleteSceneTextButton;
    ScopedPointer<TextButton> sceneUpTextButton;
    ScopedPointer<TextButton> sceneDownTextButton;
    ScopedPointer<Label> canvasInfoLabel;
    ScopedPointer<TextEditor> sceneNameTextEditor;
    ScopedPointer<GroupComponent> initialStateGroupComponent;
    ScopedPointer<TextButton> addExciterTextButton;
    ScopedPointer<TextButton> deleteAreaTextButton2;
    ScopedPointer<TextButton> editInitialStateTextButton;
    ScopedPointer<TextButton> endEditInitialStateTextButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneEditionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_CE75C11E02E72D3E__
