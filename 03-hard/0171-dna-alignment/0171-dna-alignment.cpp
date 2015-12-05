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
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <climits>
#include <algorithm>


// Defined so to avoid overflow if it were defined as INT_MIN
// or std::numeric_limits<cell_t>::min()
#define MINUSINF SHRT_MIN

typedef int cell_t;
typedef std::vector<cell_t> mrow_t;


#define MATCH 3
#define MISMATCH -3
#define INDEL_START 8
#define INDEL_EXTENSION 1


template<typename iter>
int
nw_align_affine_gap(iter a1, iter a2, iter b1, iter b2)
{
    // This extra optimized version saves no memory and gains no speed
    auto colsize = std::distance(b1, b2) + 1;

    auto m = std::vector<cell_t>(colsize, MINUSINF);
    auto ix = std::vector<cell_t>(colsize, MINUSINF);
    auto iy = std::vector<cell_t>(colsize, MINUSINF);

    // Init top-left corner and 1st iy row with gaps
    m[0] = 0;
    for(auto j=1; j < colsize; j++)
        iy[j] = -INDEL_START - (j - 1) * INDEL_EXTENSION;

    // for(auto i=1, i1=0; i < rowsize; ++i, i1=i - 1)
    auto i = 0;
    for(auto a=a1; a != a2; a++, i++) {
        auto sa = *a;

        // Cache values to enable 1-row calculation
        auto m0j1 = m[0];
        auto ix0j1 = ix[0];
        auto iy0j1 = iy[0];

        ix[0] = -INDEL_START - i * INDEL_EXTENSION;  // initialize the gap
        m[0] = MINUSINF;
        iy[0] = MINUSINF;

        // Extra cache variables for 1-row calculation
        auto m1j1 = m[0];
        auto iy1j1 = iy[0];

        auto j = 1;
        for(auto b=b1; b != b2; b++, j++) {
            // Gaps on X
            auto ix1j = std::max(m[j] - INDEL_START, ix[j] - INDEL_EXTENSION);

            // Gaps on Y
            iy1j1 = std::max(m1j1 - INDEL_START, iy1j1 - INDEL_EXTENSION);

            // Matches/Mismatches
            auto mscore = (sa == *b) ? MATCH : MISMATCH;
            m1j1 = std::max(m0j1, std::max(ix0j1, iy0j1)) + mscore;

            // Update cache variables and row values
            ix0j1 = ix[j];
            ix[j] = ix1j;

            iy0j1 = iy[j];
            iy[j] = iy1j1;

            m0j1 = m[j];
            m[j] = m1j1;
        }
    }

    return m[colsize - 1];
}


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while (std::getline(stream, line)) {
        // Do something with the line
        auto seqa_end = line.find_first_of(" |");
        auto seqb_start = line.find_first_not_of(" |", seqa_end);

        auto lbegin = line.begin();

        auto maxscore = nw_align_affine_gap(
            lbegin, std::next(lbegin, seqa_end),          // Seqa
            std::next(lbegin, seqb_start), line.end());   // Seqb

        std::cout << maxscore << std::endl;
    }
    return 0;
}
