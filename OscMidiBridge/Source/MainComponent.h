/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "OSCListenerForwarder.h"
#include <deque>

namespace Miam
{
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainComponent  : public Component,
                       public OSCListenerForwarder::Listener,
                       public Button::Listener
{
public:
    //==============================================================================
    MainComponent ();
    ~MainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.


    virtual void OnMessageThroughBridge(const MiemExpeSample& oscSample,
                                        const MiemMidiSample& midiSample) override;

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::string virtualMidiDeviceName;
    std::string baseOscListeningString;
    std::string baseOscForwardingString;

    bool isConnected = false;

    // comme le code est très, très simple... Alors le modèle appartient directement
    // ici à la classe graphique
    std::unique_ptr<OSCListenerForwarder> oscListenerForwarder;


    std::deque<std::string> logData;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> listenLabel1;
    std::unique_ptr<Label> listenLabel2;
    std::unique_ptr<Label> listenLabel3;
    std::unique_ptr<TextButton> connectButton;
    std::unique_ptr<Label> udpPortLabel;
    std::unique_ptr<Label> notConnectedLabel;
    std::unique_ptr<TextEditor> udpPortTextEditor;
    std::unique_ptr<TextButton> displayLogButton;
    std::unique_ptr<TextEditor> logTextEditor;
    std::unique_ptr<Label> forwardingLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//[EndFile] You can add extra defines here...


} // namespace Miam

//[/EndFile]

