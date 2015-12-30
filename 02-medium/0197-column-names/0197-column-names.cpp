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

    const auto ALPHALEN = 26;
    const auto OFFSET = 'A';

    auto in1 = std::istream_iterator<int>(stream);
    auto in2 = std::istream_iterator<int>();
    auto out = std::ostream_iterator<char>(std::cout);

    for(auto in=in1; in != in2; in++) {
        auto colnames = std::vector<char>();
        auto colnum = *in - 1;

        do {
            colnames.push_back(OFFSET + (colnum % ALPHALEN));
            colnum = colnum / ALPHALEN - 1;
        } while(colnum >= 0);

        std::copy(colnames.rbegin(), colnames.rend(), out);
        std::cout << std::endl;
    }

    return 0;
}
