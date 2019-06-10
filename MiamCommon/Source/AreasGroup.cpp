/*
  ==============================================================================

    AreasGroup.cpp
    Created: 8 Jun 2019 5:26:57pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "AreasGroup.h"

#include "InteractiveArea.h"

using namespace Miam;



AreasGroup::AreasGroup(int _indexInScene, juce::Colour _colour)
:
indexInScene(_indexInScene),
colour(_colour)
{
}


Colour AreasGroup::GetDefaultColour(int colorIndex)
{
    // The 16 first colors are pre-defined, and are very different.
    // The rest is random
    std::mt19937 randomGenerator;
    
    switch (colorIndex)
    {
        case 0 :
            return juce::Colour::fromRGB(0, 114, 189);
        case 1 :
            return juce::Colour::fromRGB(217, 83, 25);
        case 2 :
            return juce::Colour::fromRGB(235, 177, 32);
        case 3 :
            return juce::Colour::fromRGB(126, 47, 142);
        case 4 :
            return juce::Colour::fromRGB(120, 172, 48);
        case 5 :
            return juce::Colour::fromRGB(77, 190, 238);
        case 6 :
            return juce::Colour::fromRGB(162, 20, 47);
        case 7 : // End of first cycle : light grey
            return juce::Colour::fromRGB(140, 140, 140);
            
        case 8 :
            return juce::Colour::fromRGB(0, 0, 255);
        case 9 :
            return juce::Colour::fromRGB(0, 128, 0);
        case 10 :
            return juce::Colour::fromRGB(255, 0, 0);
        case 11 :
            return juce::Colour::fromRGB(0, 200, 191);
        case 12 :
            return juce::Colour::fromRGB(200, 0, 180);
        case 13 :
            return juce::Colour::fromRGB(191, 191, 0);
        case 14 :
            return juce::Colour::fromRGB(64, 64, 64);
        case 15 : // last is pure white : means that the cycle ends
            return juce::Colour::fromRGB(255, 255, 255);
            
            // direct seed re-init, and gen of next random number
        default :
            randomGenerator.seed(colorIndex + 5);
            return juce::Colour::fromRGB( (randomGenerator() % 226) + 30,
                                          (randomGenerator() % 226) + 30,
                                          (randomGenerator() % 226) + 30);
    }
}




void AreasGroup::buildErosionMasks(std::vector<int>& iMasks, std::vector<int>& jMasks, int erodeSize)
{
    if ( (erodeSize == 3) // 3px square mask
        || (erodeSize == 5)) // (will be inside the 5px mask)
    {
        iMasks.push_back(-1); jMasks.push_back(-1);
        iMasks.push_back(-1); jMasks.push_back( 0);
        iMasks.push_back(-1); jMasks.push_back(+1);
        iMasks.push_back( 0); jMasks.push_back(-1);
        iMasks.push_back( 0); jMasks.push_back(+1);
        iMasks.push_back(+1); jMasks.push_back(-1);
        iMasks.push_back(+1); jMasks.push_back( 0);
        iMasks.push_back(+1); jMasks.push_back(+1);
        
        if (erodeSize == 5) // 5px rounded square with 1px corner radius
        {
            iMasks.push_back(-2); jMasks.push_back(-1);
            iMasks.push_back(-2); jMasks.push_back( 0);
            iMasks.push_back(-2); jMasks.push_back(+1);
            iMasks.push_back(-1); jMasks.push_back(-2);
            iMasks.push_back(-1); jMasks.push_back(+2);
            iMasks.push_back( 0); jMasks.push_back(-2);
            iMasks.push_back( 0); jMasks.push_back(+2);
            iMasks.push_back(+1); jMasks.push_back(-2);
            iMasks.push_back(+1); jMasks.push_back(+2);
            iMasks.push_back(+2); jMasks.push_back(-1);
            iMasks.push_back(+2); jMasks.push_back( 0);
            iMasks.push_back(+2); jMasks.push_back(+1);
        }
    }
    else
        // mask for the requested size still needs to be written...
        assert(false);

}



void AreasGroup::ErodeAreasGroups(std::vector<AreasGroup*>& inImg, AreasGroup* backArea,
                                   size_t _imgW, size_t _imgH)
{
    // int copies (to avoid cast warnings....)
    int imgW = (int)_imgW;
    int imgH = (int)_imgH;
    
    
    // MASK SIZE CHOSEN HERE
    // 5 gives the more consistent results:
    // the center of the exciter (with has a displayed radius of a few
    // pixels) cannot go too close to the limit of a group.
    // (but within a group, it can to the very limit of each shape... so
    // no loss of precision)
    //
    // Différences de temps de rendu (pour Debug.mcs très chargée)
    // sous macOS, macbook pro 8 coeurs
    // DEBUG :    erode0: 1,7 s  ;  erode5: 2,4 s
    // RELEASE :  erode0: 0,27s  ;  erode5: 0,29s  --> érosion ultra optimisée par le compilateur
    const int erodeSize = 5;
    
    
    assert((erodeSize % 2) == 1); // odd mask size.... necessary
    if ( (imgW <= (2*erodeSize)) || (imgH <= (2*erodeSize)))
    {
        // Image is too small... Cannot properly execute this optimized algorithm.
        // Return of a full black image
        size_t imageSizeBackup = inImg.size();
        inImg.clear();
        inImg.resize(imageSizeBackup, backArea);
        return;
    }
    
    
    // duplication of input vector
    auto img = inImg;
    
    
    // Construction du masque = tableaux (en i et j séparés) de décalage des pixels à tester
    std::vector<int> iMasks;
    std::vector<int> jMasks;
    buildErosionMasks(iMasks, jMasks, erodeSize);
    // the index "boundW" IS NOT itself in the margin
    int boundW = (int) std::floor((double)(erodeSize) / 2.0);;
    
    
    
    // Optimization : we start by backing the boundaries (that will be eroded in any case...)
    // (a few pixel will be double-backed..... ok on millions of pixels)
    const int erodeLimitH = imgH - boundW; // the indexes "erodeLimit" ARE themselves in the margin
    const int erodeLimitW = imgW - boundW;
    // Left
    for (int i=0 ; i<imgH ; i++)
        for (int j=0 ; j<boundW ; j++)
            img[i*imgW + j] = backArea;
    // Top
    for (int i=0 ; i<boundW ; i++)
        for (int j=0 ; j<imgW ; j++)
            img[i*imgW + j] = backArea;
    // Right
    for (int i=0 ; i<imgH ; i++)
        for (int j=erodeLimitW ; j<imgW ; j++)
            img[i*imgW + j] = backArea;
    // Bottom
    for (int i=erodeLimitH ; i<imgH ; i++)
        for (int j=0 ; j<imgW ; j++)
            img[i*imgW + j] = backArea;
    
    
    // Main erosion loop : a pixel stays only if all pixels in the mask
    // are non-black (non-background) and are the same pixels (same AreasGroup*)
    // i is a row of the matrix ; j is a col of the matrix
    for (int i=boundW ; i<erodeLimitH ; i++)
    {
        for (int j=boundW ; j<erodeLimitW ; j++)
        {
            // Rérérence vers l'image de sortie
            AreasGroup* & refToTestPx = img[i*imgW + j];
            // Dès qu'on a trouvé un pixel différent autour de lui... (dans image de départ)
            // On assigne ce pixel à backArea
            for (size_t k=0 ; ( (k<iMasks.size()) && (refToTestPx != backArea) ) ; k++)
            {
                if (refToTestPx != inImg[(i+iMasks[k])*imgW + (j+jMasks[k])])
                    refToTestPx = backArea;
            }
        }
    }
    
    
    // At the end : result and image are swapped
    inImg.swap(img);
}

