/*
  Copyright (C) 2016 Daniel Rodriguez

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
#include <string>



template <typename TIn>
auto
can_be_smiley(TIn first, TIn last, char c_open='(', char c_close=')', char smile=':')
{
    // Smileys will balance opening/closing parenthesis if a single parenthesis
    // is not available for it

    // Count open/close pairs keeping a count of opening/closing smileys to compensate

    auto openbrackets = 0, opensmileys = 0, closesmileys = 0;

    for(auto cit=first; cit != last; cit++) {
        auto c = *cit;
        if(c == c_open) {
            openbrackets++;

        } else if(c == c_close) {
            if(opensmileys and not openbrackets)
                opensmileys--;  // only if no opening brackets were there
            else  // else discount an open brack
                openbrackets--;  // this can make ob < 0 -> early breakout

        } else if(c == smile) {
            if(++cit == last)
                break;  // : was the last char, break out

            c = *cit;
            if(c == smile)
                cit--;  // detected consecutives ':' reevaluate
            else if(c == c_close)
                closesmileys += 1;
            else if(c == c_open)
                opensmileys += 1;
        }

        if(openbrackets < 0)
            break;  // break-out if at anytime a close precedes the yet to come open

        closesmileys *= openbrackets > 0;  // invalidate closesmileys if needed
    }

    if(openbrackets > 0 and closesmileys >= openbrackets)

        openbrackets = 0;  // compensate rem openbrackets w closesmileys

    return not openbrackets;

}

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    std::string line;
    while (std::getline(stream, line)) {
        std::cout << (can_be_smiley(line.begin(), line.end()) ? "YES" : "NO") << std::endl;
    }
    return 0;
}
