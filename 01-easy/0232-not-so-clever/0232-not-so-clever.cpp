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
#include <vector>


template <typename TIn, typename TOut>
auto
stupid_sort(TIn first, TIn last, TOut out, int iterations)
{
    while(iterations--) {
        bool dirty = false;
        for(auto one=first, two=std::next(first); two != last; one++, two++) {
            if(*two < *one) {
                std::iter_swap(one, two);
                dirty = true;
                break;
            }
        }
        if(not dirty)  // already sorted
            break;  // could be iterations *= dirty; (x * false = 0)
    }
    std::copy(first, last, out);
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    auto inlast = std::istream_iterator<int>();
    auto out = std::ostream_iterator<int>(std::cout, " ");

    std::vector<int> tosort(50);
    auto first = tosort.begin();

    while (stream) {
        auto infirst = std::istream_iterator<int>(stream);
        auto last = std::copy(infirst, inlast, first);
        if(first == last)
            break;  // no input

        // skip error (| separator)
        stream.clear(); char tmp; stream >> tmp;

        int iterations; stream >> iterations;

        stupid_sort(first, last, out, iterations);
        std::cout << std::endl;
    }
    return 0;
}
