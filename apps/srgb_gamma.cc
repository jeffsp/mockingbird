/// @file srgb_gamma.cc
/// @brief sRGB gamma correct an image
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-01-17

#include "main.h"

const std::string usage =
"Perform sRGB gamma correction on a 16 RGB bit image\n"
"\n"
"usage: srgb_gamma < input.ppm > output.ppm";

void process (int argc, char **)
{
    using namespace std;
    using namespace horny_toad;
    using namespace jack_rabbit;

    if (argc != 1)
        throw runtime_error (usage);

    clog << "reading image from stdin" << endl;
    raster<unsigned short> p = read_rgb16 (cin);

    clog << "applying sRGB gamma correction" << endl;
    vector<unsigned short> lut = horny_toad::sRGB_gamma_curve ();
    for (size_t i = 0; i < p.size (); ++i)
    {
        assert (p[i] < lut.size ());
        p[i] = lut[p[i]];
    }

    clog << "writing 16 bit RGB to stdout" << endl;
    write_pnm (cout, p.cols () / 3, p.rows (), p, true, true);
}
