/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

#include "Model.h"
#include "Presenter.h"
#include "View.h"

#include "PlayerAppMode.h"

#include "MiamExceptions.h"


//==============================================================================
class MiemGenericControllerApplication  : public JUCEApplication
{
        
    // - - - - - The 3 main modules of our app - - - - -
    private :
    Miam::Model* model;
    Miam::Presenter* presenter;
    Miam::View* view;
    // - - - - - The 3 main modules of our app - - - - -
    
    
    
public:
    //==============================================================================
    MiemGenericControllerApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow.reset( new MainWindow (getApplicationName()) );


        // Instanciation of the 3 main parts of the application : Model, Presenter, View
        MainContentComponent* mainContentComponent = dynamic_cast<MainContentComponent*>(mainWindow->getChildComponent(0));
        if (mainContentComponent)
            view = new Miam::View(mainWindow.get(), mainContentComponent);
        else
            throw std::runtime_error("First child of Main Window is not a MainContentComponent...");
        presenter = new Miam::Presenter(view); // Will reference itself to the View module
        model = new Miam::Model(presenter);// Will reference itself to the Presenter module
        
        // Chargement de la 1ière session
        try {
            presenter->TryLoadFirstSession(commandLine.toStdString());
        }
        catch (Miam::ForceQuitException& )
        {
            quit();
        }
    }

    void shutdown() override
    {
        presenter->appModeChangeRequest(PlayerAppMode::Stopped);
        
        // Add your application's shutdown code here..
        delete model;
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
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainContentComponent(), true);

#if defined(__MIAMOBILE)
            setFullScreen(true);
            Desktop::setScreenSaverEnabled(false);
#if defined(JUCE_ANDROID)
            // Kiosk Mode android 9 :
            // - en simu : semble agrandir pour quelques secondes seulement
            // (ensuite barre de statut + boutons reviennent)
            // - en vrai (Nokia 7.1) --> OK, barre et boutons apparaissent en slide seulement
            Desktop::getInstance().setKioskModeComponent (this, false);
            // userArea n'a aucun sens sous Android... ni en simu, ni en réel
            //setBounds (Desktop::getInstance().getDisplays().getMainDisplay().userArea);
#endif
#else
            centreWithSize (800, 500);
            setResizable(true, true);
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
START_JUCE_APPLICATION (MiemGenericControllerApplication)
