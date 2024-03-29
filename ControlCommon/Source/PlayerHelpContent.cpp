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

#include "PlayerHelpContent.h"

using namespace Miam;

juce::String PlayerHelpContent::GetMainHelp(AppPurpose appPurpose)
{
    // String that will be put at the end, for any app purpose
    String endString = "\n\n\n"
    + TRANS("To get all necessary desktop applications, please follow the link below.") + "\n\n"
    + TRANS("This interface is part of the MIEM (Multitouch Interfaces for Electroacoustic Music) research project.") + "\n"
    + TRANS("To get more information, or to participate in the research program linked to this interface, please visit http://miem.laras.be");
    
    // init from JuceHeader data
    String projectName(ProjectInfo::projectName);
    
    String sessionFilesString =
#if defined(JUCE_IOS)
    TRANS("MIEM Editor will allow you to create session files, which can be easily transferred to your device using iCloud or any other cloud service.");
#elif defined(JUCE_ANDROID)
    TRANS("MIEM Editor will allow you to create session files, which can be easily transferred to your device by using an USB cable or Google Drive (or any other cloud service).");
#else
    TRANS("MIEM Editor will allow you to create session files that ") + projectName + TRANS(" can open and play.");
#endif
    
    
    switch (appPurpose)
    {
            
            
            
            
        case AppPurpose::Spatialisation :
            return TRANS("This app is a touch controller for music spatialisation, based on interactions between geometric shapes instead of typical faders and potentiometers.")
            + "\n\n"
            
            + TRANS("Because it is only a controller, it unfornately does not work alone... It must be used altogether with a dedicated VST/AU spatialisation plugin running on a desktop computer. The plugin can be inserted in Reaper, Ableton Live, Pro Tools, or any other modern Digital Audio Workstation. The plugin itself is called MIEM Matrix Router.")
            + "\n\n"
            
            + TRANS("To edit the geometric shapes and the spatialisation effects, you also need to download and install the dedicated desktop editor (it is free and available for Windows and macOS). The editor is simply called MIEM Editor.")
            + "\n"
            + sessionFilesString
            + endString;
            break;
            
            
            
            
            
        case AppPurpose::GenericController :
            return TRANS("This app is an OSC touch controller based on the interactions between geometric shapes. Given your favorite presets, you can interpolate and create new presets with touch gestures.")
            + "\n\n"
            
            + TRANS("Because it is only a controller, it unfornately does not produce any sound or video by itself... but it can control your OSC devices on the local network: sound synthesizers, filters and samplers, VJing apps and DMX controllers, etc. It can also control your MIDI devices using MIEM OSC-MIDI Bridge, or any other OSC to MIDI application.")
            + "\n\n"
            
            + TRANS("To edit the geometric shapes and linked presets, please download and install the dedicated desktop app called MIEM Editor. It is free and available for Windows and macOS.")
            + "\n"
            + sessionFilesString
            + endString;
            break;
            
            
            
            
        default :
            return TRANS("For more information, go to http://miem.laras.be or contact Gwendal Le Vaillant at glevaillant@he2b.be");
            break;
    }
}
