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

class state
{
    private:
    filenames image_filenames;
    size_t results_length;
    public:
    state ()
        : image_filenames ()
        , results_length (10)
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
};

}

#endif
