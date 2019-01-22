/*
  ==============================================================================

    PlayerHelpContent.cpp
    Created: 22 Jan 2019 5:00:28pm
    Author:  Gwendal

  ==============================================================================
*/

#include "PlayerHelpContent.h"

using namespace Miam;

juce::String PlayerHelpContent::GetMainHelp(AppPurpose appPurpose)
{
    // String that will be put at the end, for any app purpose
    String endString = "\n\n\n"
    + TRANS("To get all necessary desktop applications, please follow the link below.") + "\n\n"
    + TRANS("This interface is part of the MIEM (Multitouch Interface for Electroacoustic Music) research project.") + "\n"
    + TRANS("To get more information, or to parcipate in the research program linked to this interface, please contact the author: Gwendal Le Vaillant at glevaillant@he2b.be or visit http://laras.be");
    
    // init from JuceHeader data
    String projectName(ProjectInfo::projectName);
    
    String sessionFilesString =
#if defined(JUCE_IOS)
    TRANS("MIEM Editor will allow you to create session files, which can be easily transferred to your device using iCloud.");
#elif defined(JUCE_ANDROID)
    TRANS("MIEM Editor will allow you to create session files, which can be easily transferred to your device using Google Drive.");
#else
    TRANS("MIEM Editor will allow you to create session files that ") + projectName + TRANS(" can open and play.");
#endif
    
    
    switch (appPurpose)
    {
            
            
            
            
        case AppPurpose::Spatialisation :
            return TRANS("This app is an innovative touch controller for music spatialisation, based on geometric shapes instead of typical faders and potentiometers.")
            + "\n\n"
            
            + TRANS("Because it is only a controller, it unfornately does not work alone... It must be used altogether with a dedicated VST/AU spatialisation plugin running on a desktop computer. The plugin can be inserted in Reaper, Ableton Live, Pro Tools, or any other modern Digital Audio Workstation. This plugin itself is called MIEM Matrix Router.")
            + "\n\n"
            
            + TRANS("To edit the geometric shapes and the spatialisation effects, you also need to download and install the dedicated desktop editor (it is free and available for Windows and macOS). The editor is simply called MIEM Editor.")
            + "\n"
            + sessionFilesString
            + endString;
            break;
            
            
            
            
            
        case AppPurpose::GenericController :
            return TRANS("This app is an innovative OSC touch controller, based on geometric shapes. Given your favorite presets, you can interpolate and create new presets with touch gestures.")
            + "\n\n"
            
            + TRANS("Because it is only a controller, it unfornately does not produce any sound or video by itself... but it can control any OSC-enabled device on the local network.")
            + "\n\n"
            
            + TRANS("To edit the geometric shapes, the presets and the OSC configuration, you need to download and install a dedicated desktop editor (it is free and available for Windows and macOS). The editor is called MIEM Editor.")
            + "\n"
            + sessionFilesString
            + endString;
            break;
            
            
            
            
        default :
            return TRANS("Please go to http://laras.be or contact glevaillant@he2b.be for more information.");
            break;
    }
    }
