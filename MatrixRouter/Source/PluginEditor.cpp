/*
  ==============================================================================

    This file was auto-generated!
 Then heavily modified.....

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Presenter.h"

using namespace Miam;

//==============================================================================
/* Constructor called from the PluginProcessor
 */
MatrixRouterAudioProcessorEditor::MatrixRouterAudioProcessorEditor (MatrixRouterAudioProcessor& model, Presenter& _presenter)
    : AudioProcessorEditor (&model), //processor (p) // for MVP protection
      presenter(_presenter)
#ifdef __MIAM_DEBUG
	,
	oscLocalhostDebugger(9040)
#endif // MIAM_DEBUG
{
    // Childrens creation
    oscMatrixComponent = presenter.GetOscMatrixComponent();
    addAndMakeVisible(oscMatrixComponent);
    
    setResizable(true, true); // inutile ??
    setResizeLimits(800, 600, 100000, 100000);
    setSize (900, 600);
    
    // Timer launched at the very end
    // View will notify itself at each timer callback to the presenter
	auto updateFreq = presenter.GetUpdateFrequency_Hz(); // OK, osc-debugger-tested
    startTimerHz(updateFreq); // DOES NOT WORK IN REAPER !!!!!!!

#ifdef __MIAM_DEBUG
	AsyncParamChange debugMsg;
	debugMsg.FloatValue = 10102;
	oscLocalhostDebugger.SendParamChange(debugMsg, DataOrigin::View);
	debugMsg.FloatValue = (float)updateFreq;
	oscLocalhostDebugger.SendParamChange(debugMsg, DataOrigin::View);
#endif
}

MatrixRouterAudioProcessorEditor::~MatrixRouterAudioProcessorEditor()
{
	// notification to Presenter (to stop sending data)
	presenter.OnViewDestructed();

    stopTimer();
#ifdef __MIAM_DEBUG
		AsyncParamChange debugMsg;
	debugMsg.FloatValue = 10103;
	oscLocalhostDebugger.SendParamChange(debugMsg, DataOrigin::View);
#endif
}


// =========================== Juce Graphics ===========================
void MatrixRouterAudioProcessorEditor::paint (Graphics& /*g*/)
{
    // children components automatically painted
}

void MatrixRouterAudioProcessorEditor::resized()
{
    oscMatrixComponent->setBounds(this->getBounds());
}



// =========================== Callbacks ===========================
void MatrixRouterAudioProcessorEditor::timerCallback()
{


#ifdef __MIAM_DEBUG
	AsyncParamChange debugMsg;
	debugMsg.FloatValue = 999;
	oscLocalhostDebugger.SendParamChange(debugMsg, DataOrigin::View);
#endif

    presenter.UpdateFromView(this); 
}
