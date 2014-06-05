/// @file console.h
/// @brief console
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-29

#ifndef CONSOLE_H
#define CONSOLE_H

#include "command.h"
#include "state.h"
#include "tab_prompt.h"
#include "utils.h"
#include "version.h"
#include <iostream>
#include <string>
#include <unordered_map>

namespace mbc
{

class console
{
    private:
    typedef std::unordered_map<std::string,command> commands;
    commands c;
    public:
    console ()
    {
        // show the version
        std::clog << "mockingbird console version " << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
        // initialize the commands
        c["q"] = command ("q", "quit", [] (state &s) { s.set_status (0); s.set_done (true); });
        c["h"] = command ("h", "help", [&] (state &s) { help (); });
        c["?"] = command ("?", "help", [&] (state &s) { help (); });
        c["c"] = command ("c", "change filespec", [] (state &s)
        {
            std::string ifs = tab_prompt ("");
            if (!ifs.empty ())
                s.set_image_filespec (ifs);
        });
        c["l"] = command ("l", "list files", [] (state &s)
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
        });
    }
    void process (state &s)
    {
        const std::string commands = "q=quit h,?=help c=imagespec";

        std::string command = prompt (std::clog, std::cin, ">> ");
        if (c.find (command) == c.end ())
        {
            help ();
            std::clog << "unknown command: " << command << std::endl;
        }
        else
            c[command] (s);
    }
    void help () const
    {
        // create a vector of strings for sorting
        std::vector<std::string> s;
        // dump to vector
        for (auto i : c)
        {
            std::stringstream ss;
            ss << i.second.get_name () << '\t' << i.second.get_help ();
            s.push_back (ss.str ());
        }
        // sort them lexigraphically
        sort (s.begin (), s.end ());
        // dump them to console
        for (auto i : s)
            std::clog << i << std::endl;
    }
};

}

#endif
