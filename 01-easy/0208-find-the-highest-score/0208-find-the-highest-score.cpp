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
#include <algorithm>  // copy, stable_sort
#include <iterator>
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


#define OPTIMIZE 1

// By setting OPTIMIZE to 0, the algorithm will accept any matrix size (up to
// the limit of the numeric type of the machine)


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[])
{
    std::ifstream stream(argv[1]);  // imbue ensures \n will deliver an error
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" ")));
    auto out = std::ostream_iterator<int>(std::cout, " ");

    using ColT = std::vector<int>;

#if OPTIMIZE
    const auto MAX_ROWS = 10;
    const auto MAX_COLS = 20;
    auto m = std::vector<ColT>(MAX_COLS, ColT(MAX_ROWS));
#endif

    while(stream) {
#if not OPTIMIZE
        auto m = std::vector<ColT>{};
#endif
        // Read input directly in column format by interleaving input
        int rows = std::numeric_limits<int>::max();  // upper limit
        int cols = std::numeric_limits<int>::max();  // upper limit
        char token;
        for(auto i=0, j=0, input=0; i < rows; i++, j=0) {
            for(; j < cols && stream >> input; j++) {
#if OPTIMIZE
                m[j][i] = input;
#else
                if(not i)  // add a column vector
                    m.push_back(ColT{input});
                else
                    m[j].push_back(input);
#endif
            }
            cols = j;  // update matrix size in place to stop reading

            stream.clear();  // clear flag (non numeric token stopped input)
            if(stream >> token and token == '|')
               continue;  // stream is good and is row separator

            // End of matrix (eol or eof) save value (serves as out of loop too)
            rows = i + 1;
        }
        // find max and sent to output
        std::transform(
            m.begin(), std::next(m.begin(), cols), out,
            [rows](const ColT &col) {
                return *std::max_element(col.begin(), std::next(col.begin(), rows));
            });

        std::cout << std::endl;
    }

    return 0;
}
