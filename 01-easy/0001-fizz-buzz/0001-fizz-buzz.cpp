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


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    int x, y, n;
    while(stream >> x >> y >> n) {
        for(auto i = 1; i <= n; i++) {
            if(i > 1)
                std::cout << " ";

            auto xd = i % x;
            auto yd = i % y;

            if(xd and yd)
                std::cout << i;
            else {
                if(not xd)
                    std::cout << "F";
                if(not yd)
                    std::cout << "B";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
