/*
  ==============================================================================

    MatrixComponent.cpp
    Created: 7 Mar 2018 10:43:23pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "MatrixComponent.h"

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
            break;
            
        default :
            setSize(getParentWidth(), getParentHeight());
    }
}


// - - - - - Callbacks - - - - -

void MatrixComponent::sliderValueChanged(Slider* _slider)
{
    MatrixSlider* slider = dynamic_cast<MatrixSlider*>(_slider);
    if (slider) // Slider au sein d'une matrice
    {
        // Slider identification for a matrix slider
        int sliderId = std::stoi(slider->getComponentID().toStdString());
        auto i = idx2d(sliderId).i;
        auto j = idx2d(sliderId).j;
        
        // màj des données internes (on oublie les autres types de sliders, pas visibles...)
        rawDenseMatrix[idx(i, j)] = Decibels::decibelsToGain(slider->getValue());
        
        // Data transmission to the grandparent (and the listeners)
        grandParent->OnSliderValueChanged((int)i, (int)j,
                                          rawDenseMatrix[idx(i, j)]);
        
        // Graphical update depending on new value
        slider->SetPropertiesFromVolume();
    }
    else // Slider simple, horizontal
    {
        std::cout << "changement de valeur de slider horizontal" << std::endl;
    }
}



// = = = = = = = = = = Setters and Getters = = = = = = = = = =

void MatrixComponent::SetActiveSliders(int inputsCount, int outputsCount)
{
    // Rows updating
    if (n != inputsCount)
    {
        // For all existing cols, we do something
        for (int j=0 ; j<m ; j++)
        {
            if (inputsCount > n) // Increase of the rows count
            {
                for (int i=n ; i<inputsCount ; i++)
                    sliders[idx(i,j)]->SetIsActive(true);
            }
            else // Decrease of the rows count
            {
                for (int i=n-1 ; i>=inputsCount ; i--)
                    sliders[idx(i,j)]->SetIsActive(false);
            }
        }
        n = inputsCount;
    }
    
    // Cols updating, based on the current up-to-date rows number
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
        m = outputsCount;
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
    // Reset of the internal matrix
    for (size_t i=0 ; i<maxRowsCount ; i++)
        for (size_t j=0 ; j<maxColsCount ; j++)
            rawDenseMatrix[idx(i,j)] = 0.0;
    // Reset to an undisplayed value in dB for the sliders' matrix
    for (size_t i=0 ; i<maxRowsCount ; i++)
        for (size_t j=0 ; j<maxColsCount ; j++)
            SetSliderValue_dB((int)i, (int)j,
                              Miam_MinVolume_dB-MiamRouter_LowVolumeThreshold_dB);
    
    // Update of non-zero sliders only (input is a sparse matrix)
    for( spatMatrix->ResetIterator() ;
        spatMatrix->GetIterator() < spatMatrix->GetEndIterator();
        spatMatrix->IncIterator() )
    {
        rawDenseMatrix[spatMatrix->GetIterator1dCoord()] = spatMatrix->GetIteratorValue();
        
        Index2d current2dCoord = spatMatrix->GetIterator2dCoord();
        double sliderValue_dB = Decibels::gainToDecibels(rawDenseMatrix[spatMatrix->GetIterator1dCoord()]);
        SetSliderValue_dB((int)current2dCoord.i, (int)current2dCoord.j,
                          sliderValue_dB);
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
    
    // Mises à jour des données graphiques
    MatrixSlider* slider = sliders[idx(row,col)].get();
    // To prevent direct backwards retransmission
    slider->setValue(Decibels::gainToDecibels(newValue), juceNotification);
    
    // Display update
    slider->SetPropertiesFromVolume();
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


