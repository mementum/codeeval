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
#include <vector>

// Extra headers
#include <algorithm>  // fill_n
#include <memory>  // unique_ptr
#include <limits>  //numeric_limits
#include <cstdint>  // uint16_t

// Cell Type and Cells (Array Length) types
typedef unsigned char cell_t;
typedef uint16_t clen_t;

#if 0
const int TOTALCELLS = (std::numeric_limits<clen_t>::max() + 1);
#else
// Following experiments, 8192 seems to be the maximum expected size
const int TOTALCELLS = 8192;
#endif


void
bfck(const std::string &bfcode, const size_t &totalcells=TOTALCELLS)
{
    const int lastcell = totalcells - 1;
    // Init cells array  - TOTALCELLS may not fit on the stack
    std::unique_ptr<cell_t[]> cells(new cell_t[totalcells]());
    std::fill_n(&cells[0], 0, totalcells);

    // Loop state and addresses
    std::vector<clen_t> loops;

    clen_t cellptr = 0;
    auto loopskip = 0;
    const clen_t eocode = bfcode.length();

    for(clen_t codeptr=0; codeptr < eocode; ++codeptr)
    {
        auto codechar = bfcode[codeptr];

        if(loopskip) {
            // skip as long as loops are being skipped / consider nested []
            if(codechar == '[')
                ++loopskip;
            else if(codechar == ']')
                --loopskip;
            continue;
        }
        switch(codechar) {
            case '+':
                cells[cellptr]++;  // overflow 255 -> 0 automatic
                break;

            case '-':
                cells[cellptr]--;  // overflow 0 -> 255 automatic
                break;

            case '>':
                cellptr = cellptr == lastcell ? 0 : cellptr + 1;
                break;

            case '<':
                cellptr = cellptr == 0 ? lastcell : cellptr - 1;
                break;

            case '[':
                if(cells[cellptr])
                    loops.push_back(codeptr);  // store loop entry pos
                else
                    loopskip++;  // loop to skip
                break;

            case ']':
                if(cells[cellptr])
                    codeptr = loops.back();  // retrieve loop entry pos
                else
                    loops.pop_back();  // pop loop entry pos ... carry on
                break;

            case '.':
                std::cout << cells[cellptr];
                break;
        }
    }
    std::cout << std::endl;  // output final end of line
}


int main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;

    while(std::getline(stream, line)) {
        bfck(line);
    }
    return 0;
}
