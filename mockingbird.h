/// @file mockingbird.h
/// @brief image utilities
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-19

#ifndef MOCKINGBIRD_H
#define MOCKINGBIRD_H

#include "version.h"
#include "tiff.h"
#include <horny_toad/horny_toad.h>
#include <jack_rabbit/jack_rabbit.h>
#include <iostream>
#include <fstream>
#include <set>

namespace mockingbird
{

/// @brief convert an NEF format file to a Bayer image
///
/// @param ifs input stream
/// @param lfs log stream
///
/// @return the Bayer image
jack_rabbit::raster<unsigned short> nef2bayer (std::istream &ifs, std::ostream &lfs)
{
    lfs << "reading stdin" << std::endl;
    // An NEF file is a TIFF format file, but libtiff 3.6.2 cannot read NEF
    // files...
    tiff::parse_header (ifs);
    std::vector<tiff::tiff_ifd> ifds;
    tiff::parse_ifds (ifs, ifds);
    // The first ifd contains a thumbnail
    lfs << "make:\t" << ifds[0].make << std::endl;
    lfs << "model:\t" << ifds[0].model << std::endl;
    // The second ifd contains a jpeg
    // The third ifd contains the RAW image
    lfs << "width:\t" << ifds[2].width << std::endl;
    lfs << "height:\t" << ifds[2].height << std::endl;
    lfs << "bps:\t" << ifds[2].bps << std::endl;
    lfs << "image offset:\t" << ifds[2].offset << std::endl;
    const size_t ROWS = 2844;
    const size_t COLS = 4284;
    if (ifds[2].width != COLS + 4)
        throw std::runtime_error ("unexpected width");
    if (ifds[2].height != ROWS)
        throw std::runtime_error ("unexpected height");
    // Skip to the data
    ifs.seekg (ifds[2].offset, std::ios::beg);
    jack_rabbit::raster<unsigned short> p (ROWS, COLS);
    lfs << "reading image" << std::endl;
    for (size_t i = 0; i < ROWS; ++i)
    {
        // Skip left margin
        ifs.seekg (4, std::ios::cur);
        // Read a row
        ifs.read (reinterpret_cast<char *> (&p (i, 0)), COLS * 2);
        if (!ifs)
            throw std::runtime_error ("could not read file");
        // Skip right margin
        ifs.seekg (4, std::ios::cur);
    }
    return p;
}

/// @brief write a bayer image to a file
///
/// @param p Bayer image
/// @param ofs output file stream
/// @param lfs log stream
void write_bayer (const jack_rabbit::raster<unsigned short> &p, std::ostream &ofs, std::ostream &lfs)
{
    lfs << "writing image" << std::endl;
    // Write the header
    horny_toad::write_pnm_header (ofs, p.cols (), p.rows (), false, true);
    // Blast the whole buffer.  It is already in big endian order.
    ofs.write (reinterpret_cast<const char *> (&p[0]), static_cast<std::streamsize> (p.size () * 2));
}

};

#endif
