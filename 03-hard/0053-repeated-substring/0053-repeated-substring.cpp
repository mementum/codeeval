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
#include <functional>
#include <iterator>
#include <vector>

#include <cctype>


template <typename T>
struct istream_iterator_until {
    std::istream_iterator<T> iter;
    std::istream_iterator<T> last;
    T until;
    bool at_end;

    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

    istream_iterator_until() : at_end(true) {}

    istream_iterator_until(std::istream &is, const T &until) :
        iter(is), last(), until(until), at_end(not is or *iter == until) {}

    T operator *() const { return *iter; }

    istream_iterator_until& operator ++() {
        ++iter;
        at_end = iter == last or *iter == until;
        return *this;
    }

    istream_iterator_until operator ++(int) {
        istream_iterator_until tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator !=(const istream_iterator_until &other) const {
        return not (*this == other);
    }

    bool operator ==(const istream_iterator_until &other) const {
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

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    // no ws, eo copy is marked by istream_iterator_until
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader("")));

    // Prepare input and output
    auto in2 = istream_iterator_until<char>{};
    auto out = std::ostream_iterator<char>(std::cout);

    auto NONE = std::vector<char>{'N', 'O', 'N', 'E'};
    auto master = std::vector<char>(100);
    auto mfirst = master.begin();

    while(stream) {
        auto in1 = istream_iterator_until<char>{stream, '\n'};
        auto mlast = std::copy(in1, in2, mfirst);
        auto mlen = std::distance(mfirst, mlast);

        auto found = false;;
        auto s1first = mfirst, s1last = mfirst;  // init for auto
        for(auto l=static_cast<int>(mlen / 2); l > 0; l--) {
            for(auto i=0; i < mlen - 2 * l + 1; i++) {
                s1first = std::next(mfirst, i);  // substr start
                s1last = std::next(s1first, l);  // substr end

                // search in rest of string s1last->last for s1first-s1last
                auto s = std::search(s1last, mlast, s1first, s1last);
                if(s != mlast and not std::all_of(s1first, s1last, isspace)) {
                    found = true;
                    break;
                }
            }
            if(found)
                break;
        }

        if(not found) {
            s1first = std::begin(NONE);
            s1last = std::end(NONE);
        }

        std::copy(s1first, s1last, out);
        std::cout << std::endl;
    }
    return 0;
}
