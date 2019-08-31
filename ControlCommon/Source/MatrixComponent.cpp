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

#include "MatrixComponent.h"
#include "AudioUtils.hpp"
#include "MiamMath.h"

#include <algorithm> // std::fill

using namespace Miam;

// - - - - - Construction and Destruction - - - - -

MatrixComponent::MatrixComponent(LabelledMatrixComponent* _grandParent, unsigned int _maxRowsCount, unsigned int _maxColsCount)
:
grandParent(_grandParent),
maxRowsCount(_maxRowsCount), maxColsCount(_maxColsCount)
{
    // Initial values (to be changed after construction)
    n = 0;
    m = 0;
    
    rawDenseMatrix = new double[maxRowsCount*maxColsCount];
    std::fill(rawDenseMatrix, rawDenseMatrix + maxRowsCount*maxColsCount, (double)0.0);
    
    ReconstructGuiObjects();
}
MatrixComponent::~MatrixComponent()
{
    delete rawDenseMatrix;
}

void MatrixComponent::ReconstructGuiObjects()
{
    // Auto-sizing at first
    resized();
    
    // Pre-allocations for vector of scoped pointers
    sliders.clear();
    sliders.resize(maxRowsCount*maxColsCount);
    horizontalSliders.clear();
    horizontalSliders.resize(maxRowsCount);
    interpolationCurvesCopy.clear();
    interpolationCurvesCopy.resize(maxRowsCount, BasicInterpolationCurve<double>::GetDefault());
    
    // Actual creation of matrix sliders and horizontal sliders
    for (int i=0 ; i<maxRowsCount ; i++)
    {
        // matrix sliders
        for (int j=0 ; j<maxColsCount ; j++)
        {
            // Slider (i,j)
            sliders[idx(i,j)] = new MatrixSlider();
            sliders[idx(i,j)]->setName("Slider ID=" + boost::lexical_cast<std::string>(idx(i,j)) + " : row=" + boost::lexical_cast<std::string>(i) + " col=" + boost::lexical_cast<std::string>(j));
            sliders[idx(i,j)]->setComponentID(boost::lexical_cast<std::string>(idx(i,j)));
            addSlider(sliders[idx(i,j)]);
            
            sliders[idx(i,j)]->SetPropertiesFromVolume();
        }
        
        // horizontal slider
        horizontalSliders[i] = new MatrixRowSlider("Horizontal Slider ID="
                                                   + boost::lexical_cast<std::string>(i),
                                                   itemH);
        horizontalSliders[i]->setComponentID(boost::lexical_cast<std::string>(i));
        initAndAddHorizontalSlider(horizontalSliders[i]);
        // interp curve for horizontal slider : nothing to init (default curves on resize)
    }
    
    // Active sliders
    SetActiveSliders(n, m);
    
    // Graphical placement
    RepositionGuiObjects();
}

void MatrixComponent::RepositionGuiObjects()
{
    // Actual positionning
    // i repesents a *graphical* row (= matrix row in this case...)
    for (int i=0 ; i<maxRowsCount ; i++)
    {
        if (getPurpose() == AppPurpose::GenericController)
        {
            horizontalSliders[i]->setBounds(0, i*(int)itemH,
                                            getParentWidth(), (int)itemH);
            horizontalSliders[i]->setVisible(true);
        }
        else
            horizontalSliders[i]->setVisible(false);
        
        // j repesents a *graphical* column (= matrix col now...)
        for (int j=0 ; j<maxColsCount ; j++)
        {
            int idx = i*(int)maxRowsCount + j; // graphically one col further than the matrix
            if (getPurpose() == AppPurpose::Spatialisation)
            {
                sliders[idx]->setBounds((int)round(j*itemW), (int)round(i*itemH), (int)round(itemW), (int)round(itemH));
                sliders[idx]->setVisible(true);
            }
            else
                sliders[idx]->setVisible(false);
        }
    }
}

