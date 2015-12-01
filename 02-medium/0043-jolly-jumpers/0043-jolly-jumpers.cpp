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
#include <fstream>  // ifstream
#include <iostream>  // cout

// Extra headers
#include <bitset>  // bitset
#include <cmath>  // abs
#include <limits>  // numeric_limits
#include <utility>  // pair/make_pair

// Constant definitions for clarity
const int MAXJOLLY = 3000;

enum class IsEoF { Yes = true, No = false};
enum class IsJolly { Yes = true, No = false, Undef = false};

/*
  The solution is elegant in that it takes an input stream reads the ints from
  it using the extractio operator. It can therefore take a filestream, a
  stringstream or any other form.

  The restriction to size 3000 is given by the challenge description and helps
  to save memory, by being able to restrict (at compile time) the control array
  to a bitset of fixed size (else a dynamic vector<bool> would be needed
*/

std::pair<IsEoF, IsJolly>
is_jolly_jumper(std::istream &ss)
{
    // Return value: pair.first: EOF (true/false), second: Jolly (true/false)
    int jolly, jollylen, prev, cur;

    if( !(ss >> jollylen))
        return std::make_pair(IsEoF::Yes, IsJolly::Undef);  // eof seen

    jolly = jollylen;  // keep fixed value for check below

    ss >> prev;
    jollylen--;  // discount the initial "prev" token

    std::bitset<MAXJOLLY> s;  // keep track of seen jollies
    while (jollylen--)
    {
        ss >> cur;
        auto diff = std::abs(prev - cur);
        if (diff >= jolly || diff == 0 || s[diff]) {
            // It may be an early break, skip all chars until eol (or eof)
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            // too large/small or already seen ... not jolly
            return std::make_pair(IsEoF::No, IsJolly::No);
        }

        // mark the value as seen and continue
        s[diff] = true;
        prev = cur;
    }

    return std::make_pair(IsEoF::No, IsJolly::Yes);  // No early breakout -> Jolly
}


int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    while (true) {
        auto result = is_jolly_jumper(stream);
        if (result.first == IsEoF::Yes)
            break;  // nothing computed (eof)

        auto strout = (result.second == IsJolly::Yes) ? "Jolly" : "Not jolly";
        std::cout << strout << std::endl;
    }
    return 0;
}
