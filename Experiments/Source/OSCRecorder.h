/*
  ==============================================================================

    OSCRecorder.h
    Created: 7 Feb 2019 9:09:19pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <vector>
#include <chrono>
#include <memory>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "ExperimentState.h"
#include "MiemExpePreset.h"

#include "UserQuestionsManager.h"
#include "OSCRecorderConnection.h"

class OSCRecorderComponent;
class MainComponent;

typedef std::chrono::steady_clock MiemClock;

class OSCRecorder : public UserQuestionsManager {
    
    // ===================================================================
    // ===================================================================
    // ================== PARAMETERS OF THE EXPERIMENT ===================
    // ===================================================================
    // ===================================================================
    public :
    
    
    const size_t ExperimentPresetsCount = 2; // not counting the trial presets
    const size_t TrialPresetsCount = 2;
    
    
#ifdef __MIEM_SHORT_DELAYS
    const int delayAfterFinished_ms = 500;
    const int listeningTime_ms = 1000;
#else
    const int delayAfterFinished_ms = 3000;
    const int listeningTime_ms = 5000;
#endif
    
    const int tcpTimeOut_ms = 3000; // default Juce value : 3000 ms
    
    // Éviter stockage dans un drive (iCloud, Google, etc...) au cas où
    // le drive commence à modifier les fichiers d'expé (on copiera juste tout le dossier
    // à chaque fois... dans un drive si + pratique)
    const char filesSavingPath[1024] = "/Users/gwendal/Recherche/MIEM_Experiments/";
    
    
    
    
    const char experimentGeneralDataFile[1024] = "A_OSC_Recorder_Experiment.xml";
    
    
    // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
    private :
    
    // - - - CONFIGURATION to be read from the general data file - - -
    std::string tcpServerName;
    int tcpServerPort = -1;
    int currentExperimentUid = -1;
    
    
    
    // links to view
    MainComponent* const mainComponent;
    
    // sub-modules, init at construction only
    OSCRecorderComponent& recorderComponent;
    std::shared_ptr<OSCRecorderConnection> tcpConnection;
    bool hasConnectionBeenLostDuringLastStep = false;
    ExperimentState stateBeforeConnectionLost = ExperimentState::NotInitialized;
    
    ExperimentState state = ExperimentState::NotInitialized;
    
    /// \brief Les indexes -1 et -2 correspondent au "tours d'essais" qui ne seront pas comptabilisés
    int currentPresetIndex = -(int)TrialPresetsCount - 1;
    std::vector<MiemExpePreset> presetsList; // a preset is represented by a size_t integer
    
    // Time-related data
    MiemClock::time_point startTimePt;
    juce::Time startSystemTime;
    
    // XML and data files
    std::string dataFilePath; // CSV file
    std::string infoFilePath; // XML file
    
    
    
    // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
    
    
    
    
    // = = = = = = = = = = METHODS = = = = = = = = = =
    public :
    OSCRecorder(MainComponent* _mainComponent);
    void BeginExperiment();
    
    // Internal management
    protected :
    void reinitExperiment();
    void tryConnectSocketAndContinueExpe();
    
    /// \brief Triggers the interactive experiment of the next preset, or the next big step
    /// of the experiment
    ///
    /// might be called at startup or after a deconnection
    void nextExperimentStep();
    /// \brief Goes to the next preset
    ///
    /// Manages if the next preset does not exist (then, switches to the final questions)
    void nextPreset();
    
    
    /// \brief Main State-Changing (mode managing) function
    void changeState(ExperimentState newState);
    
    
    // OSC control of Reaper and MIEM Controller
    protected :
    void selectNewScene();
    
    
    
    public :
    
    // - - - - - Events froms View - - - - -
    void OnButtonClicked(ExperimentState requestedState);
    // will check for double-strokes (3s delay between 2 key downs ?)
    void OnSpaceBarPushed();
    
    // overrides from the UserQuestionsManager parent abstract class
    virtual void OnFirstQuestionsAnswered(UserQuestions* sender) override;
    virtual void OnFinalQuestionsAnswered(UserFinalQuestions* sender) override;
    
    /// \brief Returns whether it is possible to quit now, or not
    bool OnQuitRequest();
    
    // - - - - - Events to View - - - - -
    protected :
    void displayError(String errorMsg);
    

    
    // - - - - - Events froms network - - - - -
    public :
    void OnConnectionLost();
    
    
    
    
    
    // - - - - - XML and DATA Save and Read - - - - -
    protected :
    /// \brief allows to check whether the files can be created or not. If not,
    /// we do not event start the experiment
    void createNewDataFiles();
    /// \brief The path is a const char already defined, and the names
    /// will be built automatically from the current date and time
    void saveToFile();
    /// \brief Gets the UID, and... other parameters
    void getExperimentParametersFromXmlFile();
    

    
    
};