void MatrixComponent::addSlider(Slider* slider)
{
    addAndMakeVisible(slider);
    slider->addListener(this);
}
void MatrixComponent::initAndAddHorizontalSlider(Slider* slider)
{
    slider->setRange(0.0, 1.0);
    addSlider(slider);
}


// Auxiliary functions
Index2d MatrixComponent::idx2d(size_t indexToConvert)
{
    Index2d index2d;
    index2d.i = (size_t) std::floor((double)(indexToConvert)/(double)(maxRowsCount));
    index2d.j = indexToConvert % maxColsCount;
    return index2d;
}


// - - - - - Juce graphics - - - - -

void MatrixComponent::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);   // clear the background
    
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}
void MatrixComponent::resized()
{
    switch(getPurpose())
    {
        case AppPurpose::Spatialisation :
            setSize((int)maxColsCount*itemW, (int)maxRowsCount*itemH);
            break;
        
        case AppPurpose::GenericController :
            setSize(getParentWidth(), (int)maxRowsCount*itemH);
            RepositionGuiObjects();
            break;
            
        default :
            setSize(getParentWidth(), getParentHeight());
    }
}


// - - - - - Callbacks - - - - -

void MatrixComponent::sliderValueChanged(Slider* slider)
{
    MatrixSlider* matrixSlider = dynamic_cast<MatrixSlider*>(slider);
    if (matrixSlider && getPurpose() == AppPurpose::Spatialisation) // Slider au sein d'une matrice, et si ça convient au type d'app
    {
        // Slider identification for a matrix slider
        int sliderId = std::stoi(matrixSlider->getComponentID().toStdString());
        auto i = idx2d(sliderId).i;
        auto j = idx2d(sliderId).j;
        
        // màj des données internes (on oublie les autres types de sliders, pas visibles...)
        rawDenseMatrix[idx(i, j)] = Decibels::decibelsToGain(matrixSlider->getValue());
        
        // Data transmission to the grandparent (and the listeners)
        grandParent->OnSliderValueChanged((int)i, (int)j,
                                          rawDenseMatrix[idx(i, j)]);
        
        // Graphical update depending on new value
        matrixSlider->SetPropertiesFromVolume();
    }
    else if (getPurpose() == AppPurpose::GenericController)// Slider simple, horizontal, avec le bon purpose
    {
        int sliderId = std::stoi(slider->getComponentID().toStdString());
        
        
        // Dé-normalisation de la valeur
        // = "reverse interpolation"
        double reverseInterpolatedValue = interpolationCurvesCopy[sliderId].ReverseInterpolateValue(slider->getValue());
        //std::cout << "reverse interpolated value = " << reverseInterpolatedValue << std::endl;
        rawDenseMatrix[idx(sliderId, 0)] = reverseInterpolatedValue;
        
        // Data transmission to the grandparent (and the listeners)
        grandParent->OnSliderValueChanged(sliderId, 0,
                                          rawDenseMatrix[idx(sliderId, 0)]);
    }
}



// = = = = = = = = = = Setters and Getters = = = = = = = = = =

