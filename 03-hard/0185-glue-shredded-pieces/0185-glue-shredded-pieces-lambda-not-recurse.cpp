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
#include <iterator>
#include <sstream>
#include <string>

// Headers for the implementation
#include <algorithm>
#include <functional>
#include <map>
#include <stack>
#include <type_traits>
#include <vector>

template<typename T>
void debugout(T t)
{
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
void debugout(T t, Args... args) // recursive variadic function
{
    std::cout << t;
    debugout(args...);
}


///////////////////////////////////////////////////////////////////////////////
// Stream Imbuer for Parsing
// http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring
//    ss.imbue(std::locale(ss.getloc(), new SeparatorReader(" ,|")));
///////////////////////////////////////////////////////////////////////////////
struct SeparatorReader: std::ctype<char>
{
    SeparatorReader(const std::string &seps):
        std::ctype<char>(get_table(seps), true) {}

    std::ctype_base::mask const *get_table(const std::string &seps) {
        auto rc = new std::ctype_base::mask[std::ctype<char>::table_size]();
        for(auto &sep: seps)
            rc[sep] = std::ctype_base::space;
        return &rc[0];
    }
};

/*
  gcc 4.9.x solves the lambda recursion perfectly
  gcc 4.8.1 (CodeEval) compiles fine but execution shows that the recursion
  doesn't actually work because only the 1st path goes to the output (doing
  direct output to std::cout doesn't change the result)
*/

template<typename iter, typename iterout>
auto
unshredder(iter a1, iter a2, iterout out1, bool eol=false)
{
    using ittype = typename std::decay<decltype(*a1)>::type;

    auto vertbals = std::map<ittype, int>{};
    using vbaltype = typename std::map<ittype, int>::value_type;

    using itstack = std::stack<typename std::map<ittype, int>::iterator>;  // else ... map not defined
    auto edges = std::map<ittype, itstack>{};
    using itertype = typename std::map<ittype, itstack>::iterator;


    auto k1len = (*a1).size() - 1;

    auto kmers = 0;
    for(auto &&a=a1; a != a2 and (not eol or *a != "\n"); a++, kmers++) {
        auto &&kmer = *a;
        auto &&kl = kmer.substr(0, k1len);
        auto &&kr = kmer.substr(1, k1len);

        vertbals[kl] -= 1;
        vertbals[kr] += 1;

        edges[kl].push(vertbals.find(kr));
    }

    auto headit = std::find_if(
        vertbals.begin(), vertbals.end(),
        [&](const vbaltype &keyval) { return keyval.second < 0; });

    auto &&head = headit->first;
    std::copy(head.begin(), std::next(head.begin(), k1len - 1), out1);

    std::function<void (itertype)> nodevisit =
        [&](itertype src)
        {
            *out1++ = src->first[k1len - 1];

            auto &&nstack = src->second;
            while(not nstack.empty()) {
                auto &&dst = nstack.top();
                nstack.pop();
                auto next = edges.find(dst->first);
                if(next == edges.end()) { // Terminal node
                    *out1++ = dst->first[k1len - 1];
                    return;
                }
                nodevisit(next);
            }
        };
    nodevisit(edges.find(headit->first));
}


///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[]) {
    std::ifstream stream(argv[1]);
    std::string line;
    stream.imbue(std::locale(stream.getloc(), new SeparatorReader("|")));
    // std::stringstream ss;
    // ss.imbue(std::locale(ss.getloc(), new SeparatorReader("|\n")));

    auto itout = std::ostream_iterator<char>(std::cout);
    auto itin2 = std::istream_iterator<std::string>();

    while (stream) {
        auto itin1 = std::istream_iterator<std::string>(stream);
        unshredder(itin1, itin2, itout, true);
        std::cout << std::endl;
    }

    return 0;
}
