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
#include <vector>

template <typename InT, typename OutT>
auto
following_integer(InT first, InT last, OutT out)
{
    using InType = typename std::iterator_traits<InT>::value_type;
    auto rlast = std::reverse_iterator<InT>(last);
    auto rfirst = std::reverse_iterator<InT>(first);

    // in the 1st part we try to move one digit forward to make the figure
    // greater and then sort the lower part to have the mininum possible value
    for(auto start=rlast, target=std::next(rlast); target != rfirst; start++, target++) {
        auto mingreat = std::numeric_limits<InType>::max();
        auto minit = target;

        // find the min greater than target, to do a swap
        for(auto check=rlast; check != target; check++) {
            if(*check > *target and *check < mingreat) {
                minit = check;
                mingreat = *check;
            }
        }

        if(minit == target)
            continue;  // no min greater continue search

        std::iter_swap(minit, target);  // found: swap values

        // find out how far away from the beginning we stopped
        auto d = std::distance(target, rfirst);
        std::sort(std::next(first, d), last);  // sort only the lower part
        std::copy(first, last, out);  // send to output iterator
        return;
    }

    // no movement of digits is possible to make it larger. Need to get lowest
    // ordering and insert a 2nd left most zero. The lowest ordering is the one
    // with a significant digit (non zero) at the left most part

    std::sort(first, last);  // lower value
    // find first nonzero
    auto nonzero = std::find_if(first, last, [](const InType &a) { return a;});
    std::iter_swap(first, nonzero);  // swap values

    *out++ = *first;  // output now lower
    *out++ = not bool(*first);  // output the zero, calculate it
    std::copy(std::next(first), last, out);  // copy the rest out
}

auto
int_range(int number)
{
    // receive an int an return and return a range with the digits
    const auto MAX_POSITIONS = 6;
    static std::vector<int> vnumber(MAX_POSITIONS);  // n < 10^6
    auto vbegin = vnumber.begin();

    auto i = 0;
    while(number) {
        vnumber[i++] = number % 10;
        number /= 10;
    }

    auto vend = std::next(vbegin, i);
    std::reverse(vbegin, vend);
    return std::make_pair(vbegin, vend);
}


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    auto out = std::ostream_iterator<int>(std::cout);

    int number;
    while(stream >> number) {
        auto range = int_range(number);
        following_integer(range.first, range.second, out);
        std::cout << std::endl;

    }
    return 0;
}
