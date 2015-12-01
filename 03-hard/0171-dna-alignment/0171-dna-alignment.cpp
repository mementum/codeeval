/*

  Copyright (C) 2015 Daniel Rodriguez

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>


#define MINUSINF SHRT_MIN

typedef int cell_t;
typedef std::vector<cell_t> mrow_t;

#define MATCH 3
#define MISMATCH -3
#define INDEL_START 8
#define INDEL_EXTENSION 1


int
nw_align_affine_gap4(const std::string &seqa, const std::string &seqb)
{
    // This extra optimized version saves no memory and gains no speed
    auto rowsize = seqa.length() + 1;
    auto colsize = seqb.length() + 1;
    auto msize = colsize * 2;  // holding only 2 rows
    auto fillsize = colsize * 1;

    // Create matrixes and initialize
    auto m = new cell_t[msize];
    std::fill_n(m, fillsize, MINUSINF);
    auto ix = new cell_t[msize];
    std::fill_n(ix, fillsize, MINUSINF);
    auto iy = new cell_t[msize];
    std::fill_n(iy, fillsize, MINUSINF);

    // Pointers to the 2 rows (will be swapped at start)
    auto m0 = &m[colsize];
    auto ix0 = &ix[colsize];
    auto iy0 = &iy[colsize];

    auto m1 = &m[0];
    auto ix1 = &ix[0];
    auto iy1 = &iy[0];

    // Init top-left corner and 1st iy row with gaps
    m1[0] = 0;

    for(auto j=1; j < colsize; j++)
        iy1[j] = -INDEL_START - (j - 1) * INDEL_EXTENSION;

    // Direct array access
    auto sac = seqa.c_str();
    auto sbc = seqb.c_str();

    for(auto i=1, i1=0; i < rowsize; ++i, i1=i - 1)
    {
        auto sa = sac[i1];

        std::swap(m0, m1);
        std::swap(ix0, ix1);
        std::swap(iy0, iy1);

        ix1[0] = -INDEL_START - i1 * INDEL_EXTENSION;  // initialize the gap
        m1[0] = MINUSINF;
        iy1[0] = MINUSINF;

        for(auto j=1, j1=0; j < colsize; ++j, j1=j - 1)
        {
            // Gaps on X
            ix1[j] = std::max(m0[j] - INDEL_START, ix0[j] - INDEL_EXTENSION);

            // Gaps on Y
            iy1[j] = std::max(m1[j1] - INDEL_START, iy1[j1] - INDEL_EXTENSION);

            // Matches/Mismatches
            auto mscore = (sa == sbc[j1]) ? MATCH : MISMATCH;
            m1[j] = std::max(m0[j1], std::max(ix0[j1], iy0[j1])) + mscore;
        }
    }

    auto retval = m1[colsize - 1];
    delete m;
    delete ix;
    delete iy;
    return retval;
}

#if 0
int
nw_align_affine_gap3(const std::string &seqa, const std::string &seqb,
                     const int &match=MATCH,
                     const int mismatch=MISMATCH,
                     const int &opening=INDEL_START,
                     const int &extension=INDEL_EXTENSION)
{
    auto rowsize = seqa.length() + 1;
    auto colsize = seqb.length() + 1;
    auto msize = colsize * 2;  // holding only 2 rows

    // Create matrixes
    auto m = new cell_t[msize];
    std::fill_n(m, msize, MINUSINF);
    auto ix = new cell_t[msize];
    std::fill_n(ix, msize, MINUSINF);
    auto iy = new cell_t[msize];
    std::fill_n(iy, msize, MINUSINF);

    // Init top-left corner and 1st iy row with gaps
    m[0] = 0;

    for(auto j=1; j < colsize; j++)
        iy[j] = -opening - (j - 1) * extension;

    auto m0 = &m[colsize];
    auto ix0 = &ix[colsize];
    auto iy0 = &iy[colsize];

    auto m1 = &m[0];
    auto ix1 = &ix[0];
    auto iy1 = &iy[0];

    for(auto i=1; i < rowsize; i++)
    {
        const auto &sa = seqa[i - 1];

        std::swap(m0, m1);
        std::swap(ix0, ix1);
        std::swap(iy0, iy1);

        m1[0] = MINUSINF;
        ix1[0] = -opening - (i - 1) * extension;  // initialize the gap
        iy1[0] = MINUSINF;

        for(auto j=1, j1=0; j < colsize; ++j, j1=j-1)
        {
            // Gaps on X
            auto mval = m0[j] - opening;
            auto ixval = ix0[j] - extension;
            ix1[j] = std::max(mval, ixval);

            // Gaps on Y
            mval = m1[j1] - opening;
            auto iyval = iy1[j1] - extension;
            iy1[j] = std::max(mval, iyval);

            // Matches/Mismatches
            auto mscore = (sa == seqb[j1]) ? match : mismatch;
            mval = m0[j1];
            ixval = ix0[j1];
            iyval = iy0[j1];
            m1[j] = std::max(mval, std::max(ixval, iyval)) + mscore;
        }
    }

    auto retval =  m[((rowsize - 1) % 2) * colsize + colsize - 1];
    delete m;
    delete ix;
    delete iy;
    return retval;
}


int
nw_align_affine_gap2(const std::string &seqa, const std::string &seqb,
                     const int &match=MATCH,
                     const int mismatch=MISMATCH,
                     const int &opening=INDEL_START,
                     const int &extension=INDEL_EXTENSION)
{
    auto rowsize = seqa.length() + 1;
    auto colsize = seqb.length() + 1;

    // Create matrixes
    auto m = std::vector<mrow_t>(2, mrow_t(colsize, MINUSINF));
    auto ix = std::vector<mrow_t>(2, mrow_t(colsize, MINUSINF));
    auto iy = std::vector<mrow_t>(2, mrow_t(colsize, MINUSINF));

    // Init top-left corner and 1st iy row with gaps
    m[0][0] = 0;

    for(auto j=1; j < colsize; j++)
        iy[0][j] = -opening - (j - 1) * extension;

    for(auto i=1; i < rowsize; i++)
    {
        const auto &sa = seqa[i - 1];

        const auto &m0 = m[(i - 1) % 2];
        auto &m1 = m[i % 2];
        const auto &ix0 = ix[(i - 1) % 2];
        auto &ix1 = ix[i % 2];
        const auto &iy0 = iy[(i - 1) % 2];
        auto &iy1 = iy[i % 2];

        m1[0] = MINUSINF;
        ix1[0] = -opening - (i - 1) * extension;  // initialize the gap
        iy1[0] = MINUSINF;

        for(auto j=1; j < colsize; j++)
        {
            auto j1 = j - 1;
            // Gaps on X
            auto mval = m0[j] - opening;
            auto ixval = ix0[j] - extension;
            ix1[j] = std::max(mval, ixval);

            // Gaps on Y
            mval = m1[j1] - opening;
            auto iyval = iy1[j1] - extension;
            iy1[j] = std::max(mval, iyval);

            // Matches/Mismatches
            auto mscore = (sa == seqb[j1]) ? match : mismatch;
            mval = m0[j1];
            ixval = ix0[j1];
            iyval = iy0[j1];
            m1[j] = std::max(mval, std::max(ixval, iyval)) + mscore;
        }
    }

    return m[(rowsize - 1) % 2][colsize - 1];
}


int
nw_align_affine_gap1(const std::string &seqa, const std::string &seqb,
                     const int &match=MATCH,
                     const int mismatch=MISMATCH,
                     const int &opening=INDEL_START,
                     const int &extension=INDEL_EXTENSION)
{
    const auto &rowsize = seqa.length() + 1;
    const auto &colsize = seqb.length() + 1;

    // Create matrixes
    auto m = std::vector<mrow_t>(2, mrow_t(colsize, MINUSINF));
    auto ix = std::vector<mrow_t>(2, mrow_t(colsize, MINUSINF));
    auto iy = std::vector<mrow_t>(2, mrow_t(colsize, MINUSINF));

    // Init top-left corner and 1st row
    m[0][0] = 0;

    for(auto j=1; j < colsize; j++)
        iy[0][j] = -opening - (j - 1) * extension;

    auto r0 = 0, r1 = 0;  // To still have r1 in scope out of the loop
    for(auto i=1; i < rowsize; i++)
    {
        const auto &sa = seqa[i - 1];
        const auto &m0 = m[(i - 1) % 2];
        const auto &m1 = m[i % 2];
        const auto &ix0 = ix[(i - 1) % 2];
        const auto &ix1 = ix[i % 2];
        const auto &iy0 = iy[(i - 1) % 2];
        const auto &iy1 = iy[i % 2];

        r0 = (i - 1) % 2;
        r1 = i % 2;

        ix[r1][0] = -opening - (i - 1) * extension;
        m[r1][0] = MINUSINF;
        iy[r1][0] = MINUSINF;

        for(auto j=1; j < colsize; j++)
        {
            auto j1 = j - 1;
            // Gaps on X
            auto mval = m[r0][j] - opening;
            auto ixval = ix[r0][j] - extension;
            ix[r1][j] = std::max(mval, ixval);

            // Gaps on Y
            mval = m[r1][j1] - opening;
            auto iyval = iy[r1][j1] - extension;
            iy[r1][j] = std::max(mval, iyval);

            // Matches/Mismatches
            auto mscore = (sa == seqb[j1]) ? match : mismatch;
            mval = m[r0][j1];
            ixval = ix[r0][j1];
            iyval = iy[r0][j1];
            m[r1][j] = std::max(mval, std::max(ixval, iyval)) + mscore;
        }
    }

    return m[r1][colsize - 1];
}
#endif

std::string
trim(std::string& str)
{
    auto first = str.find_first_not_of(' ');
    auto last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}


const std::vector<std::string>
tokenize(const std::string& s, const char& c)
{
    std::string buff{""};
    std::vector<std::string> v;

    for(auto n: s)
    {
        if(n != c)
            buff+=n;
        else
            if(n == c && buff != "")
            {
                v.push_back(buff);
                buff = "";
            }
    }

    if(buff != "")
        v.push_back(buff);

    return v;
}


std::istream&
safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a
    // std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}


int main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    // while (safeGetline(stream, line)) {
    while (std::getline(stream, line)) {
        // Do something with the line
        if (line == "")
            break;

        auto sequences = tokenize(line, '|');
        auto seqa = trim(sequences[0]);
        auto seqb = trim(sequences[1]);

        auto maxscore = nw_align_affine_gap4(seqa, seqb);
        std::cout << maxscore << std::endl;
    }
    return 0;
}
