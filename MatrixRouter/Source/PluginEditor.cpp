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
    setSize (900, 600);
    
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
