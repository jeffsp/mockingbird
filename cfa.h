// jsp Fri Jan  7 15:00:37 CST 2011

#ifndef CFA_H
#define CFA_H

#include <limits.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <cassert>
#include <limits>
#include <jack_rabbit/jack_rabbit.h>
#include <horny_toad/horny_toad.h>

namespace mockingbird
{

// Portions extracted from Dave Coffin's dcraw.c:

#define FC(row,col) (filters >> ((((row) << 1 & 14) + ((col) & 1)) << 1) & 3)
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define LIM(x,min,max) MAX(min,MIN(x,max))
#define ULIM(x,y,z) ((y) < (z) ? LIM(x,y,z) : LIM(x,z,y))
#define CLIP(x) LIM(x,0,65535)
#define FORC(cnt) for (c=0; c < cnt; c++)
#define FORCC FORC(colors)
#define FORC3 FORC(3)
#define FORC4 FORC(4)
#define ABS(x) (((int)(x) ^ ((int)(x) >> 31)) - ((int)(x) >> 31))
#define SQR(x) ((x)*(x))

#define TS 256		/* Tile Size */

namespace
{
    const int colors = 3;
    const double xyz_rgb[3][3] = {
      { 0.412453, 0.357580, 0.180423 },
      { 0.212671, 0.715160, 0.072169 },
      { 0.019334, 0.119193, 0.950227 } };
    const float d65_white[3] = { 0.950456, 1, 1.088754 };

    // Only valid for Nikon D700:
    const float rgb_cam[3][4] = {{ 1.66942763, -0.484730721, -0.184696957, 0}, {-0.000960246602, 1.25266957, -0.251709342, 0}, {0.0471420474, -0.300088346, 1.25294626, 0 }};

    unsigned short width;
    unsigned short height;
    unsigned short (*image)[4];

    const size_t filters = 0x94949494;

    int fc (int row, int col)
    {
      return FC(row,col);
    }

