/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioDefines.h"

#include "ISlidersMatrixListener.h"
#include "SpatCommon/Source/LabelledMatrixComponent.h"



namespace Miam {

    // Forward declarations
    class Presenter;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OscMatrixComponent  : public Component,
                            public ISlidersMatrixListener,
                            public TextEditor::Listener,
                            public ButtonListener,
                            public SliderListener
{
public:
    //==============================================================================
    OscMatrixComponent (Presenter* _presenter);
    ~OscMatrixComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void textEditorReturnKeyPressed(TextEditor& textEditor) override;

    void OnSliderValueChanged(int row, int col, double value) override;
    void OnMatrixZeroed() override;

    void SetSliderValue(int row, int col, double value);
    void SetAttackSliderValue(double value);
    void SetUdpPortAndMessage(int udpPort, bool isConnected, std::string& oscAddress);
    void SetActiveSliders(int inputsCount, int outputsCount);

#ifdef __MIAM_DEBUG
    void __DisplayDebugMsg(std::string msg)
    {udpStatusLabel->setText(msg, NotificationType::dontSendNotification);}
#endif

    private :
    PopupMenu createHelpPopup();

    public :
    void SetNetworkHelpContent(std::vector<std::string> _helpContent) {networkHelpContent = _helpContent;}
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Presenter* presenter;

    std::vector<std::string> networkHelpContent;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> matrixGroupComponent;
    ScopedPointer<GroupComponent> preferencesGroupComponent;
    ScopedPointer<TextEditor> udpPortTextEditor;
    ScopedPointer<Label> udpPortLabel;
    ScopedPointer<Label> udpStatusLabel;
    ScopedPointer<GroupComponent> audioConfigComponent;
    ScopedPointer<ToggleButton> keyboardButton;
    ScopedPointer<Miam::LabelledMatrixComponent> slidersMatrix;
    ScopedPointer<Slider> attackSlider;
    ScopedPointer<Label> attackLabel;
    ScopedPointer<Label> attackUnitLabel;
    ScopedPointer<TextButton> helpTextButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscMatrixComponent)
};

//[EndFile] You can add extra defines here...
} // namespace Miam
//[/EndFile]
