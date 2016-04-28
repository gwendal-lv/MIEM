/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "Presenter.h"
#include "View.h"
#include "MainComponent.h"


//==============================================================================

/// \brief Class that implements the user entry point (initialization) and various OS-level
/// event functions, and creates an instance of the MainWindow. (class created by Juce from the Projucer)
class MiamEditApplication  : public JUCEApplication
{
    private :
    Miam::Presenter* presenter;
    Miam::View* view;
    
    
public:
    
    //==============================================================================
    MiamEditApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& /*commandLine*/) override
    {
        // [This method is where you should put your application's initialisation code..]
        mainWindow = new MainWindow (getApplicationName());
        
        
        // Instanciation of the 3 main parts of the application : Model, Presenter, View
        view = new Miam::View(mainWindow->getMainComponent());
        presenter = new Miam::Presenter(view); // Will reference itself to the View module
        
        
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..
        delete presenter;
        delete view;

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& /*commandLine*/) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }
    

    //==============================================================================
	/// \brief This class implements the desktop window that contains an instance of
    /// our MainContentComponent class (created by Juce from the Projucer).
    class MainWindow    : public DocumentWindow
    {
        private :
        MainContentComponent* mainComponent;
        public :
        MainContentComponent* getMainComponent() {return mainComponent;}
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            mainComponent = new MainContentComponent();
            setContentOwned (mainComponent, true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
            
            // Et pas dans le "main component"
            setResizable(true, true);
            
            // Normalement déjà fait de base...
            setUsingNativeTitleBar(true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

    // APPLICATION ATTRIBUTES
private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MiamEditApplication)
