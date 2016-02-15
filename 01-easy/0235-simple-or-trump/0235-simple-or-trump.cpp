/*
  Copyright (C) 2015, 2016 Daniel Rodriguez

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
#include <algorithm>
#include <iterator>
#include <map>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    std::map<char, int> RANKS = {
        {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3}, {'6', 4}, {'7', 5}, {'8', 6},
        {'9', 7}, {'1', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12}
    };

    const int TRUMPVAL = 100;
    const int NCARDS = 2;

    std::string cards[NCARDS] = {"\0\0\0"};
    char sep, trump;
    int values[NCARDS];

    while (stream) {
        for(auto i = 0; i < NCARDS; i++)
            stream >> cards[i];

        if(not stream)
            break;  // eof - could not read more cards

        stream >> sep >> trump;

        for(auto i = 0; i < NCARDS; i++)
            values[i] = RANKS[cards[i].front()] + (cards[i].back() == trump ? TRUMPVAL : 0);

        auto diff = values[0] - values[1];
        if(not diff)
            std::cout << cards[0] << ' ' << cards[1] << std::endl;
        else
            std::cout << (diff > 0 ? cards[0] : cards[1]) << std::endl;

    }
    return 0;
}
