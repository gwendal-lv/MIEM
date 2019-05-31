/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   InterpolationCurve_Hard1_80at1_png;
    const int            InterpolationCurve_Hard1_80at1_pngSize = 3330;

    extern const char*   InterpolationCurve_Hard2_80at1_png;
    const int            InterpolationCurve_Hard2_80at1_pngSize = 3285;

    extern const char*   InterpolationCurve_Soft1_80at1_png;
    const int            InterpolationCurve_Soft1_80at1_pngSize = 3418;

    extern const char*   InterpolationCurve_Soft2_80at1_png;
    const int            InterpolationCurve_Soft2_80at1_pngSize = 3411;

    extern const char*   InterpolationCurve_Threshold_80at1_png;
    const int            InterpolationCurve_Threshold_80at1_pngSize = 2143;

    extern const char*   InterpolationCurve_Expscale_80at1_png;
    const int            InterpolationCurve_Expscale_80at1_pngSize = 3233;

    extern const char*   InterpolationCurve_Linear_80at1_png;
    const int            InterpolationCurve_Linear_80at1_pngSize = 2146;

    extern const char*   InterpolationCurve_Logscale_80at1_png;
    const int            InterpolationCurve_Logscale_80at1_pngSize = 3335;

    extern const char*   Editor_macOS_1024at_png;
    const int            Editor_macOS_1024at_pngSize = 773650;

    extern const char*   Editor_Windows_1024at1_png;
    const int            Editor_Windows_1024at1_pngSize = 872686;

    extern const char*   newFontImg_png;
    const int            newFontImg_pngSize = 104267;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 11;

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
