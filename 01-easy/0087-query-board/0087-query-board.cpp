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

#include <cstring>

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    const int BLEN = 256;
    const int BSIZE = BLEN * BLEN;
    int board[BSIZE] = {};  // init to default entire array (0)

    char cmd[10];
    int coord, val;

    while(stream >> cmd) {
        if(::strncmp(cmd, "Set", 3) == 0) {
            stream >> coord >> val;
            if(::strncmp(&cmd[3], "Row", 3) == 0) {
                for(auto i=coord * BLEN; i < coord * BLEN + BLEN; i++)
                    board[i] = val;
            } else {
                for(auto i=coord; i < BSIZE; i += BLEN)
                    board[i] = val;
            }
        } else {
            stream >> coord;
            val = 0;
            if(::strncmp(&cmd[5], "Row", 3) == 0) {
                for(auto i=coord * BLEN; i < coord * BLEN + BLEN; i++)
                    val += board[i];
            } else {
                for(auto i=coord; i < BSIZE; i += BLEN)
                    val += board[i];
            }
            std::cout << val << std::endl;
        }
    }
    return 0;
}
