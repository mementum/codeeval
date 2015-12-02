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

/*
  This challenge can be seen as a simple application of the Needleman-Wunsch
  algorithm in which restrictions are added:

    - Diagonals are not taken into account, which enables the calculation with
      a single row of data

    - The "score" to take into account is not that of the match of the 2
      sequences but rather the value of the cell

    - See: https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm
*/

// Headers for test case input
#include <fstream>
#include <iostream>
#include <string>


// Headers for the implementation
#include <algorithm>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>


///////////////////////////////////////////////////////////////////////////////
// Stream Imbuer for Parsing
///////////////////////////////////////////////////////////////////////////////
struct csv_reader: std::ctype<char> {
    csv_reader(): std::ctype<char>(get_table()) {}
    static std::ctype_base::mask const* get_table() {
        static std::vector<std::ctype_base::mask> rc(table_size, std::ctype_base::mask());

        rc[','] = std::ctype_base::space;
        rc['\n'] = std::ctype_base::space;
        rc[' '] = std::ctype_base::space;
        return &rc[0];
    }
};

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    // Imbue ss to parse csv automatically
    std::stringstream ss;
    ss.imbue(std::locale(std::locale(), new csv_reader()));

    const size_t inf = std::numeric_limits<size_t>::max();

    while(std::getline(stream, line)) {
        // Do something with the line
        auto msize = std::stoi(line) + 1;  // input size + 1 col/row extension

        // Initialize row
        auto r0 = std::vector<size_t>(msize, inf);

        // Loop m rows
        for(auto m=1; m < msize; m++) {
            std::getline(stream, line);
            ss.str(line); ss.clear();  // Must be cleared because it was eol (eof)
            // Loop n cols
            for(size_t n=1, val=0, cache=(m - 1) ? inf : 0; ss >> val; n++) {
                r0[n] = cache = std::min(cache, r0[n]) + val;
            }
        }
        // Minimum Path Sum is in the last row/col
        std::cout << r0[msize - 1] << std::endl;
    }
    return 0;
}
