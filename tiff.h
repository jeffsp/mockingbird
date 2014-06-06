/// @file tiff.h
/// @brief TIFF image format routines
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2010-05-25

#ifndef TIFF_H
#define TIFF_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

namespace mockingbird
{

namespace tiff
{

// ifd = image file directory
struct tiff_ifd
{
    char make[64];
    char model[64];
    size_t width;
    size_t height;
    size_t bps;
    size_t comp;
    size_t offset;
    size_t samples;
    size_t bytes;
};

unsigned short get2 (std::istream &is)
{
    return (is.get () << 8) + is.get ();
}

unsigned long get4 (std::istream &is)
{
    return (is.get () << 24) + (is.get () << 16) + (is.get () << 8) + is.get ();
}

unsigned type_size (unsigned type)
{
    unsigned sz[] = {
        1, 1, 1, 2,
        4, 8, 1, 1,
        2, 4, 8, 4,
        8, 8 };
    return type < 14 ? sz[type] : 1;
}

template<typename T>
void parse_ifds (std::istream &is, T &ifds)
{
    unsigned tags = get2 (is);
    size_t ifd = ifds.size ();
    ifds.push_back (tiff_ifd ());
    //std::clog << "parsing " << tags << " tags" << std::endl;
    while (tags--)
    {
        unsigned tag  = get2 (is);
        //std::clog << "tag: \t" << tag << std::endl;
        unsigned type = get2 (is);
        unsigned count  = get4 (is);
        size_t next_tag_offset = is.tellg () + static_cast<std::streampos> (4);
        unsigned bytes = type_size (type);
        if (count * bytes > 4)
        {
            unsigned data_offset = get4 (is);
            is.seekg (data_offset, std::ios::beg);
        }
        switch (tag)
        {
            default:
                break;
            case 256:
                ifds[ifd].width = get4 (is);
                break;
            case 257:
                ifds[ifd].height = get4 (is);
                break;
            case 258:
                ifds[ifd].bps = get2 (is);
                break;
            case 259:
                ifds[ifd].comp = get2 (is);
                break;
            case 271:
                is.get (ifds[ifd].make, 64);
                break;
            case 272:
                is.get (ifds[ifd].model, 64);
                break;
            case 273:
            case 513:
                ifds[ifd].offset = get4 (is);
                break;
            case 277:
                ifds[ifd].samples = get2 (is) & 7;
                break;
            case 279:
            case 514:
                ifds[ifd].bytes = get4 (is);
                break;
            case 324:
                ifds[ifd].offset = count > 1 ? is.tellg () : static_cast<std::streampos> (get4 (is));
                break;
            case 330:
                while (count--)
                {
                    unsigned next_ifd = is.tellg () + static_cast<std::streampos> (4);
                    is.seekg (get4 (is), std::ios::beg);
                    parse_ifds (is, ifds);
                    is.seekg (next_ifd, std::ios::beg);
                }
                break;
        }
        is.seekg (next_tag_offset, std::ios::beg);
    }
}

void parse_header (std::istream &is)
{
    is.seekg (0, std::ios::beg);
    int order = get2 (is);
    if (order != 0x4d4d)
        throw std::runtime_error ("invalid byte order");
    int magic = get2(is);
    if (magic != 42)
        throw std::runtime_error ("invalid magic number");
    int first_offset = get4(is);
    //std::clog << "first_offset:\t" << first_offset << std::endl;
    is.seekg (first_offset, std::ios::beg);
}

}

}

#endif
