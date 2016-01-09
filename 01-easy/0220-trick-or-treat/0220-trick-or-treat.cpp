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
#include <fstream>
#include <iostream>

// Headers for the implementation

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    char tmp[10];
    char comma;
    int vamps, zombs, wits, houses;
    while (stream >> tmp >> vamps >> comma) {
        stream >> tmp >> zombs >> comma;
        stream >> tmp >> wits >> comma;
        stream >> tmp >> houses;

        auto children = vamps + zombs + wits;
        auto tcandies = (vamps * 3 + zombs * 4 + wits * 5) * houses;

        auto avcandies = tcandies / children;
        std::cout << avcandies << std::endl;
    }
    return 0;
}
