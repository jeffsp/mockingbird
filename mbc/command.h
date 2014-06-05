/// @file command.h
/// @brief console commands
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-06-05

#ifndef COMMAND_H
#define COMMAND_H

#include "state.h"
#include <iostream>
#include <string>

namespace mbc
{

class command
{
    public:
    typedef std::function<void (state &s)> func;
    command ()
    {
    }
    command (const std::string &name, const std::string &help, const func &f)
        : name (name)
        , help (help)
        , f (f)
    {
    }
    const std::string &get_name () const
    {
        return name;
    }
    const std::string &get_help () const
    {
        return help;
    }
    void operator() (state &s) { f (s); }
    private:
    std::string name;
    std::string help;
    func f;
};

}

#endif
