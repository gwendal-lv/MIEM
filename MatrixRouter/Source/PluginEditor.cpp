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
{
    // Childrens creation
    oscMatrixComponent = presenter.GetOscMatrixComponent();
    addAndMakeVisible(oscMatrixComponent);
    
    setResizable(true, true); // inutile ??
    setResizeLimits(800, 600, 100000, 100000);
    setSize (800, 600);
    
    // Timer launched at the very end
    // View will notify itself at each timer callback to the presenter
    startTimerHz(presenter.GetUpdateFrequency_Hz());
}

MatrixRouterAudioProcessorEditor::~MatrixRouterAudioProcessorEditor()
{
    stopTimer();
}


// =========================== Juce Graphics ===========================
void MatrixRouterAudioProcessorEditor::paint (Graphics& g)
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
    presenter.UpdateFromView(this);
}
