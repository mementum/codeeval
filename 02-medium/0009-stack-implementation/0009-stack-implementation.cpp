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
    // Only " " is ws, \n stops the input of ints
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" ")));

    auto last = std::istream_iterator<int>();
    auto out = std::ostream_iterator<int>(std::cout, " ");
    auto stack = std::vector<int>(100);

    char eol;
    while (stream) {
        stack.resize(0);
        auto first = std::istream_iterator<int>(stream);
        std::copy(first, last, std::back_inserter(stack));
        if(stack.empty())
            break;

#if 1
        auto i = 0;
        std::copy_if(stack.rbegin(), stack.rend(), out,
                     [&i](const int a) { return not (i++ % 2); });
#else
        // This two generate a memory access error (at least with GCC 4.8.1,
        // because the reverse iterator may go 1 past the beginning of the
        // array
        // The logic should return "rend" if past that point, but it doesn't
        // The alternative is to have a counter i and skip each 2nd one

        // for(auto in=stack.rbegin(); in != stack.rend(); in += 2)
        for(auto in=stack.rbegin(); in != stack.rend(); std::advance(in, 2))
            *out++ = *in;
#endif
        std::cout << std::endl;

        stream.clear();  // eol or eof stopped int input conversion
        stream >> eol;  // skip eol or meet eof
    }
    return 0;
}
