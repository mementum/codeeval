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
#include <functional>
#include <iterator>
#include <map>
#include <string>

#include <cmath>

//
// Solution compiles fine and solves the challenge but the usage of map
// function and iterators seems to be to much for the compilation with g++
// 4.8.1 under codeeval which always aborts compilation after 10 seconds

///////////////////////////////////////////////////////////////////////////////
// Stream Imbuer for Parsing
///////////////////////////////////////////////////////////////////////////////
struct SeparatorReader: std::ctype<char>
{
    template<typename T>
    SeparatorReader(T &&seps):
        std::ctype<char>(get_table(seps), true) {}

    template<typename T>
    std::ctype_base::mask const *get_table(T &&seps) {
        auto rc = new std::ctype_base::mask[std::ctype<char>::table_size]();
        for(auto &sep: seps)
            rc[static_cast<unsigned char>(sep)] = std::ctype_base::space;
        return &rc[0];
    }
};


struct Operat {
    using OperationT = std::function<double(double, double)>;
    static std::map<char, OperationT> OPERATIONS;

    Operat *parent;
    OperationT *operation;

    double opa;

    Operat(Operat *parent=nullptr, OperationT *operation=nullptr) :
        parent(parent), operation(operation), opa(NAN) {}

    Operat *addtoken(double op, Operat *child) {
        delete child;

        if(std::isnan(opa)) {  // no operand yet
            opa = op;  // store
            return this;
        }
        return parent->addtoken((*operation)(opa, op), this);
    }

    Operat *addtoken(const std::string &token) {
        if(not std::isdigit(token[0]))  // operator
            return new Operat(this, &OPERATIONS[token[0]]);

        if(std::isnan(opa)) {  // no operand yet
            opa = std::stod(token);  // store
            return this;
        }

        return parent->addtoken((*operation)(opa, std::stod(token)), this);
    }

    bool iscomplete() { return parent == nullptr; }

    void reset() { opa = NAN; }

    void destroy(Operat *child=nullptr) {
        delete child;
        parent == nullptr ? delete this : parent->destroy(this);
    }
};

std::map<char, Operat::OperationT> Operat::OPERATIONS = {
        {'+', Operat::OperationT(std::plus<double>())},
        {'*', Operat::OperationT(std::multiplies<double>())},
        {'/', Operat::OperationT(std::divides<double>())},
};


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader(" \n")));


    auto in1 = std::istream_iterator<std::string>(stream);
    auto in2 = std::istream_iterator<std::string>();
    auto out = std::ostream_iterator<int>(std::cout, "\n");

    auto curop = new Operat();
    for(auto in=in1; in != in2; in++) {
        curop = curop->addtoken(*in);
        if(curop->iscomplete()) {
            *out++ = curop->opa;
            curop->reset();
        }
    }
    curop->destroy();

    return 0;
}
