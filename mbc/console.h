/// @file console.h
/// @brief console
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-29

#ifndef CONSOLE_H
#define CONSOLE_H

#include "state.h"
#include "utils.h"
#include "version.h"
#include <iostream>
#include <string>

namespace mbc
{

class console
{
    private:
    bool done;
    int status;
    public:
    console ()
        : done (false)
        , status (0)
    {
        std::clog << "mockingbird console version " << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
    }
    bool is_done () const
    {
        return done;
    }
    int exit_status () const
    {
        return status;;
    }
    void process (const state &s)
    {
        const std::string commands = "q=quit h,?=help";

        std::string command;
        while (command != "q")
        {
            command = prompt (std::clog, std::cin, "command", 'h');
            std::clog << "command = " << command << std::endl;
            if (command == "h" || command == "?")
                std::clog << commands << std::endl;
            else if (command == "l")
            {
                const int ret = system ("sh -i -c 'tree ../.. | more'");
                if (ret != 0)
                    std::clog << "system() failed" << std::endl;
                //for (auto i : s.get_filenames ())
                //    std::clog << i << std::endl;
            }
            else if (command == "q")
            {
                done = true;
            }
            else
                std::clog << "unknown command: " << command << std::endl;
        }
    }
};

}

#endif
