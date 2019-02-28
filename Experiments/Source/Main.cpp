/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "OSCRecorder.h"
#include "OSCRecorderComponent.h"

//==============================================================================
class MIEM_ExperimentsApplication  : public JUCEApplication
{
private:
    std::unique_ptr<OSCRecorder> recorderManager;
    
    
public:
    //==============================================================================
    MIEM_ExperimentsApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        if (OSCRecorder::GetLanguage().compare("FR") == 0)
        {
            auto localisedTranslation = new LocalisedStrings(String(CharPointer_UTF8(BinaryData::Translation_FR_txt)),
                                        true); // ignorer la casse... fausse bonne idée ?
            LocalisedStrings::setCurrentMappings(localisedTranslation); // gère la destruction
        }
        
        // Pré-écrit par Juce
        mainWindow.reset (new MainWindow (getApplicationName()));
        
        // Modules perso
        recorderManager.reset( new OSCRecorder(mainWindow->mainComponent));
        
        // Démarrage quelques instants + tard
        juce::Timer::callAfterDelay(50, [this] {this->recorderManager->BeginExperiment();});
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        if (recorderManager->OnQuitRequest())
        {
            // On force l'écriture d'un fichier de résultats au moment de quitter
            // ????????
            
            // Puis on détruit les modules dans leur ordre de construction
            recorderManager.reset();
            
            // Ensuite on quitte pour de vrai
            quit();
        }
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
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
        public :
        MainComponent* mainComponent = 0; // pas ouf....
        
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            mainComponent = new MainComponent();
            setContentOwned (mainComponent, true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
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
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MIEM_ExperimentsApplication)
