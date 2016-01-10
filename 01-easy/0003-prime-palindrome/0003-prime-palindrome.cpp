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

// Headers for test case input/output
#include <iostream>

// Headers for the implementation
#include <cmath>

template<typename T>
auto
is_prime(const T &n)
{
    if(n == 2 or n == 3)
        return true;

    if(n < 2 or not (n & 1) or not (n % 3))
        return false;

    T rend = static_cast<T>(std::sqrt(n)) + 1;
    for(T i=3; i < rend; i += 2)
        if(not (n % i))
            return false;

    return true;
}

template <typename T>
auto
is_palindrome(T n)
{
    T forward = n, backwards = 0;
    while(forward) {
        backwards = 10 * backwards + forward % 10;
        forward /= 10;
    }
    return n == backwards;
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {

    int p = 1000;
    while(--p > 2) {
        if(is_palindrome(p) and is_prime(p))
            break;
    }
    std::cout << p << std::endl;
    return 0;
}
