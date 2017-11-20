/*
  ==============================================================================

    HelpContents.cpp
    Created: 2 Nov 2017 3:25:21pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "HelpContents.h"

#include "AudioDefines.h"
#include "OscDefines.h"

using namespace Miam;

std::vector<std::string> HelpContents::GetOscCommandsHelp()
{
    std::vector<std::string> returnStrings;
    
    std::string int32str = " (32 bits integer) ";
    std::string float32str = " (32 bits float) ";
    
    std::string oscMatAddress = Miam_OSC_Matrix_Address;
    std::string oscMatSetToZeroAddress = Miam_OSC_Matrix_Address;
    oscMatSetToZeroAddress += Miam_OSC_Set_to_Zero_Address;
    
    returnStrings.push_back("OSC commands available:");
    returnStrings.push_back("---> '" + oscMatAddress + " [input] [output] [volume]' sets one coefficient of the matrix");
    returnStrings.push_back("[input]" + int32str + "index (starting from 0) of the input channel to be routed to [output]");
    returnStrings.push_back("[output]" + int32str + "index (starting from 0) of the output channel");
    returnStrings.push_back("[volume]" + float32str + "linear routing volume within [ 0.0 ; 2.0 ]");
    returnStrings.push_back("---> '" + oscMatSetToZeroAddress + "' sets all coefficients of the matrix to zero");
    
    return returnStrings;
}
