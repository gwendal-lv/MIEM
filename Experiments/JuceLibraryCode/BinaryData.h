/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Recorder_AfterTrial_EN_txt;
    const int            Recorder_AfterTrial_EN_txtSize = 352;

    extern const char*   Recorder_AfterTrial_FR_txt;
    const int            Recorder_AfterTrial_FR_txtSize = 419;

    extern const char*   Recorder_Intro_EN_txt;
    const int            Recorder_Intro_EN_txtSize = 1214;

    extern const char*   Recorder_Intro_FR_txt;
    const int            Recorder_Intro_FR_txtSize = 1437;

    extern const char*   Translation_FR_txt;
    const int            Translation_FR_txtSize = 4842;

    extern const char*   _4_Faders_png;
    const int            _4_Faders_pngSize = 32310;

    extern const char*   _4_Interpolation_areas_png;
    const int            _4_Interpolation_areas_pngSize = 35496;

    extern const char*   Experiment_macOS_1024at1_png;
    const int            Experiment_macOS_1024at1_pngSize = 672439;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
