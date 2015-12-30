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
// Stream Imbuer for Parsing
///////////////////////////////////////////////////////////////////////////////
struct SeparatorReader: std::ctype<char>
{
    template<typename T>
    SeparatorReader(T &&seps):
        std::ctype<char>(get_table(seps), true) {}

    template<typename T>
    std::ctype_base::mask const *get_table(T &&seps) {
        auto rc = new std::ctype_base::mask[std::ctype<char>::table_size]();
        for(auto &sep: seps)
            rc[static_cast<unsigned char>(sep)] = std::ctype_base::space;
        return &rc[0];
    }
};


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[])
{
    std::ifstream stream(argv[1]);  // imbue ensures \n will deliver an error
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" ")));

    // The problem has 100 lines, but the const allows keeping the solution generic
    const size_t INITIAL_SIZE = 100;
    auto &&v0 = std::vector<size_t>(INITIAL_SIZE, 0);
    auto &&v1 = std::vector<size_t>(INITIAL_SIZE, 0);

    auto &&in2 = std::istream_iterator<size_t>();
    size_t lines = 0U;
    while(stream && ++lines) {
        auto &&in1 = std::istream_iterator<size_t>(stream);

        if(lines > INITIAL_SIZE) {
            v1.push_back(0);  // Extend to make room for next input
            v0.push_back(0);
        }

        if(std::copy(in1, in2, v1.begin()) == v1.begin())
            break;  // nothing read (empty line)

        size_t cache = 0; // Left part of the max comparison
        for(auto vit1=v1.begin(), vit0=v0.begin(); vit1 != v1.end(); vit1++, vit0++) {
            *vit1 += std::max(cache, *vit0);
            cache = *vit0;
        }

        if(stream.eof())
            break;

        // Clear failbit and skip the eol which stopped input reading
        stream.clear();
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // The current sum row is now the input row for the incoming row
        std::swap(v0, v1);
    }

    std::cout << *std::max_element(v1.begin(), v1.end()) << std::endl;
    return 0;
}
