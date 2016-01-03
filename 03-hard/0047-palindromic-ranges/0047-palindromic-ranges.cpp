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


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    int first, last;
    while(stream >> first and stream >> last) {

        auto palins = std::vector<int>(last - first + 1);
        std::iota(palins.begin(), palins.end(), first);
        std::transform(
            palins.begin(), palins.end(), palins.begin(),
            [] (int p) -> int {
                int forward = p;
                int backwards = 0;

                while(forward) {
                    backwards = 10 * backwards + forward % 10;
                    forward /= 10;
                }
                return static_cast<int>(p == backwards);
        });

        auto size = static_cast<ssize_t>(palins.size());
        auto iranges = 0;

        auto pbegin = palins.begin();

        for(auto rangesize=0; rangesize < size; rangesize++) {
            for(auto col=0; col < size - rangesize; col++) {
                auto s = std::accumulate(std::next(pbegin, col),
                                         std::next(pbegin, col + rangesize + 1),
                                         0);
                iranges += not (s % 2);
            }
        }
        std::cout << iranges << std::endl;
    }

    return 0;
}
