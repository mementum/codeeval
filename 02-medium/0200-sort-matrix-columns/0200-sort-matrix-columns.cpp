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

    using TRow = std::vector<int>;

#if OPTIMIZE
    const auto MAX_DIMENSION = 15;
    auto m = std::vector<TRow>(MAX_DIMENSION, TRow(MAX_DIMENSION));
#endif

    while(stream) {
#if not OPTIMIZE
        auto m = std::vector<TRow>{};
#endif

        int msize = std::numeric_limits<int>::max();  // upper limit
        char token;
        for(auto j=0, i=0, input=0; j < msize; i=0, j++) {
            for(; i < msize && stream >> input; i++) {
#if OPTIMIZE
                m[i][j] = input;
#else
                if(not j)
                    m.push_back(TRow{input});
                else
                    m[i].push_back(input);
#endif
            }
            msize = i;  // update matrix size in place to stop reading

            stream.clear();  // clear flag (non numeric token stopped input)
            stream >> token;  // skip non numeric token (eol or |)
        }
        if(not msize)
            break;  // eof found

#if not OPTIMIZE
        std::sort(m.begin(), std::next(m.begin(), msize));
#else
        // Compare vectors only down to the defined size
        std::sort(
            m.begin(), std::next(m.begin(), msize),
            [msize] (const TRow &lhs, const TRow &rhs) -> bool {
                return std::lexicographical_compare(
                    lhs.begin(), std::next(lhs.begin(), msize),
                    rhs.begin(), std::next(rhs.begin(), msize));
            }
            );
#endif


        // output interleaving columns
        for(auto j=0; j < msize; j++) {
            if(j)
                std::cout << "| ";
            for(auto i=0; i < msize; i++)
                std::cout << m[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
