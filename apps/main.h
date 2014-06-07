/// @file main.h
/// @brief App boilerplate
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-06-06

#include "mockingbird.h"
#include <string>
#include <stdexcept>

void process (int argc, char **argv);

int main (int argc, char **argv)
{
    try
    {
        process (argc, argv);

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what () << std::endl;
        return -1;
    }
}
