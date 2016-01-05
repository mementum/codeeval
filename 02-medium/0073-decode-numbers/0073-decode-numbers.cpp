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
#include <iterator>
#include <string>


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
const auto ALPHABET = 26;

template <typename InT>
auto
count_codings(InT first, InT last)
{
    if(std::distance(first, last) < 2)
        return 1;

    auto second = std::next(first);

    auto codings = count_codings(second, last);
    auto cval = static_cast<int>(*first - '0') * 10 + static_cast<int>(*second - '0');

    if(cval <= ALPHABET)
        codings += count_codings(std::next(second), last);

    return codings;
}


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    std::string input;
    while(stream >> input)
        std::cout << count_codings(input.begin(), input.end()) << std::endl;

    return 0;
}
