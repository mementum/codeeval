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

#include <algorithm>

// There is a limit to the tradeoff memoization / time spent calculating the
// primes. Pre-memoization is the best (it could of course be done off-line
// given that prime numbers is a recurring problem and the result would be
// stored in an array for quick lookup access

template <typename T>
auto
isprime(T number)
{
    if((not (number & 1)) and number != 2)
        return false;
    if(number % 3 == 0 and number != 3)
        return false;
    if(number < 2)
        return false;

    // if(number < 1373653) {
    for(T k=1; 36 * k * k - 12 * k < number; ++k)
        if((number % (6 * k + 1) == 0) or (number % (6 * k - 1) == 0))
            return false;

    return true;
    // }
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[])
{
    // memoize
    unsigned short memo[1229];
    memo[0] = 2;
    for(unsigned short i=3, count=1; i < 10000; i += 2)
        if(isprime(i))
            memo[count++] = i;

    std::ifstream stream(argv[1]);

    auto mbegin = std::begin(memo);
    auto mend = std::end(memo);
    auto m2nd = std::next(mbegin);

    unsigned int input;
    while(stream >> input) {
        auto lb = std::lower_bound(mbegin, mend, input);
        // 0 index (2) is always there
        if(lb == mend) {
            std::cout << std::endl;
            continue;
        }

        std::cout << *mbegin;
        for(auto i=m2nd; i < lb; i++)
            std::cout << ',' << *i;
        std::cout << std::endl;
    }
    return 0;
}
