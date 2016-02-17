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
        if(iter == last)
            at_end = true;
        else if(std::find(ubegin, uend, *iter) != uend) {
            at_end = true;
            is->unget();  // return the char to the stream processing
        }
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
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    auto in2 = istream_iterator_until<char, std::string>();
    auto elements = std::vector<char>(100);
    auto ebegin = elements.begin();
    while (stream) {
        auto in1 = istream_iterator_until<char, std::string>(stream, "0123456789");
        auto elast = std::copy(in1, in2, ebegin);
        int mth; stream >> mth;
        if(mth > std::distance(ebegin, elast))
            continue;

        std::cout << *std::prev(elast, mth) << std::endl;
    }
    return 0;
}
