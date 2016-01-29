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

// Headers for test case input/output
#include <fstream>
#include <iostream>

// Headers for the implementation
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>


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


template <typename TIn, typename TOut>
auto
comb_sort(TIn first, TIn last, TOut out, float shrink=1.25)
{
    auto gap = std::distance(first, last);
    auto swaps = true;  // force at least 1 pass over the array

    auto iterations = -1;
    while(gap > 1 or swaps) {
        iterations++;

        gap /= shrink;
        if(gap < 1)
            gap = 1;

        swaps = false;
        for(auto e1=first, e2=std::next(e1, gap); e2 < last; e1++, e2++) {
            if(*e2 < *e1) {
                std::iter_swap(e1, e2);
                swaps = true;
            }
        }
    }
    *out++ = iterations;
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" ")));

    auto in2 = std::istream_iterator<int>();
    auto out = std::ostream_iterator<int>(std::cout, "\n");

    // Ideally we'd allocate the vector each time and use a back_inserter
    // when copying from istream_iterator, but this suffices for the challenge
    std::vector<int> tosort(100);
    auto first = tosort.begin();

    while(stream) {
        auto in1 = std::istream_iterator<int>(stream);  // redo after each clear
        // Do something with the line
        auto last = std::copy(in1, in2, first);
        if(first == last)
            break;  // nothing read -> eof

        comb_sort(first, last, out); // do the thing

        // clear status and skip eol which stopped int conversion
        stream.clear(); char eol; stream >> eol;
    }
    return 0;
}
