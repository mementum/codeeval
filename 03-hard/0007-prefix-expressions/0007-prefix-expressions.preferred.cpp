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
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

//
// Solution compiles fine and solves the challenge but the usage of map
// function and iterators seems to be to much for the compilation with g++
// 4.8.1 under codeeval which always aborts compilation after 10 seconds

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);

    using OperationT = std::function<double(double, double)>;
    std::map<char, OperationT> OPERATIONS = {
        {'+', OperationT(std::plus<double>())},
        {'*', OperationT(std::multiplies<double>())},
        {'/', OperationT(std::divides<double>())},
    };

    auto in2 = std::istream_iterator<std::string>();
    auto out = std::ostream_iterator<int>(std::cout, "\n");

    std::string line;
    std::istringstream isstream;
    while(std::getline(stream, line)) {
        isstream.str(line);
        isstream.clear();

        auto in1 = std::istream_iterator<std::string>(isstream);

        auto ptokens = std::vector<std::string>();
        std::copy(in1, in2, std::back_inserter(ptokens));

        auto opstack = std::vector<double>();
        for(auto ptoken=ptokens.crbegin(); ptoken != ptokens.crend(); ptoken++) {
            auto &&token = *ptoken;
            if(std::isdigit(token[0])) {  // operand
                opstack.push_back(std::stod(token));
                continue;
            }

            // operator seen ... show time
            auto opa = opstack.back();  // get operand a
            opstack.pop_back();

            auto opb = opstack.back();  // get operand b
            opstack.pop_back();

            // operate and push on to the stack
            opstack.push_back(OPERATIONS[token[0]](opa, opb));
        }
        *out++ = static_cast<int>(opstack.front());
    }

    return 0;
}
