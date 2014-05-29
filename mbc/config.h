/// @file config.h
/// @brief read/write configuration file
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-28

#ifndef CONFIG_H
#define CONFIG_H

#include "horny_toad/horny_toad.h"
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace mbc
{

/// @brief read a configuration file
class config
{
    private:
    /// @brief option/value pairs
    std::map<std::string,std::string> options;
    public:
    /// @brief constructor
    config ()
    {
        options["RESULTS_LENGTH"] = "10";
    }
    /// @brief read a configuration from a file
    ///
    /// @param fn the name of the configuration file
    void read (const std::string &fn)
    {
        std::vector<std::string> lines = horny_toad::readlines (fn.c_str ());
        for (auto line : lines)
        {
            std::stringstream ss (line);
            std::string s1, s2;
            ss >> s1;
            ss >> s2;
            // ignore empty lines
            if (s1.empty ())
                continue;
            // ignore comments
            if (s1[0] == '#')
                continue;
            options[s1] = s2;
        }
        for (auto opt : options)
        {
            std::clog << opt.first << " = " << opt.second << std::endl;
        }
    }
    /// @brief get the string value of an option
    ///
    /// @param name option name
    ///
    /// @return option value
    std::string get_option (const std::string &name)
    {
        if (options.find (name) == options.end ())
            throw std::runtime_error ("could not find option");
        return options[name];
    }
};

}

#endif
