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
#include <string>
#include <vector>


template <typename T, typename T2>
struct istream_iterator_until {
    std::istream *is;
    using TIter = std::istream_iterator<T>;
    TIter iter;
    TIter last;
    T2 tuntil;
    using T2iter = typename T2::const_iterator;
    T2iter ubegin, uend;
    bool at_end;

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

    istream_iterator_until() : at_end(true) {}

    istream_iterator_until(std::istream &is, const T2 &until) :
        is(&is), iter(is), last(),
        tuntil(until), ubegin(std::begin(tuntil)), uend(std::end(tuntil)),
        at_end((not is) ? true : std::find(ubegin, uend, *iter) != uend)
        {}

    T operator *() const { return *iter; }
    T until() const { T _tmp; is->unget() >> _tmp; return _tmp; }

    istream_iterator_until& operator ++() {
        iter++;
        at_end = iter == last or std::find(ubegin, uend, *iter) != uend;
        return *this;
    }

    istream_iterator_until operator ++(int) {
        istream_iterator_until tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator !=(const istream_iterator_until &other) const { return not is_equal(other); }
    bool operator ==(const istream_iterator_until &other) const { return is_equal(other); }

    bool is_equal(const istream_iterator_until &other) const {
        if(at_end)
            return other.at_end;

        if(other.at_end)
            return false;

        return iter == other.iter;
    }
};


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


template <typename T>
auto
visit_node(T &forest, int ROWSSIZE, int m, int n, bool start=false)
    -> int
{
    auto coord = m * ROWSSIZE + n;
    if(forest[coord] <= 0)
        return 0;

    forest[coord] = start ? -1 : 0;

    for(auto y=-1; y < 2; y++)
        for(auto x=-1; x < 2; x++)
            visit_node(forest, ROWSSIZE, m + y, n + x);

    return start;
}

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" ")));

    const int MAXSIZE = 30;
    const int OVERSIZE = MAXSIZE + 2;
    const int TOTALSIZE = OVERSIZE * OVERSIZE;

    auto forest = std::vector<int>(TOTALSIZE);
    auto fbegin = forest.begin();

    auto in2 = istream_iterator_until<char, std::string>();

    while (stream) {
        std::fill_n(fbegin, TOTALSIZE, 0);  // reset forest

        // skip pad left col and pad top row
        auto ffirst = std::next(fbegin, 1 + OVERSIZE);
        auto frows = 0, fcols = 0;

        auto fstart = ffirst;
        while(true) {
            auto in1 = istream_iterator_until<char, std::string>(stream, "|\n");
            // auto fstop = std::copy(in1, in2, fstart);
            auto fstop = std::transform(
                in1, in2, fstart, [] (const char &c) -> int { return c == 'o' ? 1 : 0; });
            fcols = std::distance(fstart, fstop);
            frows++;

            if(in1.until() == '\n' or not stream)
                break;  // end of foret or eof
            std::advance(fstart, OVERSIZE);
        }

        if(not fcols)
            break;  // nothing was read

        auto lakes = 0;
        for(auto m=1; m < frows + 1; m++)
            for(auto n=1; n < fcols + 1; n++)
                lakes += visit_node(forest, OVERSIZE, m, n, true);

        std::cout << lakes << std::endl;
    }
    return 0;
}
