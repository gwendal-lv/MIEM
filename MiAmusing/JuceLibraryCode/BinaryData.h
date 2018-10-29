/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   newFontImg_png;
    const int            newFontImg_pngSize = 104267;

    extern const char*   _808Clap12_wav;
    const int            _808Clap12_wavSize = 49816;

    extern const char*   _808HiHats15_wav;
    const int            _808HiHats15_wavSize = 16902;

    extern const char*   B3Leslie2c_wav;
    const int            B3Leslie2c_wavSize = 45228;

    extern const char*   cello_wav;
    const int            cello_wavSize = 46348;

    extern const char*   epiano12c_wav;
    const int            epiano12c_wavSize = 119852;

    extern const char*   gesaffelstein1_wav;
    const int            gesaffelstein1_wavSize = 279988;

    extern const char*   gesaffelstein2_wav;
    const int            gesaffelstein2_wavSize = 306190;

    extern const char*   gesaffelstein3_wav;
    const int            gesaffelstein3_wavSize = 338830;

    extern const char*   hmd2c_wav;
    const int            hmd2c_wavSize = 251910;

    extern const char*   save_xml;
    const int            save_xmlSize = 6388;

    extern const char*   wurldel2c_wav;
    const int            wurldel2c_wavSize = 265260;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 12;

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
