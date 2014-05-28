/// @file mbc.cc
/// @brief mockingbird console
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-06

#include "mockingbird.h"
#include <getopt.h>

using namespace std;
using namespace horny_toad;
using namespace mockingbird;

void help (ostream &os)
{
    os << "This is the mockingbird console help." << endl;
}

template<typename T>
void show_options (ostream &os, const T &options)
{
    os << "options:" << endl;
    for (size_t i = 0; i + 1 < sizeof (options) / sizeof (struct option); ++i)
    {
        os << "\t--" << options[i].name;
        if (options[i].has_arg)
            os << " <arg>" << endl;
        else
            os << endl;
    }
}

int main (int argc, char **argv)
{
    try
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
                show_options (clog, options);
                throw runtime_error ("invalid option");
                break;
                case 'h':
                help (clog);
                return 0;
                break;
            }
            ++parsed_options;
        };

        vector<string> left_over_opts (&argv[parsed_options], &argv[argc]);

        // process left over options
        for (auto i : left_over_opts)
            clog << i << endl;

        // read the environment
        const string EV = "MB_IMAGE_FILESPEC";
        const string FILESPEC (getenv (EV.c_str ()) == 0 ? "." : getenv (EV.c_str ()));

        clog << "In environment found " << EV << "=" << FILESPEC << endl;

        // read the config
        const string CONFIG_FN = ".mockingbirdrc";
        const string cfgfn (string (getenv ("HOME")) + "/" + CONFIG_FN);
        clog << "reading " << cfgfn << endl;
        config cfg;
        try
        {
            cfg.read (cfgfn);
        }
        catch (const runtime_error &e)
        {
            clog << e.what () << endl;
        }

        clog << "scanning " << FILESPEC << endl;
        vector<string> fns = glob (FILESPEC);

        clog << "found " << fns.size () << " files" << endl;

        // start the console
        clog << "mockingbird console version " << MAJOR_VERSION << "." << MINOR_VERSION << endl;
        const string commands = "q=quit h,?=help";

        string command;
        while (command != "q")
        {
            command = prompt (clog, cin, "command", 'h');
            clog << "command = " << command << endl;
            if (command == "h" || command == "?")
                clog << commands << endl;
            else if (command == "l")
                for (auto i : fns)
                    std::clog << i << endl;
            else if (command == "q")
                return 0;
            else
                clog << "unknown command: " << command << endl;
        }

        return 0;
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
