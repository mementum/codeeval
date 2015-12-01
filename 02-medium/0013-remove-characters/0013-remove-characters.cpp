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

// Extra headers
#include <algorithm>  // remove_if


int main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while(std::getline(stream, line)) {
        auto comma = line.find(',');
        auto str = line.substr(0, comma);

        // 2nd string may be empty
        auto chars = (comma + 1 >= line.size() - 1) ? "" : line.substr(comma + 2);
        // This is simpler but could lead to error
        // auto chars = line.substr(comma + 2);

        // Lambda to filter out chars
        auto l = [&chars](const char &c) {
            return chars.find(c) != std::string::npos;
        };
        // remove_if returns an iterator which stops at the "to erase" chars
        str.erase(std::remove_if(str.begin(), str.end(), l), str.end());
        std::cout << str << std::endl;
    }
    return 0;
}
