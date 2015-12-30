h/*
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
#include <string>

// Headers for the implementation
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

template <typename iter, typename iterout>
auto
bubblesort_interruptus(iter a1, iter a2, iterout out, size_t iterations)
{
    using itertype = typename std::decay<decltype(*a1)>::type;

    for(auto i=0; i < iterations; i++) {
        bool ops = false;
        for(auto a=a1, b=std::next(a1); b != a2; a++, b++) {
            if(*a > *b) {
                ops = true;
                std::iter_swap(a, b);
            }
        }
        if(not ops)
            break;
    }
    std::copy(a1, a2, out);
}


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
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" |")));

    auto &&ostr = std::ostream_iterator<size_t>(std::cout, " ");
    auto &&istr2 = std::istream_iterator<size_t>();

    auto &&bubbles = std::vector<size_t>{};
    while(stream) {
        bubbles.clear();

        // Copy all input to size_t until conversion error ('\n' or eof seen)
        auto &&istr1 = std::istream_iterator<size_t>(stream);
        std::copy(istr1, istr2, std::back_inserter(bubbles));

        if(not bubbles.empty()) { // Do only work when there is content
            // Prepare input limits and get max number of iterations
            auto in1 = bubbles.begin();
            auto in2 = std::prev(bubbles.end());
            auto iterations = *in2;

            // Execute the interrupted bubblesort
            bubblesort_interruptus(in1, in2, ostr, iterations);
            std::cout << std::endl;  // separate testcases
        }

        if(not stream.eof()) {
            // Clear failbit and skip the eol which stopped input reading
            stream.clear();
            stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}
