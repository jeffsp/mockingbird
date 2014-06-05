/// @file utils.h
/// @brief utilities
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-06

#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <glob.h>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace mbc
{

template <typename T>
T lexical_cast (const std::string &s)
{
    T v;
    std::istringstream iss;
    iss.str (s);
    iss >> v;
    return v;
}

std::string prompt (std::ostream &os, std::istream &is, const std::string &txt, const std::string &def = std::string (""))
{
    os << txt;
    if (!def.empty ())
        os << " [" << def << "]: ";
    os << std::flush;
    std::string tmp;
    std::getline (is, tmp);
    if (tmp.empty ())
        return def;
    return tmp;
}

std::string exec (const std::string cmd)
{
    FILE *pipe = popen (cmd.c_str (), "r");
    if (!pipe)
        throw std::runtime_error ("could not open pipe");
    const int LEN = 16 * 1024;
    std::array<char,LEN> buffer;
    std::string result;
    while (!feof (pipe))
    {
        if (fgets (&buffer[0], LEN, pipe) != NULL)
            result += &buffer[0];
    }
    pclose (pipe);
    return result;
}

std::vector<std::string> glob (const std::string &w)
{
    glob_t r;
    glob (w.c_str(), GLOB_TILDE, NULL, &r);
    std::vector<std::string> fns;
    for (size_t i=0 ;i < r.gl_pathc; ++i)
        fns.push_back (std::string (r.gl_pathv[i]));
    globfree(&r);
    return fns;
}

}

#endif
