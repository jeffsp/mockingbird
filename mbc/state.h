/// @file state.h
/// @brief console state
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-29

#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>

namespace mbc
{

typedef std::vector<std::string> filenames;

class state
{
    private:
    std::string image_filespec;
    filenames fns;
    public:
    void set_image_filespec (const std::string &image_filespec)
    {
        this->image_filespec = image_filespec;
    }
    const std::string &get_image_filespec () const
    {
        return image_filespec;
    }
    void set_filenames (const filenames &fns)
    {
        this->fns = fns;
    }
    const filenames &get_filenames () const
    {
        return fns;
    }
};

}

#endif
