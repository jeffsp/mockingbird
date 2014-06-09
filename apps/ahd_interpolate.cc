/// @file ahd_interpolate.cc
/// @brief ahd interpolate a 16 bit bayer image
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-06-09

#include "main.h"

const std::string usage =
"AHD interpolate a 16 bit Bayer image\n"
"\n"
"usage: ahd_interpolate < input.ppm > output.ppm";

void process (int argc, char **)
{
    using namespace std;
    using namespace horny_toad;
    using namespace jack_rabbit;

    if (argc != 1)
        throw runtime_error (usage);

    clog << "reading Bayer image from stdin" << endl;
    raster<unsigned short> p = read_grayscale16 (cin);

    clog << "converting to RGBA image" << endl;
    p = mockingbird::convert_to_rgba (p);

    clog << "scaling image" << endl;
    p = mockingbird::scale_image (p);

    // Set global variable for dcraw
    mockingbird::image = reinterpret_cast<ushort (*)[4]> (&p[0]);
    mockingbird::width = p.cols () / 4;
    mockingbird::height = p.rows ();

    clog << "ahd interpolating" << endl;
    mockingbird::ahd_interpolate ();

    clog << "converting to RGB" << endl;
    p = mockingbird::convert_to_rgb (p);

    clog << "converting to sRGB" << endl;
    p = mockingbird::convert_to_sRGB (p);

    clog << "normalizing" << endl;
    p = mockingbird::normalize (p, 99.0);

    clog << "writing 16 bit RGB to stdout" << endl;
    write_pnm (cout, p.cols () / 3, p.rows (), p, true, true);
}
