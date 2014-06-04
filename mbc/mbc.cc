/// @file mbc.cc
/// @brief mockingbird console
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-06

#include "mbc.h"

using namespace std;
using namespace horny_toad;
using namespace mbc;

int main (int argc, char **argv)
{
    try
    {
        // parse the command line
        cmdline_options opts;
        try { opts = parse (argc, argv); }
        catch (int ret) { return ret; }

        // read the config
        const string CONFIG_FN = ".mockingbirdrc";
        const string config_fn (string (getenv ("HOME")) + "/" + CONFIG_FN);
        clog << "reading " << config_fn << endl;
        config cfg;
        try { cfg.read (config_fn); }
        catch (const runtime_error &e) { clog << e.what () << endl; }

        // read command line config, if specified
        if (!opts.config_fn.empty ())
        {
            clog << "reading " << opts.config_fn << endl;
            cfg.read (opts.config_fn);
        }

        // initialize the state
        state s;
        s.set_image_filespec (cfg.get_option ("IMAGE_FILESPEC"));
        if (!cfg.get_option ("RESULTS_LENGTH").empty ())
        {
            std::clog << "config specified a results length of " << cfg.get_option ("RESULTS_LENGTH") << std::endl;
            s.set_results_length (lexical_cast<size_t> (cfg.get_option ("RESULTS_LENGTH")));
            std::clog << "results length set to " << s.get_results_length () << std::endl;
        }

        // start the console
        console c;
        while (!c.is_done ())
            c.process (s);

        return c.exit_status ();
    }
    catch (const exception &e)
    {
        cerr << e.what () << endl;
        return -1;
    }
}
