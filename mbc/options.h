/// @file options.h
/// @brief parse command line options
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-29

#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>
#include <getopt.h>

namespace mbc
{

void help (std::ostream &os)
{
    os << "This is the mockingbird console help." << std::endl;
}

template<typename T>
void show_options (std::ostream &os, const T &options)
{
    os << "options:" << std::endl;
    for (size_t i = 0; i + 1 < sizeof (options) / sizeof (struct option); ++i)
    {
        os << "\t--" << options[i].name;
        if (options[i].has_arg)
            os << " <arg>" << std::endl;
        else
            os << std::endl;
    }
}

void parse (int argc, char **argv)
{
    static struct option options[] =
    {
        {"help", 0, 0, 'h'},
        {NULL, 0, NULL, 0}
    };

    int option_index;
    int arg;
    int parsed_options = 0;

    // parse the options
    while ((arg = getopt_long (argc, argv, "h", options, &option_index)) != -1)
    {
        switch (arg)
        {
            default:
            std::clog << "invalid option: " << (char) arg << std::endl;
            show_options (std::clog, options);
            // throw main()'s return value
            throw -1;

            case 'h':
            help (std::clog);
            // throw main()'s return value
            throw 0;
        }
        ++parsed_options;
    };

    std::vector<std::string> left_over_opts (&argv[parsed_options], &argv[argc]);

    // process left over options
    for (auto i : left_over_opts)
        std::clog << i << std::endl;
}

};

#endif
