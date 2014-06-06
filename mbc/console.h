/// @file console.h
/// @brief console
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-29

#ifndef CONSOLE_H
#define CONSOLE_H

#include "command.h"
#include "../mockingbird.h"
#include "state.h"
#include "tab_prompt.h"
#include "utils.h"
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
        std::clog << "mockingbird version " << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
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
            const std::vector<std::string> &fns = s.get_image_filenames ();
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
        c["n"] = command ("n", "read nef files", [] (state &s)
        {
            const std::vector<std::string> &fns = s.get_image_filenames ();
            std::clog << "clearing gs16 buffers" << std::endl;
            s.clear_gs16_images ();
            for (size_t i = 0; i < fns.size (); ++i)
            {
                std::clog << "reading " << fns[i] << std::endl;
                std::ifstream ifs (fns[i]);

                if (!ifs)
                {
                    std::clog << "warning: can't read image" << std::endl;
                    continue;
                }

                s.push_back (mockingbird::nef2bayer (ifs, std::clog));
            }
        });
        c["d"] = command ("d", "plot distribution", [] (state &s)
        {
            // images
            const std::vector<image16_t> imgs = s.get_gs16_images ();
            // distribution
            std::vector<unsigned short> d (1 << 16);
            std::clog << "creating distribution" << std::endl;
            // fill distribution
            for (auto i : imgs)
                for (auto j : i)
                    ++d[j];
            // write it to a file
            const char *t = "/tmp/mbcdata.txt";
            std::clog << "opening " << t << std::endl;
            std::ofstream ofs (t);
            if (!ofs)
                throw std::runtime_error ("can't open temporary file for writing");
            std::clog << "writing distribution" << std::endl;
            int y = 0;
            for (auto i : d)
                ofs << y++ << ' ' << i << std::endl;
            // plot it with a gnuplot system call
            std::stringstream ss;
            ss << "gnuplot -p -e \"plot '" << t << "' smooth freq with boxes\"";
            std::clog << "executing " << ss.str () << std::endl;
            const int ret = system (ss.str ().c_str ());
            std::clog << "system returned " << ret << std::endl;
        });
    }
    void process (state &s)
    {
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
