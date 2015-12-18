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
#include <algorithm>
#include <iterator>


const size_t COINS[] = {50, 25, 10, 5};
const auto CEND = std::end(COINS);


template<typename ntype, typename iter>
auto
calc_changes(ntype cents, iter itcoin)
{
    if (not cents or itcoin == CEND)
        return 1;

    auto coin = *itcoin;
    auto coincount = cents / coin;
    auto changes = 0;
    for (auto i=0; i <= coincount; i++)
        changes += calc_changes(cents - coin * i, std::next(itcoin));

    return changes;
}


template<typename iter, typename iterout>
auto
alternative_reality(iter in, iterout out)
{
    auto itcoin = std::find_if(
        std::begin(COINS), CEND,
        [&in] (size_t coin) { return coin <= *in;});

    *out++ = calc_changes(*in, itcoin);
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


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader("\n")));

    auto out = std::ostream_iterator<size_t>(std::cout, "\n");
    auto in1 = std::istream_iterator<size_t>(stream);
    auto in2 = std::istream_iterator<size_t>();

    for(auto &&in=in1; in != in2; in++)
        alternative_reality(in, out);

    return 0;
}
