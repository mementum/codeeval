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

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

// if optimize is 0 then ala Formula 1, the test cases will be used as key to
// determine how to minimize memory allocation by 1st scanning the input file
// to determine the maximum "n" and "k" and allocate the vector only once with
// this maximum values

// The testcases are later executed with the actual values but without
// reallocating memory, simply restricting indices to the n, k values

#define OPTIMIZE 1

int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(",\n")));

    int m, n, k, a, b, c, r;
#if OPTIMIZE
    int maxn = 0, maxk = 0;
    while(stream >> n >> k >> a >> b >> c >> r) {
        maxn = std::max(maxn, n);
        maxk = std::max(maxk, k);
    }
    stream.clear();
    stream.seekg(0);

    auto ks = std::vector<unsigned short>(maxn);
    auto counts = std::vector<unsigned short>(maxk, 0);
    auto cbegin = counts.begin();
#endif

    while(stream >> n >> k >> a >> b >> c >> r) {
        auto l = k + 1;
#if not OPTIMIZE
        auto ks = std::vector<unsigned short>(std::max(n, maxn));
        auto counts = std::vector<unsigned short>(l, 0);
        auto cbegin = counts.cbegin();
#else
        std::fill_n(cbegin, l, 0);  // zero the needed maximum
#endif
        auto cend = std::next(cbegin, l);  // do not look beyond needed max

        ks[0] = m = a % r;  // init value
        if(m < l)
            counts[m]++;  // do only count if within target range

        for(auto i=1; i < k; i++) {  // init 1st k values
            ks[i] = m = (b * m + c) % r;
            if(m < l)
                counts[m]++;  // do only count if in target range
        }

        for(auto i=k, oow=0, mnni=0; i < n; i++) {  // slide the window
            ks[i] = mnni = std::distance(cbegin, std::find(cbegin, cend, 0));
            counts[mnni]++;
            oow = ks[i - k];
            if(oow < l)
                counts[oow]--;  // do not update for too large never to be seen again
        }
        std::cout << ks[n - 1] << std::endl;
    }

    return 0;
}