void MatrixComponent::SetActiveSliders(int inputsCount, int outputsCount)
{
    // Sliders matrice : plusieurs milliers, système optimisé nécessaire
    // Rows updating
    if (getPurpose() == AppPurpose::Spatialisation)
    {
        if (n != inputsCount)
        {
            // For all existing cols, we do something
            for (int j=0 ; j<m ; j++)
            {
                if (inputsCount > n) // Increase of the rows count
                {
                    for (int i=n ; i<inputsCount ; i++)
                    {
                        sliders[idx(i,j)]->SetIsActive(true);
                    }
                }
                else // Decrease of the rows count
                {
                    for (int i=n-1 ; i>=inputsCount ; i--)
                    {
                        sliders[idx(i,j)]->SetIsActive(false);
                    }
                }
            }
        }
    }
    n = inputsCount;
    
    // Cols updating, based on the current up-to-date rows number
    if (getPurpose() == AppPurpose::Spatialisation)
    {
        if (m != outputsCount)
        {
            // For all existing rows, we do something
            for (int i=0 ; i<n ; i++)
            {
                if (outputsCount > m) // Increase of the cols count
                {
                    for (int j=m ; j<outputsCount ; j++)
                        sliders[idx(i,j)]->SetIsActive(true);
                }
                else // Decrease of the cols count
                {
                    for (int j=m-1 ; j>=outputsCount ; j--)
                        sliders[idx(i,j)]->SetIsActive(false);
                }
            }
        }
    }
    m = outputsCount;
    
    
    // Sliders horizontaux : il n'y en a pas des miliers... Alors on n'optimise pas
    // remise aux bons états de tout
    // On fait APRèS  avoir fait la mise à jour de n,m dans les boucles précédentes
    if (getPurpose() == AppPurpose::GenericController)
    {
        for (int i=0 ; i<maxRowsCount ; i++)
        {
            if (i<n)
            {
                horizontalSliders[i]->setVisible(true);
                horizontalSliders[i]->setEnabled(true);
            }
            else
            {
                horizontalSliders[i]->setVisible(false);
                horizontalSliders[i]->setEnabled(false);
            }
        }
    }
    
    
    // Repaint at the very end
    //repaint(); // now : done from the grand-parent LabelledMatrixComponent
}
AppPurpose MatrixComponent::getPurpose()
{
    return grandParent->GetDisplayPurpose();
}




void MatrixComponent::SetSpatMatrix(std::shared_ptr<ControlMatrix<double>> spatMatrix)
{
    // Reset to an displayed values in dB for all sliders
    for (size_t i=0 ; i<maxRowsCount ; i++)
        for (size_t j=0 ; j<maxColsCount ; j++)
            SetSliderValue((int)i, (int)j, 0.0);
    
    // Update of non-zero sliders only (input is a sparse matrix)
    for( spatMatrix->ResetIterator() ;
        spatMatrix->GetIterator() < spatMatrix->GetEndIterator();
        spatMatrix->IncIterator() )
    {
        Index2d current2dCoord = spatMatrix->GetIterator2dCoord();
        SetSliderValue((int)current2dCoord.i, (int)current2dCoord.j,
                       spatMatrix->GetIteratorValue());
    }
}
std::shared_ptr<ControlMatrix<double>> MatrixComponent::GetSpatMatrix()
{
    auto returnPtr = std::make_shared<ControlMatrix<double>>(rawDenseMatrix);
    return returnPtr;
}

void MatrixComponent::SetSliderValue_dB(int row, int col, double newValue_dB,
                                        NotificationType juceNotification)
{
    SetSliderValue(row, col, AudioUtils<double>::Amplitude_dB_to_Linear(newValue_dB), juceNotification);
}
void MatrixComponent::SetSliderValue(int row, int col, double newValue,
                                     NotificationType juceNotification)
{
    // Avant tout, sauvegarde interne
    rawDenseMatrix[idx(row,col)] = newValue;
    
    // Slider de matrice
    MatrixSlider* slider = sliders[idx(row,col)].get();
    // valeur selon que l'on soit en dessous du treshold en dB, ou non
    auto value_dB = AudioUtils<double>::Linear_to_Amplitude_dB(newValue);
    if (value_dB < Miam_MinVolume_dB)
    {
        slider->setValue(Miam_MinVolume_dB-MiamRouter_LowVolumeThreshold_dB, juceNotification);
    }
    else
    {
        slider->setValue(value_dB, juceNotification); // To prevent direct backwards retransmission
    }
    // Display update
    slider->SetPropertiesFromVolume();
    
    // Slider horizontal colonne zéro
    if (col == 0)
    {
        // Dé-normalisation ("Dé-interpolation") de la valeur
        // ATTENTION : doit dépendre de la courbe d'interpolation
        double interpValue = interpolationCurvesCopy[row].InterpolateValue(newValue);
        horizontalSliders[row]->setValue(interpValue, juceNotification);
    }
}


