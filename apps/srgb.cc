/// @file srgb.cc
/// @brief perform sSRGB conversion
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-06-19

#include "main.h"

const std::string usage =
"Perform sRGB conversion on a 16 RGB bit image\n"
"\n"
"usage: srgb < input.ppm > output.ppm";

void process (int argc, char **)
{
    using namespace std;
    using namespace horny_toad;
    using namespace jack_rabbit;

    if (argc != 1)
        throw runtime_error (usage);

    clog << "reading image from stdin" << endl;
    raster<unsigned short> p = read_rgb16 (cin);

    clog << "converting to sRGB" << endl;
    p = mockingbird::convert_to_sRGB (p);

    clog << "writing 16 bit RGB to stdout" << endl;
    write_pnm (cout, p.cols () / 3, p.rows (), p, true, true);
}
