/// @file state.h
/// @brief console state
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-29

#ifndef STATE_H
#define STATE_H

#include "utils.h"
#include <string>
#include <vector>

namespace mbc
{

typedef std::vector<std::string> filenames;
typedef jack_rabbit::raster<unsigned short> image16_t;

class state
{
    private:
    filenames image_filenames;
    size_t results_length;
    bool done;
    int status;
    std::vector<image16_t> gs16_images;
    public:
    state ()
        : image_filenames ()
        , results_length (10)
        , done (false)
        , status (0)
    {
    }
    void set_image_filespec (const std::string &image_filespec)
    {
        std::clog << "scanning " << image_filespec << std::endl;
        image_filenames = glob (image_filespec);
        std::clog << "found " << image_filenames.size () << " files" << std::endl;
    }
    const filenames &get_image_filenames () const
    {
        return image_filenames;
    }
    void set_results_length (const size_t n)
    {
        results_length = n;
    }
    size_t get_results_length () const
    {
        return results_length;
    }
    void set_done (bool d)
    {
        done = d;
    }
    bool is_done () const
    {
        return done;
    }
    void set_status (int s)
    {
        status = s;
    }
    int get_status () const
    {
        return status;
    }
    void clear_gs16_images ()
    {
        gs16_images.clear ();
    }
    void push_back (const image16_t &img)
    {
        gs16_images.push_back (img);
    }
    const std::vector<image16_t> &get_gs16_images () const
    {
        return gs16_images;
    }
};

}

#endif
