/// @file nef2bayer.cc
/// @brief Convert a Nikon D700 NEF file to a 16 bit Bayer image
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2013-01-24

#include "main.h"

const std::string usage =
"Convert .NEF to 16 bit grayscale bayer\n"
"\n"
"usage: nef2bayer < input.nef > output.ppm";

void process (int argc, char **)
{
    if (argc != 1)
        throw std::runtime_error (usage);
    std::clog << "writing 16 bit grayscale to stdout" << std::endl;
    mockingbird::write_gs16 (
        mockingbird::nef2bayer (std::cin, std::clog),
        std::cout);
}
