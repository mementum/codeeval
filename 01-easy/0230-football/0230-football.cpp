/*
  Copyright (C) 2016 Daniel Rodriguez

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
#include <tuple>
#include <vector>


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
// Use a vector of tuples approach to avoid having a map<int, vector> which is
// a lot more expensive, given prealloc is not possible, whereas a single
// vector of tuples can be preallocated and reused by copying/transforming each
// time from the beginning (since we get the last item as result)

int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    // Ensure '\n' is not ws and breaks reading ints as '|' does
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" ")));
    auto in2 = std::istream_iterator<int>{};

    auto teamfans = std::vector<std::tuple<int, int>>(100);
    auto tffirst = teamfans.begin();

    while(stream) {
        // run over the "fans" and keep a (team, fan) reference
        // transform instead of copy to create the pair
        // fans are 1 based
        auto tflast = tffirst;  // make sure we copy at the beginning
        auto sep = '\0';
        for(auto f=1; stream and sep != '\n'; f++, stream.clear(), stream >> sep) {
            auto in1 = std::istream_iterator<int>{stream};
            tflast = std::transform(
                in1, in2, tflast,
                [f](const int t) { return std::make_tuple(t, f); });
        }
        // now tflast points to the last element added to the vector

        std::sort(tffirst, tflast);  // the output is sorted team/fan

        auto lastteam = -1, iteam = 0, ifan = 0;
        for(auto tf=tffirst; tf != tflast; tf++) {
            std::tie(iteam, ifan) = *tf;  // unpack the tuple

            if(iteam != lastteam) {
                lastteam = iteam;
                std::cout << (tf != tffirst ? "; " : "") << iteam << ':' << ifan;
            } else
                std::cout << ',' << ifan;
        }
        std::cout << ';' << std::endl;  // complete the output
    }
    return 0;
}
