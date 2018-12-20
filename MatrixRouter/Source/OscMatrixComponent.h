/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioDefines.h"

#include "ISlidersMatrixListener.h"
#include "LabelledMatrixComponent.h"


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
                            public Button::Listener,
                            public Slider::Listener
{
public:
    //==============================================================================
    OscMatrixComponent (Presenter* _presenter);
    ~OscMatrixComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void textEditorReturnKeyPressed(TextEditor& textEditor) override;
    void textEditorTextChanged(TextEditor& textEditor) override;
    private :
    void parseUdpPortAndSendEvent();
    public :

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
    std::unique_ptr<GroupComponent> matrixGroupComponent;
    std::unique_ptr<Miam::LabelledMatrixComponent> slidersMatrix;
    std::unique_ptr<GroupComponent> preferencesGroupComponent;
    std::unique_ptr<TextEditor> udpPortTextEditor;
    std::unique_ptr<Label> udpPortLabel;
    std::unique_ptr<Label> udpStatusLabel;
    std::unique_ptr<GroupComponent> audioConfigComponent;
    std::unique_ptr<ToggleButton> keyboardButton;
    std::unique_ptr<Slider> attackSlider;
    std::unique_ptr<Label> attackLabel;
    std::unique_ptr<Label> attackUnitLabel;
    std::unique_ptr<TextButton> helpTextButton;
    std::unique_ptr<TextButton> connectTextButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscMatrixComponent)
};

//[EndFile] You can add extra defines here...
} // namespace Miam
//[/EndFile]
