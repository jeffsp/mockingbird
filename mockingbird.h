/// @file mockingbird.h
/// @brief image utilities
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-05-19

#ifndef MOCKINGBIRD_H
#define MOCKINGBIRD_H

#include "version.h"
#include "cfa.h"
#include "tiff.h"
#include <horny_toad/horny_toad.h>
#include <jack_rabbit/jack_rabbit.h>
#include <iostream>
#include <fstream>
#include <set>

namespace mockingbird
{

/// @brief 16 bit grayscale image
typedef jack_rabbit::raster<unsigned short> image_gs16_t;

/// @brief convert an NEF format file to a Bayer image
///
/// @param ifs input stream
/// @param lfs log stream
///
/// @return the Bayer image
image_gs16_t nef2bayer (std::istream &ifs, std::ostream &lfs)
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
    image_gs16_t p (ROWS, COLS);
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

/// @brief convert bayer image to rgba
///
/// @param p 16 bit image
///
/// @return rgba image
image_gs16_t bayer2rgba (const image_gs16_t &p)
{
    image_gs16_t q (p.rows (), p.cols () * 4);

    // Convert from Bayer
    for (size_t i = 0; i < p.rows (); ++i)
    {
        for (size_t j = 0; j < p.cols (); ++j)
        {
            unsigned offset = (i&1) + (j&1);
            assert (offset < 3);
            q (i, j * 4 + offset) = p (i, j);
        }
    }
    return q;
}

/// @brief write a 16 bit grayscale image to a file
///
/// @param p image
/// @param ofs output file stream
/// @param lfs log stream
void write_gs16 (const image_gs16_t &p, std::ostream &ofs)
{
    // Write the header
    horny_toad::write_pnm_header (ofs, p.cols (), p.rows (), false, true);
    // Blast the whole buffer.  It is already in big endian order.
    ofs.write (reinterpret_cast<const char *> (&p[0]), static_cast<std::streamsize> (p.size () * 2));
}

};

#endif
