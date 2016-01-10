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

#include <cmath>
#include <vector>

template<typename T>
void debugout(const T &t)
{
    std::cout << t << std::endl;
}

template<typename T>
void debugout(const std::vector<T> &v)
{
    std::cout << "{";
    if(v.size()) {
        auto last = std::prev(v.end(), 1);
        for(auto &&p=v.begin(); p != last; p++)
            std::cout << *p << ",";

        std::cout << *last;
    }
    std::cout << "}" << std::endl;
}

template<typename T, typename... Args>
void debugout(const T &t, Args... args) // recursive variadic function
{
    std::cout << t;
    debugout(args...);
}


// Rabin Miller seems the best solution
// https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
// http://stackoverflow.com/questions/4424374/determining-if-a-number-is-prime

long power(int a, int n, int mod)
{
    long power = a, result = 1;

    while(n) {
        if(n & 1)
            result = (result * power) % mod;
        power = (power * power) % mod;
        n >>= 1;
    }
    return result;
}

bool witness(int a, int n)
{
    auto u = n / 2;
    auto t = 1;
    while(not (u & 1)) {
        u  /= 2;
        ++t;
    }

    long curr, prev = power(a, u, n);
    for(auto i=1; i <= t; ++i, prev = curr) {
        curr = (prev * prev) % n;
        if((curr == 1) and (prev != 1) and (prev != n - 1))
            return true;
    }
    return curr != 1;
}

inline
bool isprime(int number)
{
    /*
      WARNING: Algorithm deterministic only for numbers < 4,759,123,141
      (unsigned int's max is 4294967296)

      if n < 1,373,653, it is enough to test a = 2 and 3.
      if n < 9,080,191, it is enough to test a = 31 and 73.
      if n < 4,759,123,141, it is enough to test a = 2, 7, and 61.
      if n < 2,152,302,898,747, it is enough to test a = 2, 3, 5, 7, and 11.
      if n < 3,474,749,660,383, it is enough to test a = 2, 3, 5, 7, 11, and 13.
      if n < 341,550,071,728,321, it is enough to test a = 2, 3, 5, 7, 11, 13,
      and 17.
    */
#if 0
    if((not (number & 1)) and number != 2)
        return false;
    if(number < 2)
        return false;
#endif
    if(number % 3 == 0 and number != 3)
        return false;

    if(number < 1373653) {
        for(auto k=1; 36 * k * k - 12 * k < number; ++k)
            if ((number % (6 * k + 1) == 0) or (number % (6 * k - 1) == 0))
                return false;

        return true;
    }

    if(number < 9080191) {
        if(witness(31, number))
            return false;
        if(witness(73, number))
            return false;
        return true;
    }

    if(witness(2, number))
        return false;
    if(witness(7, number))
        return false;
    if(witness(61, number))
        return false;

    return true;
}


auto
memprime(unsigned int number) -> bool
{
    // very basic memoization
    static unsigned int maxsize = 64 * 4096;  // on a 64 bit machine ... 4k
    static std::vector<bool> memo(maxsize);
    static unsigned int lastseen = 0;  // avoid resizing (reallocation)

    if(number >= maxsize)
        return isprime(number);

    if(number > lastseen) {
        lastseen = number;
        memo[number] = isprime(number);
    }

    return memo[number];
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[])
{
    std::ifstream stream(argv[1]);

    unsigned int input;
    while(stream >> input) {
        if(input < 2) {
            std::cout << std::endl;
            continue;
        }

        std::cout << 2;
        for(auto i=3U; i < input; i += 2) {
            if(memprime(i)) {
                std::cout << ',' << i;
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
