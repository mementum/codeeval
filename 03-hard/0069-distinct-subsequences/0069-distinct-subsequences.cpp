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

// Headers for test case
#include <fstream>
#include <iostream>

// Headers for the implementation
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>


template<typename TIn, typename TOut>
auto
distinct_subsequences(TIn a1, TIn a2, TIn b1, TIn b2, TOut out)
{
    // calculate row size (make room for initial left value)
    auto colsize = std::distance(b1, b2) + 1;

    // initialize operation row and cache row
    auto r0 = std::vector<unsigned int>(colsize, 0);

    for(auto pa=a1; pa != a2; pa++) {
        auto &a = *pa;

        auto r0j1 = r0[0] = 1;  // cache current leftmost value
        auto i = 1;  // skip leftmost init value (colsize is 1 longer)
        for(auto pb=b1; pb != b2; pb++, i++) {
            auto cache = r0[i];
            r0[i] += r0j1 * (a == *pb);
            r0j1 = cache;
        }
    }
    *out++ = r0.back();  // send to output iterator
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    auto out = std::ostream_iterator<int>(std::cout, "\n");

    std::string line;
    while (std::getline(stream, line)) {
        // Do something with the line
        auto seppos = line.find_first_of(",");
        auto lbegin = line.begin();

        distinct_subsequences(
            lbegin, std::next(lbegin, seppos),            // String A
            std::next(lbegin, seppos + 1), line.end(),    // String B
            out);                                         // Output
    }
    return 0;
}
