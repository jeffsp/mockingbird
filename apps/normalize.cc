/// @file normalize.cc
/// @brief normalize a 16-bit image
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-06-19

#include "main.h"

const std::string usage =
"Normalize a 16 bit image\n"
"\n"
"usage: normalize < input.ppm > output.ppm";

void process (int argc, char **)
{
    using namespace std;
    using namespace horny_toad;
    using namespace jack_rabbit;

    if (argc != 1)
        throw runtime_error (usage);

    clog << "reading image from stdin" << endl;
    raster<unsigned short> p = read_rgb16 (cin);

    clog << "normalizing" << endl;
    p = mockingbird::normalize (p, 99.0);

    clog << "writing 16 bit RGB to stdout" << endl;
    write_pnm (cout, p.cols () / 3, p.rows (), p, true, true);
}
