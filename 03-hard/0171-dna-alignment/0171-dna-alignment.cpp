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

#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>

#include <climits>


///////////////////////////////////////////////////////////////////////////////
// Stream Imbuer for Parsing
// http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring
//    ss.imbue(std::locale(ss.getloc(), new SeparatorReader(" ,|")));
///////////////////////////////////////////////////////////////////////////////

struct SeparatorReader: std::ctype<char>
{
    template<typename T>
    SeparatorReader(const T &seps): std::ctype<char>(get_table(seps), true) {}

    template<typename T>
    std::ctype_base::mask const *get_table(const T &seps) {
        auto &&rc = new std::ctype_base::mask[std::ctype<char>::table_size]();
        for(auto &&sep: seps)
            rc[static_cast<unsigned char>(sep)] = std::ctype_base::space;
        return &rc[0];
    }
};

///////////////////////////////////////////////////////////////////////////////
// Iterator which iterates a wrapped istream_iterator until a given character
///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct istream_iterator_until {
    std::istream_iterator<T> iter;
    std::istream_iterator<T> last;
    T until;
    bool at_end;

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

    istream_iterator_until() : at_end(true) {}

    istream_iterator_until(std::istream &is, const T &until) :
        iter(is), last(), until(until), at_end(not is or *iter == until) {}

    T operator *() const { return *iter; }

    istream_iterator_until& operator ++() {
        ++iter;
        at_end = iter == last or *iter == until;
        return *this;
    }

    istream_iterator_until operator ++(int) {
        istream_iterator_until tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator !=(const istream_iterator_until &other) const {
        return not (*this == other);
    }

    bool operator ==(const istream_iterator_until &other) const {
        if(at_end)
            return other.at_end;

        if(other.at_end)
            return false;

        return iter == other.iter;
    }
};


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
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader("")));

    std::vector<char> seqs(400);  // 400 is problem limit
    auto first = seqs.begin();

    auto in2 = istream_iterator_until<char>();
    while (stream) {
        auto in1 = istream_iterator_until<char>(stream, '\n');
        auto last = std::copy(in1, in2, first);
        if(first == last)
            break;  // nothing read

        auto seqa_end = std::find(seqs.begin(), last, ' ');
        auto seqb_start = ++std::find(std::next(seqa_end), last, ' ');

        auto maxscore = nw_align_affine_gap(first, seqa_end, seqb_start, last);
        std::cout << maxscore << std::endl;
    }
    return 0;
}
