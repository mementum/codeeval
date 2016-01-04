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

#include <iostream>

#include <cmath>


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
bool
p_check(const int n)
{
    const int rbegin = 3;
    auto rend = static_cast<int>(std::sqrt(n)) + 1;
    for(auto i=rbegin; i < rend; i++)
        if( !(n % i))
            return false;

    return true;
}


int
main(int argc, char *argv[]) {
    int n = 5; // skip 2 and 3
    int s = 5; // 2 + 3

    for(int i=2; i < 1000; n += 2) {
        if(p_check(n)) {
            s += n;
            i++;
        }
    }
    std::cout << s << std::endl;

    return 0;
}
