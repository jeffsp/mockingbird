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
        try { parse (argc, argv); }
        catch (int ret) { return ret; }

        // read the environment
        const string EV = "MB_IMAGE_FILESPEC";
        const string FILESPEC (getenv (EV.c_str ()) == 0 ? "." : getenv (EV.c_str ()));
        clog << "In environment found " << EV << "=" << FILESPEC << endl;

        // read the config
        const string CONFIG_FN = ".mockingbirdrc";
        const string cfgfn (string (getenv ("HOME")) + "/" + CONFIG_FN);
        clog << "reading " << cfgfn << endl;
        config cfg;
        try { cfg.read (cfgfn); }
        catch (const runtime_error &e) { clog << e.what () << endl; }

        // initialize the state
        state s;
        clog << "scanning " << FILESPEC << endl;
        s.set_image_filespec (FILESPEC);
        s.set_filenames (glob (FILESPEC));
        clog << "found " << s.get_filenames ().size () << " files" << endl;

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
