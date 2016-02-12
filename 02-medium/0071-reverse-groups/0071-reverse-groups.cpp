/*
  Copyright (C) 2015, 2016 Daniel Rodriguez

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
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(",\n")));

    auto in2 = std::istream_iterator<int>();

    int k;
    auto l = std::vector<int>(100);
    auto lfirst = l.begin();

    char sep;
    while(stream) {
        auto in1 = std::istream_iterator<int>(stream);

        auto llast = std::copy(in1, in2, lfirst);
        auto lsize = std::distance(lfirst, llast);
        stream.clear(); stream >> sep;  // clean an skip ';'

        stream >> k; // get last int
        auto rem = lsize % k;

        for(auto i=0; i < lsize - rem; i += k)  // reverse k at a time
            std::reverse(std::next(lfirst, i), std::next(lfirst, i + k));

        for(auto lit=lfirst; lit != llast; lit++)  // output
            std::cout << (lit != lfirst ? "," : "" ) << *lit;

        std::cout << std::endl;
    }
    return 0;
}
