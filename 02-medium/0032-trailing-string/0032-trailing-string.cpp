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

// Basic headers for template
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while(std::getline(stream, line)) {
        if(line == "")
            continue;

        // The comparison could be done directly on line/ending
        // saving one substr along the way

        auto comma = line.find(',');
        auto str = line.substr(0, comma);
        // 2nd string may be empty
        // auto ending = (comma == line.size() - 1) ? "" : line.substr(comma + 1);
        auto ending = line.substr(comma + 1);

        auto strlen = str.size();
        auto endinglen = ending.size();
        // use std::string buil-in method to compare the endings
        auto endswith = strlen >= endinglen ?
            !str.compare(strlen - endinglen, endinglen, ending) :
            0;

        std::cout << endswith << std::endl;
    }
    return 0;
}