double MatrixComponent::GetSliderValue(int row, int col)
{
    if (0 <= row && row < maxRowsCount && 0 <= col && col < maxColsCount)
    {
        // For SPAT or not-first-col cases : no interp, no resize is involved
        if (getPurpose() == AppPurpose::Spatialisation
            || row > 0)
            return rawDenseMatrix[idx(row,col)];
        // For GEN CON, first-col : special behavior ?
        else
        {
            // no special behavior at the moment; the reverse-interp must be processed before
            // writing data into the normalized rawDenseMatrix
            double forTest = rawDenseMatrix[idx(row,col)];
            return forTest;
        }
    }
    else
    {
        assert(false); // A bad row and col coefficient should never be asked for
        return 0.0;
    }
}


void MatrixComponent::SetHorizontalSliderInterpolationData(int row,
                                                           BasicInterpolationCurve<double> newInterpCurve)
{
    if (row < 0 || row > maxRowsCount)
        throw std::out_of_range("MatrixComponent.cpp: cannot use an out of bounds row number");
    
    // sauvegarde dans la classe
    interpolationCurvesCopy[row] = newInterpCurve;
    
    // et extraction des données utiles pour màj graphique
    auto newMin = newInterpCurve.GetMinY();
    auto newMax = newInterpCurve.GetMaxY();
    
    // skew factor depend on interp curve
    double skewFactor = 1.0;
    // largeur référencée pour l'audio, par rapport à une bande passante de
    // 20kHz
    double rangeWidenessFactor = std::abs(newMax - newMin) / 20000.0;
    rangeWidenessFactor = Math::Clamp(rangeWidenessFactor, 0.0, 1.0);
    
    double rangeDecadesNumber = 1.0;
    if ((newMin > 0.0) && (newMax > newMin))
        rangeDecadesNumber = std::log10(newMax / newMin);
    const double maxDecadesConsidered = 3.0; // 3 decades max. are considered
    rangeDecadesNumber = Math::Clamp(rangeDecadesNumber, 0.0, maxDecadesConsidered);
    
    
    switch( newInterpCurve.GetInterpolationType() )
    {
        // The skew factor actually depends on how on big the range of the slider.
            // Calibrated for audio frequencies : mid-point is :
            // exact for a 3-decades band (20Hz to 20kHz)
            // a bit too big for a 2-decades band (50Hz to 5kHz)
            // and a bit too small for a 1-decade band (100Hz to 1kHz)
        case ParamInterpolationType::Independant_Log :
            //skewFactor = 0.20 + (1.0 - rangeWidenessFactor) * 0.40;
            skewFactor = 0.20 + (1.0 - (rangeDecadesNumber/maxDecadesConsidered)) * 0.40;
            break;
            
            
        case ParamInterpolationType::Independant_Soft2 :
            skewFactor = 0.5; // 0.5 corresponds to a pure x^2, null tangent at origin
            break;
        case ParamInterpolationType::Independant_Hard2 :
            skewFactor = 2.0; // 2.0 corresponds to a pure sqrt, infinite tangent at origin
            break;
        case ParamInterpolationType::Independant_Soft1 :
            skewFactor = 0.69;
            break;
        case ParamInterpolationType::Independant_Hard1 :
            skewFactor = 1.40;
            break;
            
        case ParamInterpolationType::Independant_Linear :
        default:
            break;
    }
    
    horizontalSliders[row]->setSkewFactor(skewFactor);
    
    // range
    horizontalSliders[row]->setRange(newMin, newMax);
    
    // to force an update
    double valueBackup = horizontalSliders[row]->getValue();
    horizontalSliders[row]->setValue((valueBackup - newMin) / 2.0, // does not work if same value
                                     NotificationType::dontSendNotification);
    horizontalSliders[row]->setValue(valueBackup, 
                                     NotificationType::sendNotificationAsync);
    
    //horizontalSliders[row]->repaint(); // actually works
}


