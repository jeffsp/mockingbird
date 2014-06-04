/// @file console.h
/// @brief console
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-29

#ifndef CONSOLE_H
#define CONSOLE_H

#include "tab_prompt.h"
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
    void process (state &s)
    {
        const std::string commands = "q=quit h,?=help c=imagespec";

        std::string command = prompt (std::clog, std::cin, "command", "h");
        std::clog << "command = " << command << std::endl;
        if (command == "h" || command == "?")
            std::clog << commands << std::endl;
        else if (command == "c")
        {
            std::string ifs = tab_prompt (">> ");
            if (!ifs.empty ())
                s.set_image_filespec (ifs);
        }
        else if (command == "l")
        {
            const std::vector<std::string> fns = s.get_image_filenames ();
            for (size_t i = 0; i < fns.size (); ++i)
            {
                if (i + 2 < s.get_results_length ())
                    std::clog << fns[i] << std::endl;
                else if (i + 2 == s.get_results_length ())
                    std::clog << "..." << std::endl;
                else if (i + 1 == s.get_results_length ())
                    std::clog << fns[i] << std::endl;
            }
        }
        else if (command == "q")
        {
            done = true;
        }
        else
            std::clog << "unknown command: " << command << std::endl;
    }
};

}

#endif
