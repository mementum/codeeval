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
#include <list>
#include <string>


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
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    using isit_until = istream_iterator_until<std::string>;
    auto slast = isit_until();

    while (stream) {
        auto players = std::list<std::string>{};
        std::copy(isit_until(stream, "|"), slast, std::back_inserter(players));
        if (players.empty())
            break;  // eof (nothing read)

        size_t bc;
        stream >> bc;
        bc--;  // make it 0 based

        while(players.size() > 1)
            players.erase(std::next(players.begin(), bc % players.size()));

        std::cout << players.front() << std::endl;
    }
    return 0;
}
