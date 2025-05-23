/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   BGd_png;
    const int            BGd_pngSize = 2532263;

    extern const char*   B16f_png;
    const int            B16f_pngSize = 1567;

    extern const char*   B16n_png;
    const int            B16n_pngSize = 1567;

    extern const char*   E32d_png;
    const int            E32d_pngSize = 404428;

    extern const char*   P32d_png;
    const int            P32d_pngSize = 195548;

    extern const char*   P48d_png;
    const int            P48d_pngSize = 371366;

    extern const char*   R12f_png;
    const int            R12f_pngSize = 1436;

    extern const char*   R12n_png;
    const int            R12n_pngSize = 1116;

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
