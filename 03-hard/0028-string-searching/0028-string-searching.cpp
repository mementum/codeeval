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
#include <algorithm>  // copy, stable_sort
#include <iterator>
#include <string>


template <typename T1, typename T2>
auto
is_sub(T1 first1, T1 last1, T2 first2, T2 last2,
       bool start=true, bool escape=false)
    -> bool
{
    if(not std::distance(first2, last2))
        return true;  // consumed 2nd range -> match

    if(not std::distance(first1, last1))
        return false;  // consumed 1st range -> nothing to match -> ohhh

    auto sec0 = *first2;
    auto second2 = std::next(first2);

    // escape seen, go to next signaling it
    if(sec0 == '\\')
        return is_sub(first1, last1, second2, last2, start, true);

    // * matching ... start true ... any match after * will do
    if(not escape and sec0 == '*')
        return is_sub(first1, last1, second2, last2);

    // Regular char by char search ... start false to indicate consecutive
    for(auto fir=first1; fir != last1; fir++) {
        if(*fir == sec0)
            if(is_sub(std::next(fir), last1, second2, last2, false))
                return true;

        if(not start)  // we had matches, this breaks consecutive matching
            break;
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[])
{
    std::ifstream stream(argv[1]);  // imbue ensures \n will deliver an error

    std::string input;
    while(std::getline(stream, input)) {
        // Define the 2 strings via iterators without sub-str'ing
        auto a1 = input.begin();
        auto b2 = input.end();
        auto a2 = std::find(a1, b2, ',');
        auto b1 = std::next(a2);

        std::cout << (is_sub(a1, a2, b1, b2) ? "true" : "false") << std::endl;
    }

    return 0;
}
