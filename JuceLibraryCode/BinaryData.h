/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   bg_png;
    const int            bg_pngSize = 2532263;

    extern const char*   button_16_off_png;
    const int            button_16_off_pngSize = 1567;

    extern const char*   button_16_y_png;
    const int            button_16_y_pngSize = 1567;

    extern const char*   encoder_32_w_png;
    const int            encoder_32_w_pngSize = 404428;

    extern const char*   pot_32_png;
    const int            pot_32_pngSize = 195548;

    extern const char*   pot_48_png;
    const int            pot_48_pngSize = 371366;

    extern const char*   round_button_12_off_png;
    const int            round_button_12_off_pngSize = 1436;

    extern const char*   round_button_12_y_png;
    const int            round_button_12_y_pngSize = 1116;

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
