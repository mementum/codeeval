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
#include <map>
#include <vector>


///////////////////////////////////////////////////////////////////////////////
// Output debugging
///////////////////////////////////////////////////////////////////////////////
template<typename T>
void debugout(const T &t)
{
    std::cout << t << std::endl;
}

template<typename T>
void debugout(const std::vector<T> &v)
{
    std::cout << "{";
    if(v.size()) {
        auto last = std::prev(v.end(), 1);
        for(auto &&p=v.begin(); p != last; p++)
            std::cout << *p << ",";

        std::cout << *last;
    }
    std::cout << "}" << std::endl;
}

template<typename T, typename... Args>
void debugout(const T &t, Args... args) // recursive variadic function
{
    std::cout << t;
    debugout(args...);
}

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
    // Ensure '\n' is not ws and breaks reading ints
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" ")));

    auto in2 = std::istream_iterator<int>{};

    while(stream) {
        auto teamfans = std::map<int, std::vector<int>>{};

        char sep = '\0';
        for(auto i = 1; stream and sep != '\n'; i++) {
            auto in1 = std::istream_iterator<int>{stream};
            for(auto in=in1; in != in2; in++)
                teamfans[*in].push_back(i);

            stream.clear(); stream >> sep;
        }

        auto i = 0;
        for(auto tf=teamfans.begin(); tf != teamfans.end(); tf++, i++) {
            std::cout <<(i ? " " : "") << tf->first << ':';

            std::sort(tf->second.begin(), tf->second.end());
            auto j = 0;
            for(auto f=tf->second.begin(); f != tf->second.end(); f++, j++)
                std::cout << (j ? "," : "") << *f;

            std::cout << ';';
        }
        std::cout << std::endl;
    }
    return 0;
}
