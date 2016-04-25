/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_6228527_INCLUDED
#define BINARYDATA_H_6228527_INCLUDED

namespace BinaryData
{
    extern const char*   bird1_png;
    const int            bird1_pngSize = 120829;

    extern const char*   bird1AI_png;
    const int            bird1AI_pngSize = 164321;

    extern const char*   bird2_png;
    const int            bird2_pngSize = 124194;

    extern const char*   bird3_png;
    const int            bird3_pngSize = 117070;

    extern const char*   bird2AI_png;
    const int            bird2AI_pngSize = 192106;

    extern const char*   bird4_png;
    const int            bird4_pngSize = 92255;

    extern const char*   bird3AI_png;
    const int            bird3AI_pngSize = 211470;

    extern const char*   bird4AI_png;
    const int            bird4AI_pngSize = 252923;

    extern const char*   wallpaper2_jpg;
    const int            wallpaper2_jpgSize = 68295;

    extern const char*   larvaAI_png;
    const int            larvaAI_pngSize = 83530;

    extern const char*   larva_png;
    const int            larva_pngSize = 222766;

    extern const char*   grass_jpg;
    const int            grass_jpgSize = 3380810;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 12;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
