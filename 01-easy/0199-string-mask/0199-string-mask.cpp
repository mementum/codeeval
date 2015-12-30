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
#include <string>

// Headers for the implementation
#include <algorithm>
#include <iterator>


template <typename TItIn, typename TItOut>
auto
string_mask(TItIn first, TItIn last, TItOut out)
{
    // Get type from iterator
    using IterType = typename std::iterator_traits<TItIn>::value_type;
    // type can also be iterated ... get its type
    using ItSubType = typename std::iterator_traits<typename IterType::iterator>::value_type;

    IterType masked;

    for(auto code=first, mask=std::next(first); mask != last; code++, mask++) {
        std::transform(
            code->begin(), code->end(), mask->begin(), std::back_inserter(masked),
            [] (const ItSubType &c, const ItSubType &m) {
                return m == '1' ? std::toupper(c) : c;
            }
            );
        *out++ = masked;
        masked.clear();
    }
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    auto ostr = std::ostream_iterator<std::string>(std::cout, "\n");
    auto istr2 = std::istream_iterator<std::string>();
    auto istr1 = std::istream_iterator<std::string>(stream);

    string_mask(istr1, istr2, ostr);
    return 0;
}
