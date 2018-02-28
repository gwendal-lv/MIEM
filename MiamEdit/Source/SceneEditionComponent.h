/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include <vector>
#include <memory>

#include "SceneCanvasComponent.h"
#include "MultiSceneCanvasComponent.h"
#include "MultiCanvasComponent.h"

#include "DrawableArea.h"

#include "SpatState.hpp"

namespace Miam
{
    // forward declarations
    class GraphicSessionManager;

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
                               public TextEditor::Listener,
                               public Button::Listener,
                               public Slider::Listener,
                               public ComboBox::Listener
{
public:
    //==============================================================================
    SceneEditionComponent ();
    ~SceneEditionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    // ----- Construction helpers -----

    /// \brief Links this module to parent and/or managing classes
    void CompleteInitialization(GraphicSessionManager* _graphicSessionManager, MultiCanvasComponent* _multiCanvasComponent);

    // ----- General Setters and Getters -----


    //MultiSceneCanvasComponent* AddCanvas();
    //void DeleteCanvas(SceneCanvasComponent::Id canvasId);




    // ----- Functions that obey orders sent from the Presenter -----

    // - - - - - Common - - - - -
    public :
    void SetEnabledAllControls(bool areEnabled, bool controlsBackUp = true);
    void CloseTemporaryDisplayedObjects();

    // - - - - - Canvases group - - - - -
    void SetCanvasGroupHidden(bool _isHidden);
    void SetCanvasGroupReduced(bool _isReduced);
    void SetDeleteSceneButtonEnabled(bool isEnabled);
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
    void UpdateStatesList(std::vector< std::shared_ptr<SpatState<double>> > &newSpatStates);
    void SelectSpatState(int index, NotificationType notificationType = NotificationType::dontSendNotification);
    // - - - - - Initial Scene State group - - - - -
    void SetInitialStateGroupHidden(bool _isHidden);
    void SetInitialStateGroupReduced(bool _isReduced);
    void SetDeleteExciterButtonEnabled(bool _isEnabled);


    // ----- Other setters and getters -----
    // - - - - - Colours - - - - -
    void SetAreaColourValue(juce::Colour colour);
    // - - - - - Text Values - - - - -
    void SetSceneName(std::string _name);
    void SetCanvasInfo(SceneCanvasComponent::Id _id);


    // Helpers
    private :
    void setVisibleAllAreaControls(bool areVisible);
    void setEnabledSelectedAreaControls(bool areEnabled);
    void setVisibleSpatControls(bool areVisible);
    void colourSliderMoved();

    // Translation avec sauvegarde en interne de ce qui vient d'être effectué
    void areaGroupTranslateY(int dY);
    void spatGroupTranslateY(int dY);
    void initialStateGroupTranslateY(int dY);
    void componentTranslateY(Component* component, int dY);


    public :
    virtual void textEditorTextChanged(TextEditor& _editor) override;



    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    // À DÉGAGER, ces objets n'appartiennent au SceneEditionComponent...
    //std::vector<MultiSceneCanvasComponent*> multiSceneCanvasComponents;
    MultiCanvasComponent* multiCanvasComponent = 0;

    GraphicSessionManager* graphicSessionManager = 0;

    int canvasGroupReducedH,
        areaGroupReducedH,
        spatGroupReducedH,
        initialStateGroupReducedH ;
    int areaGroupCurrentDy = 0,
        spatGroupCurrentDy = 0,
        initialStateGroupCurrentDy = 0;
    bool isCanvasGroupHidden = false,
        isAreaGroupReduced = false,
        isAreaGroupHidden = false,
        isSpatGroupHidden = false,
        isInitialStateGroupReduced = false,
        isInitialStateGroupHidden = false;

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
    ScopedPointer<ComboBox> spatStatesComboBox;
    ScopedPointer<Label> spatLabel;
    ScopedPointer<TextButton> addSceneTextButton;
    ScopedPointer<TextButton> deleteSceneTextButton;
    ScopedPointer<TextButton> sceneLeftTextButton;
    ScopedPointer<TextButton> sceneRightTextButton;
    ScopedPointer<Label> canvasInfoLabel;
    ScopedPointer<GroupComponent> initialStateGroupComponent;
    ScopedPointer<TextButton> addExciterTextButton;
    ScopedPointer<TextButton> deleteExciterTextButton;
    ScopedPointer<Label> sceneNameLabel;
    ScopedPointer<TextEditor> sceneNameTextEditor;
    ScopedPointer<ToggleButton> excitersEditionButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneEditionComponent)
};

//[EndFile] You can add extra defines here...
}    // namespace Miam
//[/EndFile]