    void border_interpolate (int border)
    {
      unsigned row, col, y, x, f, c, sum[8];

      for (row=0; row < height; row++)
        for (col=0; col < width; col++) {
          if (int(col)==border && int(row) >= border && int(row) < height-border)
        col = width-border;
          memset (sum, 0, sizeof sum);
          for (y=row-1; y != row+2; y++)
        for (x=col-1; x != col+2; x++)
          if (y < height && x < width) {
            f = fc(y,x);
            sum[f] += image[y*width+x][f];
            sum[f+4]++;
          }
          f = fc(row,col);
        for (c=0; int(c) < colors; c++)
          if (c != f && sum[c+4])
        image[row*width+col][c] = sum[c] / sum[c+4];
        }
    }
}

/*
   Adaptive Homogeneity-Directed interpolation is based on
   the work of Keigo Hirakawa, Thomas Parks, and Paul Lee.
 */
void ahd_interpolate()
{
    int i, j, k, top, left, row, col, tr, tc, c, d, val, hm[2];
    ushort (*pix)[4], (*rix)[3];
    static const int dir[4] = { -1, 1, -TS, TS };
    unsigned ldiff[2][4], abdiff[2][4], leps, abeps;
    float r, cbrt[0x10000], xyz[3], xyz_cam[3][4];
    ushort (*rgb)[TS][TS][3];
    short (*lab)[TS][TS][3], (*lix)[3];
    char (*homo)[TS][TS], *buffer;

    for (i=0; i < 0x10000; i++) {
        r = i / 65535.0;
        cbrt[i] = r > 0.008856 ? pow(r,1/3.0) : 7.787*r + 16/116.0;
    }
    for (i=0; i < 3; i++)
        for (j=0; j < colors; j++)
            for (xyz_cam[i][j] = k=0; k < 3; k++)
                xyz_cam[i][j] += xyz_rgb[i][k] * rgb_cam[k][j] / d65_white[i];

    border_interpolate(5);
    buffer = (char *) malloc (26*TS*TS);		/* 1664 kB */
    rgb  = (ushort(*)[TS][TS][3]) buffer;
    lab  = (short (*)[TS][TS][3])(buffer + 12*TS*TS);
    homo = (char  (*)[TS][TS])   (buffer + 24*TS*TS);

    for (top=2; top < height-5; top += TS-6)
        for (left=2; left < width-5; left += TS-6) {

            /*  Interpolate green horizontally and vertically:		*/
            for (row = top; row < top+TS && row < height-2; row++) {
                col = left + (FC(row,left) & 1);
                for (c = FC(row,col); col < left+TS && col < width-2; col+=2) {
                    pix = image + row*width+col;
                    val = ((pix[-1][1] + pix[0][c] + pix[1][1]) * 2
                            - pix[-2][c] - pix[2][c]) >> 2;
                    rgb[0][row-top][col-left][1] = ULIM(val,pix[-1][1],pix[1][1]);
                    val = ((pix[-width][1] + pix[0][c] + pix[width][1]) * 2
                            - pix[-2*width][c] - pix[2*width][c]) >> 2;
                    rgb[1][row-top][col-left][1] = ULIM(val,pix[-width][1],pix[width][1]);
                }
            }
            /*  Interpolate red and blue, and convert to CIELab:		*/
            for (d=0; d < 2; d++)
                for (row=top+1; row < top+TS-1 && row < height-3; row++)
                    for (col=left+1; col < left+TS-1 && col < width-3; col++) {
                        pix = image + row*width+col;
                        rix = &rgb[d][row-top][col-left];
                        lix = &lab[d][row-top][col-left];
                        if ((c = 2 - FC(row,col)) == 1) {
                            c = FC(row+1,col);
                            val = pix[0][1] + (( pix[-1][2-c] + pix[1][2-c]
                                        - rix[-1][1] - rix[1][1] ) >> 1);
                            rix[0][2-c] = CLIP(val);
                            val = pix[0][1] + (( pix[-width][c] + pix[width][c]
                                        - rix[-TS][1] - rix[TS][1] ) >> 1);
                        } else
                            val = rix[0][1] + (( pix[-width-1][c] + pix[-width+1][c]
                                        + pix[+width-1][c] + pix[+width+1][c]
                                        - rix[-TS-1][1] - rix[-TS+1][1]
                                        - rix[+TS-1][1] - rix[+TS+1][1] + 1) >> 2);
                        rix[0][c] = CLIP(val);
                        c = FC(row,col);
                        rix[0][c] = pix[0][c];
                        xyz[0] = xyz[1] = xyz[2] = 0.5;
                        FORCC {
                            xyz[0] += xyz_cam[0][c] * rix[0][c];
                            xyz[1] += xyz_cam[1][c] * rix[0][c];
                            xyz[2] += xyz_cam[2][c] * rix[0][c];
                        }
                        xyz[0] = cbrt[CLIP((int) xyz[0])];
                        xyz[1] = cbrt[CLIP((int) xyz[1])];
                        xyz[2] = cbrt[CLIP((int) xyz[2])];
                        lix[0][0] = 64 * (116 * xyz[1] - 16);
                        lix[0][1] = 64 * 500 * (xyz[0] - xyz[1]);
                        lix[0][2] = 64 * 200 * (xyz[1] - xyz[2]);
                    }
            /*  Build homogeneity maps from the CIELab images:		*/
            memset (homo, 0, 2*TS*TS);
            for (row=top+2; row < top+TS-2 && row < height-4; row++) {
                tr = row-top;
                for (col=left+2; col < left+TS-2 && col < width-4; col++) {
                    tc = col-left;
                    for (d=0; d < 2; d++) {
                        lix = &lab[d][tr][tc];
                        for (i=0; i < 4; i++) {
                            ldiff[d][i] = ABS(lix[0][0]-lix[dir[i]][0]);
                            abdiff[d][i] = SQR(lix[0][1]-lix[dir[i]][1])
                                + SQR(lix[0][2]-lix[dir[i]][2]);
                        }
                    }
                    leps = MIN(MAX(ldiff[0][0],ldiff[0][1]),
                            MAX(ldiff[1][2],ldiff[1][3]));
                    abeps = MIN(MAX(abdiff[0][0],abdiff[0][1]),
                            MAX(abdiff[1][2],abdiff[1][3]));
                    for (d=0; d < 2; d++)
                        for (i=0; i < 4; i++)
                            if (ldiff[d][i] <= leps && abdiff[d][i] <= abeps)
                                homo[d][tr][tc]++;
                }
            }
            /*  Combine the most homogenous pixels for the final result:	*/
            for (row=top+3; row < top+TS-3 && row < height-5; row++) {
                tr = row-top;
                for (col=left+3; col < left+TS-3 && col < width-5; col++) {
                    tc = col-left;
                    for (d=0; d < 2; d++)
                        for (hm[d]=0, i=tr-1; i <= tr+1; i++)
                            for (j=tc-1; j <= tc+1; j++)
                                hm[d] += homo[d][i][j];
                    if (hm[0] != hm[1])
                        FORC3 image[row*width+col][c] = rgb[hm[1] > hm[0]][tr][tc][c];
                    else
                        FORC3 image[row*width+col][c] =
                            (rgb[0][tr][tc][c] + rgb[1][tr][tc][c]) >> 1;
                }
            }
        }
    free (buffer);
}

#undef TS

// end dcraw.c portions

template<typename T>
T convert_to_rgba (const T &p)
{
    jack_rabbit::raster<unsigned short> q (p.rows (), p.cols () * 4);

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

// Only valid for 14 bit NikonD700
const float MULTIPLIERS[3] = {8.8595705, 4.00018311, 4.77283669};

template<typename T>
T scale_image (const T &p)
{
    jack_rabbit::raster<unsigned short> q (p.rows (), p.cols ());
    for (size_t i = 0; i < p.size (); i += 4)
    {
        q[i + 0] = CLIP (p[i + 0] * mockingbird::MULTIPLIERS[0]);
        q[i + 1] = CLIP (p[i + 1] * mockingbird::MULTIPLIERS[1]);
        q[i + 2] = CLIP (p[i + 2] * mockingbird::MULTIPLIERS[2]);
    }
    return q;
}

template<typename T>
T convert_to_rgb (const T &p)
{
    jack_rabbit::raster<unsigned short> q (p.rows (), p.cols () * 3 / 4);

    // Get rid of alpha channel
    for (size_t i = 0; i < q.rows (); ++i)
    {
        for (size_t j = 0; j < q.cols () / 3; ++j)
        {
            q (i, j * 3 + 0) = p (i, j * 4 + 0);
            q (i, j * 3 + 1) = p (i, j * 4 + 1);
            q (i, j * 3 + 2) = p (i, j * 4 + 2);
        }
    }

    return q;
}

template<typename T>
T convert_to_sRGB (const T &p)
{
    jack_rabbit::raster<unsigned short> q (p.rows (), p.cols ());

    // Only valid for NikonD700
    float srgb[3][3] = {
        {1.66942763, -0.484730721, -0.184696957},
        {-0.000960246602, 1.25266957, -0.251709342},
        {0.0471420474, -0.300088346, 1.25294626}};

    for (size_t i = 0; i < p.size (); i += 3)
    {
        int r = p[i + 0];
        int g = p[i + 1];
        int b = p[i + 2];
        q[i + 0] = CLIP (r * srgb[0][0] + g * srgb[0][1] + b * srgb[0][2]);
        q[i + 1] = CLIP (r * srgb[1][0] + g * srgb[1][1] + b * srgb[1][2]);
        q[i + 2] = CLIP (r * srgb[2][0] + g * srgb[2][1] + b * srgb[2][2]);
    }
    return q;
}

template<typename T>
double get_normalize_constant (T &p, float percent)
{
    //clog << "pmax " << *max_element (p.begin (), p.end ()) << endl;
    size_t greatest_pmax = 0;
    for (size_t chan = 0; chan < 3; ++chan)
    {
        // Create histogram
        std::vector<size_t> h (65536);
        for (size_t i = 0; i < p.size (); i += 3)
            ++h[p[i + chan]];
        // Find value that clips off top (100-percent)
        size_t sum = 0;
        size_t pmax = 65535;
        for (size_t i = 0; i < h.size (); ++i)
        {
            if ((sum += h[i]) > p.size () * percent / 100.0 / 3)
            {
                pmax = i;
                break;
            }
        }
        //clog << "channel " << chan << " pmax at " << percent << "% " << pmax << endl;
        if (pmax > greatest_pmax)
            greatest_pmax = pmax;
    }
    //std::clog << "greatest pmax at " << percent << "% " << greatest_pmax << std::endl;
    // dcraw does this for some reason
    greatest_pmax &= 0xFFF8;
    //clog << "using pmax of " << greatest_pmax << endl;
    return 65535.0 / greatest_pmax;
}

template<typename T>
T normalize (T &p, float percent)
{
    const float C = mockingbird::get_normalize_constant (p, percent);
    for (size_t i = 0; i < p.size (); ++i)
        p[i] = CLIP (p[i] * C);
    return p;
}

template<typename T>
T bayer2rgb (unsigned channel, const T &p)
{
    // Extract the pixels
    T q (p.rows () / 2, p.cols () / 2);
    for (size_t i = 0; i < p.rows (); i += 2)
    {
        for (size_t j = 0; j < p.cols (); j += 2)
        {
            // |-------|   |-------|
            // | 0 | 1 |   | R | G |
            // |-------| = |-------|
            // | 2 | 3 |   | G | B |
            // |-------|   |-------|
            assert (channel < 4);
            q (i / 2, j / 2) = p (i + channel/2, j + (channel&1));
        }
    }
    return q;
}

template<typename T>
T rgb2bayer (const T &r, const T &g1, const T &g2, const T &b)
{
    assert (r.rows () == g1.rows () && r.cols () == g1.cols ());
    assert (r.rows () == g2.rows () && r.cols () == g2.cols ());
    assert (r.rows () == b.rows () && r.cols () == b.cols ());
    // Combine the pixels
    T q (r.rows () * 2, r.cols () * 2);
    for (size_t i = 0; i < q.rows (); i += 2)
    {
        for (size_t j = 0; j < q.cols (); j += 2)
        {
            // |---------|
            // | R  | G1 |
            // |---------|
            // | G2 | B  |
            // |---------|
            q (i + 0, j + 0) = r (i / 2, j / 2);
            q (i + 0, j + 1) = g1 (i / 2, j / 2);
            q (i + 1, j + 0) = g2 (i / 2, j / 2);
            q (i + 1, j + 1) = b (i / 2, j / 2);
        }
    }
    return q;
}

}

#endif
