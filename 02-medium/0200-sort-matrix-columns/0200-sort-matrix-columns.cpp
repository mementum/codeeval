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
#include <iterator>  // istream_iterator, ostream_iterator
#include <map>  // multimap
#include <math.h>  // sqrt
#include <type_traits>
#include <vector>

#include <cstdint>  // int8_t

/*
  Loosely defined challenge, because whether sorting should be stable or not
  (order preserving) is not stated. The sample from the description seems to
  show that the last of the "min" values is the one taken into account.

  Like when using std::min_elemeent, which returns the last "min" because it
  has to traverse the range before knowing which one is the last.

  That's why the insertion in the map has to be done looking at the row (vector
  range) values in reverse order, to make sure last comes first.

  The problem is really about how to sort several vectors based on the initial
  ordering of 1 vector.
*/

using MATRIXVAL_T = ssize_t;


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


template<typename TIter, typename TOut, typename TOutSep>
auto
column_sort(TIter first, TIter last, TOut out, TOutSep outsep)
{
    using ValT = typename std::decay<decltype(*first)>::type;
    std::multimap<ValT, size_t> keycols;

    auto msize = static_cast<ssize_t>(std::sqrt(std::distance(first, last)));
    for(auto i=0; i < msize; i++) {
        auto v1 = std::next(first, i * msize);
        auto v2 = std::next(v1, msize);

        // if(not std::equal(std::next(v1), v2, v1) and keycols.empty()) {
        if(keycols.empty()) {
            auto &&v2r = std::reverse_iterator<TIter>(v2);
            auto &&v1r = std::reverse_iterator<TIter>(v1);

            for(auto v=v2r; v != v1r; v++)
                keycols.emplace(*v, std::distance(v, v1r) - 1);
        }

        if(i)
            *outsep++ = "";   // std::cout << "| ";

        for(auto &&k: keycols)
            *out++ = *std::next(v1, k.second);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[])
{
    const size_t MAX_EXPECTED_SIZE = 15 * 15;
    const char SEPARATOR[] = "| ";

    std::ifstream stream(argv[1]);  // imbue ensures \n will deliver an error
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(SEPARATOR)));

    auto &&v0 = std::vector<MATRIXVAL_T>(MAX_EXPECTED_SIZE);

    auto &&out = std::ostream_iterator<MATRIXVAL_T>(std::cout, " ");
    auto &&outsep = std::ostream_iterator<const char *>(std::cout, SEPARATOR);
    auto &&in2 = std::istream_iterator<MATRIXVAL_T>();
    while(stream) {
        // Need to recreate after each stream error (eol)
        auto &&in1 = std::istream_iterator<MATRIXVAL_T>(stream);

        auto &&v0last = std::copy(in1, in2, v0.begin());
        if(v0last == v0.begin())
            break;  // nothing read (empty line)

        column_sort(v0.begin(), v0last, out, outsep);
        std::cout << std::endl;

        if(stream.eof())
            break;

        // Clear failbit and skip the eol which stopped input reading
        stream.clear();
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}
