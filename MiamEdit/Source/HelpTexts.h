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

#pragma once

#include "JuceHeader.h"

#include "GraphicSessionMode.h"
#include "AppPurpose.h"

namespace Miam
{
    class HelpTexts {
        
        public :
        
        
        /// \brief Generic help text
        static juce::String GetHideHelpButtonText()
        {return TRANS("[-]     Hide help information");}
        /// \brief Generic help text
        static juce::String GetShowHelpButtonText()
        {return TRANS("[+]     Show help information");}
        
        
        
        
        
        /// \brief Help content related to the playable/editable/interactive scenes
        static juce::String GetScenesContextualHelp(GraphicSessionMode graphicSessionMode, AppPurpose appPurpose)
        {
            String appPurposeRelatedString;
            
            switch (graphicSessionMode)
            {
                case GraphicSessionMode::CanvasSelected:
                    if (appPurpose == AppPurpose::GenericController)
                        return TRANS("The playground (the big black rectangle) can handle as many interactive shapes as you require. You can add shapes, deform them, add or delete points... And finally you can link each shape to an OSC control state.\n"
                                     "\n"
                                     "The shapes are activated by \"exciters\", and each scene requires at least one exciter. Click on the button above to edit the exciters and begin playing.\n"
                                     "\n"
                                     "The active scene can be selected by clicking on the black buttons on top of the playground.");
                    else if (appPurpose == AppPurpose::Spatialisation)
                        return TRANS("The playground (the big black rectangle) can handle as many interactive shapes as you require. You can add shapes, deform them, add or delete points... And you can link each shape to one routing matrix for music spatialisation. The routing matrices are also called \"spatialisation states\".\n"
                                     "\n"
                                     "The shapes are activated by \"exciters\", and each scene requires at least one exciter. Click on the button above to edit the exciters and begin playing.\n"
                                     "\n"
                                     "The active scene can be selected by clicking on the black buttons on top of the playground.");
                    else
                        return String("");
                    
                    // - - - - - Exciter's (and interpolation) Help - - - - -
                case GraphicSessionMode::ExcitersEdition:
                case GraphicSessionMode::ExciterSelected:
                    if (appPurpose == AppPurpose::GenericController)
                        appPurposeRelatedString = TRANS("When you move an exciter from a shape to another, the app computes a smooth interpolation between the control states linked to the shapes. So by moving exciters, you can create and explore new states.\n"
                                                        "\n"
                                                        "The MIEM app interpolates all the parameters of the states and sends them in real-time to the OSC remote device. Custom OSC adresses can be configured in the Control States tab.\n"
                                                        "\n"
                                                        "To play on Android or iOS, please download the MIEM Player app from http://miem.laras.be/downloads");
                    else if (appPurpose == AppPurpose::Spatialisation)
                        appPurposeRelatedString = TRANS("When you move an exciter from a shape to another, the app computes a smooth interpolation between the spatialisation states linked to the shapes.\n"
                                                        "\n"
                                                        "So by moving exciters, you can create and explore new spatialisation states. The MIEM app interpolates the spatialisation matrix and sends the corresponding OSC data in real-time to the MIEM Matrix Router audio plug-in (VST or AU within your DAW).\n"
                                                        "\n"
                                                        "To play on Android or iOS, please download the MIEM Spat app from http://miem.laras.be/downloads");
                    return TRANS("MIEM Editor is now sending OSC control data as you are moving, creating or deleting exciters.\n"
                                 "\n"
                                 "An exciter has a finite energy. When it is placed over one or several shapes, it transmits and shares his energy between all intersecting shapes.\n"
                                 "The closer an exciter is to the center of a shape, the more energy it gives to this shape.\n"
                                 "\n"
                                 "If an exciter is not located over any shape, it is automatically turned off.\n\n")
                    + appPurposeRelatedString;
                    
                    // - - - - - Area edition Help - - - - -
                case GraphicSessionMode::AreaSelected:
                    if (appPurpose == AppPurpose::GenericController)
                    appPurposeRelatedString = TRANS("\"Control States\"");
                else if (appPurpose == AppPurpose::Spatialisation)
                    appPurposeRelatedString = TRANS("\"Routing Matrices\"");
                    return TRANS("A shape is now selected. You can rotate it, resize it, translate it, add and delete contour points, move its center, ...\n"
                                 "The center position is quite important because it is the location at which the shape will get the more energy.\n"
                                 "\n"
                                 "To be active, a shape must be linked to a state. Please go to the ")
                    + appPurposeRelatedString +
                    TRANS(" tab to create and configure the states.\n");
                
                    // - - - - - Waiting for point creation/deletion - - - - -
                case GraphicSessionMode::WaitingForPointCreation:
                    return TRANS("Please click on a contour point to add a new point next to it.");
                case GraphicSessionMode::WaitingForPointDeletion:
                    return TRANS("Please click on a contour point to delete it.");
                    
                    // - - - - - Default : no help returned - - - - -
                case GraphicSessionMode::Null:
                default :
                    return String("");
            }
        }
        
        
    };
}
