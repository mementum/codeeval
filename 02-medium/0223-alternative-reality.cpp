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
#include <string>


// Headers for the implementation
#include <algorithm>
#include <vector>
#include <utility>

const std::vector<size_t> COINS = {50, 25, 10, 5};


template<typename ntype, typename iter>
auto
alternative_reality(ntype cents, iter itcoin)
{
    if (not cents or itcoin == COINS.end())
        return 1;

    auto coin = *itcoin;
    auto coincount = cents / coin;
    auto changes = 0;
    for (auto i = 0; i <= coincount; i++)
        changes += alternative_reality(cents - (coin * i), std::next(itcoin));

    return changes;
}


template<typename ntype>
auto
alternative_reality(ntype cents)
{
    auto itcoin = std::find_if(COINS.begin(),
                               COINS.end(),
                               [&cents] (size_t coin) { return coin <= cents;});

    return alternative_reality(cents, itcoin);
}


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while (std::getline(stream, line)) {
        std::cout << alternative_reality(std::stoi(line), COINS.begin()) << std::endl;
    }
    return 0;
}
