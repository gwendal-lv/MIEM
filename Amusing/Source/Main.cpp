/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/




#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

#include "AmusingModel.h"
#include "Presenter.h"
#include "View.h"

//==============================================================================
class MiamSpatApplication  : public JUCEApplication
{
    
    // - - - - - The 3 main modules of our app - - - - -
    private :
    Amusing::AmusingModel* model;
	Amusing::Presenter* presenter;
	Amusing::View* view;
    // - - - - - The 3 main modules of our app - - - - -
    
    
public:
    //==============================================================================
    MiamSpatApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow = new MainWindow (getApplicationName());
        mainWindow->setResizable(true, true);
		mainWindow->setFullScreen(true);
        
        // Instanciation of the 3 main parts of the application : Model, Presenter, View
        MainContentComponent* mainContentComponent = dynamic_cast<MainContentComponent*>(mainWindow->getChildComponent(0));
        if (mainContentComponent)
            view = new Amusing::View(mainContentComponent);
        else
            throw std::runtime_error("First child of Main Window is not a MainContentComponent...");
        presenter = new Amusing::Presenter(view); // Will reference itself to the View module
        model = new Amusing::AmusingModel(presenter);// Will reference itself to the Presenter module
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

		DBG("####### main destructor #########");

		delete model;
		DBG("------- model deleted --------");
        delete presenter;
		DBG("------- presenter deleted --------");
        delete view;
		DBG("------- view deleted --------");
        mainWindow = nullptr; // (deletes our window)
		DBG("------- mainWindow null --------");
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainContentComponent(), true);
			setVisible(true);

#if defined(__AMUSINGMOBILE)
			setFullScreen(true);
#else
            centreWithSize (getWidth(), getHeight());
			setResizable(true, true);
#endif
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

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MiamSpatApplication)
