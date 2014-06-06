/// @file nef2bayer.cc
/// @brief Convert a Nikon D700 NEF file to a Bayer image
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-01-24

#include "mockingbird.h"
#include <string>
#include <stdexcept>

const std::string usage = "usage: nef2bayer < fn";

int main (int argc, char **)
{
    try
    {
        if (argc != 1)
            throw std::runtime_error (usage);

        jack_rabbit::raster<unsigned short> p = mockingbird::nef2bayer (std::cin, std::clog);
        mockingbird::write_bayer (p, std::cout, std::clog);

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what () << std::endl;
        return -1;
    }
}
