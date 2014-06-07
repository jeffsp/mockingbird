/// @file nef2bayer.cc
/// @brief Convert a Nikon D700 NEF file to a Bayer image
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-01-24

#include "main.h"

const std::string usage = "usage: nef2bayer < fn";

void process (int , char **)
{
    jack_rabbit::raster<unsigned short> p = mockingbird::nef2bayer (std::cin, std::clog);
    mockingbird::write_bayer (p, std::cout, std::clog);
}
