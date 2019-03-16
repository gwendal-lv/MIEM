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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LoggerComponent  : public Component,
                         public TextEditor::Listener,
                         public InterprocessConnection,
                         public Button::Listener
{
public:
    //==============================================================================
    LoggerComponent ();
    ~LoggerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    virtual void textEditorTextChanged (TextEditor &) override;

    private :
    void update();
    void tryConnect(); ///< And continues to try connecting
    public :

    virtual void     connectionMade () override;
    virtual void     connectionLost () override;
    virtual void     messageReceived (const MemoryBlock &message) override;

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    bool isConnected = false;
    bool connectionWasLost = false;
    bool connectionWasUserDisconnected = true;
    bool couldNotConnect = false;
    bool tryToConnect = false;
    const int timeOut_ms = 1000;

    std::string receivedLog;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> textEditor;
    std::unique_ptr<TextEditor> ipTextEditor;
    std::unique_ptr<Label> ipLabel;
    std::unique_ptr<Label> portLabel;
    std::unique_ptr<TextEditor> portTextEditor;
    std::unique_ptr<TextButton> connectButton;
    std::unique_ptr<TextButton> disconnectButton;
    std::unique_ptr<TextButton> clearButton;
    std::unique_ptr<Label> statusLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoggerComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

