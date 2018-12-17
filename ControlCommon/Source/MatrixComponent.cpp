/*
  ==============================================================================

    MatrixComponent.cpp
    Created: 7 Mar 2018 10:43:23pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MatrixComponent.h"

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
    
    // Actual creation of matrix sliders and horizontal sliders
    for (int i=0 ; i<maxRowsCount ; i++)
    {
        for (int j=0 ; j<maxColsCount ; j++)
        {
            // Slider (i,j)
            sliders[idx(i,j)] = new MatrixSlider();
            sliders[idx(i,j)]->setName("Slider ID=" + boost::lexical_cast<std::string>(idx(i,j)) + " : row=" + boost::lexical_cast<std::string>(i) + " col=" + boost::lexical_cast<std::string>(j));
            sliders[idx(i,j)]->setComponentID(boost::lexical_cast<std::string>(idx(i,j)));
            addSlider(sliders[idx(i,j)]);
            
            sliders[idx(i,j)]->SetPropertiesFromVolume();
        }
        
        horizontalSliders[i] = new Slider("Horizontal Slider ID=" + boost::lexical_cast<std::string>(i));
        horizontalSliders[i]->setComponentID(boost::lexical_cast<std::string>(i));
        initAndAddHorizontalSlider(horizontalSliders[i]);
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
    slider->setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    slider->setColour(Slider::ColourIds::textBoxTextColourId, Colours::black);
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
        
        // Valeur linéaire simple mise dans la colonne 0
        rawDenseMatrix[idx(sliderId, 0)] = slider->getValue();
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
                horizontalSliders[i]->setVisible(true);
            else
                horizontalSliders[i]->setVisible(false);
        }
    }
    
    
    // Repaint at the very end
    //repaint(); // now : done from the grand-parent LabelledMatrixComponent
}
AppPurpose MatrixComponent::getPurpose()
{
    return grandParent->GetDisplayPurpose();
}




void MatrixComponent::SetSpatMatrix(std::shared_ptr<ControlMatrix> spatMatrix)
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
void MatrixComponent::SetSliderValue_dB(int row, int col, double newValue_dB,
                                        NotificationType juceNotification)
{
    SetSliderValue(row, col, Decibels::decibelsToGain(newValue_dB), juceNotification);
}
void MatrixComponent::SetSliderValue(int row, int col, double newValue,
                                     NotificationType juceNotification)
{
    // Avant tout, sauvegarde interne
    rawDenseMatrix[idx(row,col)] = newValue;
    
    // Slider de matrice
    MatrixSlider* slider = sliders[idx(row,col)].get();
    // valeur selon que l'on soit en dessous du treshold en dB, ou non
    auto value_dB = Decibels::gainToDecibels<double>(newValue);
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
        horizontalSliders[row]->setValue(newValue, juceNotification);
}


double MatrixComponent::GetSliderValue(int row, int col)
{
    if (0 <= row && row < maxRowsCount && 0 <= col && col < maxColsCount)
        return rawDenseMatrix[idx(row,col)];
    else
    {
        assert(false); // A bad row and col coefficient should never be asked for
        return 0.0;
    }
}

std::shared_ptr<ControlMatrix> MatrixComponent::GetSpatMatrix()
{
    /* Normalement : tout ça est maintenant fait au fur et à mesure...
     for (int i=0 ; i<maxRowsCount ; i++)
     {
     for (int j=0 ; j<maxColsCount ; j++)
     {
     // If non-zero only
     if (sliders[idx(i,j)]->getValue() > sliders[idx(i,j)]->GetMinVolume_dB() + MiamRouter_LowVolumeThreshold_dB)
     {
     double linearValue = Decibels::decibelsToGain(sliders[idx(i,j)]->getValue());
     rawDenseMatrix[idx(i,j)] = linearValue;
     }
     else
     rawDenseMatrix[idx(i,j)] = 0.0;
     }
     }*/
    // construction optimisée
    std::shared_ptr<ControlMatrix> returnPtr = std::make_shared<ControlMatrix>(rawDenseMatrix);
    return returnPtr;
}


